#include <gkg-processing-container/PolygonMap.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Reader_i.h>
#include <iostream>
#include <fstream>


int main()
{

  gkg::PolygonMap< 3U > polygonMap;

  polygonMap.addPolygon( 0, gkg::Polygon< 3U >( 1, 2, 3 ) );

  polygonMap.addPolygon( 1, gkg::Polygon< 3U >( 7, 8, 9 ) );
  polygonMap.addPolygon( 1, gkg::Polygon< 3U >( 4, 5, 6 ) );

  std::cout << polygonMap << std::endl;

  gkg::Writer::getInstance().write( "foo", polygonMap, true );


  gkg::PolygonMap< 3U > polygonMapBis;
  gkg::Reader::getInstance().read( "foo", polygonMapBis );
  std::cout << polygonMapBis << std::endl;

  return EXIT_SUCCESS;

}
