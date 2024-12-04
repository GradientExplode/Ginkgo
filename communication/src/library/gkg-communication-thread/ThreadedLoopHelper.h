#ifndef _gkg_communication_thread_ThreadedLoopHelper_h_
#define _gkg_communication_thread_ThreadedLoopHelper_h_


#include <gkg-communication-thread/LoopHelper.h>

#include <list>


namespace gkg
{


template < class T > class LoopThread;


template < class T = int32_t >
class ThreadedLoopHelper : public LoopHelper< T >
{

  public:

    ThreadedLoopHelper();

    void launch( ThreadedLoop< T >* threadedLoop,
                 T threadCount,
                 T grain,
                 T count );
    void join();

  private:

    std::list< LoopThread< T >* > _threadList;

};


}


#endif
