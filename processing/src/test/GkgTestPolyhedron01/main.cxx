#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/Polyhedron.h>
#include <gkg-core-io/Writer_i.h>



int main()
{

  gkg::Polyhedron polyhedron;

  polyhedron.reserve( 6 );

  gkg::Vector3d< float > p0(  2.1,  3.1, 20.5 );
  gkg::Vector3d< float > p1( 50.3, 60.3, 20.5 );
  gkg::Vector3d< float > p2( 20.8, 80.3, 20.5 );
  gkg::Vector3d< float > p3(  2.1,  3.1, 50.7 );
  gkg::Vector3d< float > p4( 50.3, 60.3, 50.7 );
  gkg::Vector3d< float > p5( 20.8, 80.3, 50.7 );


  polyhedron.addVertex( p0, 1, 3, 2 );
  polyhedron.addVertex( p1, 4, 0, 2 );
  polyhedron.addVertex( p2, 5, 1, 0 );
  polyhedron.addVertex( p3, 0, 4, 5 );
  polyhedron.addVertex( p4, 3, 1, 5 );
  polyhedron.addVertex( p5, 4, 2, 3 );


  gkg::MeshMap< int32_t, float, 3 > meshMap;

  meshMap.vertices.addSite( 0, p0 );
  meshMap.vertices.addSite( 0, p1 );
  meshMap.vertices.addSite( 0, p2 );
  meshMap.vertices.addSite( 0, p3 );
  meshMap.vertices.addSite( 0, p4 );
  meshMap.vertices.addSite( 0, p5 );

  meshMap.polygons.addPolygon( 0, gkg::Polygon< 3U >( 0, 2, 1 ) );
  meshMap.polygons.addPolygon( 0, gkg::Polygon< 3U >( 3, 4, 5 ) );

  meshMap.polygons.addPolygon( 0, gkg::Polygon< 3U >( 0, 5, 2 ) );
  meshMap.polygons.addPolygon( 0, gkg::Polygon< 3U >( 0, 3, 5 ) );

  meshMap.polygons.addPolygon( 0, gkg::Polygon< 3U >( 0, 4, 3 ) );
  meshMap.polygons.addPolygon( 0, gkg::Polygon< 3U >( 0, 1, 4 ) );

  meshMap.polygons.addPolygon( 0, gkg::Polygon< 3U >( 1, 2, 5 ) );
  meshMap.polygons.addPolygon( 0, gkg::Polygon< 3U >( 1, 5, 4 ) );

  gkg::Writer::getInstance().write( "polyhedron_mesh", meshMap );


  gkg::Volume< uint8_t > grid( 99, 99, 99 );
  grid.setResolution( gkg::Vector3d< double >( 1.0, 1.0, 1.0 ) );

  polyhedron.display();

  for ( int32_t i = 0; i < 100; i++ )
  {

    polyhedron.voxelize( gkg::Vector3d< int32_t >( 0, 0, 0 ),
                         gkg::Vector3d< int32_t >( 99, 99, 99 ),
                         grid );

  }

  gkg::Writer::getInstance().write( "polyhedron_grid", grid );

  return EXIT_SUCCESS;

}
