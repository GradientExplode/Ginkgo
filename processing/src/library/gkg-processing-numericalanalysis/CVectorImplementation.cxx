#include <gkg-processing-numericalanalysis/CVectorImplementation.h>


gkg::CVectorImplementation::CVectorImplementation( gkg::CVector* cvector,
                                                   int32_t /*size*/ )
                           : _target( cvector )
{
}


gkg::CVectorImplementation::~CVectorImplementation()
{
}


gkg::CVector* gkg::CVectorImplementation::getTarget() const
{

  return _target;

}
