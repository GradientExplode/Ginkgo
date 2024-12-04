#include <gkg-processing-plugin-gsl-lapack/GSLLMFunctionImplementation.h>
#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>


gkg::GSLLMFunctionImplementation::GSLLMFunctionImplementation(
                   const gkg::Matrix* x,
                   const gkg::Vector* y,
                   const gkg::Vector* w,
                   gkg::LevenbergMarquardtFunction* levenbergMarquardtFunction )
                     : _x( x ),
                       _y( y ),
                       _w( w ),
                       _levenbergMarquardtFunction( levenbergMarquardtFunction )
{

  try
  {

    _fdf.f = &gkg::GSLLMFunctionImplementation::getValueAt;
    _fdf.df = &gkg::GSLLMFunctionImplementation::getDerivativeAt;
    _fdf.fdf = &gkg::GSLLMFunctionImplementation::getValueAndDerivativeAt;
    _fdf.n = ( size_t )_y->getSize();
    _fdf.p = ( size_t )_levenbergMarquardtFunction->getParameters().getSize();
    _fdf.params = this;

  }
  GKG_CATCH( "gkg::GSLLMFunctionImplementation::GSLLMFunctionImplementation( "
             "const gkg::Matrix* x, "
             "const gkg::Vector* y, "
             "const gkg::Vector* w, "
             "gkg::LevenbergMarquardtFunction* levenbergMarquardtFunction )" );

}


gkg::GSLLMFunctionImplementation::~GSLLMFunctionImplementation()
{


}


const gkg::Matrix* gkg::GSLLMFunctionImplementation::getX() const
{

  return _x;

}


const gkg::Vector* gkg::GSLLMFunctionImplementation::getY() const
{

  return _y;

}


const gkg::Vector* gkg::GSLLMFunctionImplementation::getW() const
{

  return _w;

}


gkg::LevenbergMarquardtFunction* 
gkg::GSLLMFunctionImplementation::getLevenbergMarquardtFunction() const
{

  return _levenbergMarquardtFunction;

}


gsl_multifit_function_fdf* 
gkg::GSLLMFunctionImplementation::getFdfPointer()
{

  return &_fdf;

}


int32_t gkg::GSLLMFunctionImplementation::getValueAt( 
                                                  const gsl_vector* parameters,
                                                  void* otherParameters,
                                                  gsl_vector* costFunction )
{

  try
  {

    gkg::GSLLMFunctionImplementation* gslLMFunctionImplementation = 
      static_cast< gkg::GSLLMFunctionImplementation* >( otherParameters );

    const gkg::Matrix* x = gslLMFunctionImplementation->getX();
    const gkg::Vector* y = gslLMFunctionImplementation->getY();
    const gkg::Vector* w = gslLMFunctionImplementation->getW();
    gkg::LevenbergMarquardtFunction* levenbergMarquardtFunction = 
      gslLMFunctionImplementation->getLevenbergMarquardtFunction();

    int32_t trialCount = y->getSize();
    int32_t variableCount = x->getSize2();
    int32_t parameterCount = 
                          levenbergMarquardtFunction->getParameters().getSize();

    gkg::Vector temporaryX( variableCount );
    gkg::Vector temporaryParameters( parameterCount );

    int32_t t, v, p;
    double value = 0.0;
    for ( t = 0; t < trialCount; t++ )
    {

      for ( v = 0; v < variableCount; v++ )
      {

        temporaryX( v ) = ( *x )( t, v );

      }
      for ( p = 0; p < parameterCount; p++ )
      {

        temporaryParameters( p ) = gsl_vector_get( parameters, p );

      }
      value = levenbergMarquardtFunction->getValueWithParametersAt(
                                                        temporaryX,
                                                        temporaryParameters ) -
              ( *y )( t );
            
      gsl_vector_set( costFunction, t,
                      value / ( *w )( t ) );

    }

    return GSL_SUCCESS;

  }
  GKG_CATCH( "int32_t gkg::GSLLMFunctionImplementation::getValueAt( "
             "const gsl_vector* parameters, "
             "void* otherParameters, "
             "gsl_vector* costFunction )" );

}


int32_t gkg::GSLLMFunctionImplementation::getDerivativeAt(
                                              const gsl_vector* parameters,
                                              void* otherParameters,
                                              gsl_matrix* costFunctionJacobian )
{

  try
  {

    gkg::GSLLMFunctionImplementation* gslLMFunctionImplementation = 
      static_cast< gkg::GSLLMFunctionImplementation* >( otherParameters );

    const gkg::Matrix* x = gslLMFunctionImplementation->getX();
    const gkg::Vector* y = gslLMFunctionImplementation->getY();
    const gkg::Vector* w = gslLMFunctionImplementation->getW();
    gkg::LevenbergMarquardtFunction* levenbergMarquardtFunction = 
      gslLMFunctionImplementation->getLevenbergMarquardtFunction();

    int32_t trialCount = y->getSize();
    int32_t variableCount = x->getSize2();
    int32_t parameterCount = 
                          levenbergMarquardtFunction->getParameters().getSize();

    gkg::Vector temporaryX( variableCount );
    gkg::Vector temporaryParameters( parameterCount );

    int32_t t, v, p;
    gkg::Vector derivative;
    for ( t = 0; t < trialCount; t++ )
    {

      for ( v = 0; v < variableCount; v++ )
      {

        temporaryX( v ) = ( *x )( t, v );

      }
      for ( p = 0; p < parameterCount; p++ )
      {

        temporaryParameters( p ) = gsl_vector_get( parameters, p );

      }
      derivative = levenbergMarquardtFunction->getDerivativeWithParametersAt(
                                                          temporaryX,
                                                          temporaryParameters );
      for ( p = 0; p < parameterCount; p++ )
      {

        gsl_matrix_set( costFunctionJacobian, t, p,
                        derivative( p ) / ( *w )( t ) );

      }

    }

    return GSL_SUCCESS;

  }
  GKG_CATCH( "int32_t gkg::GSLLMFunctionImplementation::getDerivativeAt( "
             "const gsl_vector* parameters, "
             "void* otherParameters, "
             "gsl_matrix* costFunctionJacobian )" );

}


int32_t gkg::GSLLMFunctionImplementation::getValueAndDerivativeAt(
                                              const gsl_vector* parameters,
                                              void* otherParameters,
                                              gsl_vector* costFunction,
                                              gsl_matrix* costFunctionJacobian )
{

  try
  {

    gkg::GSLLMFunctionImplementation::getValueAt( parameters,
                                                  otherParameters,
                                                  costFunction );
    gkg::GSLLMFunctionImplementation::getDerivativeAt( parameters,
                                                       otherParameters,
                                                       costFunctionJacobian );

    return GSL_SUCCESS;

  }
  GKG_CATCH( "int32_t gkg::GSLLMFunctionImplementation::"
             "getValueAndDerivativeAt( "
             "const gsl_vector* parameters, "
             "void* otherParameters, "
             "gsl_vector* costFunction, "
             "gsl_matrix* costFunctionJacobian )" );

}


