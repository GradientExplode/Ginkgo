#include <gkg-communication-thread/SemaphoreImplementation.h>


gkg::SemaphoreImplementation::SemaphoreImplementation(
                                                      gkg::Semaphore* semaphore,
                                                      int32_t /*count*/ )
                             : _target( semaphore )
{
}


gkg::SemaphoreImplementation::~SemaphoreImplementation()
{
}


gkg::Semaphore * gkg::SemaphoreImplementation::getTarget() const
{

  return _target;

}

