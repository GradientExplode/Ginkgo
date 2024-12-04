#ifndef _gkg_processing_mesh_ComputerGraphicsImplementationFactory_h_
#define _gkg_processing_mesh_ComputerGraphicsImplementationFactory_h_


#include <gkg-processing-mesh/PointCloudAlphaShapeMesher.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class PointCloudAlphaShapeMesherImplementation;


class ComputerGraphicsImplementationFactory
{

  public:

    virtual ~ComputerGraphicsImplementationFactory();

    virtual PointCloudAlphaShapeMesherImplementation*
      createPointCloudAlphaShapeMesherImplementation( 
        PointCloudAlphaShapeMesher* pointCloudAlphaShapeMesher ) const = 0;

  protected:

    ComputerGraphicsImplementationFactory();

};


}


#endif
