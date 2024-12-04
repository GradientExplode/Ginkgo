#ifndef _gkg_processing_mesh_PolygonCache_h_
#define _gkg_processing_mesh_PolygonCache_h_

#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Volume.h>
#include <set>


namespace gkg
{


class EvolvingMesh;
template < uint32_t D > class Polygon;
template < uint32_t D > class RCPolygonCompare;


class PolygonCache
{

  public:

    // if radius of influence is greater than zero, then a same polygon may
    // belong to several voxels (this region of influence was created to take
    // into account the dynamic motion of the vertices, and replace it by
    // a static information of the vertices + the size of the region of 
    // influence of the motion of a given vertex (typically close to the maximum
    // motion magnitude
    PolygonCache( RCPointer< EvolvingMesh > evolvingMesh,
                  const Vector3d< int32_t >& size,
                  float radiusOfInfluence = 0.0 );
    virtual ~PolygonCache();

    std::set< const Polygon< 3U >* >* 
      getPolygons( const Vector3d< int32_t >& voxel ) const;
    std::set< const Polygon< 3U >* >*
      getPolygons( const Vector3d< float >& point ) const;

    void getBinaryMask( Volume< uint8_t >& mask ) const;

  protected:

    RCPointer< EvolvingMesh > _evolvingMesh;
    Volume< std::set< const Polygon< 3U >* >* > _lut;
    Vector3d< float > _resolution;

};


}


#endif
