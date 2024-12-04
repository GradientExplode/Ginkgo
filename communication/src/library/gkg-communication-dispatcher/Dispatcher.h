#ifndef _gkg_communication_dispatcher_Dispatcher_h_
#define _gkg_communication_dispatcher_Dispatcher_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-cppext/SysTime.h>



namespace gkg
{


class FdMask;
class IOHandler;
class TimerQueue;
class ChildQueue;



// wait on multiple file descriptors until a condition occurs.
// Dispatcher provides an interface to the "select" system call.
class Dispatcher : public Singleton< Dispatcher >
{

  public:

    enum Mask
    {

      ReadMask,
      WriteMask,
      ExceptMask

    };

    virtual ~Dispatcher();

    virtual IOHandler* getHandler( int32_t fd, Mask mask ) const;
    virtual void link( int32_t fd, Mask mask, IOHandler* ioHandler );
    virtual void unlink( int32_t fd );

    virtual void startTimer( int64_t sec, int64_t usec, IOHandler* ioHandler );
    virtual void stopTimer( IOHandler* ioHandler );

    virtual void startChild( int32_t pid, IOHandler* ioHandler );
    virtual void stopChild( IOHandler* iohandler );

    virtual bool setReady( int32_t fd, Mask mask );
    virtual void dispatch();
    virtual bool dispatch( int64_t& sec, int64_t& usec );

    virtual void setStatus( pid_t pid, int32_t status );

  protected:

    friend class Singleton< Dispatcher >;

    Dispatcher();

    virtual void attach( int32_t fd, Mask mask, IOHandler* ioHandler );
    virtual void detach( int32_t fd );
    virtual bool dispatch( timeval* howLong );
    virtual bool anyReady() const;
    virtual int32_t fillInReady( FdMask&, FdMask&, FdMask& );
    virtual int32_t waitFor( FdMask&, FdMask&, FdMask&, timeval* );
    virtual void notify( int32_t, FdMask&, FdMask&, FdMask& );
    virtual timeval* calculateTimeout( timeval* t ) const;
    virtual bool handleError();
    virtual void checkConnections();

    static void sigCLD( int32_t );

    int32_t	_nfds;
    FdMask* _rMask;
    FdMask* _wMask;
    FdMask* _eMask;
    FdMask* _rMaskReady;
    FdMask* _wMaskReady;
    FdMask* _eMaskReady;
    IOHandler** _rTable;
    IOHandler** _wTable;
    IOHandler** _eTable;
    TimerQueue* _timerQueue;
    ChildQueue* _childQueue;

};


}


#endif
