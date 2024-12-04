#ifndef _gkg_win32_sysinfo_MacAddress_h_
#define _gkg_win32_sysinfo_MacAddress_h_


#ifdef WIN32
#pragma warning (disable : 4786)
#pragma warning (disable : 4514)
#pragma warning (disable : 4710)
#endif


#include <gkg-core-cppext/StdInt.h>

#include <string>
#include <map>
#include <set>


namespace gkg
{


class MacAddress
{

  public:

    MacAddress();
    virtual ~MacAddress();

    uint64_t getMacAddress( std::string ip );
    std::set< std::string > getIpAddresses();

  protected:

    void scanInterfaces();

    std::map< std::string, uint64_t > _macAddress;

};


}


#endif
