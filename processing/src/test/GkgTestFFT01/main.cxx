#include <gkg-processing-signal/FourierTransform.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-processing-signal/TukeyApodization.h>
#include <gkg-processing-plot/CurvePlotter.h>
#include <gkg-processing-plot/PlotterFactory.h>
#include <gkg-communication-sysinfo/KeyboardHit.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::cout << "testing FFT : " << std::flush;
    {

      std::vector< std::complex< double > > input( 128 );

      size_t i;
      for ( i = 0; i < input.size(); i++ )
      {

        if ( ( i < 32 ) || ( 127 - i < 32 ) )
        {

          input[ i ] = 0.0;

        }
        else
        {

          input[ i ] = 1.0;

        }

      }

      gkg::CurvePlotter gnuPlotCurvePlotter1;
        gnuPlotCurvePlotter1.plot( input, "x", "input[x]", "input" );

      gkg::FourierTransform< double >::getInstance().applyCenteredDirect(
                                                                    input );


      gkg::Apodization< double >*
        apodization = 
          gkg::ApodizationFactory< double >::getInstance().createApodization(
            gkg::TukeyApodization< double >::getStaticName(),
            128,
            gkg::Apodization< double >::Centered );

      for ( i = 0; i < input.size(); i++ )
      {

        input[ i ] *= apodization->getCoefficient( i );

      }
      delete apodization;
      gkg::FourierTransform< double >::getInstance().applyCenteredInverse(
                                                                     input );

      gkg::CurvePlotter gnuPlotCurvePlotter2;
      gnuPlotCurvePlotter2.plot( input, "x", "ifft[x]", "ifft" );

      gkg::keyboardHit();

    }

    std::cout << "testing DFT : " << std::flush;
    {

      std::vector< std::complex< double > > input( 100 );

      size_t i;
      for ( i = 0; i < input.size(); i++ )
      {

        if ( ( i < 32 ) || ( 100 - i < 32 ) )
        {

          input[ i ] = 0.0;

        }
        else
        {

          input[ i ] = 1.0;

        }

      }

      gkg::CurvePlotter gnuPlotCurvePlotter1;
        gnuPlotCurvePlotter1.plot( input, "x", "input[x]", "input" );

      gkg::FourierTransform< double >::getInstance().applyCenteredDirect(
                                                                    input );

      gkg::CurvePlotter gnuPlotCurvePlotter2;
      gnuPlotCurvePlotter2.plot( input, "x", "fft[x]", "fft" );

      gkg::Apodization< double >*
        apodization = 
          gkg::ApodizationFactory< double >::getInstance().createApodization(
            gkg::TukeyApodization< double >::getStaticName(),
            100,
            gkg::Apodization< double >::Centered );

      std::vector< double > coefficients( input.size() );
      for ( i = 0; i < input.size(); i++ )
      {

        coefficients[ i ] = apodization->getCoefficient( i );

      }
      gkg::CurvePlotter gnuPlotCurvePlotter3;
      gnuPlotCurvePlotter3.plot( coefficients, "x", "apo[x]", "apodization" );


      for ( i = 0; i < input.size(); i++ )
      {

        input[ i ] *= coefficients[ i ];

      }
      delete apodization;
      gkg::FourierTransform< double >::getInstance().applyCenteredInverse(
                                                                     input );

      gkg::CurvePlotter gnuPlotCurvePlotter4;
      gnuPlotCurvePlotter4.plot( input, "x", "ifft[x]", "ifft" );

      gkg::keyboardHit();

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
