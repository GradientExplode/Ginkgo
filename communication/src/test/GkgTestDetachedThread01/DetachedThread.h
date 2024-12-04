#ifndef _gkg_communication_GkgDetachedThread01_DetachedThread_h_
#define _gkg_communication_GkgDetachedThread01_DetachedThread_h_


#include <gkg-communication-thread/Thread.h>


class DetachedThread : public gkg::Thread
{

  public:

    DetachedThread( bool verbose = false );
    virtual ~DetachedThread();

  protected:

    virtual void doRun();
    virtual void doPreLaunch();

};


#endif
