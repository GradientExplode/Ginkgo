#ifndef _gkg_communication_X11_X11ThreadImplementation_h_
#define _gkg_communication_X11_X11ThreadImplementation_h_


#include <gkg-communication-thread/ThreadImplementation.h>
#include <pthread.h>


namespace gkg
{


class X11ThreadImplementation : public ThreadImplementation
{

  public:


    X11ThreadImplementation( Thread* thread );
    ~X11ThreadImplementation();

    void reset();
    void exit();

    void launch();
    void execute();

    void join();
    void detach();

    void setCancellable( bool value, bool callStop );
    void cancel();

    bool isActive() const;
    uint32_t getId() const;

    void testCancel();

    static void cleanup( void* argument );

    static X11ThreadImplementation* getImplementation( Thread* thread );

  private:

    pthread_t _thread;
    pthread_attr_t _attributes;

};


}


#endif
