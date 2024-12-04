#include <gkg-communication-socket/SocketRegistry.h>
#include <gkg-communication-tcpip/Host.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <cerrno>

#ifndef __DECCXX

#include <cstdlib>
#include <fcntl.h>
#include <gkg-core-cppext/CUniStd.h>
#include <time.h>
#include <sys/types.h>

#ifdef _G_HAVE_SYS_SOCKET

#include <sys/socket.h>

#endif

#ifdef _G_HAVE_SYS_RESOURCE

#include <sys/time.h>
#include <sys/resource.h>

#endif

#endif

#include <cstddef>
#include <cstring>
#include <sys/param.h>

#if defined(sun) && defined(__svr4__)

#include <sys/utsname.h>
#define MAXHOSTNAMELEN SYS_NMLN

#endif

#ifdef __linux__

#include <cstdio>

#endif


// print a short error message describing the last error encountered
// during a call to a system function

static std::ostream& perror( std::ostream& s )
{

#if defined(sun) || defined( __GNUC__ )

  s << ": " << ::strerror( errno );

#else

  if ( ( errno > 0 ) && ( errno < sys_nerr ) )
  {

    s << ": " << sys_errlist[errno];

  }

#endif

  s << '\n';
  return s;

}


// record the socket service's hostname and port number in the given
// file; remove the file before opening it in case it already exists
// and the user has permission to delete it but not permission to
// write data into it
bool gkg::SocketRegistry::record( const std::string& path, in_port_t port )
{

  if ( path.empty() )
  {

    return false;

  }

  unlink( path.c_str() );

  std::ofstream registry( path.c_str() );
  if ( !registry )
  {

    std::cerr << "gkg::SocketRegistry::record: open("
              << path << ")" << perror;
    return false;

  }

  registry << gkg::Host::getName() << std::ends << port;
  if ( !registry )
  {

    std::cerr << "gkg::SocketRegistry::record: write" << perror;
    return false;

  }
  return true;

}


// remove the file which stores the socket service's hostname and port number
// so that no more clients will be able to contact the socket service
bool gkg::SocketRegistry::erase( const std::string& path )
{

  if ( path.empty() )
  {

    return false;

  }

  if ( unlink( path.c_str() ) < 0 )
  {

    std::cerr << "gkg::SocketRegistry::erase: unlink("
              << path << ")" << perror;
    return false;

  }
  return true;

}


// open the file which stores the socket service's hostname and port
// number; if the file does not exist, return failure silently; if
// the file does exist, read the socket service's hostname and port
// number from it; if the socket service and client are on the same
// host, speed up I/O between them by returning "localhost" instead of
// the host's true name
bool gkg::SocketRegistry::find( const std::string& path,
                                std::string& hostname,
                                in_port_t& port )
{

  if ( path.empty() )
  {

    return false;

  }

  std::ifstream registry( path.c_str() );
  if ( !registry )
  {

    return false;

  }

  char charHostname[ MAXHOSTNAMELEN ];

  registry.getline( charHostname, MAXHOSTNAMELEN, '\0' );
  hostname = ( std::string )charHostname;
  registry >> port;
  if ( !registry )
  {

    std::cerr << "gkg::SocketRegistry::find: error reading " << path << '\n';
    return false;

  }
  if ( strcmp( hostname.c_str(), gkg::Host::getName().c_str() ) == 0 )
  {

    hostname = "localhost";

  }
  return true;

}
