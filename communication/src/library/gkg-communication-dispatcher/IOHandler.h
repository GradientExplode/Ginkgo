#ifndef _gkg_communication_dispatcher_IOHandler_h_
#define _gkg_communication_dispatcher_IOHandler_h_


#include <sys/types.h>


namespace gkg
{


// derived classes read input on a file number, write output on a file
// number, handle an exception raised on a file number, or handle a
// timer's expiration.

class IOHandler 
{

  public:

    virtual ~IOHandler();

    virtual int32_t isInputReady( int32_t fd );
    virtual int32_t isOutputReady( int32_t fd );
    virtual int32_t isExceptionRaised( int32_t fd );
    virtual void setTimerExpired( int64_t sec, int64_t usec );
    virtual void setChildStatus( pid_t pid, int32_t status );

  protected:

    IOHandler();

};


}


#endif
