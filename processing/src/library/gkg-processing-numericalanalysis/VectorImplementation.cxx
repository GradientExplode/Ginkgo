#include <gkg-processing-numericalanalysis/VectorImplementation.h>


gkg::VectorImplementation::VectorImplementation( gkg::Vector* vector,
                                                 int32_t /*size*/ )
                          : _target( vector )
{
}


gkg::VectorImplementation::~VectorImplementation()
{
}


gkg::Vector* gkg::VectorImplementation::getTarget() const
{

  return _target;

}
