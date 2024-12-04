#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>


gkg::LevenbergMarquardtFunction::LevenbergMarquardtFunction(
                                                 const gkg::Vector& parameters )
                                : _parameters( parameters )
{
}


gkg::LevenbergMarquardtFunction::~LevenbergMarquardtFunction()
{
}


const gkg::Vector& gkg::LevenbergMarquardtFunction::getParameters() const
{

  return _parameters;

}


gkg::Vector& gkg::LevenbergMarquardtFunction::getParameters()
{

  return _parameters;

}


double 
gkg::LevenbergMarquardtFunction::getValueAt( const gkg::Vector& x ) const
{

  return this->getValueWithParametersAt( x, _parameters );

}


gkg::Vector 
gkg::LevenbergMarquardtFunction::getDerivativeAt( const gkg::Vector& x ) const
{

  return this->getDerivativeWithParametersAt( x, _parameters );

}
