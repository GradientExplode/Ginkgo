#ifndef _gkg_communication_thread_MutexImplementation_h_
#define _gkg_communication_thread_MutexImplementation_h_


namespace gkg
{


class Mutex;


class MutexImplementation
{

  public:

    virtual ~MutexImplementation();

    virtual Mutex* getTarget() const;

    virtual void lock() = 0;
    virtual void unlock() = 0;

    virtual void setProcessPrivate() = 0;
    virtual void setProcessShared() = 0;

  protected:

    MutexImplementation( Mutex* mutex );

    Mutex* _target;

};


}


#endif
