#ifndef _gkg_communication_X11_X11SemaphoreImplementation_h_
#define _gkg_communication_X11_X11SemaphoreImplementation_h_


#include <gkg-communication-thread/SemaphoreImplementation.h>
#include <semaphore.h>


namespace gkg
{


class X11SemaphoreImplementation : public SemaphoreImplementation
{

  public:

    X11SemaphoreImplementation( Semaphore* semaphore, int32_t count );
    ~X11SemaphoreImplementation();

    bool post();
    void wait();
    bool tryWait();

    int32_t getValue();

    static X11SemaphoreImplementation* 
             getImplementation( Semaphore* semaphore );

  private:

    sem_t _semaphore;

};


}


#endif

