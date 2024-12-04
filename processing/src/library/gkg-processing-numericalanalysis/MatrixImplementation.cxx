#include <gkg-processing-numericalanalysis/MatrixImplementation.h>


gkg::MatrixImplementation::MatrixImplementation( gkg::Matrix* matrix,
                                                 int32_t /*size1*/,
                                                 int32_t /*size2*/ )
                          : _target( matrix )
{
}


gkg::MatrixImplementation::~MatrixImplementation()
{
}


gkg::Matrix* gkg::MatrixImplementation::getTarget() const
{

  return _target;

}
