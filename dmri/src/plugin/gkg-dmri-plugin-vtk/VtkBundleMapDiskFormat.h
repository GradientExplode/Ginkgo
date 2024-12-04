#ifndef _gkg_dmri_plugin_vtk_VtkBundleMapDiskFormat_h_
#define _gkg_dmri_plugin_vtk_VtkBundleMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class VtkBundleMapDiskFormat :
                             public TypedDiskFormat< BundleMap< L, Compare > >,
                             public Singleton< VtkBundleMapDiskFormat< L, Compare > >
{

  public:

    ~VtkBundleMapDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               BundleMap< L, Compare >& object ) const;
    void write( const std::string& name,
                BundleMap< L, Compare >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

  protected:

    friend class Singleton< VtkBundleMapDiskFormat< L, Compare > >;

    VtkBundleMapDiskFormat();

};


}


#endif
