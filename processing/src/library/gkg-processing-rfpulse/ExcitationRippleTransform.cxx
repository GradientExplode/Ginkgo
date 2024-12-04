#include <gkg-processing-rfpulse/ExcitationRippleTransform.h>
#include <cmath>


gkg::ExcitationRippleTransform::ExcitationRippleTransform()
                               : gkg::RippleTransform()
{
}


gkg::ExcitationRippleTransform::~ExcitationRippleTransform()
{
}


double gkg::ExcitationRippleTransform::getPassBandRipple(
                                           double effectivePassBandRipple ) const
{

  return ( double )std::sqrt( effectivePassBandRipple / 2.0 );

}


double gkg::ExcitationRippleTransform::getStopBandRipple(
                                           double effectiveStopBandRipple ) const
{

  return effectiveStopBandRipple / ( double )std::sqrt( 2.0 );

}
