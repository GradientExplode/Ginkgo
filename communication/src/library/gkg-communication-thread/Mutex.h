#ifndef _gkg_communication_thread_Mutex_h_
#define _gkg_communication_thread_Mutex_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class MutexImplementation;


class Mutex
{

  public:

    Mutex();
    virtual ~Mutex();

    void lock();
    void unlock();

    void setProcessPrivate();
    void setProcessShared();

    MutexImplementation* getImplementation() const;

  private:

    MutexImplementation* _mutexImplementation;

    uint32_t _lockingThreadId;

};


}


#endif
