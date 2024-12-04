#include <gkg-processing-rfpulse/InversionRippleTransform.h>
#include <cmath>


gkg::InversionRippleTransform::InversionRippleTransform()
                              : gkg::RippleTransform()
{
}


gkg::InversionRippleTransform::~InversionRippleTransform()
{
}


double gkg::InversionRippleTransform::getPassBandRipple(
                                           double effectivePassBandRipple ) const
{

  return effectivePassBandRipple / 8.0;

}


double gkg::InversionRippleTransform::getStopBandRipple(
                                           double effectiveStopBandRipple ) const
{

  return ( double )std::sqrt( effectiveStopBandRipple / 2.0 );

}
