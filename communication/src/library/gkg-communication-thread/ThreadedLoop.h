#ifndef _gkg_communication_thread_ThreadedLoop_h_
#define _gkg_communication_thread_ThreadedLoop_h_


#include <gkg-communication-thread/LoopHelper.h>
#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T > class LoopContext;


template < class T = int32_t >
class ThreadedLoop
{

  public:

    ThreadedLoop( LoopContext< T >* loopContext,
                  T startIndex,
                  T count,
                  T maxThreadCount = ( T )0,
                  T threadsByCpu = ( T )1,
                  bool verbose = false );
    virtual ~ThreadedLoop();

    void setVerbose( bool value );
    bool isVerbose() const;

    void run();

    bool launch( bool resetGauge = true, bool resetCancel = true );

  protected:

    LoopContext< T >* _loopContext;
    LoopHelper< T >* _loopHelper;

    T _grain;
    T _startIndex;
    T _count;
    T _maxThreadCount;
    T _threadsByCpu;

    T _currentIndex;
    T _todo;
    T _remain;

    bool _verbose;

};


}


#endif
