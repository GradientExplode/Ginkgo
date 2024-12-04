#include <gkg-communication-thread/ThreadPoolHelper.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-communication-thread/LoopTask.h>
#include <gkg-communication-thread/ThreadPool.h>


template < class T >
gkg::ThreadPoolHelper< T >::ThreadPoolHelper()
{
}


template < class T >
void gkg::ThreadPoolHelper< T >::launch( gkg::ThreadedLoop< T >* threadedLoop,
                                         T threadCount,
                                         T grain,
                                         T count )
{

  if ( threadedLoop )
  {

    T n;

    for ( n = 0; ( n < threadCount ) && ( n * grain < count ); n++ )
    {

      _taskList.push_back( new gkg::LoopTask< T >( threadedLoop ) );

    }

  }

}


template < class T >
void gkg::ThreadPoolHelper< T >::join()
{

  gkg::ThreadPool::getInstance().launch( _taskList );

  _taskList.clear();

}


template class gkg::ThreadPoolHelper< int32_t >;
template class gkg::ThreadPoolHelper< uint32_t >;
template class gkg::ThreadPoolHelper< int64_t >;
template class gkg::ThreadPoolHelper< uint64_t >;
