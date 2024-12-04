#include <gkg-graphics-X11-core/Stepper.h>
#include <gkg-communication-dispatcher/Dispatcher.h>
#include <gkg-communication-dispatcher/IOCallback.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Target.h>


gkg::Stepper::Stepper( gkg::Glyph* glyph,
                       gkg::Style* style,
                       gkg::TellTaleState* tellTaleState,
                       gkg::Action* action )
             : gkg::Button( new gkg::Target( glyph,
                                              gkg::Target::PrimitiveHit ),
                            style,
                            tellTaleState,
                            action )
{

  float seconds = 0.25;
  style->findAttribute( "autorepeatStart", seconds );
  _startDelay = ( int64_t )( seconds * 1000000 );
  seconds = 0.05;
  style->findAttribute( "autorepeatDelay", seconds );
  _nextDelay = ( int64_t )( seconds * 1000000 );
  _timer = new gkg::IOCallback< Stepper >( this, &gkg::Stepper::tick );

}


gkg::Stepper::~Stepper()
{

  delete _timer;

}


void gkg::Stepper::press( const Event& event )
{

  gkg::Button::press( event );
  startStepping();

}


void gkg::Stepper::release( const Event& event )
{

  stopStepping();
  gkg::Button::release( event );

}


void gkg::Stepper::startStepping()
{

  adjust();
  if ( _startDelay > 10 )
  {

    gkg::Dispatcher::getInstance().startTimer( 0, _startDelay, _timer );

  }

}

void gkg::Stepper::stopStepping()
{

  gkg::Dispatcher::getInstance().stopTimer( _timer );

}


void gkg::Stepper::tick( int64_t, int64_t )
{

  adjust();
  gkg::Dispatcher::getInstance().startTimer( 0, _nextDelay, _timer );

}
