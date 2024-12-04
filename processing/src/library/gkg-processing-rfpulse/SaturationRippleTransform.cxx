#include <gkg-processing-rfpulse/SaturationRippleTransform.h>
#include <cmath>


gkg::SaturationRippleTransform::SaturationRippleTransform()
                               : gkg::RippleTransform()
{
}


gkg::SaturationRippleTransform::~SaturationRippleTransform()
{
}


double gkg::SaturationRippleTransform::getPassBandRipple(
                                           double effectivePassBandRipple ) const
{

  return effectivePassBandRipple / 2.0;

}


double gkg::SaturationRippleTransform::getStopBandRipple(
                                           double effectiveStopBandRipple ) const
{

  return ( double )std::sqrt( effectiveStopBandRipple );

}
