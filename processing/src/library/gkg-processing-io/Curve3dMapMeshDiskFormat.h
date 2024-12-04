#ifndef _gkg_processing_io_Curve3dMapMeshDiskFormat_h_
#define _gkg_processing_io_Curve3dMapMeshDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T > class Curve3dMap;


template < class T >
class Curve3dMapMeshDiskFormat : public TypedDiskFormat< Curve3dMap< T > >,
                                 public Singleton<
                                                 Curve3dMapMeshDiskFormat< T > >
{

  public:

    ~Curve3dMapMeshDiskFormat();

    std::string getName() const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void write( const std::string& name,
                Curve3dMap< T >& object,
                bool ascii ) const;

    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< Curve3dMapMeshDiskFormat< T > >;

    Curve3dMapMeshDiskFormat();

};


}


#endif
