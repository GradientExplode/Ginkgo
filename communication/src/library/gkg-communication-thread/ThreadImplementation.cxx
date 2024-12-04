#include <gkg-communication-thread/ThreadImplementation.h>
#include <gkg-communication-thread/Thread.h>


gkg::ThreadImplementation::ThreadImplementation( gkg::Thread* thread )
                          : _target( thread ),
                            _detached( false ),
                            _cancellable( false ),
                            _callStop( false ),
                            _running( false )
{
}


gkg::ThreadImplementation::~ThreadImplementation()
{
}


gkg::Thread* gkg::ThreadImplementation::getTarget() const
{

  return _target;

}


bool gkg::ThreadImplementation::isDetached() const
{

  return _detached;

}


bool gkg::ThreadImplementation::isCancellable() const
{

  return _cancellable;

}


bool gkg::ThreadImplementation::hasCallStop() const
{

  return _callStop;

}


void gkg::ThreadImplementation::doExecute()
{

  if ( _target )
  {

    _target->start();
    _running = true;
    _target->run();
    _running = false;
    _target->stop();

  }

}
