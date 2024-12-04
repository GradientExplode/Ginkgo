#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/EventImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>
#include <gkg-graphics-X11-core/Session.h>


gkg::Event::Event()
{

  _eventImplementation = 
    gkg::getGUIImplementationFactory()->createEventImplementation( this );

}


gkg::Event::Event( const gkg::Event& other )
{

  _eventImplementation = 
    gkg::getGUIImplementationFactory()->createEventImplementation( this,
                                                    other.getImplementation() );

}


gkg::Event::~Event()
{

  delete _eventImplementation;

}


void gkg::Event::setDisplay( gkg::Display* display )
{

  _eventImplementation->setDisplay( display );

}


gkg::Display* gkg::Event::getDisplay() const
{

  return _eventImplementation->getDisplay();

}
 

void gkg::Event::setWindow( gkg::Window* window )
{

  _eventImplementation->setWindow( window );

}


gkg::Window* gkg::Event::getWindow() const
{

  return _eventImplementation->getWindow();

}
 

bool gkg::Event::pending() const
{

  return _eventImplementation->pending();

}


void gkg::Event::read()
{

  gkg::Session::getInstance().read( *this );

}


bool gkg::Event::read( int32_t sec, int32_t usec )
{

  return gkg::Session::getInstance().read( sec, usec, *this );

}


void gkg::Event::unread()
{

  _eventImplementation->unread();

}


void gkg::Event::poll()
{

  _eventImplementation->poll();

}


gkg::EventHandler* gkg::Event::getEventHandler() const
{

  return _eventImplementation->getEventHandler();

}


void gkg::Event::handle()
{

  _eventImplementation->handle();

}


void gkg::Event::grab( gkg::EventHandler* eventHandler ) const
{

  _eventImplementation->grab( eventHandler );

}


void gkg::Event::ungrab( gkg::EventHandler* eventHandler ) const
{

  _eventImplementation->ungrab( eventHandler );

}


gkg::EventHandler* gkg::Event::getGrabber() const
{

  return _eventImplementation->getGrabber();

}


bool gkg::Event::isGrabbing( EventHandler* eventHandler ) const
{

  return _eventImplementation->isGrabbing( eventHandler );

}


gkg::Event::Type gkg::Event::getType() const
{

  return _eventImplementation->getType();

}


uint32_t gkg::Event::getTime() const
{

  return _eventImplementation->getTime();

}


gkg::FloatCoordinate gkg::Event::getPointerX() const
{

  return _eventImplementation->getPointerX();

}


gkg::FloatCoordinate gkg::Event::getPointerY() const
{

  return _eventImplementation->getPointerY();

}


gkg::FloatCoordinate gkg::Event::getPointerRootX() const
{

  return _eventImplementation->getPointerRootX();

}


gkg::FloatCoordinate gkg::Event::getPointerRootY() const
{

  return _eventImplementation->getPointerRootY();

}


gkg::Event::Button gkg::Event::getPointerButton() const
{

  return _eventImplementation->getPointerButton();

}


uint32_t gkg::Event::getKeyMask() const
{

  return _eventImplementation->getKeyMask();

}


bool gkg::Event::controlIsDown() const
{

  return _eventImplementation->controlIsDown();

}


bool gkg::Event::metaIsDown() const
{

  return _eventImplementation->metaIsDown();

}


bool gkg::Event::shiftIsDown() const
{

  return _eventImplementation->shiftIsDown();

}


bool gkg::Event::capslockIsDown() const
{

  return _eventImplementation->capslockIsDown();

}


bool gkg::Event::leftIsDown() const
{

  return _eventImplementation->leftIsDown();

}


bool gkg::Event::middleIsDown() const
{

  return _eventImplementation->middleIsDown();

}


bool gkg::Event::rightIsDown() const
{

  return _eventImplementation->rightIsDown();

}


unsigned char gkg::Event::getKeyCode() const
{

  return _eventImplementation->getKeyCode();

}


uint32_t gkg::Event::getKeySym() const
{

  return _eventImplementation->getKeySym();

}


uint32_t gkg::Event::getMapKey( char* buffer, uint32_t length ) const
{

  return _eventImplementation->getMapKey( buffer, length );

}


gkg::EventImplementation* gkg::Event::getImplementation() const
{

  return _eventImplementation;

}


