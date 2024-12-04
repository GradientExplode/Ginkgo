#ifndef _gkg_processing_mesh_BresenhamAlgorithm_h_
#define _gkg_processing_mesh_BresenhamAlgorithm_h_

#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <list>
#include <vector>
#include <set>


namespace gkg
{


class BresenhamAlgorithm
{

  public:

    BresenhamAlgorithm( const BoundingBox< float >& boundingBox,
                        const Vector3d< int32_t >& cacheSize );
    virtual ~BresenhamAlgorithm();

    void getCacheVoxel( const Vector3d< float >& point,
                        Vector3d< int32_t >& voxel ) const;
    void getRealWorldPoint( const Vector3d< int32_t >& voxel,
                            Vector3d< float >& point ) const;

    void getNeighbors( const Vector3d< int32_t >& voxel,
                       Vector3d< int32_t > stride,
                       std::vector< Vector3d< int32_t > >& neighbors,
                       std::vector< bool >& neighborMasks ) const;

    void getRayVoxels( const Vector3d< float >& from,
                       const Vector3d< float >& to,
                       std::set< Vector3d< int32_t >,
                       Vector3dCompare< int32_t > >& voxels,
                       bool clearVoxelsAtBeginning = true ) const;

    void getTriangleVoxels( const Vector3d< float >& vertex1,
                            const Vector3d< float >& vertex2,
                            const Vector3d< float >& vertex3,
                            std::set< Vector3d< int32_t >,
                                      Vector3dCompare< int32_t > >& 
                                                                 voxels,
                            bool clearVoxelsAtBeginning = true ) const;

    void getThickTriangleVoxels( const Vector3d< float >& vertex1,
                                 const Vector3d< float >& vertex2,
                                 const Vector3d< float >& vertex3,
                                 float radiusOfInfluence,
                                 std::set< Vector3d< int32_t >,
                                           Vector3dCompare< int32_t > >& 
                                                                 voxels,
                                 bool clearVoxelsAtBeginning = true ) const;

  protected:

    BoundingBox< float > _boundingBox;
    Vector3d< int32_t > _cacheSize;
    Vector3d< int32_t > _cacheSizeMinusOne;
    BoundingBox< int32_t > _integerBoundingBox;
    float _lowerX;
    float _upperX;
    float _lowerY;
    float _upperY;
    float _lowerZ;
    float _upperZ;
    float _cacheVoxelFactorX;
    float _cacheVoxelFactorY;
    float _cacheVoxelFactorZ;
    Vector3d< float > _resolution;
    float _minimumResolution;

};


}


#endif
