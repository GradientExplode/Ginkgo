#ifndef _gkg_communication_thread_Task_h_
#define _gkg_communication_thread_Task_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class Task
{

  public:

    Task();
    virtual~Task();

    void setId( int32_t id );
    int32_t getId();

    virtual void run() = 0;

  private:

    int32_t _id;

};


}


#endif
