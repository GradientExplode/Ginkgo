#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/Polyhedron.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-mesh/EvolvingMesh.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-mesh/StaticVertexEvolutionFunction.h>


#define TEST_WITH_SCENE_MODELER 1


int main()
{

  float radiusOfInfluence = 1.0;

  gkg::RCPointer< gkg::MeshMap< int32_t, float, 3 > >
    meshMap( new gkg::MeshMap< int32_t, float, 3 > );
  gkg::Reader::getInstance().read( "brain.mesh", *meshMap );

#ifndef TEST_WITH_SCENE_MODELER


  std::vector< gkg::Vector3d< float > >
    vertices( meshMap->vertices.getSites( 0 ).begin(),
              meshMap->vertices.getSites( 0 ).end() );
              

  gkg::Volume< uint8_t > grid( 160, 256, 256 );
  grid.setResolution( gkg::Vector3d< double >( 1.0, 1.0, 1.0 ) );
  grid.fill( 0U );

  int32_t count = 0;
  std::list< gkg::Polygon< 3U > >::const_iterator
    p = meshMap->polygons.getPolygons( 0 ).begin(),
    pe = meshMap->polygons.getPolygons( 0 ).end();
  while ( p != pe )
  {

    gkg::Polyhedron polyhedron;
    polyhedron.reserve( 6 );


    const gkg::Vector3d< float >& vertex1 = vertices[ p->indices[ 0 ] ];
    const gkg::Vector3d< float >& vertex2 = vertices[ p->indices[ 1 ] ];
    const gkg::Vector3d< float >& vertex3 = vertices[ p->indices[ 2 ] ];

    gkg::Vector3d< float >
      normal =
         ( ( vertex2 - vertex1 ).cross( vertex3 - vertex1 ) ).normalize();

    gkg::Vector3d< float > midPoint1 = ( vertex2 + vertex3 ) / 2.0f;
    gkg::Vector3d< float > midPoint2 = ( vertex3 + vertex1 ) / 2.0f;
    gkg::Vector3d< float > midPoint3 = ( vertex1 + vertex2 ) / 2.0f;

    gkg::Vector3d< float >
      shift1 = ( ( vertex1 - midPoint1 ).normalize() ) * radiusOfInfluence;
    gkg::Vector3d< float >
      shift2 = ( ( vertex2 - midPoint2 ).normalize() ) * radiusOfInfluence;
    gkg::Vector3d< float >
      shift3 = ( ( vertex3 - midPoint3 ).normalize() ) * radiusOfInfluence;

    gkg::Vector3d< float > p0 = vertex1 + shift1 + 
                                normal * ( - radiusOfInfluence ); 
    gkg::Vector3d< float > p1 = vertex2 + shift2 + 
                                normal * ( - radiusOfInfluence ); 
    gkg::Vector3d< float > p2 = vertex3 + shift3 + 
                                normal * ( - radiusOfInfluence ); 
    gkg::Vector3d< float > p3 = vertex1 + shift1 + 
                                normal * ( + radiusOfInfluence ); 
    gkg::Vector3d< float > p4 = vertex2 + shift2 + 
                                normal * ( + radiusOfInfluence ); 
    gkg::Vector3d< float > p5 = vertex3 + shift3 + 
                                normal * ( + radiusOfInfluence ); 

    polyhedron.addVertex( p0, 1, 3, 2 );
    polyhedron.addVertex( p1, 4, 0, 2 );
    polyhedron.addVertex( p2, 5, 1, 0 );
    polyhedron.addVertex( p3, 0, 4, 5 );
    polyhedron.addVertex( p4, 3, 1, 5 );
    polyhedron.addVertex( p5, 4, 2, 3 );

    polyhedron.voxelize( gkg::Vector3d< int32_t >( 0, 0, 0 ),
                         gkg::Vector3d< int32_t >( 159, 255, 255 ),
                         grid );

    ++ p;
    ++ count;

    //std::cout << count << std::endl;

  }

  gkg::Writer::getInstance().write( "polyhedron_grid", grid );

#endif

#ifdef TEST_WITH_SCENE_MODELER

  std::cout << "test with scene modeler" << std::endl;

//  gkg::Vector3d< int32_t > cacheSize( 160, 256, 256 );
//  gkg::BoundingBox< float > boundingBox( 0.0, 159.0,
//                                         0.0, 256.0,
//                                         0.0, 256.0 );

  gkg::Vector3d< int32_t > cacheSize( 100, 100, 100 );

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

  sceneModeler->addEvolvingMesh( "brain", evolvingMesh );
  gkg::RCPointer< gkg::Volume< uint8_t > >
    enlargedPialMask( new gkg::Volume< uint8_t > );

  sceneModeler->getEvolvingMeshCache().getBinaryMask( *enlargedPialMask );

  gkg::Writer::getInstance().write( "enlarged_brain_mask", *enlargedPialMask );

#endif

  return EXIT_SUCCESS;

}
