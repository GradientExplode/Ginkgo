#include <gkg-communication-thread/ThreadCondition.h>
#include <gkg-communication-thread/ThreadConditionImplementation.h>
#include <gkg-communication-core/CommunicationImplementationFactory.h>
#include <gkg-communication-thread/Mutex.h>
#include <gkg-core-cppext/SysTime.h>
#include <gkg-core-exception/Exception.h>
#include <ctime>
#include <string>


gkg::ThreadCondition::ThreadCondition()
{

  _threadConditionImplementation =
    gkg::getCommunicationImplementationFactory()
                                   .createThreadConditionImplementation( this );

}


gkg::ThreadCondition::~ThreadCondition()
{

  delete _threadConditionImplementation;

}


void gkg::ThreadCondition::wait( gkg::Mutex& mutex )
{

  _threadConditionImplementation->wait( mutex );

}


void gkg::ThreadCondition::timedWait( gkg::Mutex& mutex,
                                      const uint32_t millisecond )
{

  struct ::timeval now;
  struct ::timespec absTime;

  ::gettimeofday( &now, 0 );

  absTime.tv_sec = now.tv_sec + millisecond / 1000;
  absTime.tv_nsec = 1000 * ( now.tv_usec + ( millisecond % 1000 ) );

  _threadConditionImplementation->timedWait( mutex, &absTime );

}


void gkg::ThreadCondition::signal()
{

  _threadConditionImplementation->signal();

}


void gkg::ThreadCondition::broadcast()
{

  _threadConditionImplementation->broadcast();

}


void gkg::ThreadCondition::setProcessPrivate()
{

  try
  {

    _threadConditionImplementation->setProcessPrivate();

  }
  GKG_CATCH( "void gkg::ThreadCondition::setProcessPrivate()" );

}


void gkg::ThreadCondition::setProcessShared()
{

  try
  {

    _threadConditionImplementation->setProcessShared();

  }
  GKG_CATCH( "void gkg::ThreadCondition::setProcessShared()" );

}


gkg::ThreadConditionImplementation*
gkg::ThreadCondition::getImplementation() const
{

  return _threadConditionImplementation;

}
