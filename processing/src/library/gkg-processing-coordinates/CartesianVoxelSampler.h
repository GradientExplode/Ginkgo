#ifndef _gkg_processing_coordinates_CartesianVoxelSampler_h_
#define _gkg_processing_coordinates_CartesianVoxelSampler_h_


#include <gkg-processing-coordinates/VoxelSampler.h>
#include <vector>


namespace gkg
{

//
// this class create a set of offset point(s) inside the voxel in order
// to get a uniform cartesian sampling of point(s); the resulting point count
// is not necessarily the argument provided to the constructor, because
// the product pointCountX * pointCountY * pointCountZ is chosen to give
// the minimum difference to the prescribed value
//

template < class T >
class CartesianVoxelSampler : public VoxelSampler< T >
{

  public:

    CartesianVoxelSampler( const Vector3d< double >& resolution,
                           int32_t pointCount );
    virtual ~CartesianVoxelSampler();

    void getPoint( const Vector3d< T >& origin,
                   int32_t pointIndex,
                   Vector3d< T >& destination ) const;

  protected:

    std::vector< gkg::Vector3d< T > > _offsets;


};


}


#endif
