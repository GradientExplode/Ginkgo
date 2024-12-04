#ifndef _gkg_communication_thread_ThreadCancel_h_
#define _gkg_communication_thread_ThreadCancel_h_


namespace gkg
{


class ThreadCancel
{

  public:

    ThreadCancel();
    virtual ~ThreadCancel();

    virtual bool check();
    virtual void reset();

};


}


#endif
