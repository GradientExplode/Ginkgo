#ifndef _gkg_win32_license_LicenseCenter_h_
#define _gkg_win32_license_LicenseCenter_h_


#ifdef WIN32
#pragma warning (disable : 4786)
#endif


#include <gkg-win32-license/LicenseEncoder.h>
#include <gkg-core-cppext/StdInt.h>

#include <set>



namespace gkg
{


class LicenseCenter
{

  public:
  
    LicenseCenter();
    virtual ~LicenseCenter();

    bool connect( std::string file, uint64_t hardwareInformation );

    bool hasValidKeys();
    bool hasApplication( uint64_t applicationId );

  protected:
    
    bool initialize();
    void registerLicense( std::string strLicense );

    std::string _file;
    uint64_t _hardwareInformation;

    std::set< uint64_t > _applications;
    
    LicenseEncoder _licenseEncoder;

};


}


#endif
