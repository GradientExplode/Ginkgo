#include <gkg-communication-socket/SocketPeer.h>
#include <gkg-communication-dispatcher/Dispatcher.h>
#include <gkg-communication-socket/SocketBuffer.h>
#include <gkg-communication-socket/SocketRegistry.h>
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


// store our parameters for use later since we can't call a derived
// class's virtual function from a base constructor.
gkg::SocketPeer::SocketPeer( const std::string& path, in_port_t port )
             	: gkg::IOHandler(),
             	  _path( path ),
             	  _port( port ),
             	  _fd( -1 ),
             	  _buffer( 0 ),
             	  _running( false ),
             	  _remote( false ),
             	  _host( "" )
{
}


// delete our socket service if we created one; free any storage
// allocated by gkg::SocketRegistry::find for the remote service's host name.
gkg::SocketPeer::~SocketPeer()
{

  if ( _buffer )
  {

    stopListening();

  }

}


// read socket requests until something tells us to quit the run loop.
// If you're using the gkg event-reading code, you don't have
// to use this code since that code will also read socket requests.
void gkg::SocketPeer::run( timeval* timeout )
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


void gkg::SocketPeer::quitRunning()
{

  _running = false;

}


// find the host name and port number registered by the remote socket
// service and try to open a connection to it; unregister the remote
// socket service to prevent another attempt if the connection could not
// be opened; if necessary, start up our own socket service and wait for
// the remote socket service to contact us.
void gkg::SocketPeer::initialize( const std::string& path )
{

  if ( _buffer || _remote )
  {

    return;

  }

  in_port_t port;
  _remote = gkg::SocketRegistry::find( path, _host, port );

  if ( _remote && !createReaderAndWriter( _host, port ) )
  {

    gkg::SocketRegistry::erase( path );
    _remote = false;

  }

  if ( !_remote )
  {

    _fd = createSocket();
    _buffer = new gkg::SocketBuffer( _fd, gkg::FStream::In );
    startListening();

  }

}


int32_t gkg::SocketPeer::createSocket()
{

  struct sockaddr_in name;
  memset( &name, 0, sizeof( name ) );
  name.sin_family = AF_INET;
  name.sin_port = htons( _port );
  name.sin_addr.s_addr = htonl( INADDR_ANY );

  int32_t fd = socket( AF_INET, SOCK_STREAM, 0 );
  if ( fd < 0 )
  {

    perror( "gkg::SocketPeer::createSocket: socket" );
    return -1;

  }

  if ( bind( fd, ( struct sockaddr* )&name, sizeof( name ) ) < 0 )
  {

    perror( "gkg::SocketPeer::createSocket: bind" );
    ::close( fd );
    return -1;

  }

  if ( ::listen( fd, SOMAXCONN ) < 0 )
  {

    perror( "gkg::SocketPeer::listen: listen" );
    ::close( fd );
    return 0;

  }

  return fd;

}


// open an socket service at a port number (zero means any available port
// number); store our socket service's assigned port number in a file to
// allow clients to find our service; start listening for connections
// from clients.
void gkg::SocketPeer::startListening()
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
// service, and delete it.
void gkg::SocketPeer::stopListening()
{

  gkg::Dispatcher::getInstance().unlink( _buffer->getFd() );
  if ( !_path.empty() )
  {

    gkg::SocketRegistry::erase( _path );

  }
  _buffer->close();
  delete _buffer;
  _buffer = 0;

}

// accept a connection from a client; ask a derived class to attach a
// reader and a writer which will read socket requests and send socket
// requests over the same connection.

int32_t gkg::SocketPeer::isInputReady( int32_t )
{

  int32_t fd;
  gkg::SocketBuffer* ok = _buffer->accept( fd );
  if ( !ok )
  {

    abort();

  }
  createReaderAndWriter( fd );
  return 0;

}
