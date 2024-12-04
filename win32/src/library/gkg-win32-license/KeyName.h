#ifndef _gkg_win32_license_KeyName_h_
#define _gkg_win32_license_KeyName_h_


#ifdef WIN32
#pragma warning (disable : 4786)
#pragma warning (disable : 4514)
#pragma warning (disable : 4710)
#pragma warning (disable : 4702)
#endif


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-cppext/StdInt.h>

#include <map>
#include <list>
#include <string>


namespace gkg
{


class KeyName : public Singleton< KeyName >
{

  public:

    uint64_t getId( std::string );
    
    std::list< std::string > getApplicationNames() const;

  private:

    friend class Singleton< KeyName >;

    KeyName();

    void initialize();

    std::map< std::string, uint64_t > _keys;

};


}


#endif
