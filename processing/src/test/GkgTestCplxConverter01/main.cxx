#include <gkg-processing-algobase/MagnitudeConverter_i.h>
#include <gkg-processing-algobase/PhaseConverter_i.h>
#include <gkg-processing-algobase/RealPartConverter_i.h>
#include <gkg-processing-algobase/ImagPartConverter_i.h>
#include <gkg-processing-algobase/PolarConverter_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <complex>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;
  try
  {

    // base magnitude converter
    {

      float a = -125.3;
      int16_t b;
      gkg::MagnitudeConverter< float, int16_t > magnitudeConverter;
      magnitudeConverter.convert( a, b );
      std::cout << "b = " << b << std::endl;
      std::cout << std::endl;

    }

    // base phase converter
    {

      std::complex< float > a( 1.0, 1.0 );
      double b;
      gkg::MagnitudeConverter< std::complex< float >,
                               double > magnitudeConverter;
      magnitudeConverter.convert( a, b );
      std::cout << "b = " << b << std::endl;
      std::cout << std::endl;

    }

    // volume magnitude converter with real data
    {

      gkg::Volume< float > a( 2, 2 );
      a( 0, 0 ) = -2.5;
      a( 0, 1 ) = 3.8;
      a( 1, 0 ) = -5;
      a( 1, 1 ) = 7.2;

      gkg::Volume< double > b;
      gkg::MagnitudeConverter< gkg::Volume< float >,
                               gkg::Volume< double > > magnitudeConverter;
      magnitudeConverter.convert( a, b );

      std::cout << b( 0, 0 ) << "\t" << b( 0, 1 ) << std::endl;
      std::cout << b( 1, 0 ) << "\t" << b( 1, 1 ) << std::endl;
      std::cout << std::endl;

    }

    // volume magnitude converter with complex data
    {

      gkg::Volume< std::complex< double > > a( 2, 2 );
      a( 0, 0 ) = std::complex< double >( 2.5, 0.0 );
      a( 0, 1 ) = std::complex< double >( -1.0, 1.0 );
      a( 1, 0 ) = std::complex< double >( 0.0, -5.0 );
      a( 1, 1 ) = std::complex< double >( 3, -4 );

      gkg::Volume< float > b;
      gkg::MagnitudeConverter< gkg::Volume< std::complex< double > >,
                               gkg::Volume< float > > magnitudeConverter;
      magnitudeConverter.convert( a, b );

      std::cout << b( 0, 0 ) << "\t" << b( 0, 1 ) << std::endl;
      std::cout << b( 1, 0 ) << "\t" << b( 1, 1 ) << std::endl;
      std::cout << std::endl;

    }

    // volume phase converter with complex data in degrees
    {

      gkg::Volume< std::complex< double > > a( 2, 2 );
      a( 0, 0 ) = std::complex< double >( 2.5, 0.0 );
      a( 0, 1 ) = std::complex< double >( -1.0, 1.0 );
      a( 1, 0 ) = std::complex< double >( 0.0, -5.0 );
      a( 1, 1 ) = std::complex< double >( 3, -4 );

      gkg::Volume< float > b;
      gkg::PhaseConverter< gkg::Volume< std::complex< double > >,
                           gkg::Volume< float > > phaseConverter;
      phaseConverter.convert( a, b, true );

      std::cout << b( 0, 0 ) << "\t" << b( 0, 1 ) << std::endl;
      std::cout << b( 1, 0 ) << "\t" << b( 1, 1 ) << std::endl;
      std::cout << std::endl;

    }


    // volume phase converter with complex data in radians
    {

      gkg::Volume< float > a( 2, 2 );
      a( 0, 0 ) = -2.5;
      a( 0, 1 ) = 0.0;
      a( 1, 0 ) = -5;
      a( 1, 1 ) = 7.2;

      gkg::Volume< double > b;
      gkg::PhaseConverter< gkg::Volume< float >,
                           gkg::Volume< double > > phaseConverter;
      phaseConverter.convert( a, b, false );

      std::cout << b( 0, 0 ) << "\t" << b( 0, 1 ) << std::endl;
      std::cout << b( 1, 0 ) << "\t" << b( 1, 1 ) << std::endl;
      std::cout << std::endl;

    }

    // volume real part converter with complex data
    {

      gkg::Volume< std::complex< double > > a( 2, 2 );
      a( 0, 0 ) = std::complex< double >( 2.5, 0.0 );
      a( 0, 1 ) = std::complex< double >( -1.0, 1.0 );
      a( 1, 0 ) = std::complex< double >( 0.0, -5.0 );
      a( 1, 1 ) = std::complex< double >( 3, -4 );

      gkg::Volume< float > b;
      gkg::RealPartConverter< gkg::Volume< std::complex< double > >,
                              gkg::Volume< float > > realPartConverter;
      realPartConverter.convert( a, b );

      std::cout << b( 0, 0 ) << "\t" << b( 0, 1 ) << std::endl;
      std::cout << b( 1, 0 ) << "\t" << b( 1, 1 ) << std::endl;
      std::cout << std::endl;

    }

    // volume imaginary part converter with complex data
    {

      gkg::Volume< std::complex< double > > a( 2, 2 );
      a( 0, 0 ) = std::complex< double >( 2.5, 0.0 );
      a( 0, 1 ) = std::complex< double >( -1.0, 1.0 );
      a( 1, 0 ) = std::complex< double >( 0.0, -5.0 );
      a( 1, 1 ) = std::complex< double >( 3, -4 );

      gkg::Volume< float > b;
      gkg::ImagPartConverter< gkg::Volume< std::complex< double > >,
                              gkg::Volume< float > > imagPartConverter;
      imagPartConverter.convert( a, b );

      std::cout << b( 0, 0 ) << "\t" << b( 0, 1 ) << std::endl;
      std::cout << b( 1, 0 ) << "\t" << b( 1, 1 ) << std::endl;
      std::cout << std::endl;

    }

    // base polar converter
    {

      std::complex< float > a( 1.0, 1.0 );
      double m, p;
      gkg::PolarConverter< std::complex< float >,
                                          double > polarConverter;
      polarConverter.convert( a, m, p );
      std::cout << "( m, p ) = ( " << m << ", " << p << " )" << std::endl;
      std::cout << std::endl;

    }

    // base polar converter
    {

      float a = -0.6;
      double m, p;
      gkg::PolarConverter< float, double > polarConverter;
      polarConverter.convert( a, m, p );
      std::cout << "( m, p ) = ( " << m << ", " << p << " )" << std::endl;
      std::cout << std::endl;

    }


  }
  GKG_CATCH_COMMAND( result );

  return result;

}
