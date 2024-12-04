#ifndef _gkg_graphics_X11_progressive_ProgressiveThread_h_
#define _gkg_graphics_X11_progressive_ProgressiveThread_h_


#include <gkg-communication-thread/Thread.h>
#include <gkg-communication-thread/ThreadedLoop.h>


namespace gkg
{


class ProgressiveImage;


class ProgressiveThread : public Thread
{

  public:

    ProgressiveThread( ProgressiveImage* progressiveImage,
                       bool verbose = false );
    virtual ~ProgressiveThread();

  protected:

    virtual void doRun();
    virtual void doStop();
    virtual void doPreLaunch();

  private:

    ProgressiveImage* _progressiveImage;
    ThreadedLoop< int32_t >* _threadedLoop;
    bool _verbose;

};


}


#endif
