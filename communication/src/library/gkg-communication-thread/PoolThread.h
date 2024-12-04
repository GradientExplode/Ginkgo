#ifndef _gkg_communication_thread_PoolThread_h_
#define _gkg_communication_thread_PoolThread_h_


#include <gkg-communication-thread/Thread.h>


namespace gkg
{


class ThreadPool;


class PoolThread : public Thread
{

  public:

    PoolThread( ThreadPool* parent );

  protected:

    void doRun();

    ThreadPool* _parent;

};


}


#endif
