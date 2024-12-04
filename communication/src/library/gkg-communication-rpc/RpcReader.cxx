#include <gkg-communication-dispatcher/Dispatcher.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-rpc/RpcReader.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-rpc/RpcProlog.h>
#include <gkg-communication-rpc/RpcEpilog.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


// prepare to read RPC requests from somebody else's connection or
// prepare to handle RPC requests read by somebody else; assume the
// I/O format has already been negotiated; start listening for RPC
// requests on the connection, taking into account the special case
// where RPC requests have already been buffered; zero the function
// array that a derived class will initialize with addresses of static
// member functions to unmarshall RPC requests
gkg::RpcReader::RpcReader( gkg::SocketStream* client, 
                           bool nonBlocking )
               : gkg::SocketReader( client, nonBlocking ),
                 _prolog( "" ),
                 _epilog( "" )
{
}


// connect to a client through an already open file number; negotiate
// the I/O format; start listening for RPC requests on the
// connection, taking into account the special case where RPC requests
// have already been buffered (when the I/O format was negotiated);
// zero the function array that a derived class will initialize with
// addresses of static member functions to unmarshall RPC requests
gkg::RpcReader::RpcReader( int32_t fd, bool binary,
                           bool nonBlocking )
               : gkg::SocketReader( fd, binary, nonBlocking ),
                 _prolog( "" ),
                 _epilog( "" )
{
}


void gkg::RpcReader::setProlog( const std::string& prolog )
{

  _prolog = prolog;

}


void gkg::RpcReader::setEpilog( const std::string& epilog )
{

  _epilog = epilog;

}


// read only one RPC request per call to allow the program to
// interleave RPC requests from multiple clients; look up the proper
// reader to execute the request or skip over the request's data if it
// could not be executed; ask a derived class to take the appropriate
// action (perhaps closing the file number or deleting ``this'') if no
// more data is available or the data wasn't what we expected
int32_t gkg::RpcReader::isInputReady( int32_t fd )
{

  try
  {

    gkg::SocketHeader header;
    gkg::RpcProlog prolog( _prolog );

    std::streambuf* buffer = getClient().rdbuf();
    std::streamsize nbRemainingData = 0;

    if ( !buffer )
    {

      connectionClosed( fd );
      return -1;  // don't ever call me again (i.e., detach me)

    }

    do
    {

      getClient() >> prolog;
      getClient() >> header;

      if ( getClient().good() )
      {

        gkg::SocketReader* reader = map( header.getReader() );
        if ( !execute( reader, header ) )
        {

          std::cerr << "RpcReader::isInputReady() : execution failed" 
                    << std::endl;
          // getClient().ignore( header.getDataCount() );

        }
        gkg::RpcEpilog epilog( _epilog );
        getClient() >> epilog;

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
  GKG_CATCH( "int32_t gkg::RpcReader::isInputReady( int32_t fd )");

  return 0;   // call me again as soon as possible

}
