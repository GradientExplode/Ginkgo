#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <vector>
#include <fstream>



int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {


    gkg::Resampler< std::complex< float > >* resampler =
      gkg::ResamplerFactory< std::complex< float > >::getInstance().
                                                              getResampler( 1 );

    gkg::Volume< std::complex< float > > inVolume( 128, 128, 64 );
    inVolume.getHeader().addAttribute( "resolutionX", 1.0 );
    inVolume.getHeader().addAttribute( "resolutionY", 1.0 );
    inVolume.getHeader().addAttribute( "resolutionZ", 1.0 );

    int32_t x, y, z;
    for ( z = 0; z < 64; z++ )
    {

      for ( y = 0; y < 128; y++ )
      {

        for ( x = 0; x < 128; x++ )
        {

           inVolume( x, y, z ) = std::complex< float >( x + y, y + z );

        }

      }

    }

    gkg::Writer::getInstance().write( "inTestComplexResampling.ima",
                                      inVolume );

    gkg::Volume< std::complex< float > > outVolume( 64, 64, 32 );
    outVolume.getHeader().addAttribute( "resolutionX", 2.0 );
    outVolume.getHeader().addAttribute( "resolutionY", 2.0 );
    outVolume.getHeader().addAttribute( "resolutionZ", 2.0 );

    gkg::IdentityTransform3d< double > identityTransform3d;

    resampler->resample( inVolume,
                         identityTransform3d,
                         std::complex< float >( 0.0, 0.0 ),
                         outVolume,
                         true );

    gkg::Writer::getInstance().write( "outTestComplexResampling.ima",
                                      outVolume );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

