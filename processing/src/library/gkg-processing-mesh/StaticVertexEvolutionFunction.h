#ifndef _gkg_processing_mesh_StaticVertexEvolutionFunction_h_
#define _gkg_processing_mesh_StaticVertexEvolutionFunction_h_


#include <gkg-processing-mesh/VertexEvolutionFunction.h>


namespace gkg
{


class StaticVertexEvolutionFunction : public VertexEvolutionFunction
{

  public:

    StaticVertexEvolutionFunction();
    virtual ~StaticVertexEvolutionFunction();

    void modifyVertex( Vector3d< float >& /*currentVertex*/,
                       const Vector3d< float >& /*currentNormal*/ ) const;

};


}


#endif
