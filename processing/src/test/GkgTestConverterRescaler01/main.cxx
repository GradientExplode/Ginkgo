#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    // testing base converter
    {

      float a = 125.3;
      int16_t b;
      gkg::Converter< float, int16_t > converter;
      converter.convert( a, b );
      std::cout << "b = " << b << std::endl;
      std::cout << std::endl;

    }

    // testing base rescaler
    {

      float a = 125.3;
      int16_t b;
      gkg::Rescaler< float, int16_t > rescaler( 100.0, 150.0, 
                                                ( int16_t )1, ( int16_t )10 );
      rescaler.rescale( a, b );
      std::cout << "b = " << b << std::endl;
      std::cout << std::endl;

    }

    // testing volume converter with different types
    {

      gkg::Volume< float > a( 2, 2 );
      a( 0, 0 ) = 2.5;
      a( 0, 1 ) = 3.8;
      a( 1, 0 ) = 5;
      a( 1, 1 ) = 7.2;

      gkg::Volume< int16_t > b;
      gkg::Converter< gkg::Volume< float >, gkg::Volume< int16_t > > converter;
      converter.convert( a, b );

      std::cout << b( 0, 0 ) << "\t" << b( 0, 1 ) << std::endl;
      std::cout << b( 1, 0 ) << "\t" << b( 1, 1 ) << std::endl;
      std::cout << std::endl;

    }

    // testing volume converter with same type
    {

      gkg::Volume< float > a( 2, 2 );
      a( 0, 0 ) = 2.5;
      a( 0, 1 ) = 3.8;
      a( 1, 0 ) = 5;
      a( 1, 1 ) = 7.2;

      gkg::Volume< float > b;
      gkg::Converter< gkg::Volume< float >, gkg::Volume< float > > converter;
      converter.convert( a, b );

      std::cout << b( 0, 0 ) << "\t" << b( 0, 1 ) << std::endl;
      std::cout << b( 1, 0 ) << "\t" << b( 1, 1 ) << std::endl;
      std::cout << std::endl;

    }

    // testing volume rescaler with different types
    {

      gkg::Volume< float > a( 2, 2 );
      a( 0, 0 ) = 2.5;
      a( 0, 1 ) = 3.8;
      a( 1, 0 ) = 5;
      a( 1, 1 ) = 7.2;

      gkg::Volume< int16_t > b;
      gkg::Rescaler< gkg::Volume< float >, gkg::Volume< int16_t > >
        rescaler( 1.0, 10.0 );
      rescaler.rescale( a, b );

      std::cout << b( 0, 0 ) << "\t" << b( 0, 1 ) << std::endl;
      std::cout << b( 1, 0 ) << "\t" << b( 1, 1 ) << std::endl;
      std::cout << std::endl;

    }

    // testing volume rescaler with same type
    {

      gkg::Volume< float > a( 2, 2 );
      a( 0, 0 ) = 2.5;
      a( 0, 1 ) = 3.8;
      a( 1, 0 ) = 5;
      a( 1, 1 ) = 7.2;

      gkg::Volume< float > b;
      gkg::Rescaler< gkg::Volume< float >, gkg::Volume< float > >
        rescaler( 1.0, 10.0, 1.0, 10.0 );
      rescaler.rescale( a, b );

      std::cout << b( 0, 0 ) << "\t" << b( 0, 1 ) << std::endl;
      std::cout << b( 1, 0 ) << "\t" << b( 1, 1 ) << std::endl;
      std::cout << std::endl;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
