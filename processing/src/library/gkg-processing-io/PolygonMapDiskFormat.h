#ifndef _gkg_processing_io_PolygonMapDiskFormat_h_
#define _gkg_processing_io_PolygonMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < uint32_t  D > class PolygonMap;


template < uint32_t D >
class PolygonMapDiskFormat : public TypedDiskFormat< PolygonMap< D > >,
                             public Singleton< PolygonMapDiskFormat< D > >
{

  public:

    ~PolygonMapDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               PolygonMap< D >& object ) const;
    void write( const std::string& name,
                PolygonMap< D >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< PolygonMapDiskFormat< D > >;

    PolygonMapDiskFormat();

};


}


#endif
