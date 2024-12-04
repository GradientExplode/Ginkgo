#include <gkg-processing-signal/ParksMcClellanLinearPhaseMethod.h>
#include <gkg-processing-signal/Remez.h>
#include <fstream>


gkg::ParksMcClellanLinearPhaseMethod::ParksMcClellanLinearPhaseMethod()
                                     : gkg::FIRFilterDesignMethod()
{
}


gkg::ParksMcClellanLinearPhaseMethod::~ParksMcClellanLinearPhaseMethod()
{
}


std::vector< std::complex< double > >
gkg::ParksMcClellanLinearPhaseMethod::getImpulseResponse(
                                               int32_t sampleCount,
                                               double timeBandWidth,
                                               double passBandRipple,
                                               double stopBandRipple ) const
{

  std::vector< std::complex< double > > impulseResponse;

  // processing normalized half transition bandwidth
  double w = getDInfinity( passBandRipple, stopBandRipple ) / timeBandWidth;

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
  weights[ 1 ] = passBandRipple / stopBandRipple;

  // calculating the optimal "Chebychev" filter with Remez' exchange algorithm
  gkg::Remez remez( gkg::Remez::BandPass );
  remez.getOptimalFilter( impulseResponse,
                          sampleCount - 1,
                          frequencies,
                          magnitudes,
                          weights );

  return impulseResponse;

}


double gkg::ParksMcClellanLinearPhaseMethod::getDInfinity(
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
