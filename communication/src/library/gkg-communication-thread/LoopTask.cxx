#include <gkg-communication-thread/LoopTask.h>
#include <gkg-communication-thread/ThreadedLoop.h>


template < class T >
gkg::LoopTask< T >::LoopTask( gkg::ThreadedLoop< T >* parent )
                   : gkg::Task(),
                     _parent( parent )
{
}


template < class T >
void gkg::LoopTask< T >::run()
{

  if ( _parent )
  {

    _parent->run();

  }

}


template class gkg::LoopTask< int32_t >;
template class gkg::LoopTask< uint32_t >;
template class gkg::LoopTask< int64_t >;
template class gkg::LoopTask< uint64_t >;
