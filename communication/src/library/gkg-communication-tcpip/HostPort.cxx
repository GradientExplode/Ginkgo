#include <gkg-communication-tcpip/HostPort.h>


gkg::HostPort::HostPort( const std::string& host, in_port_t port )
             : _host( host ),
	       _port( port )
{
}


gkg::HostPort::HostPort( const gkg::HostPort& other )
             : _host( other._host ),
	       _port( other._port )
	        
{
}


bool gkg::HostPortCompare::operator()( const gkg::HostPort& host1, 
                                       const gkg::HostPort& host2 ) const
{

  if ( host1._host < host2._host )
  {
  
    return true;
    
  }
  else if ( host1._port < host2._port )
  {
  
    return true;
    
  }

  return false;

}
