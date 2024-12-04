#include <gkg-graphics-X11-X11/X11DragImplementation.h>
#include <gkg-graphics-X11-X11/X11DragAtoms.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11CursorImplementation.h>
#include <gkg-graphics-X11-X11/X11EventImplementation.h>
#include <gkg-graphics-X11-core/DragBox.h>
#include <gkg-graphics-X11-core/Drag.h>
#include <gkg-graphics-X11-core/PopupWindow.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/GeometryRequisition.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-communication-tcpip/Host.h>
#include <sstream>


// how is this done portably? it is used to generate a name unique to
// this process.
extern "C"
{

pid_t getpid();

}


//
// static prototypes
//

static void setDragProperty( XDisplay* xDisplay,
                             XEvent& xEvent,
                             XWindow destination,
                             Atom messageType,
                             int32_t x, int32_t y,
                             const char* value = 0,
                             int32_t length = 0 );
static bool understandsDragging( XDisplay* xDisplay, XWindow xWindow );
//static XWindow translate( XDisplay* xDisplay, XWindow xRoot, int& x, int& y );
static XWindow translate( XDisplay* xDisplay, XWindow xRoot, XWindow under,
                          int32_t& x, int32_t& y );


//
// static functions
//

static void setDragProperty( XDisplay* xDisplay,
                             XEvent& xEvent,
                             XWindow destination,
                             Atom messageType,
                             int32_t x, int32_t y,
                             const char* value,
                             int32_t length ) 
{

  Atom property = None;
  if ( length != 0 )
  {

    std::string buffer;
    std::ostringstream name( buffer );
    name << gkg::X11DragAtoms::getInstance().getDragName()
         << "_" << gkg::Host::getName()
         << "_" << getpid()
         << "_"  << gkg::X11DragAtoms::getInstance().getDropUid() << '\0';
    gkg::X11DragAtoms::getInstance().incDropUid();
    property = XInternAtom( xDisplay, name.str().c_str(), False );

    XChangeProperty( xDisplay, destination, property, XA_STRING, 8,
                     PropModePrepend, ( unsigned char* )value, length );

  }

  xEvent.xclient.type = ClientMessage;
  xEvent.xclient.window = destination;
  xEvent.xclient.display = xDisplay;
  xEvent.xclient.message_type = messageType;
  xEvent.xclient.format = 32;

  xEvent.xclient.data.l[0] = x;
  xEvent.xclient.data.l[1] = y;

  xEvent.xclient.data.l[2] = destination;
  xEvent.xclient.data.l[3] = property;
  xEvent.xclient.data.l[4] = length;

}


static bool understandsDragging( XDisplay* xDisplay, XWindow xWindow )
{

  if ( xWindow == None )
  {

    return false;

  }

  Atom actualType;
  int32_t actualFormat;
  unsigned long nItems; // Do not modify since it is architecture dependent
  unsigned long bytesAfter;
  unsigned char* buffer = 0;
  if ( XGetWindowProperty( xDisplay, xWindow,
                           gkg::X11DragAtoms::getInstance().drag( xDisplay ),
                           0, 0,
	                   False, AnyPropertyType, &actualType, &actualFormat,
                           &nItems, &bytesAfter, ( unsigned char** )&buffer )
       != Success )
  {

    return false;

  }
  if ( buffer )
  {

    XFree( ( caddr_t )buffer );

  }
  return actualType != None;

}


//  never used
/*static XWindow translate( XDisplay* xDisplay, XWindow xRoot, int& x, int& y )
{

  XWindow destination = None;
  XWindow previous = xRoot;
  XWindow xChild;
  int nx, ny;
  XTranslateCoordinates( xDisplay, xRoot, xRoot, x, y, &nx, &ny, &xChild );
  while ( xChild != None )
  {

    destination = xChild;
    XTranslateCoordinates( xDisplay, previous, destination, x, y, &nx, &ny,
                           &xChild );
    x = nx;
    y = ny;
    previous = destination;

  }
  return understandsDragging( xDisplay, destination ) ? destination : None;

}
*/


static XWindow translate( XDisplay* xDisplay, XWindow xRoot, XWindow under,
                          int32_t& x, int32_t& y )
{

  XWindow parent;
  XWindow* children;
  uint32_t kids;
  Status status;
  status = XQueryTree( xDisplay, xRoot, &xRoot, &parent, &children, &kids );
  if ( status == 0 )
  {

    return None;

  }
  int32_t i;
  for ( i = kids - 1; i >= 0 && children[ i ] != under ; i-- );
  for ( --i; i >= 0; --i )
  {

    XWindowAttributes attributes;
    XGetWindowAttributes( xDisplay, children[ i ], &attributes );
    if ( ( attributes.map_state == IsViewable ) &&
         ( attributes.x <= x ) &&
         ( attributes.x + attributes.width >= x ) &&
         ( attributes.y <= y ) &&
         ( attributes.y + attributes.height >= y ) )
    {

      break;

    }

  }
  if ( i < 0 )
  {

    return None;

  }

  XWindow destination = None;
  XWindow previous = xRoot;
  XWindow child = children[ i ];
  XFree( ( caddr_t )children );
  int32_t nx, ny;
  while ( child != None )
  {

    destination = child;
    XTranslateCoordinates( xDisplay, previous, destination, x, y,
                           &nx, &ny, &child );
    x = nx;
    y = ny;
    previous = destination;

  }
  return understandsDragging( xDisplay, destination ) ? destination : None;

}


//
// struct DragMethod
//

gkg::DragMethod::~DragMethod()
{
}


//
// struct DragMethodCursor
//

XWindow gkg::DragMethodCursor::setup( XDisplay* xDisplay,
                                      gkg::Event& event,
                                      Drag* )
{

  int32_t x, y;
  XWindow xRoot;
  XWindow xChild;
  int32_t wx, wy;
  uint32_t keys;

  gkg::X11WindowImplementation* x11WindowImpl =
  gkg::X11WindowImplementation::getImplementation(
                                       event.getImplementation()->getWindow() );
  XQueryPointer( xDisplay, x11WindowImpl->getXWindow(), &xRoot, &xChild,
                 &x, &y, &wx, &wy, &keys );

  XWindow xWindow;
  XSetWindowAttributes attributes;
  attributes.override_redirect = true;
  xWindow = XCreateWindow(
                      xDisplay, xRoot, 0, 0,
                      event.getDisplay()->getImplementation()->getPixelWidth(),
                      event.getDisplay()->getImplementation()->getPixelHeight(),
                      0, ( int32_t )CopyFromParent, InputOnly, CopyFromParent,
                      CWOverrideRedirect, &attributes );
  XMapWindow( xDisplay, xWindow );

  return xWindow;

}


bool gkg::DragMethodCursor::moveWindow( XDisplay*, XWindow, int32_t, int32_t )
{

  return false;

}


void gkg::DragMethodCursor::cleanup( XDisplay* xDisplay, XWindow xWindow )
{

  XDestroyWindow( xDisplay, xWindow );

}


//
// class DragMethodWindow
//

XWindow gkg::DragMethodWindow::setup( XDisplay* xDisplay,
                                      gkg::Event& event,
                                      Drag* drag )
{

  drag->getDragOffset( event, _dx, _dy );

  gkg::WidgetFactory*
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
  gkg::DragBox* 
    dragBox = new gkg::DragBox( drag->getDragGlyph(),
                                   widgetFactory->getBackground() );
  _dragWindow = new gkg::PopupWindow( dragBox );
  gkg::Style* dragStyle = new gkg::Style;
  dragStyle->addAttribute( "saveUnder", "true" );
  _dragWindow->setStyle( dragStyle );

  int32_t x, y;
  XWindow xRoot;
  XWindow xChild;
  int32_t wx, wy;
  uint32_t keys;

  // What we really want is for the window to appear with the drag glyph
  // already drawn (it looks really bad if that is not done).
  //
  // This is how it seems like we should be able to do it:
  //   1) bind the gkg window to an X window
  //   2) repair the gkg window
  //   3) flush the X display connection
  //   4) sync with the X display
  //   5) map the X window
  //
  // But we can't bind then map the gkg window, so the following is a
  // hack that seems to work.
  XSynchronize( xDisplay, true );

  gkg::X11WindowImplementation* x11WindowImpl =
  gkg::X11WindowImplementation::getImplementation(
                                       event.getImplementation()->getWindow() );

  XQueryPointer( xDisplay, x11WindowImpl->getXWindow(), &xRoot, &xChild,
                 &x, &y, &wx, &wy, &keys );
  gkg::GeometryRequisition geometryRequisition;
  dragBox->request( geometryRequisition );
  _dragWindow->setPlace(
           event.getDisplay()->toFloatCoordinate( x - _dx ),
           event.getDisplay()->getHeight() -
           event.getDisplay()->toFloatCoordinate( y - _dy ) -
           geometryRequisition.getRequirement( gkg::DimensionY ).getNatural() );
  _dragWindow->map();


  XWindow xWindow = gkg::X11WindowImplementation::getImplementation(
                                                    _dragWindow )->getXWindow();
  gkg::Event dummyEvent;
  dummyEvent.setDisplay( event.getDisplay() );
  while ( !dragBox->isEverDrawn() )
  {

    if ( dummyEvent.read( 0, 10 ) && drag->commit( dummyEvent ) )
    {

      cleanup( xDisplay, xWindow );
      XSynchronize( xDisplay, false );
      return None;

    }

  }
  XSynchronize( xDisplay, false );

  XQueryPointer( xDisplay, xWindow, &xRoot, &xChild, &x, &y, &wx, &wy, &keys );
  _wlx = x - _dx;     ///////  originally it was int _wlx=...
  _wly = y - _dy;     ///////  originally it was int _wly=...
  XMoveWindow( xDisplay, xWindow, _wlx, _wly );
  return xWindow;

}


bool gkg::DragMethodWindow::moveWindow( XDisplay* xDisplay,
                                        XWindow xWindow,
                                        int32_t x, int32_t y )
{

  // check to see if motion was due to pointer moving or window moving
  if ( ( _wlx == ( x - _dx ) ) && ( _wly == ( y - _dy ) ) )
  {

    return false;

  }
  _wlx = x - _dx;
  _wly = y - _dy;
  XMoveWindow( xDisplay, xWindow, _wlx, _wly );
  return true;

}


void gkg::DragMethodWindow::cleanup( XDisplay*, XWindow )
{

  _dragWindow->unmap();
  delete _dragWindow;

}


//
// class X11DragImplementation
//

gkg::X11DragImplementation::X11DragImplementation( gkg::Drag* drag )
                           : gkg::DragImplementation( drag )
{
}



gkg::X11DragImplementation::~X11DragImplementation()
{
}


bool gkg::X11DragImplementation::event( gkg::Event& event )
{

  if ( _target )
  {

    _target->addReference();

  }
  if ( _target->getDragGlyph() )
  {

    _method = &_methodWindow;

  }
  else
  {

    _method = &_methodCursor;

  }

  gkg::Display* display = event.getDisplay();

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( display );

  XDisplay* xDisplay = x11DisplayImpl->getXDisplay();

  XWindow xWindow = _method->setup( xDisplay, event, _target );
  if ( xWindow == None )
  {

    if ( _target )
    {

      _target->removeReference();

    }
    return true;

  }

  int32_t x, y;
  XWindow xRoot;
  XWindow xChild;
  int32_t wx, wy;
  uint32_t keys;
  XQueryPointer( xDisplay, xWindow, &xRoot, &xChild, &x, &y, &wx, &wy, &keys );

  XCursor xCursor = None;
  gkg::Cursor* cursor = _target->getDragCursor();
  if ( cursor )
  {

    
    xCursor = 
    gkg::X11CursorImplementation::getImplementation( cursor )->getXId(
         display, x11DisplayImpl->getDefaultVisual() );

  }

  gkg::Event dragEvent;
  dragEvent.setDisplay( display );
  if ( XGrabPointer( xDisplay, xWindow, False,
                     ( uint32_t )( ButtonMotionMask |
                                   ButtonPressMask |
                                   ButtonReleaseMask ),
                     GrabModeAsync, GrabModeAsync,
                     None, xCursor, CurrentTime ) != GrabSuccess )
  {

    // if we can't grab the pointer then we can't track the pointer properly
    // so just give up.
    return true;

  }

  XWindow last = None;
  bool aborted = false;
  int32_t lx = x;
  int32_t ly = y;

  // send a drag enter event to the window that the pointer is over.
  last = translate( xDisplay, xRoot, xWindow, lx, ly );
  if ( last != None )
  {

    const char* value;
    int32_t length;
    _target->getDragType( value, length );
    XEvent xEvent;
    setDragProperty( xDisplay, xEvent, last,
                     gkg::X11DragAtoms::getInstance().enter( xDisplay ),
                     lx, ly,
                     value, length );
    XSendEvent( xDisplay, last, False, NoEventMask, &xEvent );

  }

  lx = 0;
  ly = 0;
  do
  {

    dragEvent.read();
    if ( dragEvent.getType() == gkg::Event::Motion )
    {

      dragEvent.getImplementation()->acknowledgeMotion();

      gkg::X11EventImplementation* x11EventImpl =
        gkg::X11EventImplementation::getImplementation( &dragEvent );

      x = x11EventImpl->getXEvent().xmotion.x_root;
      y = x11EventImpl->getXEvent().xmotion.y_root;

      // check to see if motion was due to pointer or window.
      _method->moveWindow( xDisplay, xWindow, x, y );

      // translate ignores the drag window.
      int32_t zx = x;
      int32_t zy = y;
      XWindow zone = translate( xDisplay, xRoot, xWindow, zx, zy );
      if ( ( last != None ) && ( last != zone ) )
      {

        // send drag leave to last window that was dragged over.
        XEvent xEvent;
        setDragProperty( xDisplay, xEvent, last,
                         gkg::X11DragAtoms::getInstance().leave( xDisplay ),
                         zx, zy );
        XSendEvent( xDisplay, last, False, NoEventMask, &xEvent );

      }
      if ( zone != None )
      {

        // send drag motion (or enter) to window that pointer is being moved
        // over
        XEvent xEvent;
        const char* value;
        int32_t length;
        _target->getDragType( value, length );
        Atom 
        messageType = ( last != zone ) ?
                      gkg::X11DragAtoms::getInstance().enter( xDisplay ) :
                      gkg::X11DragAtoms::getInstance().motion( xDisplay );
        setDragProperty( xDisplay, xEvent, zone, messageType, zx, zy,
                         value, length );
        XSendEvent( xDisplay, zone, False, NoEventMask, &xEvent );

      }
      last = zone;
      lx = zx;
      ly = zy;

    }
    else if ( _target->abort( dragEvent ) )
    {

      if ( last != None )
      {

        // send drag abort to window that pointer is over.
        XEvent xEvent;
        setDragProperty( xDisplay, xEvent, last,
                         gkg::X11DragAtoms::getInstance().leave( xDisplay ),
                         lx, ly	);
        XSendEvent( xDisplay, last, False, NoEventMask, &xEvent );

      }
      aborted = true;

    }
    else if ( gkg::X11DragAtoms::getInstance().enter( dragEvent ) ||
              gkg::X11DragAtoms::getInstance().motion( dragEvent ) ||
              gkg::X11DragAtoms::getInstance().leave( dragEvent ) ||
              gkg::X11DragAtoms::getInstance().drop( dragEvent ) )
    {

      // deliver drag events to a drag zone in this session.
      dragEvent.handle();

    }

  }
  while ( !( _target->commit( dragEvent ) || aborted ) );

  XUngrabPointer( xDisplay, CurrentTime );
  _method->cleanup( xDisplay, xWindow );

  if ( !( aborted || ( last == None ) ) )
  {

    const char* value;
    int32_t length;
    _target->getDragData( value, length );
    XEvent xEvent;
    setDragProperty( xDisplay, xEvent, last,
                     gkg::X11DragAtoms::getInstance().drop( xDisplay ),
                     lx, ly,
                     value, length );
    XSendEvent( xDisplay, last, False, NoEventMask, &xEvent );

  }
  if ( _target )
  {

    _target->removeReference();

  }
  return true;

}


gkg::X11DragImplementation* 
gkg::X11DragImplementation::getImplementation( gkg::Drag* drag )
{

  return static_cast< X11DragImplementation* >( drag->getImplementation() );

}


