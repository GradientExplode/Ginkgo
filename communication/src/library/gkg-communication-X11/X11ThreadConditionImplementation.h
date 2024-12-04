#ifndef _gkg_communication_X11_X11ThreadConditionImplementation_h_
#define _gkg_communication_X11_X11ThreadConditionImplementation_h_


#include <gkg-communication-thread/ThreadConditionImplementation.h>
#include <pthread.h>


namespace gkg
{


class X11ThreadConditionImplementation : public ThreadConditionImplementation
{

  public:


    X11ThreadConditionImplementation( ThreadCondition* threadCondition );
    ~X11ThreadConditionImplementation();

    void wait( Mutex& mutex );
    void timedWait( Mutex& mutex, const struct ::timespec* t );
    void signal();
    void broadcast();

    void setProcessPrivate();
    void setProcessShared();

    static X11ThreadConditionImplementation* 
             getImplementation( ThreadCondition* threadCondition );

  private:

    pthread_cond_t _condition;
    pthread_condattr_t _attributes;

};


}


#endif
