#ifndef _gkg_processing_io_Curve3dMeshDiskFormat_h_
#define _gkg_processing_io_Curve3dMeshDiskFormat_h_


#include <gkg-core-io/TypedDiskFormat.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T > class Curve3d;


template < class T >
class Curve3dMeshDiskFormat : public TypedDiskFormat< Curve3d< T > >,
                              public Singleton< Curve3dMeshDiskFormat< T > >
{

  public:

    ~Curve3dMeshDiskFormat();

    std::string getName() const;

    void writeHeader( const std::string& name,
                      const HeaderedObject& object ) const;

    void write( const std::string& name,
                Curve3d< T >& object,
                bool ascii ) const;

    bool hasWriter() const;

    std::string getHeaderExtension() const;

  protected:

    friend class Singleton< Curve3dMeshDiskFormat< T > >;

    Curve3dMeshDiskFormat();

};


}


#endif
