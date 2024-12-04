#ifndef _gkg_processing_plugin_cgal_CGALPointCloudAlphaShapeMesherImplementation_h_
#define _gkg_processing_plugin_cgal_CGALPointCloudAlphaShapeMesherImplementation_h_


#include <gkg-processing-mesh/PointCloudAlphaShapeMesherImplementation.h>


namespace gkg
{


class CGALPointCloudAlphaShapeMesherImplementation :
                                 public PointCloudAlphaShapeMesherImplementation
{

  public:

    CGALPointCloudAlphaShapeMesherImplementation( 
                       PointCloudAlphaShapeMesher* pointCloudAlphaShapeMesher );
    virtual ~CGALPointCloudAlphaShapeMesherImplementation();


    void computeAlphaShapeMesh(
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
                             bool verbose ) const;

    static CGALPointCloudAlphaShapeMesherImplementation* 
      getImplementation( 
                       PointCloudAlphaShapeMesher* pointCloudAlphaShapeMesher );

};


}


#endif
