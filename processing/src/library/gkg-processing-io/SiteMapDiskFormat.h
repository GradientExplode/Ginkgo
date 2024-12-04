#ifndef _gkg_processing_io_SiteMapDiskFormat_h_
#define _gkg_processing_io_SiteMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class R, class S, class Compare > class SiteMap;


template < class R, class S, class Compare = std::less< R > >
class SiteMapDiskFormat : public TypedDiskFormat< SiteMap< R, S, Compare > >,
                          public Singleton< SiteMapDiskFormat< R, S, Compare > >
{

  public:

    ~SiteMapDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               SiteMap< R, S, Compare >& object ) const;
    void write( const std::string& name,
                SiteMap< R, S, Compare >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< SiteMapDiskFormat< R, S, Compare > >;

    SiteMapDiskFormat();

};


}


#endif
