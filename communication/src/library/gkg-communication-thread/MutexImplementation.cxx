#include <gkg-communication-thread/MutexImplementation.h>


gkg::MutexImplementation::MutexImplementation( gkg::Mutex* mutex )
                         : _target( mutex )
{
}


gkg::MutexImplementation::~MutexImplementation()
{
}


gkg::Mutex* gkg::MutexImplementation::getTarget() const
{

  return _target;

}

