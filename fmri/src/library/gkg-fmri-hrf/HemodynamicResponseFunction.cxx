#include <gkg-fmri-hrf/HemodynamicResponseFunction.h>


gkg::HemodynamicResponseFunction::HemodynamicResponseFunction(
                                                        int32_t regressorCount )
                                 : _regressorCount( regressorCount )
{
}


gkg::HemodynamicResponseFunction::~HemodynamicResponseFunction()
{
}


int32_t gkg::HemodynamicResponseFunction::getRegressorCount() const
{

  return _regressorCount;

}

