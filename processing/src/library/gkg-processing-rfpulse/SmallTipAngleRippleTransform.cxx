#include <gkg-processing-rfpulse/SmallTipAngleRippleTransform.h>
#include <cmath>


gkg::SmallTipAngleRippleTransform::SmallTipAngleRippleTransform()
                                  : gkg::RippleTransform()
{
}


gkg::SmallTipAngleRippleTransform::~SmallTipAngleRippleTransform()
{
}


double gkg::SmallTipAngleRippleTransform::getPassBandRipple(
                                           double effectivePassBandRipple ) const
{

  return effectivePassBandRipple;

}


double gkg::SmallTipAngleRippleTransform::getStopBandRipple(
                                           double effectiveStopBandRipple ) const
{

  return effectiveStopBandRipple;

}
