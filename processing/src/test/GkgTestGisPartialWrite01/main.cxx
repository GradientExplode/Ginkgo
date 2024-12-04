#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::VolumeProxy< int16_t > volumeProxy( 128, 128, 3 );
    volumeProxy.getHeader().addAttribute( "resolutionX", 2.0 );
    volumeProxy.getHeader().addAttribute( "resolutionY", 2.0 );
    volumeProxy.getHeader().addAttribute( "resolutionZ", 2.0 );
    volumeProxy.getHeader().addAttribute( "resolutionT", 1.0 );
    
    gkg::TypedDiskFormat< gkg::Volume< int16_t > >*
      format = gkg::DiskFormatFactory< gkg::Volume< int16_t > >::getInstance().
                getDiskFormat( "gis" );

    format->preparePartialBinaryWrite( "volume", volumeProxy );


    gkg::Volume< int16_t > subVolume( 64, 64 );
    subVolume.getHeader().addAttribute( "resolutionX", 2.0 );
    subVolume.getHeader().addAttribute( "resolutionY", 2.0 );
    subVolume.getHeader().addAttribute( "resolutionZ", 2.0 );
    subVolume.getHeader().addAttribute( "resolutionT", 1.0 );

    int32_t x, y;
    for ( y = 0; y < 64; y++ )
    {

      for ( x = 0; x < 64; x++ )
      {

        subVolume( x, y ) = ( int16_t )( x + y );

      }

    }

    gkg::BoundingBox< int32_t > boundingBox( 32, 95,
                                             32, 95,
                                             1, 1 );
    format->partialBinaryWrite( volumeProxy, subVolume, boundingBox );


  }
  GKG_CATCH_COMMAND( result );

  return result;

}

