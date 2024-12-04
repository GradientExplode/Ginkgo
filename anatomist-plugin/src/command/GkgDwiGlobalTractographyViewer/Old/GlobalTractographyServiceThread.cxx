#include "GlobalTractographyServiceThread.h"
#include "GlobalTractographyService.h"

#include <gkg-core-exception/Exception.h>


gkg::GlobalTractographyServiceThread::GlobalTractographyServiceThread(
                     gkg::GlobalTractographyService* globalTractographyService )
                                     : gkg::Thread(),
                                       _globalTractographyService(
                                                     globalTractographyService )
{

  try
  {

    detach();
    setCancellable( true, true );

  }
  GKG_CATCH( "gkg::GlobalTractographyServiceThread::"
             "GlobalTractographyServiceThread( "
             "gkg::GlobalTractographyService* globalTractographyService )" );

}


gkg::GlobalTractographyServiceThread::~GlobalTractographyServiceThread()
{
}


void gkg::GlobalTractographyServiceThread::doRun()
{

  try
  {

    if ( _globalTractographyService )
    {
  
      _globalTractographyService->run();

    }

  }
  GKG_CATCH( "void gkg::GlobalTractographyServiceThread::doRun()" );

}


void gkg::GlobalTractographyServiceThread::doPreLaunch()
{

  try
  {

    if ( isCancellable() && isActive() )
    {
  
      cancel();
  
    }

  }
  GKG_CATCH( "void gkg::GlobalTractographyServiceThread::doPreLaunch()" );

}

