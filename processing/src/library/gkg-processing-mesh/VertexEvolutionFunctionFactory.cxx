#include <gkg-processing-mesh/VertexEvolutionFunctionFactory.h>
#include <gkg-processing-mesh/SinusVertexEvolutionFunction.h>
#include <gkg-processing-mesh/StaticVertexEvolutionFunction.h>
#include <gkg-core-exception/Exception.h>


gkg::VertexEvolutionFunctionFactory::VertexEvolutionFunctionFactory()
{
}


gkg::VertexEvolutionFunctionFactory::~VertexEvolutionFunctionFactory()
{
}


gkg::RCPointer< gkg::VertexEvolutionFunction >
gkg::VertexEvolutionFunctionFactory::getStaticVertexEvolutionFunction()
{

  try
  {

    gkg::RCPointer< gkg::VertexEvolutionFunction >
      staticVertexEvolutionFunction( new gkg::StaticVertexEvolutionFunction() );

    return staticVertexEvolutionFunction;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::VertexEvolutionFunction > "
             "gkg::VertexEvolutionFunctionFactory:: "
             "getStaticVertexEvolutionFunction()" );

}


gkg::RCPointer< gkg::VertexEvolutionFunction >
gkg::VertexEvolutionFunctionFactory::getSinusVertexEvolutionFunction(
                               gkg::RCPointer< gkg::SceneModeler > sceneModeler,
                               float amplitude,
                               float period,
                               float phase )
{

  try
  {

    gkg::RCPointer< gkg::VertexEvolutionFunction >
      sinusVertexEvolutionFunction( new gkg::SinusVertexEvolutionFunction(
                                                                   sceneModeler,
                                                                   amplitude,
                                                                   period,
                                                                   phase ) );

    return sinusVertexEvolutionFunction;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::VertexEvolutionFunction > "
             "gkg::VertexEvolutionFunctionFactory:: "
             "getSinusVertexEvolutionFunction( "
             "gkg::RCPointer< gkg::SceneModeler > sceneModeler, "
             "float amplitude, "
             "float period, "
             "float phase )" );

}

