#ifndef _gkg_communication_thread_SemaphoreImplementation_h_
#define _gkg_communication_thread_SemaphoreImplementation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class Semaphore;


class SemaphoreImplementation
{

  public:

    virtual ~SemaphoreImplementation();

    Semaphore* getTarget() const;

    virtual bool post() = 0;
    virtual void wait() = 0;
    virtual bool tryWait() = 0;

    virtual int32_t getValue() = 0;

  protected:

    SemaphoreImplementation( Semaphore* semaphore, int32_t count );

    Semaphore* _target;

};


}


#endif
