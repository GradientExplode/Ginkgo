#ifndef _gkg_processing_mesh_EvolvingMeshCache_h_
#define _gkg_processing_mesh_EvolvingMeshCache_h_

#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-mesh/EvolvingMesh.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Volume.h>
#include <set>
#include <string>


namespace gkg
{


class SceneModeler;


class EvolvingMeshCache
{

  public:

    EvolvingMeshCache( RCPointer< SceneModeler > sceneModeler,
                       const Vector3d< int32_t >& size );
    virtual ~EvolvingMeshCache();

    std::set< RCPointer< EvolvingMesh > >* 
      getEvolvingMeshes( const Vector3d< int32_t >& voxel ) const;
    std::set< RCPointer< EvolvingMesh > >* 
      getEvolvingMeshes( int32_t x, int32_t y, int32_t z ) const;

    std::set< RCPointer< EvolvingMesh > >*
      getEvolvingMeshes( const Vector3d< float >& point ) const;

    void update( RCPointer< EvolvingMesh > evolvingMesh );

    void getBinaryMask( Volume< uint8_t >& mask ) const;

  protected:

    RCPointer< SceneModeler > _sceneModeler;
    Volume< std::set< RCPointer< EvolvingMesh > >* > _lut;
    Vector3d< double > _resolution;

};


}


#endif
