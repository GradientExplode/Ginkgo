#include <gkg-processing-signal/ParksMcClellanMaximumPhaseMethod.h>
#include <gkg-processing-signal/Remez.h>
#include <gkg-processing-signal/FourierTransform.h>
#include <fstream>


gkg::ParksMcClellanMaximumPhaseMethod::ParksMcClellanMaximumPhaseMethod()
                                      : gkg::FIRFilterDesignMethod()
{
}


gkg::ParksMcClellanMaximumPhaseMethod::~ParksMcClellanMaximumPhaseMethod()
{
}


std::vector< std::complex< double > >
gkg::ParksMcClellanMaximumPhaseMethod::getImpulseResponse(
                                                   int32_t sampleCount,
                                                   double timeBandWidth,
                                                   double passBandRipple,
                                                   double stopBandRipple ) const
{

  std::vector< std::complex< double > > impulseResponseLinear;
  std::vector< std::complex< double > > impulseResponse;

  // processing normalized half transition bandwidth
  double w = 0.5 * getDInfinity( 2 * passBandRipple,
                                 0.5 * stopBandRipple * stopBandRipple ) /
             timeBandWidth;

  // building the frequencies
  std::vector< double > frequencies( 4 );
  frequencies[ 0 ] = 0.0f;
  frequencies[ 1 ] = ( 1 - w ) * ( timeBandWidth / 2.0f ) / ( sampleCount / 2 );
  frequencies[ 2 ] = ( 1 + w ) * ( timeBandWidth / 2.0f ) / ( sampleCount / 2 );
  frequencies[ 3 ] = 1.0f;
            
  // building the corresponding magnitudes
  std::vector< double > magnitudes( 4 );
  magnitudes[ 0 ] = 1.0f;
  magnitudes[ 1 ] = 1.0f;
  magnitudes[ 2 ] = 0.0f;
  magnitudes[ 3 ] = 0.0f;

  // building weights applied to each band
  std::vector< double > weights( 2 );
  weights[ 0 ] = 1.0f;
  weights[ 1 ] = 2.0 * passBandRipple /
                 ( 0.5 * stopBandRipple * stopBandRipple );


  // calculating the optimal "Chebychev" filter with Remez' exchange algorithm
  gkg::Remez remez( gkg::Remez::BandPass );
  remez.getOptimalFilter( impulseResponseLinear,
                          ( 2 * sampleCount - 1 ) - 1,
                          frequencies,
                          magnitudes,
                          weights );

  getMaxPhaseFromLinPhaseResponse( impulseResponse, impulseResponseLinear );

  return impulseResponse;

}


double gkg::ParksMcClellanMaximumPhaseMethod::getDInfinity(
                                                   double passBandRipple,
                                                   double stopBandRipple ) const
{

  double a1 = 5.309e-3;
  double a2 = 7.114e-2;
  double a3 = -4.761e-1;
  double a4 = -2.66e-3;
  double a5 = -5.941e-1;
  double a6 = -4.278e-1;

  double L1 = ( double )log10( passBandRipple );
  double L2 = ( double )log10( stopBandRipple );

  return ( a1 * L1 * L1 + a2 * L1 + a3 ) * L2 + ( a4 * L1 * L1 + a5 * L1 + a6 );


}


void gkg::ParksMcClellanMaximumPhaseMethod::getMaxPhaseFromLinPhaseResponse( 
      std::vector< std::complex< double > >& impulseResponse,
      const std::vector< std::complex< double > >& impulseResponseLinear ) const
{

  int32_t l = ( int32_t )impulseResponseLinear.size();
  double lp = 8.0 * std::exp( std::ceil( std::log( ( double )l ) /
                                         std::log( 2.0 ) ) * std::log( 2.0 ) );
  int32_t zeroCount1 = ( int32_t )( std::ceil( ( lp - l ) / 2.0 ) + 0.5 );
  int32_t zeroCount2 = ( int32_t )( std::floor( ( lp - l ) / 2.0 ) + 0.5 );

  std::vector< std::complex< double > > hp( zeroCount1 + l + zeroCount2,
                                            std::complex< double >( 0.0,0.0 ) );

  int32_t i;
  for ( i = 0; i < l; i++ )
  {

    hp[ zeroCount1 + i ] = impulseResponseLinear[ i ];

  }

  // applying Fourier tranform with 0 at the center of hp
  gkg::FourierTransform< double >::getInstance().applyCenteredDirect( hp );
  for ( i = 1; i < ( int32_t )hp.size(); i += 2 )
  {

    hp[ i ] *= -1.0f;

  }

  double minimum = std::real( hp[ i ] );
  for ( i = 1; i < ( int32_t )hp.size(); i++ )
  {

    if ( std::real( hp[ i ] ) < minimum )
    {

      minimum = std::real( hp[ i ] );

    }

  }

  for ( i = 0; i < ( int32_t )hp.size(); i++ )
  {

    hp[ i ] = hp[ i ] - minimum * 1.000001;

  }

  for ( i = 0; i < ( int32_t )hp.size(); i++ )
  {

    hp[ i ] = std::complex< double >( 
                                 std::sqrt( std::sqrt( std::norm( hp[ i ] ) ) ),
                                 0.0 );

  }

  // fft of analytic signal
  for ( i = 0; i < ( int32_t )hp.size(); i++ )
  {

    hp[ i ] = std::complex< double >( std::log( std::real( hp[ i ] ) ), 0.0 );

  }


  gkg::FourierTransform< double >::getInstance().applyRawDirect( hp );
  for ( i = 1; i < ( int32_t )hp.size() / 2; i++ )
  {

    hp[ i ] = hp[ i ] * 2.0;

  }

  for ( i = ( int32_t )hp.size() / 2 + 1; i < ( int32_t )hp.size(); i++ )
  {

    hp[ i ] = 0.0;

  }

  for ( i = 0; i < ( int32_t )hp.size(); i++ )
  {

    hp[ i ] /= (double)hp.size();

  }

  gkg::FourierTransform< double >::getInstance().applyRawInverse( hp );

  for ( i = 0; i < ( int32_t )hp.size(); i ++ )
  {

    hp[ i ] = std::conj( hp[ i ] );

  }

  for ( i = 0; i < ( int32_t )hp.size(); i++ )
  {

    hp[ i ] = std::exp( hp[ i ] );

  }

  for ( i = 0; i < ( int32_t )hp.size() / 2; i++ )
  {

    std::swap( hp[ i ], hp[ i + ( int32_t )hp.size() / 2 ] );

  }

  gkg::FourierTransform< double >::getInstance().applyRawInverse( hp );
  for ( i = 0; i < ( int32_t )hp.size(); i ++ )
  {

    hp[ i ] = std::conj( hp[ i ] );

  }
  for ( i = 0; i < ( int32_t )hp.size(); i++ )
  {

    hp[ i ] /= ( double )hp.size();

  }

  impulseResponse = std::vector< std::complex< double > >( ( l + 1 ) / 2 );
  for ( i = 0; i < ( int32_t )impulseResponse.size(); i++ )
  {

    impulseResponse[ i ] = hp[ i ];

  }

}
