#ifndef _gkg_communication_thread_ThreadConditionImplementation_h_
#define _gkg_communication_thread_ThreadConditionImplementation_h_


#include <ctime>


namespace gkg
{


class Mutex;
class ThreadCondition;


class ThreadConditionImplementation
{

  public:


    virtual ~ThreadConditionImplementation();

    virtual ThreadCondition* getTarget() const;

    virtual void wait( Mutex& mutex ) = 0;
    virtual void timedWait( Mutex& mutex, const struct ::timespec* t ) = 0;
    virtual void signal() = 0;
    virtual void broadcast() = 0;

    virtual void setProcessPrivate() = 0;
    virtual void setProcessShared() = 0;


  protected:

    ThreadConditionImplementation( ThreadCondition* threadCondition );

    ThreadCondition* _target;

};


}


#endif
