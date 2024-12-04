#include <gkg-graphics-X11-progressive/ProgressiveThread.h>
#include <gkg-graphics-X11-progressive/ProgressiveImage.h>
#include <gkg-graphics-X11-progressive/ProgressiveModel.h>
#include <gkg-graphics-X11-progressive/ProgressiveImageContext.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::ProgressiveThread::ProgressiveThread(
                                        gkg::ProgressiveImage* progressiveImage,
                                        bool verbose )
                       : gkg::Thread( verbose ),
                         _progressiveImage( progressiveImage ),
                         _verbose( verbose )
{


  if ( _verbose )
  {

    std::cerr << "ProgressiveThread::ProgressiveThread() [" << pthread_self()
              << "]" << std::endl;

  }

  // that thread should be detached in order not to block the event loop
  // that should receive the progressive draw events
  detach();
  // that thread should be cancelled when launched another time and
  // stop() function should be called after cancellation
  setCancellable( true, true );

  int32_t startIndex = 0;
  int32_t pixelHeight = 
                     _progressiveImage->getProgressiveModel()->getPixelHeight();
  int32_t resolutionDivider = _progressiveImage->getResolutionDivider();
  int32_t count = pixelHeight / resolutionDivider +
                  ( pixelHeight % resolutionDivider ? 1 : 0 );
  int32_t threadsByCpu = 1;

  _threadedLoop = new gkg::ThreadedLoop< int32_t >(
                                _progressiveImage->getProgressiveImageContext(),
                                startIndex, count, 0, threadsByCpu );

}


gkg::ProgressiveThread::~ProgressiveThread()
{

  if ( _verbose )
  {

    std::cerr << "ProgressiveThread::~ProgressiveThread() [" << pthread_self()
              << "]"
              << std::endl;

  }

  delete _threadedLoop;

}


void gkg::ProgressiveThread::doRun()
{

  try
  {

    if ( _verbose )
    {

#if ( defined(__APPLE__) && defined(__MACH__) )

      std::cerr << "ProgressiveThread::doRun() ["
                << ( int32_t )( int64_t )pthread_self() << "]"
                << std::endl;

#else

      std::cerr << "ProgressiveThread::doRun() ["
                << ( int32_t )pthread_self() << "]"
                << std::endl;

#endif

    }

    do
    {

      _threadedLoop->launch();
      _progressiveImage->updatePatch();
      testCancel();
         
    }
    while ( !_progressiveImage->hasFullResolution() );

  }
  GKG_CATCH( "void gkg::ProgressiveThread::doRun()" );

}


void gkg::ProgressiveThread::doPreLaunch()
{

  try
  {

    if ( _verbose )
    {

#if ( defined(__APPLE__) && defined(__MACH__) )

      std::cerr << "ProgressiveThread::doPreLaunch() ["
                << ( int32_t )( int64_t )pthread_self() << "]"
                << std::endl;

#else

      std::cerr << "ProgressiveThread::doPreLaunch() ["
                << ( int32_t )pthread_self() << "]"
                << std::endl;

#endif

    }

    if ( isCancellable() && isActive() )
    {

      cancel();

    }

  }
  GKG_CATCH( "void gkg::ProgressiveThread::doPreLaunch()" );

}


void gkg::ProgressiveThread::doStop()
{

  try
  {
 
    if ( _verbose )
    {

#if ( defined(__APPLE__) && defined(__MACH__) )

      std::cerr << "ProgressiveThread::doStop() ["
                << ( int32_t )( int64_t )pthread_self() << "]"
                << std::endl;

#else

      std::cerr << "ProgressiveThread::doStop() ["
                << ( int32_t )pthread_self() << "]"
                << std::endl;

#endif

    }

    _progressiveImage->resetProgressiveImageContext();

  }
  GKG_CATCH( "void gkg::ProgressiveThread::doStop()" );

}
