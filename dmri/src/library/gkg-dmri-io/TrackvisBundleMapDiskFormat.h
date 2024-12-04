#ifndef _gkg_dmri_io_TrakcvisBundleMapDiskFormat_h_
#define _gkg_dmri_io_TrakcvisBundleMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class TrackvisBundleMapDiskFormat :
                   public TypedDiskFormat< BundleMap< L, Compare > >,
                   public Singleton< TrackvisBundleMapDiskFormat< L, Compare > >
{

  public:

    ~TrackvisBundleMapDiskFormat();

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

    friend class Singleton< TrackvisBundleMapDiskFormat< L, Compare > >;

    TrackvisBundleMapDiskFormat();

};


}


#endif
