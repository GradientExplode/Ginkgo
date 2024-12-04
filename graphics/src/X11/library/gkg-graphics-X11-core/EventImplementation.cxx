#include <gkg-graphics-X11-core/EventImplementation.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/EventHandler.h>


gkg::EventImplementation::EventImplementation( gkg::Event* event )
                         : _target( event ),
                           _display( 0 ),
                           _window( 0 ),
                           _pointerX( 0 ),
                           _pointerY( 0 ),
                           _pointerRootX( 0 ),
                           _pointerRootY( 0 ),
                           _locationValid( false ),
                           _hasPointerLocation( false )
{
}


gkg::EventImplementation::EventImplementation( gkg::Event* event,
                                               gkg::EventImplementation* other )
                         : _target( event ),
                           _display( other->_display ),
                           _window( other->_window ),
                           _pointerX( other->_pointerX ),
                           _pointerY( other->_pointerY ),
                           _pointerRootX( other->_pointerRootX ),
                           _pointerRootY( other->_pointerRootY ),
                           _locationValid( other->_locationValid ),
                           _hasPointerLocation( other->_hasPointerLocation )
{
}


gkg::EventImplementation::~EventImplementation()
{

}


void gkg::EventImplementation::setDisplay( gkg::Display* display )
{

  _display = display;

}


gkg::Display* gkg::EventImplementation::getDisplay() const
{

  return _display;

}
 

void gkg::EventImplementation::setWindow( gkg::Window* window )
{

  _window = window;

}


gkg::Window* gkg::EventImplementation::getWindow() const
{

  return _window;

}
 

bool gkg::EventImplementation::pending() const
{

  gkg::Event event;
  if ( _display->getEvent( event ) )
  {

    _display->putEvent( event );
    return true;

  }
  return false;

}


void gkg::EventImplementation::unread()
{

  _display->putEvent( *_target );

}


gkg::EventHandler* gkg::EventImplementation::getEventHandler() const
{

  gkg::EventHandler* eventHandler = 0;
  if ( _window )
  {

    eventHandler = _window->getTarget( *_target );

  }
  return eventHandler;

}


void gkg::EventImplementation::handle()
{

  gkg::EventHandler* eventHandler = 0;
  if ( getType() != gkg::Event::Key )
  {

    eventHandler = getGrabber();

  }
  if ( !eventHandler )
  {

    eventHandler = getEventHandler();

  }
  if ( eventHandler )
  {

    bool b = gkg::RCGUIObject::defer( true );
    eventHandler->addReference();
    eventHandler->event( *_target );
    eventHandler->removeReference();
    gkg::RCGUIObject::flush();
    gkg::RCGUIObject::defer( b );

  }

}


void gkg::EventImplementation::grab( gkg::EventHandler* eventHandler ) const
{

  _display->grab( _window, eventHandler );

}


void gkg::EventImplementation::ungrab( gkg::EventHandler* eventHandler ) const
{

  _display->ungrab( eventHandler );

}


gkg::EventHandler* gkg::EventImplementation::getGrabber() const
{

  return _display->getGrabber();

}


bool 
gkg::EventImplementation::isGrabbing( gkg::EventHandler* eventHandler ) const
{

  return _display->isGrabbing( eventHandler );

}


gkg::FloatCoordinate gkg::EventImplementation::getPointerX()
{

  locate();
  return _pointerX;

}


gkg::FloatCoordinate gkg::EventImplementation::getPointerY()
{

  locate();
  return _pointerY;

}


gkg::FloatCoordinate gkg::EventImplementation::getPointerRootX()
{

  locate();
  return _pointerRootX;

}


gkg::FloatCoordinate gkg::EventImplementation::getPointerRootY()
{

  locate();
  return _pointerRootY;

}


void gkg::EventImplementation::clear()
{

  _locationValid = false;

}


bool gkg::EventImplementation::hasPointerLocation()
{

  locate();
  return _hasPointerLocation;

}


