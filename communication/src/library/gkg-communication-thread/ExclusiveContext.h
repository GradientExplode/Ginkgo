#ifndef _gkg_communication_thread_ExclusiveContext_h_
#define _gkg_communication_thread_ExclusiveContext_h_


#include <gkg-communication-thread/Mutex.h>


namespace gkg
{


class ExclusiveContext
{

  public:

    ExclusiveContext();
    virtual ~ExclusiveContext();

    void lock();
    void unlock();

  private:

    Mutex _mutex;

};


}


#endif
