#include <gkg-mri-reconstruction-correction/TrapezoidROGradient.h>
#include <gkg-mri-reconstruction-correction/RegridingCorrection.h>
#include <gkg-processing-plot/CurvePlotter.h>
#include <gkg-processing-plot/PlotterFactory.h>
#include <gkg-processing-signal/FourierTransform.h>
#include <gkg-processing-signal/FourierTransform.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-communication-sysinfo/KeyboardHit.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <complex>



int16_t dataLineS16[ 488 ][ 2 ] =
{

  { 4, 2 },
  { 1, 0 },
  { 2, 3 },
  { 3, 6 },
  { 3, 5 },
  { 0, 5 },
  { 1, 3 },
  { 2, 1 },
  { 0, 4 },
  { 0, 3 },
  { 2, 2 },
  { 0, 1 },
  { 1, -1 },
  { 4, -1 },
  { 1, -1 },
  { 0, 0 },
  { 3, -3 },
  { 3, 0 },
  { -2, 3 },
  { -4, 0 },
  { -1, 0 },
  { -3, 0 },
  { -6, -2 },
  { -2, -3 },
  { -3, -2 },
  { 0, -3 },
  { 1, -4 },
  { 0, -2 },
  { 0, -4 },
  { 1, -2 },
  { -2, 3 },
  { 0, 0 },
  { 2, 0 },
  { 1, 2 },
  { 1, 1 },
  { 3, 2 },
  { 0, 3 },
  { 1, 2 },
  { 2, 1 },
  { 1, 1 },
  { 2, 1 },
  { 4, -2 },
  { 1, -3 },
  { 1, -1 },
  { 2, -4 },
  { 0, -5 },
  { -1, -2 },
  { -2, -4 },
  { -3, -4 },
  { 0, -1 },
  { -1, -3 },
  { 0, -3 },
  { 2, -1 },
  { 1, 0 },
  { 2, 0 },
  { 4, 0 },
  { -1, 0 },
  { -2, 1 },
  { 2, 2 },
  { 0, 3 },
  { 0, 1 },
  { -2, 1 },
  { -3, -1 },
  { -2, -2 },
  { -5, -2 },
  { -8, -1 },
  { -4, -3 },
  { -2, -5 },
  { -1, -2 },
  { -1, -1 },
  { 0, 3 },
  { 2, 5 },
  { 2, 0 },
  { 2, -2 },
  { 2, 0 },
  { 0, 1 },
  { -4, -1 },
  { -2, -3 },
  { -1, -2 },
  { -1, 0 },
  { -3, -3 },
  { -5, -4 },
  { -2, 0 },
  { 1, 2 },
  { -2, 0 },
  { 2, 1 },
  { 7, 0 },
  { 5, 0 },
  { 5, 0 },
  { 4, 1 },
  { 0, 1 },
  { -1, -2 },
  { -2, -2 },
  { -4, -3 },
  { -5, -2 },
  { -5, 1 },
  { -4, 1 },
  { -1, -3 },
  { 2, 1 },
  { 5, 2 },
  { 4, -1 },
  { 4, -4 },
  { 1, -2 },
  { -1, 0 },
  { -3, -1 },
  { -4, -3 },
  { -3, -3 },
  { -3, -2 },
  { -3, 0 },
  { 4, 0 },
  { 8, 0 },
  { 6, 0 },
  { 4, -2 },
  { 2, -3 },
  { 0, 1 },
  { -3, 1 },
  { -5, 1 },
  { -4, -1 },
  { -1, -2 },
  { 1, 1 },
  { 4, 0 },
  { 6, -2 },
  { 5, -2 },
  { 0, 1 },
  { 0, 2 },
  { -2, 2 },
  { -6, 2 },
  { -3, 0 },
  { 0, -2 },
  { 2, -2 },
  { 3, -2 },
  { 5, 0 },
  { 5, 0 },
  { 2, 0 },
  { 0, 3 },
  { -3, 6 },
  { -4, 4 },
  { -1, 3 },
  { 1, 2 },
  { 5, -4 },
  { 6, -6 },
  { 2, -1 },
  { -2, -2 },
  { -2, 0 },
  { -4, 2 },
  { -5, 4 },
  { -3, 4 },
  { 1, 0 },
  { 3, -2 },
  { 2, -5 },
  { 1, -6 },
  { -1, -2 },
  { -5, 4 },
  { -7, 9 },
  { -5, 6 },
  { 0, 0 },
  { 2, -5 },
  { 2, -5 },
  { 5, -6 },
  { 3, -1 },
  { -5, 5 },
  { -6, 4 },
  { -2, 4 },
  { -2, 1 },
  { -1, -4 },
  { 3, -7 },
  { 0, -5 },
  { 0, 1 },
  { 0, 4 },
  { -2, 6 },
  { 1, 4 },
  { 3, -1 },
  { 1, -9 },
  { 1, -12 },
  { 1, -5 },
  { 2, 2 },
  { 1, 7 },
  { 0, 8 },
  { 2, 4 },
  { 2, -7 },
  { 2, -15 },
  { 0, -10 },
  { -3, -2 },
  { -2, 10 },
  { 0, 14 },
  { 3, 3 },
  { 0, -7 },
  { 0, -12 },
  { 1, -7 },
  { 2, 2 },
  { 5, 10 },
  { 7, 12 },
  { 5, 4 },
  { -2, -9 },
  { -8, -15 },
  { -5, -11 },
  { 1, 2 },
  { 4, 16 },
  { 7, 17 },
  { 4, 0 },
  { -5, -14 },
  { -8, -14 },
  { -3, -6 },
  { 7, 9 },
  { 14, 18 },
  { 9, 12 },
  { 0, -3 },
  { -12, -19 },
  { -19, -17 },
  { -10, -2 },
  { 8, 15 },
  { 21, 20 },
  { 13, 7 },
  { -8, -12 },
  { -24, -21 },
  { -20, -15 },
  { 2, 5 },
  { 31, 22 },
  { 41, 20 },
  { 17, 6 },
  { -20, -14 },
  { -43, -22 },
  { -35, -9 },
  { 5, 15 },
  { 48, 30 },
  { 58, 24 },
  { 20, -1 },
  { -41, -25 },
  { -69, -23 },
  { -30, 3 },
  { 56, 37 },
  { 136, 47 },
  { 142, 25 },
  { 46, -17 },
  { -103, -46 },
  { -208, -36 },
  { -183, 26 },
  { -12, 93 },
  { 218, 118 },
  { 384, 76 },
  { 395, -34 },
  { 250, -171 },
  { 50, -284 },
  { -77, -332 },
  { -38, -309 },
  { 134, -243 },
  { 326, -154 },
  { 406, -67 },
  { 312, -2 },
  { 95, 48 },
  { -124, 75 },
  { -235, 71 },
  { -200, 37 },
  { -63, -11 },
  { 74, -49 },
  { 129, -62 },
  { 84, -35 },
  { -7, 5 },
  { -79, 32 },
  { -84, 33 },
  { -34, 11 },
  { 25, -19 },
  { 47, -33 },
  { 26, -19 },
  { -14, 9 },
  { -40, 27 },
  { -38, 25 },
  { -6, 2 },
  { 29, -24 },
  { 38, -30 },
  { 22, -14 },
  { -8, 6 },
  { -26, 24 },
  { -17, 21 },
  { 3, -3 },
  { 18, -18 },
  { 16, -20 },
  { 3, -7 },
  { -11, 16 },
  { -18, 27 },
  { -12, 11 },
  { 4, -12 },
  { 12, -22 },
  { 10, -19 },
  { 4, -3 },
  { -6, 12 },
  { -14, 16 },
  { 0, 6 },
  { 9, -9 },
  { 6, -18 },
  { 4, -11 },
  { 0, 7 },
  { -6, 17 },
  { -6, 14 },
  { 0, 2 },
  { 2, -11 },
  { 3, -15 },
  { 0, -8 },
  { -3, 5 },
  { -1, 13 },
  { 1, 11 },
  { 3, 0 },
  { 2, -9 },
  { 1, -12 },
  { 0, -5 },
  { -1, 7 },
  { 0, 12 },
  { -1, 12 },
  { 1, 3 },
  { 1, -9 },
  { -2, -13 },
  { 1, -7 },
  { 1, 1 },
  { 2, 6 },
  { 1, 9 },
  { 2, 1 },
  { 1, -5 },
  { -1, -8 },
  { -2, -7 },
  { 0, -3 },
  { 5, 3 },
  { 4, 9 },
  { 3, 7 },
  { 2, 0 },
  { -1, -6 },
  { -5, -10 },
  { -5, -6 },
  { -2, -1 },
  { 1, 3 },
  { 4, 7 },
  { 5, 5 },
  { 3, -3 },
  { -3, -6 },
  { -5, -7 },
  { -2, -5 },
  { -3, -1 },
  { 0, 0 },
  { 1, 3 },
  { 5, 3 },
  { 5, 1 },
  { 1, 0 },
  { -3, -4 },
  { -7, -6 },
  { -3, -4 },
  { 2, -1 },
  { 4, 1 },
  { 5, 6 },
  { 4, 5 },
  { 0, -2 },
  { 0, -2 },
  { -3, -3 },
  { -4, -4 },
  { -1, -2 },
  { 2, -3 },
  { 6, -1 },
  { 5, 3 },
  { 2, 3 },
  { 3, 3 },
  { -2, 1 },
  { -8, -2 },
  { -10, -3 },
  { -8, -3 },
  { 1, -1 },
  { 2, 0 },
  { 3, 1 },
  { 5, 1 },
  { 6, -1 },
  { 2, -2 },
  { -2, 2 },
  { -2, -2 },
  { -4, -5 },
  { -9, 0 },
  { -5, 2 },
  { 3, 1 },
  { 5, 1 },
  { 4, 0 },
  { 4, -1 },
  { -1, -2 },
  { -3, -1 },
  { -1, -4 },
  { -2, -2 },
  { -2, 2 },
  { -3, -1 },
  { -2, -4 },
  { 1, -3 },
  { 3, -1 },
  { 6, 0 },
  { 5, 0 },
  { 4, 1 },
  { 2, 2 },
  { -4, 0 },
  { -4, -2 },
  { -6, 0 },
  { -4, 0 },
  { 0, -1 },
  { -1, 0 },
  { -1, 0 },
  { 1, -5 },
  { 3, -5 },
  { 5, -2 },
  { 2, -2 },
  { 2, -2 },
  { 2, -1 },
  { -3, 0 },
  { -4, -1 },
  { -6, -1 },
  { -5, 2 },
  { -3, 2 },
  { 1, 0 },
  { 0, 0 },
  { 1, -1 },
  { 3, -2 },
  { 6, 0 },
  { 1, -2 },
  { -1, -4 },
  { 2, -2 },
  { 1, -1 },
  { -3, 0 },
  { -4, 1 },
  { -1, 1 },
  { -3, -1 },
  { -7, 1 },
  { -2, 1 },
  { -2, -2 },
  { -3, 0 },
  { 2, 0 },
  { 4, -1 },
  { 2, 1 },
  { 5, -1 },
  { 4, -3 },
  { 2, -3 },
  { 4, -3 },
  { 1, -6 },
  { -2, -1 },
  { -3, 3 },
  { -5, 0 },
  { -2, 0 },
  { -1, 0 },
  { 1, 2 },
  { 0, 2 },
  { -1, -1 },
  { -1, 3 },
  { 0, 3 },
  { 1, 0 },
  { 1, 3 },
  { 3, 3 },
  { 3, 2 },
  { 4, 0 },
  { 5, -2 },
  { 7, -4 },
  { 2, -4 },
  { -2, -1 },
  { 1, 0 },
  { 1, -1 },
  { 1, -2 },
  { -1, -1 },
  { -1, 1 },
  { -1, 3 },
  { 1, 1 },
  { -4, 2 },
  { -3, 3 },
  { 0, 1 },
  { -1, 2 },
  { -3, 0 },
  { -1, 0 },
  { -2, 1 },
  { -3, 0 },
  { 0, -1 },
  { -1, 0 },
  { 1, -2 },
  { 2, -3 },
  { 1, -4 },
  { 3, -5 },
  { 4, -5 },
  { 2, -1 },
  { 4, -2 },
  { 4, -2 },
  { -2, -3 },
  { -1, -4 },
  { 3, -4 },
  { 3, -2 },
  { 4, -4 },
  { 3, -3 },
  { 0, -1 },
  { 1, -2 },
  { 2, -5 },
  { 1, -1 },
  { -2, 0 }

};


std::vector< double > 
getModule( const std::vector< std::complex< double > >& in )
{

  std::vector< double > out( in.size() );

  for ( size_t i = 0; i < in.size(); i++ )
  {

    out[ i ] = std::sqrt( std::norm( in[ i ] ) );

  }
  return out;

}


int main( int /*argc*/, char* argv[] )
{


  int32_t regridSize = atoi( argv[ 1 ] );
  int32_t vrgfxres = 128;

  std::vector< std::complex< double > > dataLine( 488 );
  int32_t i;
  for ( i = 0; i < 488; i++ )
  {

    dataLine[ i ] = std::complex< double >( ( double )dataLineS16[ i ][ 0 ],
                                            ( double )dataLineS16[ i ][ 1 ] );

  }

  // displaying raw line without x axis management

  gkg::CurvePlotter gnuPlotCurvePlotter1;
  gnuPlotCurvePlotter1.plot( getModule( dataLine ), "kx", "mag", "raw line" );


  // displaying raw line with x axis management
  gkg::TrapezoidROGradient< double > gradient( 488, 268, 84 );


  gkg::CurvePlotter gnuPlotCurvePlotter2;
  gnuPlotCurvePlotter2.plot( gradient.getTrajectory(),
                             getModule( dataLine ), "kx", "mag",
                             "raw line with x" );


  // regriding
  gkg::RegridingCorrection< double > 
    regridingCorrection( gradient.getTrajectory() );

  std::cout << "effective size: " << regridingCorrection.getEffectiveSize( 0 )
            << std::endl;

  regridingCorrection.setOutputSampleCount( regridSize,
                                            ( float )vrgfxres );

  std::vector< std::complex< double > > regridedLine( regridSize );
  regridingCorrection.apply( dataLine, regridedLine );


  gkg::CurvePlotter gnuPlotCurvePlotter3;
  gnuPlotCurvePlotter3.plot( getModule( regridedLine ), "kx", "mag",
                             "regrided line to regridSize points" );

  // applying Fourier
  int32_t powerOfTwo = 1 << ( int32_t )std::ceil( 
                                              std::log( ( double )regridSize ) /
                                              std::log( 2.0 ) );
  std::cout << "power of two : " << powerOfTwo << std::endl;
  std::vector< std::complex< double > > regridedLinePowerOfTwo( powerOfTwo );
  for ( i = 0; i < powerOfTwo; i++ )
  {

    if ( i < regridSize )
    {

      regridedLinePowerOfTwo[ i ] = regridedLine[ i ];

    }
    else
    {

      regridedLinePowerOfTwo[ i ] = std::complex< double >( 0.0, 0.0 );

    }

  }

  gkg::Apodization< double >*
    apodization = gkg::ApodizationFactory< double >::getInstance().
                 createApodization( 
                                   "tukey", regridSize,
                                   gkg::Apodization< double >::Centered );
  gkg::FourierTransform< double >::getInstance().applyCenteredInverse(
                                          regridedLinePowerOfTwo, apodization );


  gkg::CurvePlotter gnuPlotCurvePlotter4;
  gnuPlotCurvePlotter4.plot( getModule( regridedLinePowerOfTwo ), "x", "mag",
                             "IFFT regridSize points" );

  gkg::keyboardHit();

  return EXIT_SUCCESS;

}
