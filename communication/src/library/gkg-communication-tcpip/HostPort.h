#ifndef _gkg_communication_tcpip_HostPort_h_
#define _gkg_communication_tcpip_HostPort_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-cppext/NetInetIn.h>

#include <functional>
#include <string>


namespace gkg
{


struct HostPort
{

  HostPort( const std::string& host, in_port_t port );
  HostPort( const HostPort& other );
  
  std::string _host;
  in_port_t _port;

};


struct HostPortCompare /* : public std::binary_function< HostPort,
                                                      HostPort,
						      bool > deprecated in c++17
                       */
{

  typedef HostPort first_argument_type;
  typedef HostPort second_argument_type;
  typedef bool result_type;

  bool operator()( const HostPort& host1, const HostPort& host2 ) const;

};


}


#endif
