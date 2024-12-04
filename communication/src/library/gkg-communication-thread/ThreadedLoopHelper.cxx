#include <gkg-communication-thread/ThreadedLoopHelper.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-communication-thread/LoopThread.h>


template < class T >
gkg::ThreadedLoopHelper< T >::ThreadedLoopHelper()
{
}


template < class T >
void gkg::ThreadedLoopHelper< T >::launch( gkg::ThreadedLoop< T >* threadedLoop,
                                           T threadCount,
                                           T grain,
                                           T count )
{

  if ( threadedLoop )
  {

    T n;

    for ( n = 0; ( n < threadCount ) && ( n * grain < count ); n++ )
    {

      gkg::LoopThread< T >* 
        loopThread = new gkg::LoopThread< T >( threadedLoop );

      _threadList.push_back( loopThread );
      loopThread->launch();

    }

  }

}


template < class T >
void gkg::ThreadedLoopHelper< T >::join()
{

  typename std::list< gkg::LoopThread< T >* >::iterator
    t = _threadList.begin(),
    te = _threadList.end();

  while ( t != te )
  {

    (*t)->join();
    delete *t;
    ++t;

  }

  _threadList.clear();

}


template class gkg::ThreadedLoopHelper< int32_t >;
template class gkg::ThreadedLoopHelper< uint32_t >;
template class gkg::ThreadedLoopHelper< int64_t >;
template class gkg::ThreadedLoopHelper< uint64_t >;
