#include <gkg-graphics-X11-X11/X11DragZoneSinkImplementation.h>
#include <gkg-graphics-X11-X11/X11DragAtoms.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-X11/X11EventImplementation.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/DragZoneSink.h>


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

    length = ( int )len;

  }

}


//
// class X11DragZoneSinkImplementation
//

gkg::X11DragZoneSinkImplementation::X11DragZoneSinkImplementation(
                                              gkg::DragZoneSink * dragZoneSink )
                               : gkg::DragZoneSinkImplementation( dragZoneSink )
{
}


gkg::X11DragZoneSinkImplementation::~X11DragZoneSinkImplementation()
{
}


void gkg::X11DragZoneSinkImplementation::drop( gkg::Event&, const char*, 
                                               int32_t )
{
}


void gkg::X11DragZoneSinkImplementation::draw( gkg::Canvas* canvas )
{

  // publish that this X window as capable of understanding drag messages
  if ( !_dragPublished )
  {

    gkg::Window* window = canvas->getWindow();
    if ( window )
    {

      gkg::X11WindowImplementation* x11WindowImpl =
        gkg::X11WindowImplementation::getImplementation( window );
      XDisplay* xDisplay = x11WindowImpl->getXDisplay();
      XWindow xWindow = x11WindowImpl->getXWindow();
      XChangeProperty( xDisplay, xWindow,
                       gkg::X11DragAtoms::getInstance().drag( xDisplay ),
                       XA_STRING, 8,
                       PropModePrepend, 0, 0 );
      _dragPublished = true;

    }

  }

}


void gkg::X11DragZoneSinkImplementation::pick( int32_t depth, gkg::Hit& hit )
{

  const gkg::Event* event = hit.getEvent();
  if ( event &&
       ( gkg::X11DragAtoms::getInstance().enter( *event ) ||
         gkg::X11DragAtoms::getInstance().motion( *event ) ||
         gkg::X11DragAtoms::getInstance().leave( *event ) ||
         gkg::X11DragAtoms::getInstance().drop( *event ) ) )
  {

    hit.setTarget( depth, _target, 0, _eventHandler );

  }

}


bool gkg::X11DragZoneSinkImplementation::event( gkg::Event& event )
{

  if ( gkg::X11DragAtoms::getInstance().enter( event ) ||
       gkg::X11DragAtoms::getInstance().motion( event ) ||
       gkg::X11DragAtoms::getInstance().drop( event ) )
  {

    char* type;
    int32_t length;
    gkg::X11EventImplementation* x11EventImpl =
      gkg::X11EventImplementation::getImplementation( &event );
    const XEvent& xEvent = x11EventImpl->getXEvent();
    getDragProperty( xEvent, type, length );
    if ( type )
    {

      XFree( type );

    }

  }
  return true;

}


gkg::X11DragZoneSinkImplementation* 
gkg::X11DragZoneSinkImplementation::getImplementation(
                                               gkg::DragZoneSink* dragZoneSink )
{

  return static_cast< X11DragZoneSinkImplementation* >(
                     dragZoneSink->getImplementation() );

}




