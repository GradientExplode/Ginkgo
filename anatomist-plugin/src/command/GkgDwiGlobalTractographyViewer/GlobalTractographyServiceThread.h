#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_GlobalTractographyServiceThread_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_GlobalTractographyServiceThread_h_


#include <gkg-communication-thread/Thread.h>


namespace gkg
{


class GlobalTractographyService;


class GlobalTractographyServiceThread : public Thread
{

  public:

    GlobalTractographyServiceThread(
                         GlobalTractographyService* globalTractographyService );
    virtual ~GlobalTractographyServiceThread();

  protected:

    void doRun();
    void doPreLaunch();

    GlobalTractographyService* _globalTractographyService;

};


}


#endif

