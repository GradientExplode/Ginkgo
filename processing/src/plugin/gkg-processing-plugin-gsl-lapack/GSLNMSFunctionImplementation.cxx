#include <gkg-processing-plugin-gsl-lapack/GSLNMSFunctionImplementation.h>
#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>


gkg::GSLNMSFunctionImplementation::GSLNMSFunctionImplementation(
                     gkg::NelderMeadSimplexFunction* nelderMeadSimplexFunction )
                                  : _nelderMeadSimplexFunction(
                                                     nelderMeadSimplexFunction )
{

  try
  {

    _gslMultiminFunction.n = 
                      ( size_t )_nelderMeadSimplexFunction->getParameterCount();
    _gslMultiminFunction.f = &gkg::GSLNMSFunctionImplementation::getValueAt;
    _gslMultiminFunction.params = this;

  }
  GKG_CATCH( "gkg::GSLNMSFunctionImplementation::GSLNMSFunctionImplementation( "
             " gkg::NelderMeadSimplexFunction* nelderMeadSimplexFunction )" );

}


gkg::GSLNMSFunctionImplementation::~GSLNMSFunctionImplementation()
{


}


gkg::NelderMeadSimplexFunction*
gkg::GSLNMSFunctionImplementation::getNelderMeadSimplexFunction() const
{

  return _nelderMeadSimplexFunction;

}


gsl_multimin_function*
gkg::GSLNMSFunctionImplementation::getMultiminFunctionPointer()
{

  return &_gslMultiminFunction;

}


double gkg::GSLNMSFunctionImplementation::getValueAt( 
                                                  const gsl_vector* parameters,
                                                  void* otherParameters )
{

  try
  {

    gkg::GSLNMSFunctionImplementation* gslNMSFunctionImplementation = 
      static_cast< gkg::GSLNMSFunctionImplementation* >( otherParameters );

    gkg::NelderMeadSimplexFunction* nelderMeadSimplexFunction = 
      gslNMSFunctionImplementation->getNelderMeadSimplexFunction();

    int32_t parameterCount = nelderMeadSimplexFunction->getParameterCount();

    gkg::Vector temporaryParameters( parameterCount );

    int32_t p;
    for ( p = 0; p < parameterCount; p++ )
    {

      temporaryParameters( p ) = gsl_vector_get( parameters, p );

    }
    return nelderMeadSimplexFunction->getValueAt( temporaryParameters );

  }
  GKG_CATCH( "double gkg::GSLNMSFunctionImplementation::getValueAt( "
             "const gsl_vector* parameters, "
             "void* otherParameters )" );

}


