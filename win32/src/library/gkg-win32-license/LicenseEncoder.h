#ifndef _gkg_win32_license_LicenseEncoder_h_
#define _gkg_win32_license_LicenseEncoder_h_


#ifdef WIN32
#pragma warning (disable : 4786)
#pragma warning (disable : 4514)
#pragma warning (disable : 4710)
#endif


#include <gkg-core-cppext/StdInt.h>

#include <string>


namespace gkg
{


class LicenseEncoder
{

  public:

    LicenseEncoder();
    virtual ~LicenseEncoder();

    uint64_t keyEncode( uint64_t key );
    uint64_t keyDecode( uint64_t key );

    std::string getProtectionCode( uint64_t key, uint64_t applicationId );
    bool checkLicense( uint64_t applicationId, 
                       std::string strLicense,
                       uint64_t hardwareInformation );
    uint64_t getApplicationId( uint64_t license, uint64_t hardwareInformation );

  private:

    uint64_t encode( uint64_t hid, uint64_t license );
    uint64_t decode( std::string strLicense );
    uint64_t decode( uint64_t license );

    uint64_t _mask0;
    uint64_t _mask1;
    uint64_t _mask2;
    uint64_t _mask3;

};


}


#endif
