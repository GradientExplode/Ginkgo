#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/Polyhedron.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-mesh/EvolvingMesh.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-mesh/StaticVertexEvolutionFunction.h>



int main()
{

  float radiusOfInfluence = 1.1f * 1.0f;

  gkg::RCPointer< gkg::MeshMap< int32_t, float, 3 > >
    meshMap( new gkg::MeshMap< int32_t, float, 3 > );
  gkg::Reader::getInstance().read( "pial.mesh", *meshMap );

  std::cout << "pial mesh read" << std::endl;

  gkg::Vector3d< int32_t > cacheSize( 100, 100, 20 );

  gkg::BoundingBox< float >
    boundingBox( meshMap->vertices.getBoundingBox( 0 ) );
  boundingBox.setLowerX( boundingBox.getLowerX() - radiusOfInfluence );
  boundingBox.setLowerY( boundingBox.getLowerY() - radiusOfInfluence );
  boundingBox.setLowerZ( boundingBox.getLowerZ() - radiusOfInfluence );

  boundingBox.setUpperX( boundingBox.getUpperX() + radiusOfInfluence );
  boundingBox.setUpperY( boundingBox.getUpperY() + radiusOfInfluence );
  boundingBox.setUpperZ( boundingBox.getUpperZ() + radiusOfInfluence );

  boundingBox.setLowerX( - radiusOfInfluence );
  boundingBox.setLowerY( - radiusOfInfluence );
  boundingBox.setLowerZ( - radiusOfInfluence );
                                         
  gkg::RCPointer< gkg::SceneModeler >
    sceneModeler( new gkg::SceneModeler(
                    boundingBox,
                    1.0,
                    1,
                    1,
                    cacheSize,
                    true ) );

  std::cout << "scene modeler created" << std::endl;

  gkg::RCPointer< gkg::VertexEvolutionFunction >
    staticVertexEvolutionFunction(
                                 new gkg::StaticVertexEvolutionFunction() );

  gkg::RCPointer< gkg::EvolvingMesh >
    evolvingMesh( new gkg::EvolvingMesh(
                        sceneModeler,
                        meshMap,
                        staticVertexEvolutionFunction,
                        radiusOfInfluence,
                        false ) );

  std::cout << "evolving mesh created" << std::endl;

  sceneModeler->addEvolvingMesh( "brain", evolvingMesh );

  std::cout << "evolving mesh added to scene modeler" << std::endl;


  gkg::RCPointer< gkg::Volume< uint8_t > >
    enlargedPialMask( new gkg::Volume< uint8_t > );

  sceneModeler->getEvolvingMeshCache().getBinaryMask( *enlargedPialMask );

  std::cout << "enlarged brain mask computed" << std::endl;

  gkg::Writer::getInstance().write( "enlarged_brain_mask", *enlargedPialMask );

  std::cout << "enlarged brain mask saved" << std::endl;

  return EXIT_SUCCESS;

}
