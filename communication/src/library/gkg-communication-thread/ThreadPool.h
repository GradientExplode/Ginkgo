#ifndef _gkg_communication_thread_ThreadPool_h_
#define _gkg_communication_thread_ThreadPool_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-communication-thread/ThreadCondition.h>

#include <list>
#include <vector>
#include <queue>
#include <map>


namespace gkg
{


class Task;
class PoolThread;


class ThreadPool : public Singleton< ThreadPool >
{

  public:

    void execute( PoolThread* thread );
    void addTask( Task* task );
    void addTask( std::list< Task* >& tasks );
    void launch( Task* task, uint32_t microsecond = 50 );
    void launch( std::list< Task* >& tasks, uint32_t microsecond = 50 );
    void stop();

  protected:

    friend class Singleton< ThreadPool >;

    ThreadPool();
    virtual ~ThreadPool();

  private:

    void wait( int32_t id, uint32_t microsecond = 50 );
    int32_t getTaskId();

    Mutex _mutex;
    ThreadCondition _condition;

    std::vector< PoolThread* > _threads;
    std::queue< Task* > _tasks;
    std::map< int32_t, std::pair< int32_t, int32_t > > _done;

    volatile bool _running;

};


}


#endif
