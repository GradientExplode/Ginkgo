#ifndef _gkg_win32_license_LicenseFormat_h_
#define _gkg_win32_license_LicenseFormat_h_


#ifdef WIN32
#pragma warning (disable : 4514)
#pragma warning (disable : 4710)
#endif


#include <gkg-core-cppext/StdInt.h>

#include <string>


namespace gkg
{


class LicenseFormat
{

  public:

    LicenseFormat();
    virtual ~LicenseFormat();

    std::string toString( uint64_t value );
    uint64_t fromString( std::string text );

};


}


#endif
