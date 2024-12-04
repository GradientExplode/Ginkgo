#include <gkg-communication-thread/LoopHelper.h>


template < class T >
gkg::LoopHelper< T >::LoopHelper()
{
}


template < class T >
gkg::LoopHelper< T >::~LoopHelper()
{
}


template class gkg::LoopHelper< int32_t >;
template class gkg::LoopHelper< uint32_t >;
template class gkg::LoopHelper< int64_t >;
template class gkg::LoopHelper< uint64_t >;
