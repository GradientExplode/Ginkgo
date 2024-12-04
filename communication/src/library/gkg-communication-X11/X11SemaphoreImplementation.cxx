#include <gkg-communication-X11/X11SemaphoreImplementation.h>
#include <gkg-communication-thread/Semaphore.h>


gkg::X11SemaphoreImplementation::X11SemaphoreImplementation(
                                                      gkg::Semaphore* semaphore,
                                                      int32_t count )
                                : gkg::SemaphoreImplementation( semaphore,
                                                                count )
{

  sem_init( &_semaphore, 0, count );

}


gkg::X11SemaphoreImplementation::~X11SemaphoreImplementation()
{

  sem_destroy( &_semaphore );

}


bool gkg::X11SemaphoreImplementation::post()
{

  return !sem_post( &_semaphore );

}


void gkg::X11SemaphoreImplementation::wait()
{

  sem_wait( &_semaphore );

}


bool gkg::X11SemaphoreImplementation::tryWait()
{

  return !sem_trywait( &_semaphore );

}


int32_t gkg::X11SemaphoreImplementation::getValue()
{

  int32_t value;
  sem_getvalue( &_semaphore, &value );
  return value;

}


gkg::X11SemaphoreImplementation*
gkg::X11SemaphoreImplementation::getImplementation( gkg::Semaphore* semaphore )
{

  return static_cast< gkg::X11SemaphoreImplementation* >(
                                               semaphore->getImplementation() );

}
