#include <gkg-processing-numericalanalysis/ParametricFunction.h>


gkg::ParametricFunction::ParametricFunction( const Vector& parameters )
                       : _parameters( parameters )
{
}


gkg::ParametricFunction::~ParametricFunction()
{
}


const gkg::Vector& gkg::ParametricFunction::getParameters() const
{

  return _parameters;

}
