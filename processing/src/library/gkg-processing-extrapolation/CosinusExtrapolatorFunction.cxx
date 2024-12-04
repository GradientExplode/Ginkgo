#include <gkg-processing-extrapolation/CosinusExtrapolatorFunction.h>
#include <cmath>


gkg::CosinusExtrapolatorFunction::CosinusExtrapolatorFunction()
                                 : gkg::ExtrapolatorFunction()
{
}


gkg::CosinusExtrapolatorFunction::~CosinusExtrapolatorFunction()
{
}


double 
gkg::CosinusExtrapolatorFunction::getValue( int32_t x, int32_t degree, 
                                                       int32_t size ) const
{

  return std::cos( M_PI * x * ( ( double )degree / ( double )size ) );

}

