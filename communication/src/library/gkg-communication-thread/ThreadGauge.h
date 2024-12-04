#ifndef _gkg_communication_thread_ThreadGauge_h_
#define _gkg_communication_thread_ThreadGauge_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T = int32_t >
class ThreadGauge
{

  public:

    ThreadGauge();
    virtual ~ThreadGauge();

    virtual void add( T /*value*/ );
    virtual void reset();

};


}


#endif
