#include <gkg-processing-mesh/PointCloudAlphaShapeMesherImplementation.h>
#include <gkg-processing-container/MeshMap_i.h>



gkg::PointCloudAlphaShapeMesherImplementation::
                                       PointCloudAlphaShapeMesherImplementation(
                   gkg::PointCloudAlphaShapeMesher* pointCloudAlphaShapeMesher )
                                    : _target( pointCloudAlphaShapeMesher )
{
}


gkg::PointCloudAlphaShapeMesherImplementation::
                                     ~PointCloudAlphaShapeMesherImplementation()
{
}


gkg::PointCloudAlphaShapeMesher* 
gkg::PointCloudAlphaShapeMesherImplementation::getTarget() const
{

  return _target;

}
