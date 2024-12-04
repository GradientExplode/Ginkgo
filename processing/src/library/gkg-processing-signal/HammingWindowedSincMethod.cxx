#include <gkg-processing-signal/HammingWindowedSincMethod.h>
#include <gkg-processing-signal/HammingApodization.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <cmath>


gkg::HammingWindowedSincMethod::HammingWindowedSincMethod()
                               : gkg::FIRFilterDesignMethod()
{
}


gkg::HammingWindowedSincMethod::~HammingWindowedSincMethod()
{
}


std::vector< std::complex< double > >
gkg::HammingWindowedSincMethod::getImpulseResponse(
                                               int32_t sampleCount,
                                               double timeBandWidth,
                                               double /*passBandRipple*/,
                                               double /*stopBandRipple*/ ) const
{

  std::vector< std::complex< double > > impulseResponse( sampleCount );

  int32_t i;
  double x, sinc, hamming;
  double middle = timeBandWidth / 4;

  gkg::Apodization< double >*
    apodization =
      gkg::ApodizationFactory< double >::getInstance().createApodization(
        gkg::HammingApodization< double >::getStaticName(),
        sampleCount,
        gkg::Apodization< double >::Centered );

  for ( i = 0; i < sampleCount; i++ )
  {

    x = ( - ( double )sampleCount / 2.0 + ( double )i ) /
        ( ( double )sampleCount / 2.0 );
    sinc = std::sin( middle * 2 * M_PI * x + 0.00001 ) /
           ( middle * 2 * M_PI * x + 0.00001 );
    hamming = apodization->getCoefficient( i );
    impulseResponse[ i ] = sinc * hamming * 4 * middle / ( double )sampleCount;

  }
  delete apodization;

  return impulseResponse;

}
