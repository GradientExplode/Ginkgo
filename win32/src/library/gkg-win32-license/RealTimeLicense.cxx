#include <gkg-win32-license/RealTimeLicense.h>
#include <gkg-win32-sysinfo/MacAddress.h>
#include <gkg-win32-license/KeyNameDef.h>
#include <gkg-core-cppext/StdInt.h>


gkg::RealTimeLicense::RealTimeLicense()
                    : gkg::Singleton< gkg::RealTimeLicense >(),
                      _connected( false )
{
}


bool gkg::RealTimeLicense::connect( std::string file,
                                    std::set< std::string > ipAddresses )
{

  bool found = false;
  gkg::MacAddress macAddress;
  std::set< std::string >::iterator
    i = ipAddresses.begin(),
    ie = ipAddresses.end();

  while ( !found && ( i != ie ) )
  {

    uint64_t macAddr = macAddress.getMacAddress( *i );
    _connected = _licenseCenter.connect( file, macAddr );

    if ( _connected && _licenseCenter.hasValidKeys() )
    {

      found = true;

    }

    ++ i;

  }

  return found;

}


bool gkg::RealTimeLicense::hasT1Sequence()
{

  bool status = false;
  
  if ( _connected )
  {
  
    status = _licenseCenter.hasApplication( RT_T1_SEQUENCE );
  
  }
  
  return status;
}


bool gkg::RealTimeLicense::hasDMRISequence()
{

  bool status = false;
  
  if ( _connected )
  {
  
    status = _licenseCenter.hasApplication( RT_DMRI_SEQUENCE );
  
  }
  
  return status;
}


bool gkg::RealTimeLicense::hasFMRISequence()
{

  bool status = false;
  
  if ( _connected )
  {
  
    status = _licenseCenter.hasApplication( RT_FMRI_SEQUENCE );
  
  }
  
  return status;
}
