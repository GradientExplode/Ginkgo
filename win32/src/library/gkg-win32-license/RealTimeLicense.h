#ifndef _gkg_realtime_mri_license_RealTimeLicense_h_
#define _gkg_realtime_mri_license_RealTimeLicense_h_


#include <gkg-win32-license/LicenseCenter.h>
#include <gkg-core-pattern/Singleton.h>

#include <string>
#include <set>


namespace gkg
{


class RealTimeLicense : public Singleton< RealTimeLicense >
{

  public:
  
    bool connect( std::string file, std::set< std::string > ipAddresses );
    
    bool hasT1Sequence();
    bool hasDMRISequence();
    bool hasFMRISequence();
    
  private:
  
    friend class Singleton< RealTimeLicense >;
    
    RealTimeLicense();
    
    bool _connected;
    LicenseCenter _licenseCenter;

};


}


#endif
