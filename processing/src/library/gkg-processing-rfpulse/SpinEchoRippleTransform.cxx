#include <gkg-processing-rfpulse/SpinEchoRippleTransform.h>
#include <cmath>


gkg::SpinEchoRippleTransform::SpinEchoRippleTransform()
                             : gkg::RippleTransform()
{
}


gkg::SpinEchoRippleTransform::~SpinEchoRippleTransform()
{
}


double gkg::SpinEchoRippleTransform::getPassBandRipple(
                                           double effectivePassBandRipple ) const
{

  return effectivePassBandRipple / 4.0;

}


double gkg::SpinEchoRippleTransform::getStopBandRipple(
                                           double effectiveStopBandRipple ) const
{

  return ( double )std::sqrt( effectiveStopBandRipple );

}
