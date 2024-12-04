#include "DetachedThread.h"
#include <gkg-core-cppext/CUniStd.h>
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <iostream>


DetachedThread::DetachedThread( bool verbose )
               : gkg::Thread( verbose )
{

  if ( _verbose )
  {

    std::cerr << "gkg::DetachedThread::DetachedThread() ["
              << gkg::SystemInfo::getInstance().getCurrentThreadId()
              << "]" << std::endl;

  }

  // that thread should be detached in order not to block the event loop
  // that should receive the progressive draw events
  detach();
  // that thread should be cancelled when launched another time and
  // stop() function should be called after cancellation
  setCancellable( true, true );

}


DetachedThread::~DetachedThread()
{

  if ( _verbose )
  {

    std::cerr << "gkg::DetachedThread::~DetachedThread() ["
              << gkg::SystemInfo::getInstance().getCurrentThreadId()
              << "]" << std::endl;

  }

}


void DetachedThread::doRun()
{

  if ( _verbose )
  {

    std::cerr << "gkg::DetachedThread::doRun() ["
              << gkg::SystemInfo::getInstance().getCurrentThreadId()
              << "]" << std::endl;

  }
  int32_t i = 0;
  do
  {

    ++ i;
    std::cerr << "  step " << i << std::endl;
    sleep( 1 );
    testCancel();
           
  }
  while ( i != 4 );

}


void DetachedThread::doPreLaunch()
{
 
  if ( _verbose )
  {

    std::cerr << "gkg::DetachedThread::doPreLaunch() ["
              << gkg::SystemInfo::getInstance().getCurrentThreadId()
              << "]" << std::endl;

  }

  if ( isCancellable() && isActive() )
  {

    cancel();

  }

}
