#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Writer_i.h>



int main( int32_t argc, char* argv[] )
{


  // managing the command line options
  gkg::Application application( argc, argv, "Test phantom for watershed" );
  application.initialize();

  gkg::Volume< float > volume( 60, 60, 60 );
  volume.getHeader().addAttribute( "resolutionX", ( double )1.0 );
  volume.getHeader().addAttribute( "resolutionY", ( double )1.0 );
  volume.getHeader().addAttribute( "resolutionZ", ( double )1.0 );

  volume.fill( 0 );

  int32_t x, y, z;
  for ( z = 0; z < volume.getSizeZ(); z++ )
    for ( y = 0; y < volume.getSizeY(); y++ )
      for ( x = 0; x < volume.getSizeX(); x++ )
        if ( ( x >= 10 ) && ( x < 50 ) &&
             ( y >= 10 ) && ( y < 50 ) &&
             ( z >= 10 ) && ( z < 50 ) )
        {

          float X = ( float )x - 10.0;
          volume( x, y, z ) = 30 + 10 * std::sin( X * 3 * M_PI / 40.0 );

        }

  gkg::Writer::getInstance().write( "volume", volume, false);

  return EXIT_SUCCESS;

}
