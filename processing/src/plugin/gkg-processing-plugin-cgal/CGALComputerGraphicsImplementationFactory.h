#ifndef _gkg_processing_plugin_cgal_CGALComputerGraphicsImplementationFactory_h_
#define _gkg_processing_plugin_cgal_CGALComputerGraphicsImplementationFactory_h_


#include <gkg-processing-mesh/ComputerGraphicsImplementationFactory.h>
#include <gkg-core-pattern/SingletonAccessor.h>


namespace gkg
{


class CGALComputerGraphicsImplementationFactory : 
           public ComputerGraphicsImplementationFactory,
           public SingletonAccessor< CGALComputerGraphicsImplementationFactory,
                                     ComputerGraphicsImplementationFactory >
{

  public:

    ~CGALComputerGraphicsImplementationFactory();

    PointCloudAlphaShapeMesherImplementation*
      createPointCloudAlphaShapeMesherImplementation( 
        PointCloudAlphaShapeMesher* pointCloudAlphaShapeMesher ) const;

  protected:

    friend class Singleton< CGALComputerGraphicsImplementationFactory >;

    CGALComputerGraphicsImplementationFactory();

};


}


#endif
