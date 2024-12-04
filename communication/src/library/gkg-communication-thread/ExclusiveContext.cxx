#include <gkg-communication-thread/ExclusiveContext.h>


gkg::ExclusiveContext::ExclusiveContext()
{
}


gkg::ExclusiveContext::~ExclusiveContext()
{
}


void gkg::ExclusiveContext::lock()
{

  _mutex.lock();

}


void gkg::ExclusiveContext::unlock()
{

  _mutex.unlock();

}
