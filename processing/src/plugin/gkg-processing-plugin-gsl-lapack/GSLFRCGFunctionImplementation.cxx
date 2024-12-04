#include <gkg-processing-plugin-gsl-lapack/GSLFRCGFunctionImplementation.h>
#include <gkg-processing-numericalanalysis/FletcherReevesConjugateGradientFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>


gkg::GSLFRCGFunctionImplementation::GSLFRCGFunctionImplementation(
                                gkg::FletcherReevesConjugateGradientFunction*
                                       fletcherReevesConjugateGradientFunction )
                                   : _fletcherReevesConjugateGradientFunction(
                                       fletcherReevesConjugateGradientFunction )
{

  try
  {

    _gslMultiminFunctionFdf.n = 
        ( size_t )_fletcherReevesConjugateGradientFunction->getParameterCount();
    _gslMultiminFunctionFdf.f =
      &gkg::GSLFRCGFunctionImplementation::getValueAt;
    _gslMultiminFunctionFdf.df =
      &gkg::GSLFRCGFunctionImplementation::getDerivativeAt;
    _gslMultiminFunctionFdf.fdf =
      &gkg::GSLFRCGFunctionImplementation::getValueAndDerivativeAt;
    _gslMultiminFunctionFdf.params = this;

  }
  GKG_CATCH( "gkg::GSLFRCGFunctionImplementation::"
             "GSLFRCGFunctionImplementation( "
             "gkg::FletcherReevesConjugateGradientFunction* "
             "fletcherReevesConjugateGradientFunction )" );

}


gkg::GSLFRCGFunctionImplementation::~GSLFRCGFunctionImplementation()
{


}


gkg::FletcherReevesConjugateGradientFunction*
gkg::GSLFRCGFunctionImplementation::
                              getFletcherReevesConjugateGradientFunction() const
{

  return _fletcherReevesConjugateGradientFunction;

}


gsl_multimin_function_fdf*
gkg::GSLFRCGFunctionImplementation::getFdfPointer()
{

  return &_gslMultiminFunctionFdf;

}


double gkg::GSLFRCGFunctionImplementation::getValueAt( 
                                                  const gsl_vector* parameters,
                                                  void* otherParameters )
{

  try
  {

    gkg::GSLFRCGFunctionImplementation* gslNMSFunctionImplementation = 
      static_cast< gkg::GSLFRCGFunctionImplementation* >( otherParameters );

    gkg::FletcherReevesConjugateGradientFunction*
      fletcherReevesConjugateGradientFunction = 
     gslNMSFunctionImplementation->getFletcherReevesConjugateGradientFunction();

    int32_t parameterCount = fletcherReevesConjugateGradientFunction->getParameterCount();

    gkg::Vector temporaryParameters( parameterCount );
    int32_t p;
    for ( p = 0; p < parameterCount; p++ )
    {

      temporaryParameters( p ) = gsl_vector_get( parameters, p );

    }
    return fletcherReevesConjugateGradientFunction->getValueAt(
                                                          temporaryParameters );

  }
  GKG_CATCH( "double gkg::GSLFRCGFunctionImplementation::getValueAt( "
             "const gsl_vector* parameters, "
             "void* otherParameters )" );

}


void gkg::GSLFRCGFunctionImplementation::getDerivativeAt( 
                                                  const gsl_vector* parameters,
                                                  void* otherParameters,
                                                  gsl_vector* derivative )
{

  try
  {

    gkg::GSLFRCGFunctionImplementation* gslNMSFunctionImplementation = 
      static_cast< gkg::GSLFRCGFunctionImplementation* >( otherParameters );

    gkg::FletcherReevesConjugateGradientFunction*
      fletcherReevesConjugateGradientFunction = 
     gslNMSFunctionImplementation->getFletcherReevesConjugateGradientFunction();

    int32_t parameterCount = fletcherReevesConjugateGradientFunction->getParameterCount();

    gkg::Vector temporaryParameters( parameterCount );

    int32_t p;
    for ( p = 0; p < parameterCount; p++ )
    {

      temporaryParameters( p ) = gsl_vector_get( parameters, p );

    }
    gkg::Vector temporaryDerivative = 
      fletcherReevesConjugateGradientFunction->getDerivativeAt(
                                                          temporaryParameters );
    for ( p = 0; p < parameterCount; p++ )
    {

      gsl_vector_set( derivative, p,temporaryDerivative( p ) );

    }

  }
  GKG_CATCH( "void gkg::GSLFRCGFunctionImplementation::getDeivativeAt( "
             "const gsl_vector* parameters, "
             "void* otherParameters, "
             "gsl_vector* derivative )" );

}


void gkg::GSLFRCGFunctionImplementation::getValueAndDerivativeAt( 
                                                  const gsl_vector* parameters,
                                                  void* otherParameters,
                                                  double* value,
                                                  gsl_vector* derivative )
{

  try
  {

    *value = gkg::GSLFRCGFunctionImplementation::getValueAt( parameters,
                                                             otherParameters );
    gkg::GSLFRCGFunctionImplementation::getDerivativeAt( parameters,
                                                         otherParameters,
                                                         derivative );

  }
  GKG_CATCH( "void gkg::GSLFRCGFunctionImplementation::getValueAndDerivativeAt("
             "const gsl_vector* parameters, "
             "void* otherParameters, "
             "double* value, "
             "gsl_vector* derivative )" );

}

