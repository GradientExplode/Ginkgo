#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-core-exception/Exception.h>


gkg::NonLinearProgrammingFunction::NonLinearProgrammingFunction(
                                                        int32_t parameterCount,
                                                        bool hasDerivative )
                               : _parameterCount( parameterCount ),
                                 _hasDerivative( hasDerivative )
{
}


gkg::NonLinearProgrammingFunction::~NonLinearProgrammingFunction()
{
}


int32_t gkg::NonLinearProgrammingFunction::getParameterCount() const
{

  try
  {

    return _parameterCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::NonLinearProgrammingFunction::getParameterCount() const" );

}


bool gkg::NonLinearProgrammingFunction::hasDerivative() const
{

  try
  {

    return _hasDerivative;

  }
  GKG_CATCH( "bool "
             "gkg::NonLinearProgrammingFunction::hasDerivative() const" );

}


void gkg::NonLinearProgrammingFunction::getDerivativeAt( 
                                           const gkg::Vector& /* parameters */,
                                           gkg::Vector& /* derivative */ ) const
{

  try
  {

    throw std::runtime_error( "not implemented yet" );

  }
  GKG_CATCH( "void gkg::NonLinearProgrammingFunction::getDerivativeAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& derivative ) const" );

}


double gkg::NonLinearProgrammingFunction::getValueAndDerivativeAt(
                                           const gkg::Vector& /* parameters */,
                                           gkg::Vector& /* derivative */ ) const
{

  try
  {

    throw std::runtime_error( "not implemented yet" );
    return 0.0; 

  }
  GKG_CATCH( "double "
             "gkg::NonLinearProgrammingFunction::getValueAndDerivativeAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& derivative ) const" );

}
