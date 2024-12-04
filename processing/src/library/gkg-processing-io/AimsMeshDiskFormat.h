#ifndef _gkg_processing_io_AimsMeshDiskFormat_h_
#define _gkg_processing_io_AimsMeshDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-container/MeshMap.h>


namespace gkg
{


template < uint32_t D >
class AimsMeshDiskFormat :
                       public TypedDiskFormat< MeshMap< int32_t, float, D > >,
                       public Singleton< AimsMeshDiskFormat< D > >
{

  public:

    ~AimsMeshDiskFormat();

    std::string getName() const;

    void readHeader( const std::string& name,
                     HeaderedObject& object ) const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void read( const std::string& name,
               MeshMap< int32_t, float, D >& object ) const;
    void write( const std::string& name,
                MeshMap< int32_t, float, D >& object,
                bool ascii ) const;

    bool hasReader() const;
    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< AimsMeshDiskFormat< D > >;

    AimsMeshDiskFormat();

};


}


#endif
