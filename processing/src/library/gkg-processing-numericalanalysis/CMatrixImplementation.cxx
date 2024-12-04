#include <gkg-processing-numericalanalysis/CMatrixImplementation.h>


gkg::CMatrixImplementation::CMatrixImplementation( gkg::CMatrix* cmatrix,
                                                   int32_t /*size1*/,
                                                   int32_t /*size2*/ )
                           : _target( cmatrix )
{
}


gkg::CMatrixImplementation::~CMatrixImplementation()
{
}


gkg::CMatrix* gkg::CMatrixImplementation::getTarget() const
{

  return _target;

}
