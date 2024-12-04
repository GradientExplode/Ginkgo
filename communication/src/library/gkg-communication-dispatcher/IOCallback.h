#ifndef _gkg_communication_dispatcher_IOCallback_h_
#define _gkg_communication_dispatcher_IOCallback_h_


#include <gkg-communication-dispatcher/IOHandler.h>


namespace gkg
{


// IOCallback allows a member function to be used as an IO handler
// instead of requiring derivation of a subclass

template < class T >
class IOCallback : public IOHandler
{

  public:

    typedef int32_t ( T::*ReadyFunction )( int32_t fd );
    typedef void ( T::*TimerFunction )( int64_t sec, int64_t usec );

    IOCallback( T* object,
                ReadyFunction input,
                ReadyFunction output = 0,
                ReadyFunction except = 0 );

    IOCallback( T*,
                TimerFunction timer,
                ReadyFunction input = 0,
                ReadyFunction output = 0,
                ReadyFunction except = 0 );

    virtual int32_t isInputReady( int32_t fd );
    virtual int32_t isOutputReady( int32_t fd );
    virtual int32_t isExceptionRaised( int32_t fd );
    virtual void setTimerExpired( int64_t sec, int64_t usec );

  private:

    T* _object;
    TimerFunction _timer;
    ReadyFunction _input;
    ReadyFunction _output;
    ReadyFunction _except;

};


}


template < class T >
inline
gkg::IOCallback< T >::IOCallback( T* object,
                                  gkg::IOCallback< T >::ReadyFunction input,
                                  gkg::IOCallback< T >::ReadyFunction output,
                                  gkg::IOCallback< T >::ReadyFunction except )
                     : gkg::IOHandler(),
                       _object( object ),
                       _timer( 0 ),
                       _input( input ),
                       _output( output ),
                       _except( except )
{
}


template < class T >
inline
gkg::IOCallback< T >::IOCallback( T* object,
                                  gkg::IOCallback< T >::TimerFunction timer,
                                  gkg::IOCallback< T >::ReadyFunction input,
                                  gkg::IOCallback< T >::ReadyFunction output,
                                  gkg::IOCallback< T >::ReadyFunction except )
                     : gkg::IOHandler(),
                       _object( object ),
                       _timer( timer ),
                       _input( input ),
                       _output( output ),
                       _except( except )
{
}


template < class T >
inline
int32_t gkg::IOCallback< T >::isInputReady( int32_t fd )
{

   return ( _object->*_input )( fd );

}


template < class T >
inline
int32_t gkg::IOCallback< T >::isOutputReady( int32_t fd )
{

   return ( _object->*_output )( fd );

}


template < class T >
inline
int32_t gkg::IOCallback< T >::isExceptionRaised( int32_t fd )
{

   return ( _object->*_except )( fd );

}


template < class T >
inline
void gkg::IOCallback< T >::setTimerExpired( int64_t sec, int64_t usec )
{

   return ( _object->*_timer )( sec, usec );

}


#endif
