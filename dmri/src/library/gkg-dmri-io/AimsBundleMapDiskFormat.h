#ifndef _gkg_dmri_io_AimsBundleMapDiskFormat_h_
#define _gkg_dmri_io_AimsBundleMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class AimsBundleMapDiskFormat :
                             public TypedDiskFormat< BundleMap< std::string > >,
                             public Singleton< AimsBundleMapDiskFormat >
{

  public:

    ~AimsBundleMapDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               BundleMap< std::string >& object ) const;
    void write( const std::string& name,
                BundleMap< std::string >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< AimsBundleMapDiskFormat >;

    AimsBundleMapDiskFormat();

};


}


#endif
