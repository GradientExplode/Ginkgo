#ifndef _gkg_communication_thread_ThreadPoolHelper_h_
#define _gkg_communication_thread_ThreadPoolHelper_h_


#include <gkg-communication-thread/LoopHelper.h>

#include <list>


namespace gkg
{


class Task;


template < class T = int32_t >
class ThreadPoolHelper : public LoopHelper< T >
{

  public:

    ThreadPoolHelper();

    void launch( ThreadedLoop< T >* threadedLoop,
                 T threadCount,
                 T grain,
                 T count );
    void join();

  private:

    std::list< Task* > _taskList;

};


}


#endif
