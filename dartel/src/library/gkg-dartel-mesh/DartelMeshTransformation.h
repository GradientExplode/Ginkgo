#ifndef _gkg_dartel_mesh_DartelMeshTransformation_h_
#define _gkg_dartel_mesh_DartelMeshTransformation_h_


#include <gkg-dartel-core/DartelTransformation.h>
#include <gkg-processing-container/MeshMap.h>


namespace gkg
{


template < class L, class S >
class DartelMeshTransformation : public DartelTransformation< S, float >
{

  public:

    DartelMeshTransformation( int32_t k );
    virtual ~DartelMeshTransformation();

    void wrap( const MeshMap< L, float, 3U >& inMeshMap,
               const Volume< S >& inVolumeProxy,
               const Volume< float >& flowField, 
               MeshMap< L, float, 3U >& outMeshMap );
    void unwrap( const MeshMap< L, float, 3U >& inMeshMap,
                 const Volume< S >& inVolumeProxy,
                 const Volume< float >& flowField, 
                 const Volume< S >& templateProxy,
                 MeshMap< L, float, 3U >& outMeshMap );

};


}


#endif
