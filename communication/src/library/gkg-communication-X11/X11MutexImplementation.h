#ifndef _gkg_communication_X11_X11MutexImplementation_h_
#define _gkg_communication_X11_X11MutexImplementation_h_


#include <gkg-communication-thread/MutexImplementation.h>
#include <pthread.h>


namespace gkg
{


class X11MutexImplementation : public MutexImplementation
{

  public:


    X11MutexImplementation( Mutex* mutex );
    ~X11MutexImplementation();

    void lock();
    void unlock();

    void setProcessPrivate();
    void setProcessShared();

    static X11MutexImplementation* getImplementation( Mutex* mutex );

  private:

    friend class X11ThreadConditionImplementation;

    pthread_mutex_t _mutex;
    pthread_mutexattr_t _attributes;

};


}


#endif
