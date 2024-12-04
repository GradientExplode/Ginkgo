#include <gkg-processing-numericalanalysis/FletcherReevesConjugateGradientFunction.h>


gkg::FletcherReevesConjugateGradientFunction::
               FletcherReevesConjugateGradientFunction( int32_t parameterCount )
                                             : _parameterCount( parameterCount )
{
}


gkg::FletcherReevesConjugateGradientFunction::
                                      ~FletcherReevesConjugateGradientFunction()
{
}


int32_t gkg::FletcherReevesConjugateGradientFunction::getParameterCount() const
{

  return _parameterCount;

}
