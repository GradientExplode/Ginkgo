#ifndef _gkg_processing_mesh_MeshDeformation_h_
#define _gkg_processing_mesh_MeshDeformation_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-cppext/StdInt.h>

#include <vector>
#include <list>


namespace gkg
{


template < class T >
class MeshDeformation
{

  public:

    MeshDeformation( int32_t deformationCount = 0,
                     int32_t normalCount = 0 );
    MeshDeformation( const MeshDeformation< T >& other );
    virtual ~MeshDeformation();

    void resize( int32_t deformationCount,
                 int32_t normalCount );

    void addNormals( const std::list< Vector3d< T > >& normals );

    std::vector< T >& getDeformations();
    std::list< Vector3d< T > >& getNormals();

    const std::vector< T >& getDeformations() const;
    const std::list< Vector3d< T > >& getNormals() const;

  private:

    std::vector< T > _deformations;
    std::list< Vector3d< T > > _normals;

};


}


#endif
