#include <gkg-processing-plugin-gsl-lapack/GSLNLPFunctionImplementation.h>
#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>


gkg::GSLNLPFunctionImplementation::GSLNLPFunctionImplementation(
  gkg::NonLinearProgrammingFunction* nonLinearProgrammingFunction,
  const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >& constraints )
                                  : _nonLinearProgrammingFunction(
                                                 nonLinearProgrammingFunction ),
                                    _constraints( constraints )
{

  try
  {

    _gslMultiminFunction.n = 
      ( size_t )_nonLinearProgrammingFunction->getParameterCount();
    _gslMultiminFunction.f =
      &gkg::GSLNLPFunctionImplementation::getValueAt;
    _gslMultiminFunction.params = this;
   

    if ( _nonLinearProgrammingFunction->hasDerivative() )
    {

      _gslMultiminFunctionFdf.n = 
        ( size_t )_nonLinearProgrammingFunction->getParameterCount();
      _gslMultiminFunctionFdf.f =
        &gkg::GSLNLPFunctionImplementation::getValueAt;
      _gslMultiminFunctionFdf.df =
        &gkg::GSLNLPFunctionImplementation::getDerivativeAt;
      _gslMultiminFunctionFdf.fdf =
        &gkg::GSLNLPFunctionImplementation::getValueAndDerivativeAt;
      _gslMultiminFunctionFdf.params = this;

    }

  }
  GKG_CATCH( "gkg::GSLNLPFunctionImplementation::"
             "GSLNLPFunctionImplementation( "
             "gkg::NonLinearProgrammingFunction* "
             "nonLinearProgrammingFunction, "
             "const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >& "
             "constraints )" );

}


gkg::GSLNLPFunctionImplementation::~GSLNLPFunctionImplementation()
{


}


gkg::NonLinearProgrammingFunction*
gkg::GSLNLPFunctionImplementation::getNonLinearProgrammingFunction() const
{

  try
  {

    return _nonLinearProgrammingFunction;

  }
  GKG_CATCH( "gkg::NonLinearProgrammingFunction* "
             "gkg::GSLNLPFunctionImplementation::"
             "getNonLinearProgrammingFunction() const" );

}


const gkg::OptimizerConstraint& 
gkg::GSLNLPFunctionImplementation::getConstraint( int32_t p ) const
{

  try
  {

    return *_constraints[ p ];

  }
  GKG_CATCH( "const gkg::OptimizerConstraint& "
             "gkg::GSLNLPFunctionImplementation::getConstraint( "
             "int32_t p ) const" );

}

gsl_multimin_function*
gkg::GSLNLPFunctionImplementation::getFPointer()
{

  try
  {

    return &_gslMultiminFunction;

  }
  GKG_CATCH( "gsl_multimin_function* "
             "gkg::GSLNLPFunctionImplementation::getFPointer()" );

}


gsl_multimin_function_fdf*
gkg::GSLNLPFunctionImplementation::getFdfPointer()
{

  try
  {

    return &_gslMultiminFunctionFdf;

  }
  GKG_CATCH( "gsl_multimin_function_fdf* "
             "gkg::GSLNLPFunctionImplementation::getFdfPointer()" );

}


double gkg::GSLNLPFunctionImplementation::getValueAt( 
                                                  const gsl_vector* parameters,
                                                  void* otherParameters )
{

  try
  {

    gkg::GSLNLPFunctionImplementation* gslNLPFunctionImplementation = 
      static_cast< gkg::GSLNLPFunctionImplementation* >( otherParameters );

    gkg::NonLinearProgrammingFunction*
      nonLinearProgrammingFunction = 
        gslNLPFunctionImplementation->getNonLinearProgrammingFunction();

    int32_t parameterCount = nonLinearProgrammingFunction->getParameterCount();

    gkg::Vector temporaryParameters( parameterCount );
    int32_t p;
    for ( p = 0; p < parameterCount; p++ )
    {

      temporaryParameters( p ) = gslNLPFunctionImplementation->getConstraint(
                                                           p ).inverseTransform(
                                              gsl_vector_get( parameters, p ) );

    }
    return nonLinearProgrammingFunction->getValueAt( temporaryParameters );

  }
  GKG_CATCH( "double gkg::GSLNLPFunctionImplementation::getValueAt( "
             "const gsl_vector* parameters, "
             "void* otherParameters )" );

}


void gkg::GSLNLPFunctionImplementation::getDerivativeAt( 
                                                  const gsl_vector* parameters,
                                                  void* otherParameters,
                                                  gsl_vector* derivative )
{

  try
  {

    gkg::GSLNLPFunctionImplementation* gslNLPFunctionImplementation = 
      static_cast< gkg::GSLNLPFunctionImplementation* >( otherParameters );

    gkg::NonLinearProgrammingFunction*
      nonLinearProgrammingFunction = 
        gslNLPFunctionImplementation->getNonLinearProgrammingFunction();

    int32_t parameterCount = nonLinearProgrammingFunction->getParameterCount();

    gkg::Vector temporaryParameters( parameterCount );
    gkg::Vector temporaryInverseDerivativeOfConstraint( parameterCount );
    double tmp = 0.0;
    int32_t p;
    for ( p = 0; p < parameterCount; p++ )
    {

      tmp = gsl_vector_get( parameters, p );
      temporaryParameters( p ) = gslNLPFunctionImplementation->getConstraint(
                                                    p ).inverseTransform( tmp );
      temporaryInverseDerivativeOfConstraint( p ) =
                                    gslNLPFunctionImplementation->getConstraint(
                                                   p ).inverseDerivative( tmp );

    }
    gkg::Vector temporaryDerivative( parameterCount );
    nonLinearProgrammingFunction->getDerivativeAt( temporaryParameters,
                                                   temporaryDerivative );
    for ( p = 0; p < parameterCount; p++ )
    {

      gsl_vector_set( derivative, p,
                      temporaryDerivative( p ) *
                      temporaryInverseDerivativeOfConstraint( p ) );

    }

  }
  GKG_CATCH( "void gkg::GSLNLPFunctionImplementation::getDeivativeAt( "
             "const gsl_vector* parameters, "
             "void* otherParameters, "
             "gsl_vector* derivative )" );

}


void gkg::GSLNLPFunctionImplementation::getValueAndDerivativeAt( 
                                                  const gsl_vector* parameters,
                                                  void* otherParameters,
                                                  double* value,
                                                  gsl_vector* derivative )
{

  try
  {

    gkg::GSLNLPFunctionImplementation* gslNLPFunctionImplementation = 
      static_cast< gkg::GSLNLPFunctionImplementation* >( otherParameters );

    gkg::NonLinearProgrammingFunction*
      nonLinearProgrammingFunction = 
        gslNLPFunctionImplementation->getNonLinearProgrammingFunction();

    int32_t parameterCount = nonLinearProgrammingFunction->getParameterCount();

    gkg::Vector temporaryParameters( parameterCount );
    gkg::Vector temporaryInverseDerivativeOfConstraint( parameterCount );
    double tmp = 0.0;
    int32_t p;
    for ( p = 0; p < parameterCount; p++ )
    {

      tmp = gsl_vector_get( parameters, p );
      temporaryParameters( p ) = gslNLPFunctionImplementation->getConstraint(
                                                    p ).inverseTransform( tmp );
      temporaryInverseDerivativeOfConstraint( p ) =
                                    gslNLPFunctionImplementation->getConstraint(
                                                   p ).inverseDerivative( tmp );

    }

    *value = nonLinearProgrammingFunction->getValueAt( temporaryParameters );

    gkg::Vector temporaryDerivative( parameterCount );
    nonLinearProgrammingFunction->getDerivativeAt( temporaryParameters,
                                                   temporaryDerivative );
    for ( p = 0; p < parameterCount; p++ )
    {

      gsl_vector_set( derivative, p,
                      temporaryDerivative( p ) *
                      temporaryInverseDerivativeOfConstraint( p ) );

    }

  }
  GKG_CATCH( "void gkg::GSLNLPFunctionImplementation::getValueAndDerivativeAt("
             "const gsl_vector* parameters, "
             "void* otherParameters, "
             "double* value, "
             "gsl_vector* derivative )" );

}

