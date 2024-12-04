#ifndef _gkg_communication_thread_Semaphore_h_
#define _gkg_communication_thread_Semaphore_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class SemaphoreImplementation;


class Semaphore
{

  public:

    Semaphore( int32_t count );
    ~Semaphore();

    bool post();
    void wait();
    bool tryWait();

    int32_t getValue();

    SemaphoreImplementation* getImplementation() const;

  private:

    SemaphoreImplementation* _semaphoreImplementation;

};


}


#endif
