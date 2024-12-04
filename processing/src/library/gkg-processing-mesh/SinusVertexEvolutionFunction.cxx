#include <gkg-processing-mesh/SinusVertexEvolutionFunction.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::SinusVertexEvolutionFunction::SinusVertexEvolutionFunction(
                               gkg::RCPointer< gkg::SceneModeler > sceneModeler,
                               float amplitude,
                               float period,
                               float phase )
                                  : gkg::VertexEvolutionFunction(),
                                    _sceneModeler( sceneModeler ),
                                    _amplitude( amplitude ),
                                    _period( period ),
                                    _phase( phase )
{
}


gkg::SinusVertexEvolutionFunction::~SinusVertexEvolutionFunction()
{
}


gkg::RCPointer< gkg::SceneModeler > 
gkg::SinusVertexEvolutionFunction::getSceneModeler() const
{

  try
  {

    return _sceneModeler;

  }
  GKG_CATCH( "gkg::RCPointer< gkg::SceneModeler > "
             "gkg::SinusVertexEvolutionFunction::getSceneModeler() const" );

}


float gkg::SinusVertexEvolutionFunction::getPeriod() const
{

  try
  {

    return _period;

  }
  GKG_CATCH( "float gkg::SinusVertexEvolutionFunction::getPeriod() const" );

}


float gkg::SinusVertexEvolutionFunction::getAmplitude() const
{

  try
  {

    return _amplitude;

  }
  GKG_CATCH( "float gkg::SinusVertexEvolutionFunction::getAmplitude() const" );

}


void gkg::SinusVertexEvolutionFunction::modifyVertex(
                             gkg::Vector3d< float >& currentVertex,
                             const gkg::Vector3d< float >& currentNormal ) const
{

  try
  {

    float modification =
      _amplitude * ( ( float )std::sin( ( double )
                                        ( ( 2 * M_PI * _step *
                                          ( _sceneModeler->getTimeStep() ) /
                                            _period ) + _phase )
                                       ) -
                     ( float )std::sin( _phase ) );

    currentVertex += currentNormal * modification;

  }
  GKG_CATCH( "void gkg::SinusVertexEvolutionFunction::modifyVertex( "
             "gkg::Vector3d< float >& currentVertex, "
             "const gkg::Vector3d< float >& currentNormal ) const" );

}

