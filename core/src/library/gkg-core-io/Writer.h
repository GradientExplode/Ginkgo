#ifndef _gkg_core_io_Writer_h_
#define _gkg_core_io_Writer_h_


#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class HeaderedObject;
class GenericObject;
class RegionInformation;


class Writer : public Singleton< Writer >
{

  public:

    virtual ~Writer();

    template < class O >
    std::string writeHeader( const std::string& name,
                             O& object,
                             const std::string& format = "" ) const;

    template < class O >
    std::string write( const std::string& name,
                       O& object,
                       bool ascii = false,
                       const std::string& format = "" ) const;

    template < class O >
    std::string 
      preparePartialBinaryWrite( const std::string& name,
                                 HeaderedObject& proxy,
                                 const std::string& format = "" ) const;
    template < class O >
    std::string partialBinaryWrite( const HeaderedObject& proxy,
                                    const GenericObject& subpart,
                                    const RegionInformation& regionInformation,
                                    const std::string& format = "" ) const;

  private:

    friend class Singleton< Writer >;

    Writer();

};


}


#endif
