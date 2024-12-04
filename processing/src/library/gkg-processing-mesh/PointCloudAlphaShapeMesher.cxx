#include <gkg-processing-mesh/PointCloudAlphaShapeMesher.h>
#include <gkg-processing-mesh/PointCloudAlphaShapeMesherImplementation.h>
#include <gkg-processing-mesh/ComputerGraphicsSelector.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::PointCloudAlphaShapeMesher::PointCloudAlphaShapeMesher()
{

  try
  {

    _pointCloudAlphaShapeMesherImplementation =
      gkg::ComputerGraphicsSelector::getInstance().getImplementationFactory()->
                   createPointCloudAlphaShapeMesherImplementation( this  );


  }
  GKG_CATCH( "gkg::PointCloudAlphaShapeMesher::PointCloudAlphaShapeMesher()" );

}


gkg::PointCloudAlphaShapeMesher::~PointCloudAlphaShapeMesher()
{

  delete _pointCloudAlphaShapeMesherImplementation;

}


void gkg::PointCloudAlphaShapeMesher::computeAlphaShapeMesh(
                        const std::vector< gkg::Vector3d< float > >& pointCloud,
                        gkg::MeshMap< int32_t, float, 3U >& meshMap,
                        int32_t rank,
                        bool removingOutliers,
                        int32_t neighborCount,
                        float outlierDistanceScale,
                        bool simplifyingPointSet,
                        bool smoothingPointSet,
                        bool subdividingMesh,
                        int32_t subdivisionIterationCount,
                        const std::string& intermediateFileName,
                        bool verbose ) const
{

  try
  {

    _pointCloudAlphaShapeMesherImplementation->computeAlphaShapeMesh(
                                                      pointCloud,
                                                      meshMap,
                                                      rank,
                                                      removingOutliers,
                                                      neighborCount,
                                                      outlierDistanceScale,
                                                      simplifyingPointSet,
                                                      smoothingPointSet,
                                                      subdividingMesh,
                                                      subdivisionIterationCount,
                                                      intermediateFileName,
                                                      verbose );


  }
  GKG_CATCH( "void gkg::PointCloudAlphaShapeMesher::computeAlphaShapeMesh( "
             "const std::vector< gkg::Vector3d< float > >& pointCloud, "
             "gkg::MeshMap< int32_t, float, 3U >& meshMap, "
             "int32_t rank, "
             "bool removingOutliers, "
             "int32_t neighborCount, "
             "float outlierDistanceScale, "
             "bool simplifyingPointSet, "
             "bool smoothingPointSet, "
             "bool subdividingMesh, "
             "int32_t subdivisionIterationCount, "
             "const std::string& intermediateFileName, "
             "bool verbose ) const" );

}


gkg::PointCloudAlphaShapeMesherImplementation* 
gkg::PointCloudAlphaShapeMesher::getImplementation() const
{

  try
  {

    return _pointCloudAlphaShapeMesherImplementation;

  }
  GKG_CATCH( "gkg::PointCloudAlphaShapeMesherImplementation* "
             "gkg::PointCloudAlphaShapeMesher::getImplementation() const" );

}

