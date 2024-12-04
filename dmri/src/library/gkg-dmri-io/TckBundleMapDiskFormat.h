#ifndef _gkg_dmri_io_TckBundleMapDiskFormat_h_
#define _gkg_dmri_io_TckBundleMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class TckBundleMapDiskFormat :
                             public TypedDiskFormat< BundleMap< std::string > >,
                             public Singleton< TckBundleMapDiskFormat >
{

  public:

    ~TckBundleMapDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void read( const std::string& name,
               BundleMap< std::string >& object ) const;

    bool hasReader() const;

  protected:

    friend class Singleton< TckBundleMapDiskFormat >;

    TckBundleMapDiskFormat();

};


}


#endif
