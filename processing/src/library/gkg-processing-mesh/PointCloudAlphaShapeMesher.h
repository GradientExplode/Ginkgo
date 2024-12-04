#ifndef _gkg_processing_mesh_PointCloudAlphaShapeMesher_h_
#define _gkg_processing_mesh_PointCloudAlphaShapeMesher_h_



#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>



namespace gkg
{


class PointCloudAlphaShapeMesherImplementation;


class PointCloudAlphaShapeMesher
{

  public:

    PointCloudAlphaShapeMesher();
    virtual ~PointCloudAlphaShapeMesher();

    void computeAlphaShapeMesh(
                             const std::vector< Vector3d< float > >& pointCloud,
                             MeshMap< int32_t, float, 3U >& meshMap,
                             int32_t rank,
                             bool removingOutliers = true,
                             int32_t neighborCount = 24,
                             float outlierDistanceScale = 2.0,
                             bool simplifyingPointSet = true,
                             bool smoothingPointSet = true,
                             bool subdividingMesh = true,
                             int32_t subdivisionIterationCount = 1,
                             const std::string& intermediateFileName = "",
                             bool verbose = false ) const; 

    PointCloudAlphaShapeMesherImplementation* getImplementation() const;

  protected:

    PointCloudAlphaShapeMesherImplementation*
                                      _pointCloudAlphaShapeMesherImplementation;

};



}



#endif
