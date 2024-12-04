#include <gkg-communication-dispatcher/Dispatcher.h>
#include <gkg-communication-socket/SocketBuffer.h>
#include <gkg-communication-socket/SocketRegistry.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-core-exception/Exception.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <arpa/inet.h>


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


// start up an socket service; if a filename is given, store our
// service's assigned port number in it so clients can find us
gkg::SocketService::SocketService( in_port_t port )
                   : gkg::IOHandler(),
                     _path( "" ),
                     _port( port ),
                     _running( false )
{

  try
  {

    startSocket( port );

  }
  GKG_CATCH( "gkg::SocketService::SocketService( in_port_t port )" );

}


gkg::SocketService::SocketService( const std::string& path, in_port_t port )
                   : gkg::IOHandler(),
                     _path( path ),
                     _port( port ),
                     _running( false )
{

  try
  {

    startSocket( port );

  }
  GKG_CATCH( "gkg::SocketService::SocketService( "
             "const std::string& path, int32_t port )" );

}


// close our socket service if it was open
gkg::SocketService::~SocketService()
{

  stopSocket();

}


void gkg::SocketService::startSocket( in_port_t port )
{

  try
  {

    if ( port > 0 )
    {

      _port = port;

    }

    _fd = createSocket();

    if ( _fd != -1 )
    {

      _buffer = new gkg::SocketBuffer( _fd, gkg::FStream::In );
      startListening();

    }
    else
    {

      throw std::runtime_error( "bad file descriptor" );

    }

  }
  GKG_CATCH( "void gkg::SocketService::startSocket( in_port_t port )" );
}


void gkg::SocketService::stopSocket()
{

  try
  {

    if ( _buffer->isOpened() )
    {

      stopListening();

    }

    if( _buffer )
    {

      delete _buffer;
      _buffer = NULL;

    }

    if ( _fd >= 0 )
    {

      ::close( _fd );

    }

  }
  GKG_CATCH( "void gkg::SocketService::stopSocket()" );

}


void gkg::SocketService::restartSocket( in_port_t port )
{

  try
  {

    stopSocket();
    startSocket( port );

  }
  GKG_CATCH( "void gkg::SocketService::restartSocket( in_port_t port )" );

}


// read socket requests until something tells us to quit the run loop;
// if you're using the InterViews event-reading code, you don't have
// to use this code since that code will also read socket requests
void gkg::SocketService::run( timeval* timeout )
{

  _running = true;

  if ( timeout )
  {
  
    int64_t sec = timeout->tv_sec;
    int64_t usec = timeout->tv_usec;

    while ( _running && ( ( sec > 0 ) || ( usec > 0 ) ) )
    {

      gkg::Dispatcher::getInstance().dispatch( sec, usec );

    }

  }
  else
  {

    while ( _running )
    {

      gkg::Dispatcher::getInstance().dispatch();

    }

  }

}


void gkg::SocketService::quitRunning()
{

  _running = false;

}


int32_t gkg::SocketService::createSocket()
{

  try
  {

    struct sockaddr_in name;
    memset( &name, 0, sizeof( name ) );
    name.sin_family = AF_INET;
    name.sin_port = htons( _port );
    name.sin_addr.s_addr = htonl( INADDR_ANY );

    int32_t fd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( fd < 0 )
    {

      perror( "gkg::SocketService::createSocket: socket" );
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

      perror( "gkg::SocketService::createSocket: setsockopt( SO_REUSEADDR )" );
      return -1;

    }

    if ( bind( fd, ( struct sockaddr* )&name, sizeof( name ) ) < 0 )
    {

      perror( "gkg::SocketService::createSocket: bind" );
      ::close( fd );
      return -1;

    }

    if ( ::listen( fd, SOMAXCONN ) < 0 )
    {

      perror( "gkg::SocketService::listen: listen" );
      ::close( fd );
      return -1;

    }

    return fd;

  }
  GKG_CATCH( "int32_t gkg::SocketService::createSocket()" );

}


// open an socket service at a port number (zero means any available port
// number); if a filename was given, store our service's assigned
// port number in it to allow clients to find us; start listening for
// connections from clients
void gkg::SocketService::startListening()
{

  gkg::SocketBuffer* ok = _buffer->listen();
  if ( !ok )
  {

    abort();

  }
  if ( !_path.empty() && !gkg::SocketRegistry::record( _path, _port ) )
  {

    abort();

  }
  gkg::Dispatcher::getInstance().link( _fd, gkg::Dispatcher::ReadMask, this );

}


// stop listening for connections from clients, unregister our socket
// service if it was registered, and close it
void gkg::SocketService::stopListening()
{

  gkg::Dispatcher::getInstance().unlink( _fd );
  if ( !_path.empty() )
  {

    gkg::SocketRegistry::erase( _path );

  }
  _buffer->close();

}


// accept a connection from a client; ask a derived class to attach a
// reader which will read socket requests from the connection
int32_t gkg::SocketService::isInputReady( int32_t ) 
{

  int32_t fd;
  gkg::SocketBuffer* ok = _buffer->accept( fd );
  if ( !ok )
  {

    abort();

  }
  createReader( fd );
  return 0;

}
