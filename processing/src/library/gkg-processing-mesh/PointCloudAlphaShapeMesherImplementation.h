#ifndef _gkg_processing_mesh_PointCloudAlphaShapeMesherImplementation_h_
#define _gkg_processing_mesh_PointCloudAlphaShapeMesherImplementation_h_


#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>



namespace gkg
{


class PointCloudAlphaShapeMesher;


class PointCloudAlphaShapeMesherImplementation
{

  public:

    virtual ~PointCloudAlphaShapeMesherImplementation();

    virtual PointCloudAlphaShapeMesher* getTarget() const;


    virtual void computeAlphaShapeMesh(
                             const std::vector< Vector3d< float > >& pointCloud,
                             MeshMap< int32_t, float, 3U >& meshMap,
                             int32_t rank,
                             bool removingOutliers,
                             int32_t neighborCount,
                             float outlierDistanceScale,
                             bool simplifyingPointSet,
                             bool smoothingPointSet,
                             bool subdividingMesh,
                             int32_t subdivisionIterationCount,
                             const std::string& intermediateFileName,
                             bool verbose ) const = 0; 

  protected:

    PointCloudAlphaShapeMesherImplementation(
                       PointCloudAlphaShapeMesher* pointCloudAlphaShapeMesher );

    PointCloudAlphaShapeMesher* _target;

};


}


#endif
