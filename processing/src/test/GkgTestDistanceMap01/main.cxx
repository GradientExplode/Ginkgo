#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-morphology/DanielssonDistanceMapper.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::Volume< int32_t > volume( 256, 256, 32 );
    volume.fill( 0 );

    int32_t x, y, z;
    for ( z = 0; z < volume.getSizeZ(); z++ )
    {

      for ( y = 0; y < volume.getSizeY(); y++ )
      {

        for ( x = 0; x < volume.getSizeX(); x++ )
        {


          if ( ( x > 96 ) && ( x < 160 ) &&
               ( y > 96 ) && ( y < 160 ) &&
               ( z > 12 ) && ( z < 20 ) )
          {

            volume( x, y, z ) = 255;

          }

        }

      }

    }

    // writing data
    gkg::Writer::getInstance().write( "volume", volume, false );

    gkg::EqualToFunction< int32_t > equalToFunction( 255 );
    gkg::DanielssonDistanceMapper< int32_t > distanceMapper( equalToFunction );

    gkg::Volume< float > distance;
    distanceMapper.getDistance( volume, distance, true );
    gkg::Writer::getInstance().write( "distance", distance, false );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
