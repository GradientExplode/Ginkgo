#ifndef _gkg_processing_mesh_SinusVertexEvolutionFunction_h_
#define _gkg_processing_mesh_SinusVertexEvolutionFunction_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-mesh/VertexEvolutionFunction.h>


namespace gkg
{


class SinusVertexEvolutionFunction : public VertexEvolutionFunction
{

  public:


    SinusVertexEvolutionFunction( RCPointer< SceneModeler > sceneModeler,
                                  float amplitude,
                                  float period,
                                  float phase );
    virtual ~SinusVertexEvolutionFunction();

    RCPointer< SceneModeler > getSceneModeler() const;
    float getPeriod() const;
    float getAmplitude() const;

    void modifyVertex( Vector3d< float >& currentVertex,
                       const Vector3d< float >& currentNormal ) const;

  protected:

    RCPointer< SceneModeler > _sceneModeler;
    float _amplitude;
    float _period;
    float _phase;

};


}


#endif
