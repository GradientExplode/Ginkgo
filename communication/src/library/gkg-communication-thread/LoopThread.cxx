#include <gkg-communication-thread/LoopThread.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <iostream>


template < class T >
gkg::LoopThread< T >::LoopThread( gkg::ThreadedLoop< T >* parent )
                     : gkg::Thread(),
                       _parent( parent )
{

  if ( _parent )
  {

    setVerbose( parent->isVerbose() );

  }

  if ( _verbose )
  {

    std::cerr << "template < class T > "
              << "gkg::LoopThread< T >::LoopThread() ["
              << gkg::SystemInfo::getInstance().getCurrentThreadId()
              << "]" << std::endl;

  }

}


template < class T >
gkg::LoopThread< T >::~LoopThread()
{

  if ( _verbose )
  {

    std::cerr << "template < class T > "
              << "gkg::LoopThread< T >::LoopThread() ["
              << gkg::SystemInfo::getInstance().getCurrentThreadId()
              << "]" << std::endl;

  }

}


template < class T >
void gkg::LoopThread< T >::doRun()
{

  if ( _parent )
  {

    _parent->run();

  }

}


template class gkg::LoopThread< int32_t >;
template class gkg::LoopThread< uint32_t >;
template class gkg::LoopThread< int64_t >;
template class gkg::LoopThread< uint64_t >;
