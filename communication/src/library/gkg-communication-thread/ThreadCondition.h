#ifndef _gkg_communication_thread_ThreadCondition_h_
#define _gkg_communication_thread_ThreadCondition_h_


#include <gkg-communication-thread/Mutex.h>


namespace gkg
{


class ThreadConditionImplementation;


class ThreadCondition
{

  public:

    ThreadCondition();
    virtual ~ThreadCondition();

    void wait( Mutex& mutex );
    void timedWait( Mutex& mutex, const uint32_t millisecond );
    void signal();
    void broadcast();

    void setProcessPrivate();
    void setProcessShared();

    ThreadConditionImplementation* getImplementation() const;

  private:

    ThreadConditionImplementation* _threadConditionImplementation;

};


}


#endif
