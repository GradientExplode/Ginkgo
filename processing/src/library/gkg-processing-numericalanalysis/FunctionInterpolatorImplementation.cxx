#include <gkg-processing-numericalanalysis/FunctionInterpolatorImplementation.h>


gkg::FunctionInterpolatorImplementation::FunctionInterpolatorImplementation( 
                                gkg::FunctionInterpolator* functionInterpolator,
                                gkg::FunctionInterpolator::Type /*type*/,
                                const std::vector< double >& /*xs*/,
                                const std::vector< double >& /*ys*/ )
                                        : _target( functionInterpolator )
{
}


gkg::FunctionInterpolatorImplementation::FunctionInterpolatorImplementation( 
                                gkg::FunctionInterpolator* functionInterpolator,
                                gkg::FunctionInterpolator::Type /*type*/,
                                const gkg::Vector& /*xs*/,
                                const gkg::Vector& /*ys*/ )
                                        : _target( functionInterpolator )
{
}


gkg::FunctionInterpolatorImplementation::~FunctionInterpolatorImplementation()
{
}


gkg::FunctionInterpolator* 
gkg::FunctionInterpolatorImplementation::getTarget() const
{

  return _target;

}
