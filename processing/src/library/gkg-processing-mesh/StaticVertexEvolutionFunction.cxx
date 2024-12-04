#include <gkg-processing-mesh/StaticVertexEvolutionFunction.h>


gkg::StaticVertexEvolutionFunction::StaticVertexEvolutionFunction()
                                   : gkg::VertexEvolutionFunction()
{
}


gkg::StaticVertexEvolutionFunction::~StaticVertexEvolutionFunction()
{
}


void gkg::StaticVertexEvolutionFunction::modifyVertex(
                         gkg::Vector3d< float >& /*currentVertex*/,
                         const gkg::Vector3d< float >& /*currentNormal*/ ) const
{
}

