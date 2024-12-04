#include <gkg-processing-container/SiteMapFactory_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-core-io/Writer_i.h>
#include <iostream>
#include <fstream>


int main()
{

  gkg::Volume< int16_t > volume( 100, 100, 100 );
  volume.getHeader().addAttribute( "resolutionX", ( double )0.5 );
  volume.getHeader().addAttribute( "resolutionY", ( double )0.5 );
  volume.getHeader().addAttribute( "resolutionZ", ( double )0.5 );
  volume.fill( 0 );

  volume( 10, 10, 10 ) = 255;
  volume( 11, 10, 10 ) = 255;
  volume( 12, 11, 10 ) = 255;
  volume( 13, 14, 15 ) = 255;

  volume( 80, 80, 80 ) = 13;
  volume( 80, 81, 81 ) = 13;
  volume( 80, 81, 82 ) = 13;
  volume( 80, 81, 83 ) = 13;
  volume( 80, 81, 84 ) = 13;

  gkg::EqualToFunction< int16_t > inBackgroundTest( 0 );

  // testing with voxel sites
  gkg::SiteMap< int16_t, int32_t > siteMap1 =
    gkg::SiteMapFactory< int16_t, int32_t >::getInstance().create(
                                                     volume, inBackgroundTest );
  gkg::Writer::getInstance().write( "foo1", siteMap1, true );

  // testing with real world sites
  gkg::SiteMap< int16_t, float > siteMap2 =
    gkg::SiteMapFactory< int16_t, float >::getInstance().create(
                                                     volume, inBackgroundTest );
  gkg::Writer::getInstance().write( "foo2", siteMap2, true );

  // testing with real world sites
  gkg::SiteMap< int16_t, float > siteMap3 =
    gkg::SiteMapFactory< int16_t, float >::getInstance().create(
                                                 volume, inBackgroundTest, 13 );
  gkg::Writer::getInstance().write( "foo3", siteMap3, true );


  return EXIT_SUCCESS;

}
