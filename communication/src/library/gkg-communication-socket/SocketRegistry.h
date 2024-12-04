#ifndef _gkg_communication_socket_SocketRegistry_h_
#define _gkg_communication_socket_SocketRegistry_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-cppext/NetInetIn.h>
#include <string>


namespace gkg
{


// store or retrieve a socket service's hostname and port number so that
// clients can contact the socket service

struct SocketRegistry
{

  static bool record( const std::string& path, in_port_t port );
  static bool erase( const std::string& path );
  static bool find( const std::string& path, 
                    std::string& host, 
                    in_port_t& port );

};


}


#endif
