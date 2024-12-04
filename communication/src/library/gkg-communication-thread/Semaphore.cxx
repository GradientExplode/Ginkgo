#include <gkg-communication-thread/Semaphore.h>
#include <gkg-communication-thread/SemaphoreImplementation.h>
#include <gkg-communication-core/CommunicationImplementationFactory.h>


gkg::Semaphore::Semaphore( int32_t count )
{

  _semaphoreImplementation = gkg::getCommunicationImplementationFactory()
                                  .createSemaphoreImplementation( this, count );

}


gkg::Semaphore::~Semaphore()
{

  delete _semaphoreImplementation;

}


bool gkg::Semaphore::post()
{

  return _semaphoreImplementation->post();

}


void gkg::Semaphore::wait()
{

  _semaphoreImplementation->wait();

}


bool gkg::Semaphore::tryWait()
{

  return _semaphoreImplementation->tryWait();

}


int32_t gkg::Semaphore::getValue()
{

  return _semaphoreImplementation->getValue();

}


gkg::SemaphoreImplementation* gkg::Semaphore::getImplementation() const
{

  return _semaphoreImplementation;

}
