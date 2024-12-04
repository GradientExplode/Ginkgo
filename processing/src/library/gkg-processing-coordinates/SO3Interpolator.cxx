#include <gkg-processing-coordinates/SO3Interpolator.h>


gkg::SO3Interpolator::SO3Interpolator( int32_t neighborCount )
                     : _neighborCount( neighborCount )
{
}


gkg::SO3Interpolator::~SO3Interpolator()
{
}


int32_t gkg::SO3Interpolator::getNeighborCount() const
{

  return _neighborCount;

}



