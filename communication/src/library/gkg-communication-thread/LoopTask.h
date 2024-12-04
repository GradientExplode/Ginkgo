#ifndef _gkg_communication_thread_LoopTask_h_
#define _gkg_communication_thread_LoopTask_h_


#include <gkg-communication-thread/Task.h>


namespace gkg
{


template < class T > class ThreadedLoop;


template < class T = int32_t >
class LoopTask : public Task
{

  public:

    LoopTask( ThreadedLoop< T >* parent );

    void run();

  protected:

    ThreadedLoop< T >* _parent;


};


}


#endif
