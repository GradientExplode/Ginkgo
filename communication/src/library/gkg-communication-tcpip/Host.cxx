#include <gkg-communication-tcpip/Host.h>
#include <gkg-core-cppext/CUniStd.h>


std::string gkg::Host::_name = "\0";


const std::string& gkg::Host::getName()
{

  if ( _name[ 0 ] == '\0' )
  {

    char name[ 100 ];
    gethostname( name, sizeof( name ) );
    _name = ( std::string )name;

  }
  return _name;

}
