#include <gkg-communication-thread/Mutex.h>
#include <gkg-communication-thread/MutexImplementation.h>
#include <gkg-communication-core/CommunicationImplementationFactory.h>
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-core-exception/Exception.h>
#include <string>


gkg::Mutex::Mutex()
          : _lockingThreadId( 0 )
{

  _mutexImplementation =
    gkg::getCommunicationImplementationFactory().createMutexImplementation(
                                                                       this );

}


gkg::Mutex::~Mutex()
{

  delete _mutexImplementation;

}


void gkg::Mutex::lock()
{

  try
  {

    // Avoid a mutex to be locked twice by a thread to prevent
    // blocking situations
    uint32_t threadId = gkg::SystemInfo::getInstance().getCurrentThreadId();
    if ( threadId != _lockingThreadId )
    {

      _lockingThreadId = threadId;
      _mutexImplementation->lock();

    }

  }
  GKG_CATCH( "void gkg::Mutex::lock()" );

}


void gkg::Mutex::unlock()
{

  _lockingThreadId = 0;
  _mutexImplementation->unlock();

}


void gkg::Mutex::setProcessPrivate()
{

  try
  {

    _mutexImplementation->setProcessPrivate();

  }
  GKG_CATCH( "void gkg::Mutex::setProcessPrivate()" );

}


void gkg::Mutex::setProcessShared()
{

  try
  {

    _mutexImplementation->setProcessShared();

  }
  GKG_CATCH( "void gkg::Mutex::setProcessShared()" );

}


gkg::MutexImplementation* gkg::Mutex::getImplementation() const
{

  return _mutexImplementation;

}
