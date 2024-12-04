#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>



int main()
{


  gkg::PluginLoader::getInstance().load();

  gkg::Volume< int16_t > volume;
  gkg::Reader::getInstance().read( "tractography_mask.ima", volume );

  // axial
/*
  int32_t x, y, z;
  for ( z = 0; z < volume.getSizeZ(); z++ )
    for ( y = 0; y < volume.getSizeY(); y++ )
      for ( x = 0; x < volume.getSizeX(); x++ )
        if ( ( z < 80 ) || ( z >= 100 ) )
          volume( x, y, z ) = 0;
             
  gkg::Writer::getInstance().write( "tractography_mask_axial_slices80_100",
                                    volume );
*/

  // sagittal

  int32_t x, y, z;
  for ( z = 0; z < volume.getSizeZ(); z++ )
    for ( y = 0; y < volume.getSizeY(); y++ )
      for ( x = 0; x < volume.getSizeX(); x++ )
        if ( ( x < 110 ) || ( x >= 120 ) )
          volume( x, y, z ) = 0;
             
  gkg::Writer::getInstance().write( "tractography_mask_sagittal_slices110_120",
                                    volume );


  // axial
/*
  int32_t x, y, z;
  for ( z = 0; z < volume.getSizeZ(); z++ )
    for ( y = 0; y < volume.getSizeY(); y++ )
      for ( x = 0; x < volume.getSizeX(); x++ )
        if ( ( y < 110 ) || ( y >= 120 ) )
          volume( x, y, z ) = 0;
             
  gkg::Writer::getInstance().write( "tractography_mask_coronal_slices110_120",
                                    volume );
*/


  return EXIT_SUCCESS;

}
