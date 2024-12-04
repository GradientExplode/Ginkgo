#include <gkg-communication-socket/SocketRegistry.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-socket/SocketWriter.h>
#include <gkg-core-exception/Exception.h>
#include <cstdlib>  // for abort()
#include <cstdio>
#include <cerrno>
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

#ifndef __DECCXX

#include <cstdlib>
#include <gkg-core-cppext/CUniStd.h>
#include <time.h>

#ifdef _G_HAVE_SYS_RESOURCE

#include <sys/time.h>
#include <sys/resource.h>

#endif

#endif

#include <sys/socket.h>
#include <netinet/tcp.h>



// open a connection to an socket service at its registered host name and
// port number, or give the host name and port number needed to open a
// connection to the socket service, or give the number of an already
// open file descriptor, or give the address of an already open
// rpcstream (connection); negotiate the I/O format if opening a
// connection; terminate the program if errors should be fatal

gkg::SocketWriter::SocketWriter( const std::string& path, 
                           	 bool fatal, 
                           	 bool binary,
                           	 bool noDelay )
                  : _delete( true ),
                    _host( "" ),
                    _port( 0 ),
                    _noDelay( noDelay )
{

  if ( gkg::SocketRegistry::find( path, _host, _port ) )
  {

    _fd = createSocket();

    if( _fd != -1 )
    {

      _server = new gkg::SocketStream(
                  new gkg::SocketBuffer( _fd, gkg::FStream::Out ) );
      getServer().setVerbose( fatal );
      getServer().connect();
      getServer().negotiate( binary );

    }
    else
    {

      _server = 0;

    }

  }
  else if ( fatal )
  {

    _server = 0;
    std::cerr << "gkg::SocketWriter::SocketWriter: service " << path
              << " not found" << "\n";
    std::cerr.flush();
    abort();

  }

}


gkg::SocketWriter::SocketWriter( const std::string& host, 
                                 in_port_t port,
                                 bool fatal,
                                 bool binary,
                                 bool noDelay )
                  : _delete( true ),
                    _host( host ),
                    _port( port ),
                    _noDelay( noDelay )
{

  try
  {

    _fd = createSocket();

    if( _fd != -1 )
    {

      _server = new gkg::SocketStream(
                    new gkg::SocketBuffer( _fd, gkg::FStream::Out ) );
      getServer().setVerbose( fatal );
      getServer().connect();
      getServer().negotiate( binary );

      if ( !getServer() && fatal )
      {

        abort();

      }

    }
    else
    {

      _server = 0;

    }

  }
  GKG_CATCH( "gkg::SocketWriter::SocketWriter( "
             "const std::string& host, int32_t port,"
             "bool fatal, bool binary, bool noDelay )" );

}


gkg::SocketWriter::SocketWriter( int32_t fd, bool fatal, 
                                 bool binary, bool noDelay )
                  : _delete( true ),
                    _host( "" ),
                    _noDelay( noDelay )
{

  _fd = fd;
  _server = new gkg::SocketStream(
                new gkg::SocketBuffer( _fd, gkg::FStream::Out ) );

  getServer().setVerbose( fatal );
  getServer().negotiate( binary );

  if ( !getServer() && fatal )
  {

    abort();

  }

}


gkg::SocketWriter::SocketWriter( gkg::SocketStream* server, bool noDelay )
                  : _server( server ),
                    _delete( false ),
                    _host( "" ),
                    _noDelay( noDelay )
{
}


// close the connection to the server, although the file number won't
// be closed if we attached the connection to it; free any storage
// allocated by SocketRegistry::find for the host name
gkg::SocketWriter::~SocketWriter()
{

  if ( _delete )
  {

    delete _server;

  }

}


// give this function public access so that programs can test this
// connection's state or attach a SocketReader to it to read as well
// as send socket requests over the same connection
gkg::SocketStream& gkg::SocketWriter::getServer()
{

  try
  {

    if( _server )
    {

      return *_server;

    }
    else
    {

      throw std::runtime_error( "getServer(): server is NULL" );

      // Program stopped before
      return *_server;

    }

  }
  GKG_CATCH( "gkg::SocketStream& gkg::SocketWriter::getServer()" );

}



bool gkg::SocketWriter::isConnected()
{

  if( _server )
  {

    return true;

  }
  else
  {

    return false;

  }

}


int32_t gkg::SocketWriter::createSocket()
{

  struct sockaddr_in name;
  memset( &name, 0x0, sizeof( name ) );
  name.sin_family = AF_INET;
  name.sin_port = htons( _port );

  const in_addr_t INVALIDADDR = ( in_addr_t )-1;
  in_addr_t hostinetaddr = INVALIDADDR;

  if ( isascii( _host[ 0 ] ) && isdigit( _host[ 0 ] ) )
  {

    hostinetaddr = inet_addr( _host.c_str() );
    name.sin_addr.s_addr = hostinetaddr;

  }

  if ( hostinetaddr == INVALIDADDR )
  {

    // cast to workaround bug in prototype on some systems
    struct hostent* hp = gethostbyname( ( char* )_host.c_str() );
    if ( !hp )
    {

      std::cerr << "gkg::SocketWriter::connect: gethostbyname: no such host"
                << "\n";
      std::cerr.flush();
      return 0;

    }
    if ( hp->h_addrtype != AF_INET )
    {

      std::cerr << 
      "gkg::SocketWriter::connect: gethostbyname: not an Internet host" << "\n";
      std::cerr.flush();
      return -1;

    }
    memcpy( &name.sin_addr, hp->h_addr, sizeof( name.sin_addr ) );

  }

  int32_t fd = -1;
  int32_t retries = 2;
  do
  {

    fd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( fd < 0 )
    {

      perror( "gkg::SocketWriter::connect: socket" );
      return -1;

    }
    // Set socket option SO_REUSEADDR to avoid the "Address already in use"
    // messages when trying to restart after a crash
    int32_t reuseAddr = 1;
    if ( setsockopt( fd, 
                     SOL_SOCKET,
                     SO_REUSEADDR,
                     ( char * )&reuseAddr,
                     sizeof( int32_t ) ) == -1 )
    {

      perror( "gkg::SocketWriter::connect: setsockopt( SO_REUSEADDR )" );
      return -1;

    }
    if ( _noDelay )
    {

      // Set TCP option to TCP_NODELAY
      // -> Disable nagle's algorithm (buffering of data before sending frame)
      // -> drawback : decrease efficiency of TCP/IP (increase overhead!)
      int32_t flag = 1;
      if ( setsockopt( fd,                         // socket affected
                       IPPROTO_TCP,                // set option at TCP level
                       TCP_NODELAY,                // name of option
                       ( char * )&flag,            // the cast is historical
                       sizeof( int32_t ) ) == -1 ) // length of option value
      {

        perror( "gkg::SocketWriter::connect: setsockopt( TCP_NODELAY )" );
        return -1;

      }

      // Set send buffer size to 0
      // Force rpc buffer to be sent immediately after flush
      size_t sendBufferSize = 0;
      if ( setsockopt( fd, 
                       SOL_SOCKET,
                       SO_SNDBUF,
                       ( char * )&sendBufferSize,
                       sizeof( size_t ) ) == -1 )
      {

        perror( "gkg::SocketWriter::connect: setsockopt( SO_SNDBUF )" );
        return -1;

      }

    }
    if ( ::connect( fd, ( struct sockaddr* )&name, sizeof( name ) ) < 0 )
    {

      if ( errno == ECONNREFUSED && retries > 0 )
      {

        // try again since peer's backlog may just be full
        ::close( fd );
        sleep( 1 );
        continue;

      }
      else
      {

        perror( "gkg::SocketWriter::connect: connect" );
        ::close( fd );
        return -1;

      }

    }
    break;  // the connection succeeded

  }
  while ( retries-- > 0 );

  return fd;

}
