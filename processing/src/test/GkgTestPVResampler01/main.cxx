#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <complex>


int main( int, char* argv[] )
{

  gkg::Volume< int16_t > t2;
  gkg::Reader::getInstance().read( argv[ 1 ], t2 );

  gkg::Volume< int16_t > input( 128, 128, 60 );

  gkg::Vector3d< double > resolution( 1.875, 1.875, 2 );

  input.getHeader().addAttribute( "resolutionX", resolution.x );
  input.getHeader().addAttribute( "resolutionY", resolution.y );
  input.getHeader().addAttribute( "resolutionZ", resolution.z );

  input.fill( 0 );

  int32_t x, y, z;
  for ( z = 0; z < 60; z++ )
  {

    for ( y = 0; y < 128; y++ )
    {

      for ( x = 0; x < 128; x++ )
      {

        input( x, y, z ) = t2( x, y, z );

      }

    }

  }


  gkg::Volume< int16_t > output( 256, 256, 120 );
  gkg::Vector3d< double > outResolution( 1.875 / 2, 1.875 / 2, 2 / 2 );
  output.getHeader().addAttribute( "resolutionX", outResolution.x );
  output.getHeader().addAttribute( "resolutionY", outResolution.y );
  output.getHeader().addAttribute( "resolutionZ", outResolution.z );

  gkg::PartialVolumingQuickResampler< int16_t >& resampler =
    gkg::PartialVolumingQuickResampler< int16_t >::getInstance();

  for ( z = 0; z < 120; z++ )
  {

    for ( y = 0; y < 256; y++ )
    {

      for ( x = 0; x < 256; x++ )
      {


        gkg::Vector3d< double > site( x * outResolution.x,
                                      y * outResolution.y,
                                      z * outResolution.z );
  
        resampler.resample( input,
                            ( int16_t )0,
                            site,
                            output( x, y, z ),
                            &resolution,
                            0 );

      }

    }

  }

  gkg::Writer::getInstance().write( "input", input );
  gkg::Writer::getInstance().write( "output", output );

  return 1;

}
