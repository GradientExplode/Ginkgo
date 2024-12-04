#ifndef _gkg_processing_io_MeshMapDiskFormat_h_
#define _gkg_processing_io_MeshMapDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare > class MeshMap;


template < class R, class S,  uint32_t D, class Compare = std::less< R > >
class MeshMapDiskFormat :
                       public TypedDiskFormat< MeshMap< R, S, D, Compare > >,
                       public Singleton< MeshMapDiskFormat< R, S, D, Compare > >
{

  public:

    ~MeshMapDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               MeshMap< R, S, D, Compare >& object ) const;
    void write( const std::string& name,
                MeshMap< R, S, D, Compare >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< MeshMapDiskFormat< R, S, D, Compare > >;

    MeshMapDiskFormat();

};


}


#endif
