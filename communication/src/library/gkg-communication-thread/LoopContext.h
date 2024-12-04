#ifndef _gkg_communication_thread_LoopContext_h_
#define _gkg_communication_thread_LoopContext_h_


#include <gkg-communication-thread/ExclusiveContext.h>
#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T > class ThreadGauge;
class ThreadCancel;


template < class T = int32_t >
class LoopContext : public ExclusiveContext
{

  public:

    LoopContext( ThreadGauge< T >* gauge = 0, ThreadCancel* cancel = 0 );
    virtual ~LoopContext();

    virtual void doIt( T startIndex, T countIndex ) = 0;
    virtual void gaugeAdd( T value );
    virtual bool cancel();
    virtual void resetGauge();
    virtual void resetCancel();

  protected:

    ThreadGauge< T >* _gauge;
    ThreadCancel* _cancel;

};


}


#endif
