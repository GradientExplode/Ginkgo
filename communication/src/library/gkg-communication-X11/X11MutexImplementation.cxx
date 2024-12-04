#include <gkg-communication-X11/X11MutexImplementation.h>
#include <gkg-communication-thread/Mutex.h>
#include <gkg-core-exception/Exception.h>
#include <string>


gkg::X11MutexImplementation::X11MutexImplementation( gkg::Mutex* thread )
                            : gkg::MutexImplementation( thread )
{

  pthread_mutex_init( &_mutex, 0 );
  pthread_mutexattr_init( &_attributes );

}


gkg::X11MutexImplementation::~X11MutexImplementation()
{

  pthread_mutexattr_destroy( &_attributes );
  pthread_mutex_destroy( &_mutex );

}


void gkg::X11MutexImplementation::lock()
{

  if ( pthread_mutex_trylock( &_mutex ) )
  {

    pthread_mutex_lock( &_mutex );

  }

}


void gkg::X11MutexImplementation::unlock()
{

  pthread_mutex_unlock( &_mutex );

}


void gkg::X11MutexImplementation::setProcessPrivate()
{

#if defined(linux)

  try
  {

    if ( pthread_mutexattr_setpshared( &_attributes, PTHREAD_PROCESS_PRIVATE ) )
    {

      throw std::runtime_error( "failed in setting private mode" );

    }

  }
  GKG_CATCH( "void gkg::X11MutexImplementation::setProcessPrivate()" );

#endif

}


void gkg::X11MutexImplementation::setProcessShared()
{

#if defined(linux)

  try
  {

    if ( pthread_mutexattr_setpshared( &_attributes, PTHREAD_PROCESS_SHARED ) )
    {

    throw std::runtime_error( "failed in setting shared mode" );

    }

  }
  GKG_CATCH( "void gkg::X11MutexImplementation::setProcessShared()" );

#endif

}


gkg::X11MutexImplementation* 
gkg::X11MutexImplementation::getImplementation( gkg::Mutex* mutex )
{

  return static_cast< X11MutexImplementation* >( mutex->getImplementation() );

}
