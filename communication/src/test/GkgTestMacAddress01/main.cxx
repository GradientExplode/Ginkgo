#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <map>
#include <set>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::map< std::string, uint64_t > macAddresses =
                               gkg::SystemInfo::getInstance().getMacAddresses();
    std::set< std::string > ipAddresses=
                                gkg::SystemInfo::getInstance().getIpAddresses();

    std::map< std::string, uint64_t >::const_iterator
      m = macAddresses.begin(),
      me = macAddresses.end();

    std::cout << "Mac addresses: " << std::endl;
    while ( m != me )
    {

      std::cout << "\t" << std::hex << std::setfill( '0' ) << std::setw( 2 )
                << m->second << std::endl;
      ++m;

    }

    std::set< std::string >::const_iterator
      i = ipAddresses.begin(),
      ie = ipAddresses.end();

    std::cout << "IP addresses: " << std::endl;
    while ( i != ie )
    {

      std::cout << "\t" << *i << std::endl;
      ++i;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
