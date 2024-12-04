#include <gkg-communication-dispatcher/Dispatcher.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-socket/SocketReader.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


// prepare to read socket requests from somebody else's connection or
// prepare to handle socket requests read by somebody else; assume the
// I/O format has already been negotiated; start listening for socket
// requests on the connection, taking into account the special case
// where socket requests have already been buffered; zero the function
// array that a derived class will initialize with addresses of static
// member functions to unmarshall socket requests
gkg::SocketReader::SocketReader( gkg::SocketStream* client, 
                                 bool nonBlocking )
                  : gkg::IOHandler(),
                    _fd( client ? client->getSocketBuffer()->getFd() : -1 ),
                    _client( client ),
                    _delete( false )
{

  if ( _client )
  {

    getClient().setNonBlocking( nonBlocking );
    gkg::Dispatcher::getInstance().link( _fd, gkg::Dispatcher::ReadMask, this );
    // gkg::Dispatcher::getInstance().setReady( _fd,
    //                                          gkg::Dispatcher::ReadMask );

  }

}


// connect to a client through an already open file number; negotiate
// the I/O format; start listening for socket requests on the
// connection, taking into account the special case where socket requests
// have already been buffered (when the I/O format was negotiated);
// zero the function array that a derived class will initialize with
// addresses of static member functions to unmarshall socket requests
gkg::SocketReader::SocketReader( int32_t fd, bool binary,
                                 bool nonBlocking )
                  : gkg::IOHandler(),
                    _fd( fd ),
                    _client( new gkg::SocketStream(
                             new gkg::SocketBuffer( fd, gkg::FStream::In ) ) ),
                    _delete( true )
{

  getClient().negotiate( binary );
  getClient().setNonBlocking( nonBlocking );
  gkg::Dispatcher::getInstance().link( fd, gkg::Dispatcher::ReadMask, this );
  //  gkg::Dispatcher::getInstance().setReady( fd,
  //                                           gkg::Dispatcher::ReadMask );

}


// stop listening for socket requests on the connection if we have one;
// delete the connection if we created it ourselves; note that
// deleting a connection attached to a file number will not close that
// file number
gkg::SocketReader::~SocketReader()
{

  if ( _fd >= 0 )
  {

    gkg::Dispatcher::getInstance().unlink( _fd );

  }
  if ( _delete )
  {

    delete _client;

  }
  _functions.clear();

}


void gkg::SocketReader::addFunction( int32_t id, 
                                     gkg::SocketReader::PF function )
{

  _functions.insert( std::make_pair( id, function ) );

}


gkg::SocketReader::PF gkg::SocketReader::getFunction( int32_t id )
{

  std::map< int32_t, gkg::SocketReader::PF>::iterator
    f = _functions.find( id );

  if ( f != _functions.end() )
  {

    return f->second;

  }

  return 0;

}


gkg::SocketStream& gkg::SocketReader::getClient()
{

  return *_client;

}


// read only one socket request per call to allow the program to
// interleave socket requests from multiple clients; look up the proper
// reader to execute the request or skip over the request's data if it
// could not be executed; ask a derived class to take the appropriate
// action (perhaps closing the file number or deleting ``this'') if no
// more data is available or the data wasn't what we expected
int32_t gkg::SocketReader::isInputReady( int32_t fd )
{

  try
  {

    gkg::SocketHeader header;

    std::streambuf* buffer = getClient().rdbuf();
    std::streamsize nbRemainingData = 0;

    if ( !buffer )
    {

      connectionClosed( fd );
      return -1;  // don't ever call me again (i.e., detach me)

    }

    do
    {

      getClient() >> header;

      if ( getClient().good() )
      {

        gkg::SocketReader* reader = map( header.getReader() );
        if ( !execute( reader, header ) )
        {

          std::cerr << "SocketReader::isInputReady() : execution failed" 
                    << std::endl;
          // getClient().ignore( header.getDataCount() );

        }

      }

      if ( getClient().eof() || getClient().fail() )
      {

        connectionClosed( fd );
        return -1;  // don't ever call me again (i.e., detach me)

      }

      nbRemainingData = buffer->in_avail();

    }
    while ( nbRemainingData > 0 );

  }
  GKG_CATCH( "int32_t gkg::SocketReader::isInputReady( int32_t fd )");

  return 0;   // call me again as soon as possible

}


// map the number to the reader that should unmarshall the socket
// request; return this reader itself by default; derived classes
// could return a different reader.

gkg::SocketReader* gkg::SocketReader::map( uint64_t )
{

  return this;

}


// look up the static member function that will unmarshall the socket
// request's arguments and execute the request; call that function
// with the reader, socket request header, and connection as arguments;
// return true if the function was called, false otherwise
bool gkg::SocketReader::execute( gkg::SocketReader* reader, 
                                 gkg::SocketHeader& header )
{

  if ( !reader )
  {

    return false;

  }

  if ( ( header.getRequest() < 0 ) ||
       ( header.getRequest() >= (int32_t)reader->_functions.size() ) )
  {

    return false;

  }

  gkg::SocketReader::PF function = reader->getFunction( header.getRequest() );
  if ( !function )
  {

    return false;

  }

  ( *function )( reader, header, getClient() );
  return true;

}
