#ifndef _gkg_processing_io_AimsBucketDiskFormat_h_
#define _gkg_processing_io_AimsBucketDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class R, class S, class Compare > class SiteMap;


class AimsBucketDiskFormat : public TypedDiskFormat< SiteMap<
                                                       int32_t, int32_t,
                                                       std::less< int32_t > > >,
                             public Singleton< AimsBucketDiskFormat >
{

  public:

    ~AimsBucketDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void read( const std::string& name,
               SiteMap< int32_t, int32_t,
                        std::less< int32_t > >& object ) const;

    bool hasReader() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< AimsBucketDiskFormat >;

    AimsBucketDiskFormat();

};


}


#endif
