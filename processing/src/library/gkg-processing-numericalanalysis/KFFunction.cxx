#include <gkg-processing-numericalanalysis/KFFunction.h>


gkg::KFFunction::KFFunction( int32_t parameterCount )
               : _parameterCount( parameterCount )
{
}


gkg::KFFunction::~KFFunction()
{
}


void gkg::KFFunction::setParameters( const gkg::Vector& parameters )
{

  _parameters = parameters;

}


int32_t gkg::KFFunction::getParameterCount() const
{

  return _parameterCount;

}


void gkg::KFFunction::getValueAt( const Vector& X, double& Y ) const
{

  Y = _parameters.getComposition( X );

}


void gkg::KFFunction::getValuesAt( const Matrix& /* X */, 
                                   Vector& /* Y */ ) const
{
}


void gkg::KFFunction::getJacobian( const Vector& /* X */,
                                   Vector& /* Y */ ) const
{
}
