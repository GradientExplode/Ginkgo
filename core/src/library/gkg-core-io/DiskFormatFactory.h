#ifndef _gkg_core_io_DiskFormatFactory_h_
#define _gkg_core_io_DiskFormatFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <map>
#include <string>


namespace gkg
{


template < class O > class TypedDiskFormat;

template < class O >
class DiskFormatFactory : public Singleton< DiskFormatFactory< O > >
{

  public:

    virtual ~DiskFormatFactory();

    void registerDiskFormat( const std::string& name,
                             TypedDiskFormat< O >* typedDiskFormat );

    const std::map< std::string, TypedDiskFormat< O >* >&
      getDiskFormats() const;

    TypedDiskFormat< O >* getDiskFormat( const std::string& name ) const;

  private:

    friend class Singleton< DiskFormatFactory< O > >;

    DiskFormatFactory();

    std::map< std::string, TypedDiskFormat< O >* > _diskFormats;

};


}


#endif
