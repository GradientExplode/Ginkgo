#include <gkg-communication-X11/X11ThreadImplementation.h>
#include <gkg-communication-thread/Thread.h>
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <string>


gkg::X11ThreadImplementation::X11ThreadImplementation( gkg::Thread* thread )
                             : gkg::ThreadImplementation( thread )
{

  try
  {

    reset();

    if ( pthread_attr_init( &_attributes ) )
    {

      throw std::runtime_error( 
      std::string( "failed in attributes initialization for thread " ) +
      gkg::StringConverter::toString( 
                       gkg::SystemInfo::getInstance().getCurrentThreadId() ) );

    }

  }
  GKG_CATCH( "gkg::X11ThreadImplementation::X11ThreadImplementation( "
             "gkg::Thread* thread )" );

}


gkg::X11ThreadImplementation::~X11ThreadImplementation()
{

  if ( _running )
  {

    pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, 0 );
    pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, 0 );
    cancel();

  }

  pthread_attr_destroy( &_attributes );

}


void gkg::X11ThreadImplementation::reset()
{

  _thread = 0;

}


void gkg::X11ThreadImplementation::exit()
{

  pthread_exit( 0 );

}


void gkg::X11ThreadImplementation::launch()
{
 
  try
  {

    int32_t error = pthread_create( &_thread, &_attributes,
                                    gkg::Thread::bootstrap, this );
    if ( error )
    {

      throw std::runtime_error(  std::string( "failed to create thread : " ) +
                                 gkg::StringConverter::toString( error ) );

    }
 
    _idSet = true;

  }
  GKG_CATCH( "void gkg::X11ThreadImplementation::launch()" );

}


void gkg::X11ThreadImplementation::execute()
{

  try
  {

    pthread_cleanup_push( gkg::X11ThreadImplementation::cleanup, &_running );
    doExecute();
    pthread_cleanup_pop( 0 );

  }
  GKG_CATCH( "void gkg::X11ThreadImplementation::execute()" );

}


void gkg::X11ThreadImplementation::join()
{
 
  pthread_join( _thread, 0 );

}


void gkg::X11ThreadImplementation::detach()
{
 
  try
  {

    if ( pthread_attr_setdetachstate( &_attributes, PTHREAD_CREATE_DETACHED ) )
    {

      _detached = false;
      throw std::runtime_error( "failed in setting detached mode" );

    }
    else
    {

      _detached = true;

    }

  }
  GKG_CATCH( "void gkg::X11ThreadImplementation::detach()" );

}


void gkg::X11ThreadImplementation::setCancellable( bool value, bool callStop )
{

  _cancellable = value;
  _callStop = callStop;
  if ( value )
  {

    pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, 0 );

  }

}


void gkg::X11ThreadImplementation::cancel()
{

  try
  {

    if ( pthread_cancel( _thread ) )
    {

      std::cerr << "failed in cancellation" << std::endl;
      //throw std::runtime_error( "failed in cancellation" );

    }

  }
  GKG_CATCH( "void gkg::X11ThreadImplementation::cancel()" );

}


bool gkg::X11ThreadImplementation::isActive() const
{
 
  return _thread != 0;

}


void gkg::X11ThreadImplementation::testCancel()
{
 
  if ( _cancellable )
  {

    pthread_testcancel();

  }

}


uint32_t gkg::X11ThreadImplementation::getId() const
{

#if ( defined(__APPLE__) && defined(__MACH__) )

  if ( _idSet )
  {

    return ( uint32_t )( uint64_t )_thread;

  }

  return ( uint32_t )( uint64_t )pthread_self();

#else

  if ( _idSet )
  {

    return ( uint32_t )_thread;

  }

  return ( uint32_t )pthread_self();

#endif

}


void gkg::X11ThreadImplementation::cleanup( void* argument )
{

  bool* running = static_cast< bool* >( argument );

  if ( running )
  {

    *running = false;

  }

}


gkg::X11ThreadImplementation* 
gkg::X11ThreadImplementation::getImplementation( gkg::Thread* thread )
{

  return static_cast< X11ThreadImplementation* >( thread->getImplementation() );

}
