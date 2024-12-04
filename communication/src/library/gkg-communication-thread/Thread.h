#ifndef _gkg_communication_thread_Thread_h_
#define _gkg_communication_thread_Thread_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class ThreadImplementation;


class Thread
{

  public:

    Thread( bool verbose = false );
    virtual ~Thread();

    virtual void setVerbose( bool value );

    virtual bool isVerbose() const;
    virtual bool isDetached() const;
    virtual bool isCancellable() const;
    virtual bool hasCallStop() const;

    virtual void start();
    virtual void run();
    virtual void stop();
    virtual void reset();
    virtual void exit();

    virtual void launch();

    virtual void join();
    virtual void detach();

    virtual void setCancellable( bool value, bool callStop );
    virtual void cancel();

    virtual bool isActive() const;
    virtual uint32_t getId() const;

    ThreadImplementation* getImplementation() const;

    static void* bootstrap( void* thread );

  protected:

    virtual void testCancel();
    virtual void doStart();
    virtual void doRun() = 0;
    virtual void doStop();
    virtual void doPreLaunch();
    virtual void doPostLaunch();

    ThreadImplementation* _threadImplementation;
    bool _verbose;

};


}


#endif
