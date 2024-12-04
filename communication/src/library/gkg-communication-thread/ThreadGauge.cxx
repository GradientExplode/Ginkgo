#include <gkg-communication-thread/ThreadGauge.h>


template < class T >
gkg::ThreadGauge< T >::ThreadGauge()
{
}


template < class T >
gkg::ThreadGauge< T >::~ThreadGauge()
{
}


template < class T >
void gkg::ThreadGauge< T >::add( T /*value*/ )
{
}


template < class T >
void gkg::ThreadGauge< T >::reset()
{
}


template class gkg::ThreadGauge< int32_t >;
template class gkg::ThreadGauge< uint32_t >;
template class gkg::ThreadGauge< int64_t >;
template class gkg::ThreadGauge< uint64_t >;
