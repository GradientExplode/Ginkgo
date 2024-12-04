#include <gkg-communication-socket/SocketBuffer.h>
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <gkg-core-cppext/NetInetIn.h>
#include <cctype>
#include <errno.h>
#include <fcntl.h>

#ifndef __DECCXX

#include <cstdlib>
#include <gkg-core-cppext/CUniStd.h>
#include <time.h>

#ifdef _G_HAVE_SYS_RESOURCE

#include <sys/time.h>
#include <sys/resource.h>

#endif

#endif

#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>

#ifndef SOMAXCONN

#define SOMAXCONN 5

#endif


// I need a pointer to an IOStream so I can insert and extract values
// in the length field of requests; if I don't have a stream, I
// won't allow you to call startRequest().

gkg::SocketBuffer::SocketBuffer( int32_t fd, gkg::FStream::OpenMode mode )
                  : gkg::FStream( fd, mode, 8192 ),
                    _fd( fd ),
                    _stream( 0 ),
                    _opened( false ),
                    _close( false ),
                    _nonBlocking( false ),
                    _verbose( true )
{

  if ( _fd < 0 )
  {

    setSysError( "gkg::SocketBuffer::connect: socket file descriptor < 0" );

  }

}


// free the buffer used to store the put area; the std::streambuf
// destructor will free the buffer used to store the get area.
gkg::SocketBuffer::~SocketBuffer()
{

  close();

}



void gkg::SocketBuffer::setStream( gkg::IOStream* stream )
{

  _stream = stream;

}


int32_t gkg::SocketBuffer::getFd()
{

  return _fd;

}


bool gkg::SocketBuffer::isOpened()
{

  return _opened;

}


bool gkg::SocketBuffer::isNonBlocking()
{

  return _nonBlocking;

}


// create a socket, bind the socket to a port address, and prepare to
// accept incoming connections
gkg::SocketBuffer* gkg::SocketBuffer::listen()
{

  if ( _opened )
  {

    setError( "gkg::SocketBuffer::listen: already using a file number" );
    return 0;

  }
  _opened = true;
  _close = true;
  _nonBlocking = false;
  return this;

}


// create a socket and make a connection to another socket; do retries in case
// the peer has hit its backlog, but not too many retries since the error is 
// indistinguishable from there not being a peer listening at all
gkg::SocketBuffer* gkg::SocketBuffer::connect()
{

  if ( _opened )
  {

    setError( "gkg::SocketBuffer::connect: already using a file number" );
    return 0;

  }
  _opened = true;
  _close = true;
  _nonBlocking = false;
  return this;

}


// accept an incoming connection, allocate a new file descriptor for
// it, and return the new file descriptor.
gkg::SocketBuffer* gkg::SocketBuffer::accept( int32_t& fd )
{

  struct sockaddr_in name;
  socklen_t nameLength = sizeof( name );

  if ( !_opened )
  {

    setError( "gkg::SocketBuffer::accept: not using a file number yet" );
    return 0;

  }

  fd = ::accept( _fd, ( struct sockaddr* )&name, &nameLength );
  if ( fd < 0 )
  {

    setSysError( "gkg::SocketBuffer::accept: accept" );
    return 0;

  }
  return this;

}


// empty the get/put areas, close the file descriptor if nothing else
// might use it, and detach the std::streambuf from the file descriptor.
gkg::SocketBuffer* gkg::SocketBuffer::close()
{

  if ( !_opened )
  {

    return 0;

  }

  sync();

  int32_t ok = 0;

// code removed because gkg::Fstream destructor close the stream
// pointed to by _fd
/*
  if ( _close )
  {

    ok = ::close( _fd );
    if ( ok < 0 )
      setSysError( "gkg::SocketBuffer::close: close" );

  }
*/

  _opened = false;
  _close = false;
  _nonBlocking = false;
  return ( ok < 0 ) ? 0 : this;

}


// set or clear non-blocking I/O on the file descriptor
gkg::SocketBuffer* gkg::SocketBuffer::setNonBlocking( bool value )
{

  if ( _nonBlocking != value )
  {

    int32_t flags = fcntl( _fd, F_GETFL, 0 );
    if ( flags < 0 )
    {

      setSysError( "gkg::SocketBuffer::nonblocking: F_GETFL fcntl" );
      return 0;

    }
    if ( value )
    {

      flags |= O_NDELAY;

    }
    else
    {

      flags &= ~O_NDELAY;

    }
    if ( fcntl( _fd, F_SETFL, flags ) < 0 )
    {

      setSysError( "gkg::SocketBuffer::nonblocking: F_SETFL fcntl" );
      return 0;

    }

  }
  _nonBlocking = value;
  return this;

}


// set or clear printing of system error messages.
gkg::SocketBuffer* gkg::SocketBuffer::setVerbose( bool value )
{

  _verbose = value;
  return this;

}


// print a user error message.
void gkg::SocketBuffer::setError( const std::string& message )
{

  if ( _verbose )
  {

    std::cerr << message << "\n";
    std::cerr.flush();

  }

}


// print a system error message.
void gkg::SocketBuffer::setSysError( const std::string& message )
{

  if ( _verbose )
  {

    perror( message.c_str() );

  }

}


// get the stream which will format the length field of requests
gkg::IOStream& gkg::SocketBuffer::getStream()
{

  return *_stream;

}
