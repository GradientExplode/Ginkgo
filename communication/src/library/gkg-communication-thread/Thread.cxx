#include <gkg-communication-thread/Thread.h>
#include <gkg-communication-thread/ThreadImplementation.h>
#include <gkg-communication-core/CommunicationImplementationFactory.h>
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <iostream>


gkg::Thread::Thread( bool verbose )
            : _verbose( verbose )
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::Thread() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }

    _threadImplementation =
    gkg::getCommunicationImplementationFactory().createThreadImplementation(
                                                                        this );

  }
  GKG_CATCH( "gkg::Thread::Thread( bool verbose )" );

}


gkg::Thread::~Thread()
{

  if ( _verbose )
  {

    std::cerr << "gkg::Thread::~Thread() ["
              << gkg::SystemInfo::getInstance().getCurrentThreadId()
              << "]" << std::endl;

  }

  delete _threadImplementation;

}


void gkg::Thread::setVerbose( bool value )
{

  _verbose = value;

}


bool gkg::Thread::isVerbose() const
{

  return _verbose;

}


bool gkg::Thread::isDetached() const
{

  return _threadImplementation->isDetached();

}


bool gkg::Thread::isCancellable() const
{

  return _threadImplementation->isCancellable();

}


bool gkg::Thread::hasCallStop() const
{

  return _threadImplementation->hasCallStop();

}


void gkg::Thread::start()
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::start() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }

    doStart();

  }
  GKG_CATCH( "void gkg::Thread::start()" );

}


void gkg::Thread::run()
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::run() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }

    doRun();

  }
  GKG_CATCH( "void gkg::Thread::run()" );

}


void gkg::Thread::stop()
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::stop() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }
    doStop();

  }
  GKG_CATCH( "void gkg::Thread::stop()" );

}


void gkg::Thread::reset()
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::reset() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }
    _threadImplementation->reset();

  }
  GKG_CATCH( "void gkg::Thread::reset()" );

}


void gkg::Thread::exit()
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::exit() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }
    _threadImplementation->exit();

  }
  GKG_CATCH( "void gkg::Thread::exit()" );

}


void gkg::Thread::launch()
{
 
  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::launch() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }

    doPreLaunch();
    _threadImplementation->launch();
    doPostLaunch();

  }
  GKG_CATCH( "void gkg::Thread::launch()" );

}


void gkg::Thread::join()
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::join() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }
    _threadImplementation->join();

    reset();

  }
  GKG_CATCH( "void gkg::Thread::join()" );

}


void gkg::Thread::detach()
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::detach() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }
    _threadImplementation->detach();

  }
  GKG_CATCH( "void gkg::Thread::detach()" );

}


void gkg::Thread::setCancellable( bool value, bool callStop )
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::setCancellable() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }
    _threadImplementation->setCancellable( value, callStop );

  }
  GKG_CATCH( "void gkg::Thread::setCancellable( bool value, bool callStop )" );

}


void gkg::Thread::cancel()
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::cancel() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }

    _threadImplementation->cancel();
    if ( _threadImplementation->hasCallStop() )
    {

      stop();

    }
    reset();

  }
  GKG_CATCH( "void gkg::Thread::cancel()" );

}


bool gkg::Thread::isActive() const
{

  return _threadImplementation->isActive();

}


uint32_t gkg::Thread::getId() const
{

  return _threadImplementation->getId();

}


void* gkg::Thread::bootstrap( void* threadImplementation )
{

  try
  {

    gkg::ThreadImplementation*
      theThreadImplementation = static_cast< gkg::ThreadImplementation* >(
                                                         threadImplementation );

    if ( theThreadImplementation->getTarget()->isVerbose() )
    {

      std::cerr << "gkg::Thread::bootstrap() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }

    if ( theThreadImplementation )
    {

      theThreadImplementation->execute();

    }

    return 0;

  }
  GKG_CATCH( "void* gkg::Thread::bootstrap( void* threadImplementation )" );

}


void gkg::Thread::testCancel()
{

  try
  {

    if ( _verbose )
    {

      std::cerr << "gkg::Thread::testCancel() ["
                << gkg::SystemInfo::getInstance().getCurrentThreadId()
                << "]" << std::endl;

    }
    _threadImplementation->testCancel();

  }
  GKG_CATCH( "void gkg::Thread::testCancel()" );

}


void gkg::Thread::doStart()
{
}


void gkg::Thread::doStop()
{
}


void gkg::Thread::doPreLaunch()
{
}


void gkg::Thread::doPostLaunch()
{
}


gkg::ThreadImplementation* gkg::Thread::getImplementation() const
{

  return _threadImplementation;

}

