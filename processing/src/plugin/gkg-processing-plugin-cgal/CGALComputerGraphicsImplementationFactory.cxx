#include <gkg-processing-plugin-cgal/CGALComputerGraphicsImplementationFactory.h>
#include <gkg-processing-plugin-cgal/CGALPointCloudAlphaShapeMesherImplementation.h>
#include <gkg-processing-mesh/ComputerGraphicsSelector.h>
#include <gkg-processing-mesh/PointCloudAlphaShapeMesher.h>
#include <gkg-core-exception/Exception.h>


ForceSingletonInstanciation( gkg::CGALComputerGraphicsImplementationFactory,
                             libcgal );


gkg::CGALComputerGraphicsImplementationFactory::
                                    CGALComputerGraphicsImplementationFactory()
           : gkg::ComputerGraphicsImplementationFactory(),
             gkg::SingletonAccessor< CGALComputerGraphicsImplementationFactory,
                                     ComputerGraphicsImplementationFactory >()
{

  try
  {

    gkg::ComputerGraphicsSelector::getInstance().registerFactory(
                "libcgal",
                &gkg::CGALComputerGraphicsImplementationFactory::getAccessor );

  }
  GKG_CATCH( "gkg::CGALComputerGraphicsImplementationFactory::"
             "CGALComputerGraphicsImplementationFactory()" );

}


gkg::CGALComputerGraphicsImplementationFactory::
                                   ~CGALComputerGraphicsImplementationFactory()
{
}


gkg::PointCloudAlphaShapeMesherImplementation* 
gkg::CGALComputerGraphicsImplementationFactory::
                                 createPointCloudAlphaShapeMesherImplementation(
             gkg::PointCloudAlphaShapeMesher* pointCloudAlphaShapeMesher ) const
{

  try
  {

    return new gkg::CGALPointCloudAlphaShapeMesherImplementation(
                                                   pointCloudAlphaShapeMesher );

  }
  GKG_CATCH( "gkg::PointCloudAlphaShapeMesherImplementation* "
             "gkg::CGALComputerGraphicsImplementationFactory::"
             "createPointCloudAlphaShapeMesherImplementation( "
             "gkg::PointCloudAlphaShapeMesher* pointCloudAlphaShapeMesher ) "
             "const" );

}
