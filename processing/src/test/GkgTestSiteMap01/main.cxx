#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Reader_i.h>
#include <iostream>
#include <fstream>


int main()
{

  gkg::SiteMap< int32_t, float > siteMap;

  siteMap.addSite( 2, gkg::Vector3d< float >( 1.0, 2.0, 3.0 ) );

  siteMap.addSite( 5, gkg::Vector3d< float >( 7.0, 8.0, 9.0 ) );
  siteMap.addSite( 5, gkg::Vector3d< float >( 4.0, 5.0, 6.0 ) );

  std::cout << siteMap << std::endl;

  gkg::Writer::getInstance().write( "foo", siteMap, true );


  gkg::SiteMap< int32_t, float > siteMapBis;
  gkg::Reader::getInstance().read( "foo", siteMapBis );
  std::cout << siteMapBis << std::endl;

  return EXIT_SUCCESS;

}
