#include <gkg-communication-dispatcher/Dispatcher.h>
#include <gkg-communication-dispatcher/IOHandler.h>
#include <sys/types.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <sys/param.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <ctime>
#include <cstring>
#include <iostream>


#if defined(sun) && NOFILE==20

#undef NOFILE
#define NOFILE 256

#endif

#ifndef SA_INTERRUPT

#define SA_INTERRUPT 0

#endif


#if defined(__GLIBC__) && (__GLIBC__==2 && __GLIBC_MINOR__>0 || __GLIBC__>2) \
    && __GNUG__<3
#define fds_bits __fds_bits
#endif


//
// class FdMask
//

namespace gkg
{


class FdMask : public fd_set 
{

  public:

    FdMask();

    void setZero();
    void setBit( int32_t fd );
    void clearBit( int32_t fd );
    bool isSet( int32_t fd ) const;
    bool hasAnySet() const;
    int32_t getSetCount() const;

};


}


gkg::FdMask::FdMask()
{

  setZero();

}


void gkg::FdMask::setZero()
{

  memset( this, 0x0, sizeof( gkg::FdMask ) );

}


void gkg::FdMask::setBit( int32_t fd )
{

  FD_SET( fd, this );

}


void gkg::FdMask::clearBit( int32_t fd )
{

  FD_CLR( fd, this );

}


bool gkg::FdMask::isSet( int32_t fd ) const
{

  return FD_ISSET( fd, this );

}


bool gkg::FdMask::hasAnySet() const
{

  const int32_t mskcnt = howmany( FD_SETSIZE, NFDBITS );
  for ( int32_t i = 0; i < mskcnt; i++ )
  {

    if ( fds_bits[ i ] )
    {

      return true;

    }

  }
  return false;

}


int32_t gkg::FdMask::getSetCount() const
{

  const int32_t mskcnt = howmany( FD_SETSIZE, NFDBITS );
  int32_t n = 0;
  for ( int32_t i = 0; i < mskcnt; i++ )
  {

    if ( fds_bits[ i ] )
    {

      for ( int32_t j = 0; j < ( int32_t )NFDBITS; j++ )
      {

        if ( ( fds_bits[ i ] & ( 1 << j ) ) != 0 )
        {

          ++ n;

        }

      }

    }

  }
  return n;

}


//
// operations on timeval structures
//

const int64_t ONE_SECOND = 1000000;


timeval operator+( timeval src1, timeval src2 )
{

  timeval sum;
  sum.tv_sec = src1.tv_sec + src2.tv_sec;
  sum.tv_usec = src1.tv_usec + src2.tv_usec;
  if ( sum.tv_usec >= ONE_SECOND )
  {

    sum.tv_usec -= ONE_SECOND;
    ++ sum.tv_sec;

  }
  else if ( ( sum.tv_sec >= 1 ) && ( sum.tv_usec < 0 ) )
  {

    sum.tv_usec += ONE_SECOND;
    -- sum.tv_sec;

  }
  return sum;

}


timeval operator-( timeval src1, timeval src2 )
{

  timeval delta;
  delta.tv_sec = src1.tv_sec - src2.tv_sec;
  delta.tv_usec = src1.tv_usec - src2.tv_usec;
  if ( delta.tv_usec < 0 )
  {

    delta.tv_usec += ONE_SECOND;
    -- delta.tv_sec;

  }
  else if ( delta.tv_usec >= ONE_SECOND )
  {

    delta.tv_usec -= ONE_SECOND;
    ++ delta.tv_sec;

  }
  return delta;

}


bool operator>( timeval src1, timeval src2 )
{

  if ( src1.tv_sec > src2.tv_sec )
  {

    return true;

  }
  else if ( ( src1.tv_sec == src2.tv_sec ) && ( src1.tv_usec > src2.tv_usec ) )
  {

    return true;

  }
  else
  {

    return false;

  }

}


bool operator<( timeval src1, timeval src2 )
{

  if ( src1.tv_sec < src2.tv_sec )
  {

    return true;

  }
  else if ( src1.tv_sec == src2.tv_sec && src1.tv_usec < src2.tv_usec )
  {

    return true;

  }
  else
  {

    return false;

  }

}


//
// struct Timer
//

namespace gkg
{


struct Timer
{

  Timer( timeval t, IOHandler* theHandler, Timer* theNext );

  timeval timerValue;
  IOHandler* handler;
  Timer* next;

};


}


gkg::Timer::Timer( timeval t,
                   gkg::IOHandler* theHandler,
                   gkg::Timer* theNext )
           : timerValue( t ),
             handler( theHandler ),
             next( theNext )
{
}


//
// class TimerQueue
//


namespace gkg
{


class TimerQueue 
{

  public:

    TimerQueue();
    virtual ~TimerQueue();

    bool isEmpty() const;
    timeval getEarliestTime() const;

    void insert( timeval futureTime, IOHandler* handler );
    void remove( IOHandler* handler );
    void expire( timeval currentTime );

    static timeval getZeroTime();
    static timeval getCurrentTime();

  private:

    Timer* _first;

    static timeval _zeroTime;

};


}


timeval gkg::TimerQueue::_zeroTime;

gkg::TimerQueue::TimerQueue()
                : _first( 0 )
{
}


gkg::TimerQueue::~TimerQueue()
{

  gkg::Timer* doomed = _first;
  while ( doomed )
  {

    gkg::Timer* next = doomed->next;
    delete doomed;
    doomed = next;

  }

}


inline
bool gkg::TimerQueue::isEmpty() const
{

  return _first == 0;

}


inline
timeval gkg::TimerQueue::getEarliestTime() const
{

  return _first->timerValue;

}


void gkg::TimerQueue::insert( timeval futureTime,
                              gkg::IOHandler* handler )
{

  if ( isEmpty() || ( futureTime < getEarliestTime() ) )
  {

    _first = new gkg::Timer( futureTime, handler, _first );

  }
  else
  {

    gkg::Timer* before = _first;
    gkg::Timer* after = _first->next;
    while ( after && ( futureTime > after->timerValue ) )
    {

      before = after;
      after = after->next;

    }
    before->next = new gkg::Timer( futureTime, handler, after );

  }

}


void gkg::TimerQueue::remove( gkg::IOHandler* handler )
{

  gkg::Timer* before = 0;
  gkg::Timer* doomed = _first;
  while ( doomed && ( doomed->handler != handler ) )
  {

    before = doomed;
    doomed = doomed->next;

  }
  if ( doomed )
  {

    if ( !before )
    {

      _first = doomed->next;

    }
    else
    {

      before->next = doomed->next;

    }
    delete doomed;

  }

}


void gkg::TimerQueue::expire( timeval currentTime )
{

  while ( !isEmpty() && ( getEarliestTime() < currentTime ) )
  {

    gkg::Timer* expired = _first;
    _first = _first->next;
    expired->handler->setTimerExpired( currentTime.tv_sec,
                                       currentTime.tv_usec );
    delete expired;

  }

}


timeval gkg::TimerQueue::getZeroTime()
{

  return _zeroTime;

}


timeval gkg::TimerQueue::getCurrentTime()
{

  timeval currentTime;
  struct timezone currentZone;
  gettimeofday( &currentTime, &currentZone );

  return currentTime;

}


//
// class Child
//

namespace gkg
{


struct Child
{

  Child( pid_t thePid, IOHandler* theHandler, Child* theNext );

  pid_t pid;            // process's PID
  int32_t status;       // wait status
  IOHandler* handler;   // associated handler
  Child* next;

};


}


gkg::Child::Child( pid_t thePid,
                   gkg::IOHandler* theHandler,
                   gkg::Child* theNext )
           : pid( thePid ),
             status( -1 ),
             handler( theHandler ),
             next( theNext )
{
}


//
// class ChildQueue
//

namespace gkg
{


class ChildQueue
{

  public:

    ChildQueue();
    virtual ~ChildQueue();

    bool isEmpty() const;
    bool isReady() const;

    void insert( pid_t pid, IOHandler* handler );
    void remove( IOHandler* handler );
    void notify();
    void setStatus( pid_t pid, int32_t status );

  private:

    Child* _first;        // queue head
    bool _ready;          // something is ready

};


}


gkg::ChildQueue::ChildQueue()
                : _first( 0 ),
                  _ready( false )
{

}


gkg::ChildQueue::~ChildQueue()
{

  gkg::Child* doomed = _first;
  while ( doomed )
  {

    gkg::Child* next = doomed->next;
    delete doomed;
    doomed = next;

  }

}


inline
bool gkg::ChildQueue::isEmpty() const
{

  return _first == 0;

}


inline
bool gkg::ChildQueue::isReady() const
{

  return _ready;

}


void gkg::ChildQueue::insert( pid_t pid, gkg::IOHandler* handler )
{

  if ( isEmpty() )
  {

    _first = new gkg::Child( pid, handler, _first );

  }
  else
  {

    gkg::Child* before = _first;
    gkg::Child* after = _first->next;
    while ( after && ( pid > after->pid ) )
    {

      before = after;
      after = after->next;

    }
    before->next = new gkg::Child( pid, handler, after );

  }

}


void gkg::ChildQueue::remove( IOHandler* handler )
{

  gkg::Child* before = 0;
  gkg::Child* doomed = _first;
  while ( doomed && ( doomed->handler != handler ) )
  {

    before = doomed;
    doomed = doomed->next;

  }
  if ( doomed )
  {

    if ( !before )
    {

      _first = doomed->next;

    }
    else
    {

      before->next = doomed->next;

    }
    delete doomed;

  }

}


void gkg::ChildQueue::notify()
{

  gkg::Child** previous = &_first;
  gkg::Child* child;

  while ( ( child = *previous ) != 0 )
  {

    if ( child->status != -1 )
    {

      child->handler->setChildStatus( child->pid, child->status );
      *previous = child->next;
      delete child;

    }
    else
    {

      previous = &child->next;

    }

  }
  _ready = false;

}


void gkg::ChildQueue::setStatus( pid_t pid, int32_t status )
{

  for ( gkg::Child* child = _first; child; child = child->next )
  {

    if ( child->pid == pid )
    {

      child->status = status;
      _ready = true;
      break;

    }

  }

}


//
// class Dispatcher
//


gkg::Dispatcher::Dispatcher()
                : _nfds( 0 ),
                  _rMask( new gkg::FdMask ),
                  _wMask( new gkg::FdMask ),
                  _eMask( new gkg::FdMask ),
                  _rMaskReady( new gkg::FdMask ),
                  _wMaskReady( new gkg::FdMask ),
                  _eMaskReady( new gkg::FdMask ),
                  _rTable( new gkg::IOHandler*[ NOFILE ] ),
                  _wTable( new gkg::IOHandler*[ NOFILE ] ),
                  _eTable( new gkg::IOHandler*[ NOFILE ] ),
                  _timerQueue( new gkg::TimerQueue ),
                  _childQueue( new gkg::ChildQueue )
{

  for ( int32_t i = 0; i < NOFILE; i++ )
  {

    _rTable[ i ] = 0;
    _wTable[ i ] = 0;
    _eTable[ i ] = 0;

  }

}


gkg::Dispatcher::~Dispatcher()
{

  delete _rMask;
  delete _wMask;
  delete _eMask;
  delete _rMaskReady;
  delete _wMaskReady;
  delete _eMaskReady;
  delete [] _rTable;
  delete [] _wTable;
  delete [] _eTable;
  delete _timerQueue;
  delete _childQueue;

}


gkg::IOHandler* 
gkg::Dispatcher::getHandler( int32_t fd, gkg::Dispatcher::Mask mask ) const
{

  if ( ( fd < 0 ) || ( fd >= NOFILE ) )
  {

    abort();

  }
  gkg::IOHandler* current = 0;
  if ( mask == gkg::Dispatcher::ReadMask )
  {

    current = _rTable[ fd ];

  }
  else if ( mask == gkg::Dispatcher::WriteMask )
  {

    current = _wTable[ fd ];

  }
  else if ( mask == gkg::Dispatcher::ExceptMask )
  {

    current = _eTable[ fd ];

  }
  else
  {

    abort();

  }
  return current;

}


void gkg::Dispatcher::link( int32_t fd,
                            gkg::Dispatcher::Mask mask,
                            gkg::IOHandler* handler )
{

  if ( ( fd < 0 ) || ( fd >= NOFILE ) )
  {

    abort();

  }
  attach( fd, mask, handler );

}


void gkg::Dispatcher::unlink( int32_t fd )
{

  if ( ( fd < 0 ) || ( fd >= NOFILE ) )
  {

    abort();

  }
  detach( fd );

}


void gkg::Dispatcher::startTimer( int64_t sec, int64_t usec,
                                  gkg::IOHandler* handler )
{

  timeval deltaTime;
  deltaTime.tv_sec = sec;
  deltaTime.tv_usec = usec;
  _timerQueue->insert( gkg::TimerQueue::getCurrentTime() + deltaTime,
                       handler );

}


void gkg::Dispatcher::stopTimer( gkg::IOHandler* handler )
{

  _timerQueue->remove( handler );

}


void gkg::Dispatcher::startChild( int32_t pid, gkg::IOHandler* handler )
{

  _childQueue->insert( pid, handler );

}


void gkg::Dispatcher::stopChild( gkg::IOHandler* handler )
{

  _childQueue->remove( handler );

}


bool gkg::Dispatcher::setReady( int32_t fd, gkg::Dispatcher::Mask mask )
{

  if ( !getHandler( fd, mask ) )
  {

    return false;

  }
  if ( mask == gkg::Dispatcher::ReadMask )
  {

    _rMaskReady->setBit( fd );

  }
  else if ( mask == gkg::Dispatcher::WriteMask )
  {

    _wMaskReady->setBit( fd );

  }
  else if ( mask == gkg::Dispatcher::ExceptMask )
  {

    _eMaskReady->setBit( fd );

  }
  else
  {

    return false;

  }
  return true;

}


void gkg::Dispatcher::dispatch()
{

  // Set short waiting time to dispatch (100ms) so that the function regularly
  // returns. Though the dispatcher can be properly stopped.
  // ( dispatch( 0 ); would wait forever that an event happens )
  timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 100000;
  dispatch( &tv );

}


bool gkg::Dispatcher::dispatch( int64_t& sec, int64_t& usec )
{

  timeval howLong;
  timeval previousTime;
  timeval elapsedTime;

  howLong.tv_sec = sec;
  howLong.tv_usec = usec;
  previousTime = gkg::TimerQueue::getCurrentTime();

  bool success = dispatch( &howLong );

  elapsedTime = gkg::TimerQueue::getCurrentTime() - previousTime;
  if ( howLong > elapsedTime )
  {

    howLong = howLong - elapsedTime;

  }
  else
  {

    howLong = gkg::TimerQueue::getZeroTime(); // Used all of timeout

  }

  sec = howLong.tv_sec;
  usec = howLong.tv_usec;
  return success;

}


void gkg::Dispatcher::setStatus( pid_t pid, int32_t status )
{

  _childQueue->setStatus( pid, status );

}


void gkg::Dispatcher::attach( int32_t fd,
                              gkg::Dispatcher::Mask mask,
                              gkg::IOHandler* handler )
{

  if ( mask == gkg::Dispatcher::ReadMask )
  {

    _rMask->setBit( fd );
    _rTable[ fd ] = handler;

  }
  else if ( mask == gkg::Dispatcher::WriteMask )
  {

    _wMask->setBit( fd );
    _wTable[ fd ] = handler;

  }
  else if ( mask == gkg::Dispatcher::ExceptMask )
  {

    _eMask->setBit( fd );
    _eTable[ fd ] = handler;

  }
  else
  {

    abort();

  }
  if ( _nfds < fd + 1 )
  {

    _nfds = fd + 1;

  }

}


void gkg::Dispatcher::detach( int32_t fd )
{

  _rMask->clearBit( fd );
  _rTable[ fd ] = 0;
  _wMask->clearBit( fd );
  _wTable[ fd ] = 0;
  _eMask->clearBit( fd );
  _eTable[ fd ] = 0;
  if ( _nfds == fd + 1 )
  {

    while ( ( _nfds > 0 ) &&
            ( _rTable[ _nfds - 1 ] == 0 ) &&
            ( _wTable[ _nfds - 1 ] == 0 ) &&
            ( _eTable[ _nfds - 1 ] == 0 ) )
    {

      -- _nfds;

    }

  }

}


bool gkg::Dispatcher::dispatch( timeval* howLong )
{

  gkg::FdMask rMaskRet;
  gkg::FdMask wMaskRet;
  gkg::FdMask eMaskRet;
  int32_t nFound;

  if ( anyReady() )
  {

    nFound = fillInReady( rMaskRet, wMaskRet, eMaskRet );

  }
  else
  {

    nFound = waitFor( rMaskRet, wMaskRet, eMaskRet, howLong );

  }
  notify( nFound, rMaskRet, wMaskRet, eMaskRet );
  return ( nFound != 0 );

}


bool gkg::Dispatcher::anyReady() const
{

  return _rMaskReady->hasAnySet() ||
         _wMaskReady->hasAnySet() ||
         _eMaskReady->hasAnySet();

}


int32_t gkg::Dispatcher::fillInReady( gkg::FdMask& rMaskRet,
                                      gkg::FdMask& wMaskRet,
                                      gkg::FdMask& eMaskRet )
{

  rMaskRet = *_rMaskReady;
  wMaskRet = *_wMaskReady;
  eMaskRet = *_eMaskReady;
  _rMaskReady->setZero();
  _wMaskReady->setZero();
  _eMaskReady->setZero();
  return rMaskRet.getSetCount() +
         wMaskRet.getSetCount() +
         eMaskRet.getSetCount();

}


int32_t gkg::Dispatcher::waitFor( gkg::FdMask& rMaskRet,
                                  gkg::FdMask& wMaskRet,
                                  gkg::FdMask& eMaskRet,
                                  timeval* howLong )
{


  int32_t nFound = 0;
  static struct sigaction sa, osa;


  if ( !_childQueue->isEmpty() )
  {

    sa.sa_handler = &gkg::Dispatcher::sigCLD;
    sa.sa_flags = SA_INTERRUPT;
    sigaction( SIGCLD, &sa, &osa );

  }

  do
  {

    rMaskRet = *_rMask;
    wMaskRet = *_wMask;
    eMaskRet = *_eMask;
    howLong = calculateTimeout( howLong );

    nFound = select( _nfds, &rMaskRet, &wMaskRet, &eMaskRet, howLong );



  }
  while ( ( nFound < 0 ) && !handleError() );

  if ( !_childQueue->isEmpty() )
  {

    sigaction( SIGCLD, &osa, ( struct sigaction* )0 );

  }

  return nFound;


}


void gkg::Dispatcher::notify( int32_t nFound,
                              gkg::FdMask& rMaskRet,
                              gkg::FdMask& wMaskRet,
                              gkg::FdMask& eMaskRet )
{

  for ( int32_t i = 0; i < _nfds && ( nFound > 0 ); i++ )
  {

    if ( rMaskRet.isSet( i ) )
    {

      int32_t status = _rTable[ i ]->isInputReady( i );
      if ( status < 0 )
      {

        detach( i );

      }
      else if ( status > 0 )
      {

        _rMaskReady->setBit( i );

      }
      -- nFound;

    }
    if ( wMaskRet.isSet( i ) )
    {

      int32_t status = _wTable[ i ]->isOutputReady( i );
      if ( status < 0 )
      {

        detach( i );

      }
      else if ( status > 0 )
      {

        _wMaskReady->setBit( i );

      }
      -- nFound;

    }
    if ( eMaskRet.isSet( i ) )
    {

      int32_t status = _eTable[ i ]->isExceptionRaised( i );
      if ( status < 0 )
      {

        detach( i );

      }
      else if ( status > 0 )
      {

        _eMaskReady->setBit( i );

      }
      -- nFound;

    }

  }

  if ( !_timerQueue->isEmpty() )
  {

    _timerQueue->expire( gkg::TimerQueue::getCurrentTime() );

  }
  if ( _childQueue->isReady() )
  {

    _childQueue->notify();

  }

}


timeval* gkg::Dispatcher::calculateTimeout( timeval* howLong ) const
{

  static timeval timeOut;

  if ( !_timerQueue->isEmpty() )
  {

    timeval currentTime;

    currentTime = gkg::TimerQueue::getCurrentTime();
    if ( _timerQueue->getEarliestTime() > currentTime )
    {

      timeOut = _timerQueue->getEarliestTime() - currentTime;
      if ( !howLong || ( *howLong > timeOut ) )
      {

        howLong = &timeOut;

      }

    }
    else
    {

      timeOut = gkg::TimerQueue::getZeroTime();
      howLong = &timeOut;

    }

  }
  return howLong;

}


bool gkg::Dispatcher::handleError()
{

  switch ( errno )
  {

    case EBADF:

      checkConnections();
      break;

    case EINTR:

      if ( _childQueue->isReady() )
      {

        return true;

      }
      break;

    default:

      std::perror( "gkg::Dispatcher: select" );
      ::exit( 1 ); //NOTREACHED

  }
  return false; // retry select

}


void gkg::Dispatcher::checkConnections()
{

  gkg::FdMask rMask;
  timeval poll = gkg::TimerQueue::getZeroTime();

  for ( int32_t fd = 0; fd < _nfds; fd++ )
  {

    if ( _rTable[ fd ] )
    {

      rMask.setBit( fd );

      if ( select( fd + 1, &rMask, 0, 0, &poll ) < 0 )
      {

        detach( fd );

      }
      rMask.clearBit( fd );

    }

  }

}


void gkg::Dispatcher::sigCLD( int32_t )
{

  pid_t pid;
  int32_t status;

  while ( ( pid = waitpid( -1, &status, WNOHANG ) ) > 0 )
  {

    gkg::Dispatcher::getInstance().setStatus(pid, status);

  }

}
