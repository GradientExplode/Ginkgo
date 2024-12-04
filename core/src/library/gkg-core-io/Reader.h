#ifndef _gkg_core_io_Reader_h_
#define _gkg_core_io_Reader_h_


#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class HeaderedObject;
class GenericObject;
class RegionInformation;


class Reader : public Singleton< Reader >
{

  public:

    virtual ~Reader();

    template < class O >
    std::string readHeader( const std::string& name,
                     O& object,
                     const std::string& format = "" ) const;

    template < class O >
    std::string read( const std::string& name,
                      O& object,
                      const std::string& format = "" ) const;

    template < class O >
    std::string 
      preparePartialBinaryRead( const std::string& name,
                                HeaderedObject& proxy,
                                const std::string& format = "" ) const;
    template < class O >
    std::string partialBinaryRead( const HeaderedObject& proxy,
                                   GenericObject& subpart,
                                   const RegionInformation& regionInformation,
                                   const std::string& format = "" ) const;

  private:

    friend class Singleton< Reader >;

    Reader();

};


}


#endif
