#ifndef _gkg_communication_thread_LoopHelper_h_
#define _gkg_communication_thread_LoopHelper_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T > class ThreadedLoop;


template < class T = int32_t >
class LoopHelper
{

  public:

    LoopHelper();
    virtual ~LoopHelper();

    virtual void launch( ThreadedLoop< T >* threadedLoop,
                         T threadCount,
                         T grain,
                         T count ) = 0;
    virtual void join() = 0;

};


}


#endif
