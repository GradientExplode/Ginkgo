#ifndef _gkg_global_tractography_plugin_plihdf5_PLIOdfTextureMapDiskFormat_h_
#define _gkg_global_tractography_plugin_plihdf5_PLIOdfTextureMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class PLIOdfTextureMapDiskFormat :
                 public TypedDiskFormat< TextureMap<
                                            OrientationDistributionFunction > >,
                 public Singleton< PLIOdfTextureMapDiskFormat >
{

  public:

    ~PLIOdfTextureMapDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               TextureMap< OrientationDistributionFunction >& object ) const;
    void write( const std::string& name,
                TextureMap< OrientationDistributionFunction >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

  protected:

    friend class Singleton< PLIOdfTextureMapDiskFormat >;

    PLIOdfTextureMapDiskFormat();

};


}


#endif
