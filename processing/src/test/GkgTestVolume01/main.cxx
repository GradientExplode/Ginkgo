#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Writer_i.h>



int main()
{

  gkg::Volume< uint8_t > volume1( 256,256,124 );
  volume1.getHeader().addAttribute( "resolutionX", ( double )0.8 );
  volume1.getHeader().addAttribute( "resolutionY", ( double )0.8 );
  volume1.getHeader().addAttribute( "resolutionZ", ( double )1.6 );

  volume1.fill( 0 );

  int32_t x, y, z;
  for ( z = 0; z < volume1.getSizeZ(); z++ )
    for ( y = 0; y < volume1.getSizeY(); y++ )
      for ( x = 0; x < volume1.getSizeX(); x++ )
        if ( ( x >= 80 ) && ( x < 160 ) &&
             ( y >= 80 ) && ( y < 160 ) &&
             ( z >= 40 ) && ( z < 80 ) )
          volume1( x, y, z ) = 255;
             
  gkg::Writer::getInstance().write( "reference_cube", volume1 );


  gkg::Volume< uint8_t > volume2( 256,256,124 );
  volume2.getHeader().addAttribute( "resolutionX", ( double )0.8 );
  volume2.getHeader().addAttribute( "resolutionY", ( double )0.8 );
  volume2.getHeader().addAttribute( "resolutionZ", ( double )1.6 );

  volume2.fill( 0 );

  for ( z = 0; z < volume1.getSizeZ(); z++ )
    for ( y = 0; y < volume1.getSizeY(); y++ )
      for ( x = 0; x < volume1.getSizeX(); x++ )
        if ( ( x >= 120 ) && ( x < 200 ) &&
             ( y >= 80 ) && ( y < 160 ) &&
             ( z >= 40 ) && ( z < 80 ) )
          volume2( x, y, z ) = 255;
             
  gkg::Writer::getInstance().write( "floating_cube", volume2 );

  return EXIT_SUCCESS;

}
