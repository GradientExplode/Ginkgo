#ifndef _gkg_processing_mesh_VertexEvolutionFunctionFactory_h_
#define _gkg_processing_mesh_VertexEvolutionFunctionFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-mesh/VertexEvolutionFunction.h>


namespace gkg
{


class VertexEvolutionFunctionFactory :
                              public Singleton< VertexEvolutionFunctionFactory >
{

  public:

    ~VertexEvolutionFunctionFactory();

    RCPointer< VertexEvolutionFunction >
      getStaticVertexEvolutionFunction();

    RCPointer< VertexEvolutionFunction >
      getSinusVertexEvolutionFunction( RCPointer< SceneModeler > sceneModeler,
                                       float amplitude,
                                       float period,
                                       float phase );

  protected:

    friend class Singleton< VertexEvolutionFunctionFactory >;

    VertexEvolutionFunctionFactory();

};


}


#endif

