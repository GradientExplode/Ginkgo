#include <gkg-communication-X11/X11ThreadConditionImplementation.h>
#include <gkg-communication-thread/ThreadCondition.h>
#include <gkg-communication-X11/X11MutexImplementation.h>
#include <gkg-core-exception/Exception.h>
#include <string>


gkg::X11ThreadConditionImplementation::X11ThreadConditionImplementation(
                                         gkg::ThreadCondition* threadCondition )
                                      : gkg::ThreadConditionImplementation(
                                                               threadCondition )
{

  pthread_cond_init( &_condition, 0 );
  pthread_condattr_init( &_attributes );

}


gkg::X11ThreadConditionImplementation::~X11ThreadConditionImplementation()
{

  pthread_condattr_destroy( &_attributes );
  pthread_cond_destroy( &_condition );

}


void gkg::X11ThreadConditionImplementation::wait( gkg::Mutex& mutex )
{

  gkg::X11MutexImplementation* x11MutexImpl = 
    gkg::X11MutexImplementation::getImplementation( &mutex );
  pthread_cond_wait( &_condition, &x11MutexImpl->_mutex );

}


void 
gkg::X11ThreadConditionImplementation::timedWait( gkg::Mutex& mutex,
                                                 const struct ::timespec* t )
{

  gkg::X11MutexImplementation* x11MutexImpl = 
    gkg::X11MutexImplementation::getImplementation( &mutex );
  pthread_cond_timedwait( &_condition, &x11MutexImpl->_mutex, t );

}


void gkg::X11ThreadConditionImplementation::signal()
{

  pthread_cond_signal( &_condition );

}


void gkg::X11ThreadConditionImplementation::broadcast()
{

  pthread_cond_broadcast( &_condition );

}


void gkg::X11ThreadConditionImplementation::setProcessPrivate()
{

#if defined(linux)

  try
  {

    if ( pthread_condattr_setpshared( &_attributes, PTHREAD_PROCESS_PRIVATE ) )
    {

      throw std::runtime_error(  "failed in setting private mode" );

    }

  }
  GKG_CATCH(
    "void gkg::X11ThreadConditionImplementation::setProcessPrivate()" );

#endif

}


void gkg::X11ThreadConditionImplementation::setProcessShared()
{

#if defined(linux)

  try
  {

    if ( pthread_condattr_setpshared( &_attributes, PTHREAD_PROCESS_SHARED ) )
    {

      throw std::runtime_error(  "failed in setting shared mode" );

    }

  }
  GKG_CATCH( "void gkg::X11ThreadConditionImplementation::setProcessShared()" );

#endif

}


gkg::X11ThreadConditionImplementation* 
gkg::X11ThreadConditionImplementation::getImplementation(
                                         gkg::ThreadCondition* threadCondition )
{

  return static_cast< X11ThreadConditionImplementation* >(
                                         threadCondition->getImplementation() );

}
