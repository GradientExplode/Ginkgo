#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Writer_i.h>
#include <cmath>


int main()
{


  gkg::PluginLoader::getInstance().load();

  gkg::Volume< float > volume( 256,256, 256 );
  volume.getHeader().addAttribute( "resolutionX", ( double )1.0 );
  volume.getHeader().addAttribute( "resolutionY", ( double )1.0 );
  volume.getHeader().addAttribute( "resolutionZ", ( double )1.0 );

  volume.fill( 0 );

  int32_t x, y, z;
  for ( z = 0; z < volume.getSizeZ(); z++ )
  {

    for ( y = 0; y < volume.getSizeY(); y++ )
    {

      for ( x = 0; x < volume.getSizeX(); x++ )
      {


        if ( std::sqrt( ( float )( x - 127 ) *
                        ( float )( x - 127 ) +
                        ( float )( y - 60 ) *
                        ( float )( y - 60 ) +
                        ( float )( z - 127 ) *
                        ( float )( z - 127 ) ) < 20.0 )
        {

          float d = std::sqrt( ( float )( x - 127 ) *
                                      ( float )( x - 127 ) +
                                      ( float )( y - 60 ) *
                                      ( float )( y - 60 ) +
                                      ( float )( z - 127 ) *
                                      ( float )( z - 127 ) );
          
          volume( x, y, z ) = 200.0f + 100 * std::sin( 2 * d * M_PI / 20 -
                                                       M_PI / 2);

        }
        else if ( std::sqrt( ( float )( x - 127 ) *
                             ( float )( x - 127 ) +
                             ( float )( y - 194 ) *
                             ( float )( y - 194 ) +
                             ( float )( z - 127 ) *
                             ( float )( z - 127 ) ) < 20.0 )
        {

          float d = std::sqrt( ( float )( x - 127 ) *
                               ( float )( x - 127 ) +
                               ( float )( y - 194 ) *
                               ( float )( y - 194 ) +
                               ( float )( z - 127 ) *
                               ( float )( z - 127 ) );
          
          volume( x, y, z ) = 200.0f + 100 * std::sin( 2 * d * M_PI / 20 -
                                                       M_PI / 2);

        }
        else if ( std::sqrt( ( float )( x - 60 ) *
                             ( float )( x - 60 ) +
                             ( float )( y - 127 ) *
                             ( float )( y - 127 ) +
                             ( float )( z - 127 ) *
                             ( float )( z - 127 ) ) < 20.0 )
        {

          float d = std::sqrt( ( float )( x - 60 ) *
                               ( float )( x - 60 ) +
                               ( float )( y - 127 ) *
                               ( float )( y - 127 ) +
                               ( float )( z - 127 ) *
                               ( float )( z - 127 ) );
          
          volume( x, y, z ) = 200.0f + 100 * std::sin( 2 * d * M_PI / 20 -
                                                       M_PI / 2);

        }
        else if ( std::sqrt( ( float )( x - 194 ) *
                             ( float )( x - 194 ) +
                             ( float )( y - 127 ) *
                             ( float )( y - 127 ) +
                             ( float )( z - 127 ) *
                             ( float )( z - 127 ) ) < 20.0 )
        {

          float d = std::sqrt( ( float )( x - 194 ) *
                               ( float )( x - 194 ) +
                               ( float )( y - 127 ) *
                               ( float )( y - 127 ) +
                               ( float )( z - 127 ) *
                               ( float )( z - 127 ) );
          
          volume( x, y, z ) = 200.0f + 100 * std::sin( 2 * d * M_PI / 20 -
                                                       M_PI / 2);

        }
        else if ( std::sqrt( ( float )( x - 127 ) *
                             ( float )( x - 127 ) +
                             ( float )( y - 127 ) *
                             ( float )( y - 127 ) +
                             ( float )( z - 127 ) *
                             ( float )( z - 127 ) ) < 100.0 )
        {

          volume( x, y, z ) = 100.0f;

        }

      }

    }

  }
             
  gkg::Writer::getInstance().write( "test_watershed.ima", volume );

  return EXIT_SUCCESS;

}
