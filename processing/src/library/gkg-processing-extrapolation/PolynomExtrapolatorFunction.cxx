#include <gkg-processing-extrapolation/PolynomExtrapolatorFunction.h>
#include <cmath>


gkg::PolynomExtrapolatorFunction::PolynomExtrapolatorFunction()
                                      : gkg::ExtrapolatorFunction()
{
}


gkg::PolynomExtrapolatorFunction::~PolynomExtrapolatorFunction()
{
}


double 
gkg::PolynomExtrapolatorFunction::getValue( int32_t x, int32_t degree, 
                                                       int32_t size ) const
{

  return std::pow( x, ( double )degree ) /
         std::pow( ( double )size, ( double )degree );

}
