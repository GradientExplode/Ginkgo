#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Reader_i.h>
#include <iostream>


int main()
{

  gkg::MeshMap< int32_t, float, 3 > meshMap;

  meshMap.vertices.addSite( 2, gkg::Vector3d< float >( 0.0, 0.0, 0.0 ) );
  meshMap.vertices.addSite( 2, gkg::Vector3d< float >( 1.0, 0.0, 0.0 ) );
  meshMap.vertices.addSite( 2, gkg::Vector3d< float >( 1.0, 1.0, 0.0 ) );
  meshMap.vertices.addSite( 2, gkg::Vector3d< float >( 0.0, 1.0, 0.0 ) );
  meshMap.vertices.addSite( 2, gkg::Vector3d< float >( 0.0, 1.0, 1.0 ) );
  meshMap.vertices.addSite( 2, gkg::Vector3d< float >( 0.0, 0.0, 1.0 ) );
  meshMap.vertices.addSite( 2, gkg::Vector3d< float >( 1.0, 0.0, 1.0 ) );
  meshMap.vertices.addSite( 2, gkg::Vector3d< float >( 1.0, 1.0, 1.0 ) );

  meshMap.vertices.addSite( 5, gkg::Vector3d< float >( 2.0, 2.0, 2.0 ) );
  meshMap.vertices.addSite( 5, gkg::Vector3d< float >( 2.5, 2.0, 2.0 ) );
  meshMap.vertices.addSite( 5, gkg::Vector3d< float >( 2.5, 2.5, 2.0 ) );
  meshMap.vertices.addSite( 5, gkg::Vector3d< float >( 2.0, 2.5, 2.0 ) );
  meshMap.vertices.addSite( 5, gkg::Vector3d< float >( 2.0, 2.5, 2.5 ) );
  meshMap.vertices.addSite( 5, gkg::Vector3d< float >( 2.0, 2.0, 2.5 ) );
  meshMap.vertices.addSite( 5, gkg::Vector3d< float >( 2.5, 2.0, 2.5 ) );
  meshMap.vertices.addSite( 5, gkg::Vector3d< float >( 2.5, 2.5, 2.5 ) );

  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 0, 3, 1 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 0, 2, 3 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 0, 1, 4 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 1, 5, 4 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 1, 3, 7 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 1, 7, 5 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 3, 2, 6 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 3, 6, 7 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 0, 4, 2 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 2, 4, 6 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 6, 4, 5 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 2 ),
                               gkg::Polygon< 3U >( 5, 7, 6 ) );

  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 0, 3, 1 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 0, 2, 3 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 0, 1, 4 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 1, 5, 4 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 1, 3, 7 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 1, 7, 5 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 3, 2, 6 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 3, 6, 7 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 0, 4, 2 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 2, 4, 6 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 6, 4, 5 ) );
  meshMap.polygons.addPolygon( meshMap.vertices.getIndex( 5 ),
                               gkg::Polygon< 3U >( 5, 7, 6 ) );


  std::cout << "mesh map : "
            << std::endl
            << meshMap
            << std::endl;

  gkg::Writer::getInstance().write( "foo1", meshMap, true, "meshmap" );

  gkg::Writer::getInstance().write( "foo2", meshMap, true, "aimsmesh" );

  gkg::MeshMap< int32_t, float, 3 > meshMapBis;
  gkg::Reader::getInstance().read( "foo1", meshMapBis, "meshmap" );
  std::cout << "read mesh map in MeshMapDiskFormat : "
            << std::endl
            << meshMapBis
            << std::endl;

  gkg::MeshMap< int32_t, float, 3 > meshMapTer;
  gkg::Reader::getInstance().read( "foo2", meshMapTer, "aimsmesh" );
  std::cout << "read mesh map AimsMeshDiskFormat: "
            << std::endl
            << meshMapBis
            << std::endl;

  return EXIT_SUCCESS;

}
