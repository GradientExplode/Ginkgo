#ifndef _gkg_communication_thread_LoopThread_h_
#define _gkg_communication_thread_LoopThread_h_


#include <gkg-communication-thread/Thread.h>


namespace gkg
{


template < class T > class ThreadedLoop;


template < class T = int32_t >
class LoopThread : public Thread
{

  public:

    LoopThread( ThreadedLoop< T >* parent );
    virtual ~LoopThread();

  protected:

    virtual void doRun();

    ThreadedLoop< T >* _parent;

};


}


#endif
