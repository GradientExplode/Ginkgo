#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-communication-thread/LoopContext.h>
//#include <gkg-communication-thread/ThreadedLoopHelper.h>
#include <gkg-communication-thread/ThreadPoolHelper.h>
#include <gkg-communication-sysinfo/SystemInfo.h>

#include <iostream>


template < class T >
gkg::ThreadedLoop< T >::ThreadedLoop( gkg::LoopContext< T >* loopContext,
                                      T startIndex,
                                      T count,
                                      T maxThreadCount,
                                      T threadsByCpu,
                                      bool verbose )
                       : _loopContext( loopContext ),
                         _grain( ( T )0 ),
                         _startIndex( startIndex ),
                         _count( count ),
                         _maxThreadCount( maxThreadCount ),
                         _threadsByCpu( threadsByCpu ),
                         _currentIndex( startIndex ),
                         _todo( count ),
                         _remain( ( T )0 ),
                         _verbose( verbose )
{

  //_loopHelper = new gkg::ThreadedLoopHelper();
  _loopHelper = new gkg::ThreadPoolHelper< T >();

}


template < class T >
gkg::ThreadedLoop< T >::~ThreadedLoop()
{

  delete _loopHelper;

}


template < class T >
void gkg::ThreadedLoop< T >::setVerbose( bool value )
{

  _verbose = value;

}


template < class T >
bool gkg::ThreadedLoop< T >::isVerbose() const
{

  return _verbose;

}


template < class T >
void gkg::ThreadedLoop< T >::run()
{

  if ( _loopContext )
  {

    _loopContext->lock();

    if ( ( _todo <= 0 ) || ( _grain < ( T )1 ) || ( _loopContext->cancel() ) )
    {

      _loopContext->unlock();

      if ( _loopContext->cancel() && _verbose )
      {

        std::cerr << "=======================> aborted by cancel" << std::endl;

      }

      if ( _verbose )
      {

        std::cerr << "  ThreadedLoop< T >::run() return immediatly"
                  << std::endl;

      }

    }
    else
    {

      T startIndex = _currentIndex;
      T count = _grain;

      _todo -= _grain;

      if ( _remain )
      {

        count++;
        _todo--;
        _remain--;

      }

      _currentIndex += count;

      if ( _verbose )
      {

        std::cerr << "  ThreadedLoop< T >::run() "
                  << "{ start=" << startIndex << ", count=" << count << " } "
                  << "[" << gkg::SystemInfo::getInstance().getCurrentThreadId()
                  << "]" << std::endl;

      }

      _loopContext->unlock();

      if ( _verbose )
      {

        std::cerr << "  ThreadedLoop::run() succeeded" << std::endl;

      }

      _loopContext->doIt( startIndex, count );

    }

  }

}


template < class T >
bool gkg::ThreadedLoop< T >::launch( bool resetGauge, bool resetCancel )
{

  if ( !_loopContext )
  {

    return false;

  }

  bool result = true;
  T nCpu = ( T )( gkg::SystemInfo::getInstance().getCpuCount() - 1 );

  if ( nCpu < ( T )1 )
  {

    nCpu = ( T )1;

  }

  if ( ( _maxThreadCount >= ( T )1 ) && ( _maxThreadCount < nCpu ) )
  {

    nCpu = _maxThreadCount;

  }

  T threadCount = nCpu * _threadsByCpu;

  if ( _verbose )
  {

    std::cerr << "CPU count     : " << nCpu << std::endl;
    std::cerr << "thread by CPU : " << _threadsByCpu << std::endl;

  }

  _loopContext->unlock();
  _loopContext->lock();

  _currentIndex = _startIndex;
  _todo = _count;
  _grain = ( _count <= threadCount ) ? ( T )1 : _count / threadCount;
  _remain = ( _count <= threadCount ) ? ( T )0 : _count % threadCount;

  _loopHelper->launch( this, threadCount, _grain, _count );
  _loopContext->unlock();
  _loopHelper->join();

  if ( _loopContext->cancel() )
  {

    result = false;

  }

  if ( resetGauge )
  {

    _loopContext->resetGauge();

  }

  if ( resetCancel )
  {

    _loopContext->resetCancel();

  }

  return result;

}


template class gkg::ThreadedLoop< int32_t >;
template class gkg::ThreadedLoop< uint32_t >;
template class gkg::ThreadedLoop< int64_t >;
template class gkg::ThreadedLoop< uint64_t >;
