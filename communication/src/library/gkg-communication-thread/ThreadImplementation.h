#ifndef _gkg_communication_thread_ThreadImplementation_h_
#define _gkg_communication_thread_ThreadImplementation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class Thread;


class ThreadImplementation
{

  public:


    virtual ~ThreadImplementation();

    virtual Thread* getTarget() const;

    virtual bool isDetached() const;
    virtual bool isCancellable() const;
    virtual bool hasCallStop() const;

    virtual void reset() = 0;
    virtual void exit() = 0;

    virtual void launch() = 0;

    virtual void execute() = 0;
    virtual void doExecute();

    virtual void join() = 0;
    virtual void detach() = 0;

    virtual void setCancellable( bool value, bool callStop ) = 0;
    virtual void cancel() = 0;

    virtual bool isActive() const = 0;
    virtual uint32_t getId() const = 0;

    virtual void testCancel() = 0;

  protected:

    ThreadImplementation( Thread* thread );

    Thread* _target;

    bool _detached;
    bool _cancellable;
    bool _callStop;
    bool _running;
    bool _idSet;

};


}


#endif
