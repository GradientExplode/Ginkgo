#include <gkg-graphics-X11-X11/X11DragZoneImplementation.h>
#include <gkg-graphics-X11-X11/X11DragAtoms.h>
#include <gkg-graphics-X11-X11/X11EventImplementation.h>
#include <gkg-graphics-X11-core/DragZone.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Event.h>


//
// static functions
//

static void getDragProperty( const XEvent& xEvent, char*& buffer, 
                             int32_t& length )
{

  length = 0;
  buffer = 0;

  int64_t len = xEvent.xclient.data.l[ 4 ];
  if ( !len )
  {

    return;

  }
  XDisplay* xDisplay = xEvent.xclient.display;
  XWindow xWindow = xEvent.xclient.data.l[ 2 ];
  Atom property = xEvent.xclient.data.l[ 3 ];

  Atom actualType;
  int32_t actualFormat;
  unsigned long nItems; // Do not modify since it is architecture dependent
  unsigned long bytesAfter;
  if ( XGetWindowProperty( xDisplay, xWindow, property, 0, len, True,
                           AnyPropertyType, &actualType, &actualFormat,
                           &nItems, &bytesAfter, ( unsigned char** )&buffer )
       == Success )
  {

    length = ( int32_t )len;

  }

}


//
// class X11DragZoneImplementation
//

gkg::X11DragZoneImplementation::X11DragZoneImplementation(
                                                       gkg::DragZone* dragZone )
                               : gkg::DragZoneImplementation( dragZone )
{
}


gkg::X11DragZoneImplementation::~X11DragZoneImplementation()
{
}


bool gkg::X11DragZoneImplementation::caught( const gkg::Event& event ) const
{

  if ( !_sensitive )
  {

    return false;

  }
  return gkg::X11DragAtoms::getInstance().enter( event ) ||
         gkg::X11DragAtoms::getInstance().motion( event ) ||
         gkg::X11DragAtoms::getInstance().leave( event ) ||
         gkg::X11DragAtoms::getInstance().drop( event );

}


bool gkg::X11DragZoneImplementation::event( gkg::Event& event )
{

  gkg::Glyph* glyph = _target->getBody();
  gkg::Hit hit( &event );
  glyph->pick( _canvas, _geometryAllocation, 0, hit );
  gkg::EventHandler* eventHandler = hit.getEventHandler();
  if ( eventHandler && eventHandler != _eventHandler )
  {

    // event wasn't for us; ungrab and deliver event
    _target->leave( event );
    event.ungrab( _eventHandler );
    _grabbing = false;
    event.handle();
    return true;

  }

  gkg::X11EventImplementation* x11EventImpl =
    gkg::X11EventImplementation::getImplementation( &event );
  const XEvent& xEvent = x11EventImpl->getXEvent();
  if ( gkg::X11DragAtoms::getInstance().enter( event ) )
  {

    if ( !_grabbing )
    {

      event.grab( _eventHandler );
      _grabbing = true;

    }
    char* type;
    int32_t length;
    getDragProperty( xEvent, type, length );
    _target->enter( event, type, length );
    if ( type )
    {

      XFree( type );

    }
    return true;

  }

  if ( gkg::X11DragAtoms::getInstance().motion( event ) )
  {

    char* type;
    int32_t length;
    getDragProperty( xEvent, type, length );
    gkg::FloatCoordinate x = event.getPointerX();
    gkg::FloatCoordinate y = event.getPointerY();
    bool in = ( _geometryExtension.getLeft() <= x &&
                _geometryExtension.getRight() >= x &&
                _geometryExtension.getBottom() <= y &&
                _geometryExtension.getTop() >= y );
    if ( in && !_grabbing )
    {

      event.grab( _eventHandler );
      _grabbing = true;
      _target->enter( event, type, length );

    }
    if ( in )
    {

      _target->motion( event );

    }
    if ( !in && _grabbing )
    {

      _target->leave( event );
      event.ungrab( _eventHandler );
      _grabbing = false;
      event.handle();

    }
    if ( type )
    {

      XFree( type );

    }
    return true;

  }

  if ( gkg::X11DragAtoms::getInstance().leave( event ) )
  {

    if ( _grabbing )
    {

      event.ungrab( _eventHandler );
      _grabbing = false;

    }
    _target->leave( event );
    return true;

  }

  if ( !gkg::X11DragAtoms::getInstance().drop( event ) )
  {

    return true;

  }
  if ( _grabbing )
  {

    event.ungrab( _eventHandler );
    _grabbing = false;

  }

  char* buffer;
  int32_t length;
  getDragProperty( xEvent, buffer, length );
  _target->drop( event, buffer, length );
  if ( buffer )
  {

    XFree( buffer );

  }
  return true;

}


gkg::X11DragZoneImplementation* 
gkg::X11DragZoneImplementation::getImplementation( gkg::DragZone* dragZone )
{

  return static_cast< X11DragZoneImplementation* >(
                                                dragZone->getImplementation() );

}


