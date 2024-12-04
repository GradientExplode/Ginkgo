#include <gkg-processing-resampling/SpreaderFactory.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {


    gkg::Spreader< std::complex< float > >* spreader =
      gkg::SpreaderFactory< std::complex< float > >::getInstance().
                                     getSpreader( "Partial Voluming Spreader" );

    gkg::Volume< std::complex< float > > volume( 32, 32, 32 );
    volume.getHeader().addAttribute( "resolutionX", 1.0 );
    volume.getHeader().addAttribute( "resolutionY", 1.0 );
    volume.getHeader().addAttribute( "resolutionZ", 1.0 );

    volume.fill( std::complex< float >( 0.0, 0.0 ) );

    spreader->spread( std::complex< float >( 3.0, 8.0 ),
                      gkg::Vector3d< double >( 16.5, 16.5, 16.5 ),
                      volume );
    spreader->spread( std::complex< float >( 8.0, 8.0 ),
                      gkg::Vector3d< double >( 4.2, 4.5, 6.7 ),
                      volume );

    gkg::Writer::getInstance().write( "volumeTestSpreader01.ima",
                                      volume );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

