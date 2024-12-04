#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int, char* argv[] )
{

  gkg::Volume< int16_t > input;
  gkg::Reader::getInstance().read( argv[ 1 ], input );

  gkg::Vector3d< double > inputResolution( 1, 1, 1 );
  input.getHeader().getAttribute( "resolutionX", inputResolution.x );
  input.getHeader().getAttribute( "resolutionY", inputResolution.y );
  input.getHeader().getAttribute( "resolutionZ", inputResolution.z );

  gkg::IdentityTransform3d< double > identityTransform3d;
  gkg::Resampler< int16_t >* resampler = 
    gkg::ResamplerFactory< int16_t >::getInstance().getResampler(
                                                            atoi( argv[ 2 ] ) );


  gkg::Volume< int16_t > output( input.getSizeX(),
                                 input.getSizeY(),
                                 input.getSizeZ() );
  output.getHeader().addAttribute( "resolutionX", inputResolution.x );
  output.getHeader().addAttribute( "resolutionY", inputResolution.y );
  output.getHeader().addAttribute( "resolutionZ", inputResolution.z );
  resampler->resample( input,
                       identityTransform3d,
                       ( int16_t )0,
                       output,
                       true );

  gkg::Writer::getInstance().write( "input", input );
  gkg::Writer::getInstance().write( "output", output );

  return 1;

}
