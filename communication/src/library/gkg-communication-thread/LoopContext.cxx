#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadGauge.h>
#include <gkg-communication-thread/ThreadCancel.h>


template < class T >
gkg::LoopContext< T >::LoopContext( gkg::ThreadGauge< T >* gauge,
                                    gkg::ThreadCancel* cancel )
                      : gkg::ExclusiveContext(),
                        _gauge( gauge ),
                        _cancel( cancel )
{
}


template < class T >
gkg::LoopContext< T >::~LoopContext()
{
}


template < class T >
void gkg::LoopContext< T >::gaugeAdd( T value )
{

  if ( _gauge )
  {

    _gauge->add( value );

  }

}


template < class T >
bool gkg::LoopContext< T >::cancel()
{

  if ( _cancel )
  {

    return _cancel->check();

  }
  return false;

}


template < class T >
void gkg::LoopContext< T >::resetGauge()
{

  if ( _gauge )
  {

    _gauge->reset();

  }

}


template < class T >
void gkg::LoopContext< T >::resetCancel()
{

  if ( _cancel )
  {

    _cancel->reset();

  }

}


template class gkg::LoopContext< int32_t >;
template class gkg::LoopContext< uint32_t >;
template class gkg::LoopContext< int64_t >;
template class gkg::LoopContext< uint64_t >;
