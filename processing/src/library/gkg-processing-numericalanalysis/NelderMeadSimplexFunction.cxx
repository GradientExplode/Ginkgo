#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>


gkg::NelderMeadSimplexFunction::NelderMeadSimplexFunction(
                                                        int32_t parameterCount )
                               : _parameterCount( parameterCount )
{
}


gkg::NelderMeadSimplexFunction::~NelderMeadSimplexFunction()
{
}


int32_t gkg::NelderMeadSimplexFunction::getParameterCount() const
{

  return _parameterCount;

}
