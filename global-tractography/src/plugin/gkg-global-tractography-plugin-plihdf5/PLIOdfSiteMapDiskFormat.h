#ifndef _gkg_global_tractography_plugin_plihdf5_PLIOdfSiteMapDiskFormat_h_
#define _gkg_global_tractography_plugin_plihdf5_PLIOdfSiteMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class PLIOdfSiteMapDiskFormat :
                      public TypedDiskFormat< SiteMap< int32_t,
                                                       int32_t,
                                                       std::less< int32_t > > >,
                      public Singleton< PLIOdfSiteMapDiskFormat >
{

  public:

    ~PLIOdfSiteMapDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
                SiteMap< int32_t,
                         int32_t,
                         std::less< int32_t > >& object ) const;
    void write( const std::string& name,
                 SiteMap< int32_t,
                          int32_t,
                          std::less< int32_t > >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

  protected:

    friend class Singleton< PLIOdfSiteMapDiskFormat >;

    PLIOdfSiteMapDiskFormat();

};


}


#endif
