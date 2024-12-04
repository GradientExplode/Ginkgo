#include <gkg-processing-mesh/VertexEvolutionFunction.h>
#include <gkg-core-exception/Exception.h>


gkg::VertexEvolutionFunction::VertexEvolutionFunction()
                             : gkg::RCObject()
{
}


gkg::VertexEvolutionFunction::~VertexEvolutionFunction()
{
}


int32_t gkg::VertexEvolutionFunction::getStep() const
{

  try
  {

    return _step;

  }
  GKG_CATCH( "int32_t gkg::VertexEvolutionFunction::getStep() const" );

}


void gkg::VertexEvolutionFunction::setStep( int32_t iteration )
{

  try
  {

    _step = iteration;

  }
  GKG_CATCH( "void gkg::VertexEvolutionFunction::setStep( int32_t iteration )" );

}
