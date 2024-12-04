#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11CursorImplementation.h>
#include <gkg-graphics-X11-X11/X11CanvasImplementation.h>
#include <gkg-graphics-X11-X11/X11TopLevelWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11EventImplementation.h>
#include <gkg-graphics-X11-X11/X11SelectionManagerImplementation.h>
#include <gkg-graphics-X11-X11/X11Visual.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/EventHandler.h>
#include <gkg-graphics-X11-core/SelectionManager.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Style.h>


Atom gkg::X11WindowImplementation::_wmDeleteAtom = None;
Atom gkg::X11WindowImplementation::_wmProtocolsAtom = None;


gkg::X11WindowImplementation::X11WindowImplementation( gkg::Window* window,
                                                       gkg::Glyph* glyph )
                             : gkg::WindowImplementation( window, glyph ),
                               _xWindow( gkg::X11WindowImplementation::Unbound),
                               _xAttributeMask( 0 ),
                               _xClass( InputOutput )
{
}


gkg::X11WindowImplementation::~X11WindowImplementation()
{
}


void gkg::X11WindowImplementation::setCursor( gkg::Cursor* cursor )
{

  if ( _cursor != cursor )
  {

    checkBinding();
    _cursor = cursor;
    if ( _xWindow != gkg::X11WindowImplementation::Unbound )
    {

      XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
      gkg::X11CursorImplementation*
         x11CursorImpl = gkg::X11CursorImplementation::getImplementation(
                                                                    _cursor );
      if ( cursor )
      {

        XDefineCursor( xDisplay, _xWindow,
                       x11CursorImpl->getXId( _display, _visual ) );

      }
      else
      {

        XUndefineCursor( xDisplay, _xWindow );

      }
      XFlush( xDisplay );

    }

  }

}


// bind the current window description to its X counterpart, creating
// the X window if necessary
void gkg::X11WindowImplementation::bind()
{

  if ( _xWindow != gkg::X11WindowImplementation::Unbound )
  {

    _display->getImplementation()->removeWindow( _target );

  }

  _target->setAttributes();

  gkg::X11DisplayImplementation*
    x11DisplayImpl = gkg::X11DisplayImplementation::getImplementation(
                                                                     _display );
  gkg::X11Visual* x11Visual = gkg::X11Visual::getImplementation(_visual);

  _xWindow = XCreateWindow( x11DisplayImpl->getXDisplay(),
                            x11DisplayImpl->getXRoot(),
                            _xPosition, _yPosition,
                            _canvas->getPixelWidth(), _canvas->getPixelHeight(),
                            0, // border width
                            x11Visual->getDepth(),
                            _xClass,
                            x11Visual->getXVisual(),
                            _xAttributeMask, &_xAttributes );

  gkg::X11CanvasImplementation*
    x11CanvasImpl = gkg::X11CanvasImplementation::getImplementation(_canvas);
  x11CanvasImpl->setXDrawable( _xWindow );

  _display->getImplementation()->addWindow( _target );

  gkg::X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation( 
                                                       _topLevelWindow );
  _xTopLevelWindow = x11WindowImpl->getXWindow();

}


// unbind the window from its X counterpart, removing the window from the xid ->
// window table and making sure it is not on the damage list.  X will destroy
// subwindows automatically, so there is no need to destroy the window if it has
// a parent; in fact, it will cause an error if the windows are destroyed
// out of order (parent destroyed before children).
//
// If the window is top-level, then we must undraw the associated glyph.
// It is important that we remove the window from the lookup table
// before calling undraw so that an interior glyph with a subwindow
// can tell detect the top-level window is being unbound.
void gkg::X11WindowImplementation::unbind()
{

  if ( _display && _xWindow != gkg::X11WindowImplementation::Unbound )
  {

    _display->getImplementation()->removeWindow( _target );
    if ( _topLevelWindow == _target )
    {

      _glyph->undraw();

      XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
      XDestroyWindow( xDisplay, _xWindow );

    }

  }
  _xWindow = gkg::X11WindowImplementation::Unbound;
  clearMappingInfo();
  _canvas->getImplementation()->unbind();
  _canvas->getImplementation()->clearDamage();

}


bool gkg::X11WindowImplementation::isBound() const
{

  if ( _xWindow == gkg::X11WindowImplementation::Unbound )
  {

    return false;

  }

  gkg::X11DisplayImplementation*
    x11DisplayImpl = gkg::X11DisplayImplementation::getImplementation(
                                                                     _display );

  return ( _topLevelWindow == _target ) ||
	   ( find( _xTopLevelWindow,
                   x11DisplayImpl->getWindows() ) == _topLevelWindow );

}


void gkg::X11WindowImplementation::raise()
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XRaiseWindow( xDisplay, _xWindow );

}


void gkg::X11WindowImplementation::lower()
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XLowerWindow( xDisplay, _xWindow );

}


void gkg::X11WindowImplementation::move( gkg::FloatCoordinate left,
                                         gkg::FloatCoordinate bottom )
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XMoveWindow( xDisplay, _xWindow,
               _display->toPixelCoordinate( left ),
               _display->getPixelHeight() -
               _display->toPixelCoordinate( bottom ) -
               _canvas->getPixelHeight() );

}


void gkg::X11WindowImplementation::resize()
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XResizeWindow( xDisplay, _xWindow,
                 _canvas->getPixelWidth(), _canvas->getPixelHeight() );
  _needsResize = true;

}


// Look at an event that has been received for this window.
// Here we handle map/expose/configure events.  We also directly
// dispatch focus and delete events.
void gkg::X11WindowImplementation::receive( const gkg::Event& event )
{

  gkg::X11EventImplementation*
    x11EventImpl = gkg::X11EventImplementation::getImplementation(
                                                        ( gkg::Event* )&event );
  // hack to be suppressed

  const XEvent& xEvent = x11EventImpl->getXEvent();
  gkg::EventHandler* eventHandler = 0;
  gkg::SelectionManager* selectionManager;
  gkg::X11SelectionManagerImplementation* x11SelectionManagerImpl = 0;

  switch ( xEvent.type )
  {

    case MapNotify:

      mapNotify( xEvent.xmap );
      break;

    case UnmapNotify:

      unmapNotify( xEvent.xunmap );
      break;

    case Expose:

      expose( xEvent.xexpose );
      break;

    case ConfigureNotify:

      configureNotify( xEvent.xconfigure );
      break;

    case MotionNotify:

      // allow next pointer motion
      event.getImplementation()->acknowledgeMotion();
      break;

    case FocusIn:

      eventHandler = _focusIn;
      break;

    case FocusOut:

      eventHandler = _focusOut;
      break;

    case ClientMessage:

      if ( ( xEvent.xclient.message_type == getWmProtocolsAtom() ) &&
           ( xEvent.xclient.data.l[ 0 ] == ( int32_t )getWmDeleteAtom() ) )
      {

        eventHandler = _wmDelete;
        if ( !eventHandler )
        {

          gkg::Session::getInstance().quit();

        }

      }
      break;

    case SelectionRequest:

      selectionManager = _display->getPrimarySelection();
      x11SelectionManagerImpl =
      gkg::X11SelectionManagerImplementation::getImplementation(
                                                             selectionManager );
      x11SelectionManagerImpl->request( selectionManager,
                                        xEvent.xselectionrequest);
      break;

    case SelectionNotify:

      selectionManager = _display->getPrimarySelection();
      x11SelectionManagerImpl =
      gkg::X11SelectionManagerImplementation::getImplementation(
                                                             selectionManager );
      x11SelectionManagerImpl->notify( selectionManager,
                                       xEvent.xselection );
      break;

  }
  if ( eventHandler )
  {

    gkg::Event writableEvent( event );
    eventHandler->event( writableEvent );

  }

}


// Search for a handler for the given event.
// For events that have no associated pointer location, return nil.
// Otherwise, use pick on the glyph to find a handler.
gkg::EventHandler* 
gkg::X11WindowImplementation::getTarget( const gkg::Event& event ) const
{

  gkg::X11EventImplementation*
    x11EventImpl = gkg::X11EventImplementation::getImplementation(
                                                        ( gkg::Event* )&event );
  // hack to be suppressed

  const XEvent& xEvent = x11EventImpl->getXEvent();

  if ( ( xEvent.type == LeaveNotify ) || !x11EventImpl->hasPointerLocation() )
  {

    return 0;

  }

  gkg::Hit hit( &event );
  _glyph->pick( _canvas, _geometryAllocation, 0, hit );

  gkg::EventHandler* eventHandler = hit.getEventHandler();
  if ( eventHandler &&
       ( !event.getGrabber() || 
         ( event.getType() == gkg::Event::Key ) ||
         event.isGrabbing( eventHandler ) ) )
  {

    return eventHandler;

  }
  return 0;

}


void 
gkg::X11WindowImplementation::getGrabPointer( gkg::Cursor* cursor ) const
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();

  if ( cursor )
  {

    X11CursorImplementation* x11CursorImpl =
      gkg::X11CursorImplementation::getImplementation( cursor );

    XGrabPointer( xDisplay, _xWindow, True,
                  ( uint32_t )( _xAttributes.event_mask &
                                ( PointerMotionMask|PointerMotionHintMask ) ),
                  GrabModeAsync, // pointer mode
                  GrabModeAsync, // keyboard mode
                  None,         // confine to
                  x11CursorImpl->getXId( _display, _visual ),
                  CurrentTime );

  }
  else
  {

    XGrabPointer( xDisplay, _xWindow, True,
                  ( uint32_t )( _xAttributes.event_mask &
                                ( PointerMotionMask|PointerMotionHintMask ) ),
                  GrabModeAsync, // pointer mode
                  GrabModeAsync, // keyboard mode
                  None,         // confine to
                  None,         // cursor
                  CurrentTime );

  }

}


void gkg::X11WindowImplementation::ungrabPointer() const
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XUngrabPointer( xDisplay, CurrentTime );

}


void gkg::X11WindowImplementation::clearMappingInfo()
{

  _xTopLevelWindow = gkg::X11WindowImplementation::Unbound;
  _needsResize = false;
  _resized = false;
  _moved = false;
  _unmapped = false;
  _wmMapped = false;
  _mapPending = false;

}


void gkg::X11WindowImplementation::checkPosition()
{

  if ( _moved )
  {

    gkg::X11DisplayImplementation*
      x11DisplayImpl = gkg::X11DisplayImplementation::getImplementation(
                                                                     _display );
    int32_t x, y;
    XWindow xWindow;
    XTranslateCoordinates( x11DisplayImpl->getXDisplay(), _xWindow,
                           x11DisplayImpl->getXRoot(), 0, 0, &x, &y, &xWindow );
    _xPosition = x;
    _yPosition = y;
    _moved = false;

  }

}


XDisplay* gkg::X11WindowImplementation::getXDisplay() const
{

  return gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();

}


Atom gkg::X11WindowImplementation::getWmDeleteAtom() const
{

  if ( _wmDeleteAtom == None )
  {

    XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
    _wmDeleteAtom = XInternAtom( xDisplay, "WM_DELETE_WINDOW", False );

  }
  return _wmDeleteAtom;

}


Atom gkg::X11WindowImplementation::getWmProtocolsAtom() const
{

  if ( _wmProtocolsAtom == None )
  {

    XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
    _wmProtocolsAtom = XInternAtom( xDisplay, "WM_PROTOCOLS", False );

  }
  return _wmProtocolsAtom;

}


void gkg::X11WindowImplementation::mapNotify( const XMapEvent& )
{

  _needsResize = true;
  _wmMapped = true;
  _mapPending = false;

}


// We can only see an unmap if it is generated external (e.g.,
// by a window manager).  Application unmaps will unbind the window,
// thus removing it from the xid->window table.
void gkg::X11WindowImplementation::unmapNotify( const XUnmapEvent& )
{

  _glyph->undraw();
  _wmMapped = false;

}


// Handle an expose event.  Because window managers generate a variety
// of event sequences in response to maps, We defer the first resize
// until when a window is first exposed.
void gkg::X11WindowImplementation::expose( const XExposeEvent& exposeEvent )
{

  gkg::PixelCoordinate pixelWidth = _canvas->getPixelWidth();
  gkg::PixelCoordinate pixelHeight = _canvas->getPixelHeight();

  if ( _needsResize )
  {

    _needsResize = false;
    resizeInPixelCoordinate( pixelWidth, pixelHeight );

  }
  else
  {

    FloatCoordinate left = _display->toFloatCoordinate( exposeEvent.x );
    FloatCoordinate right = left +
                            _display->toFloatCoordinate( exposeEvent.width );
    FloatCoordinate top = _display->toFloatCoordinate( pixelHeight -
                                                       exposeEvent.y );
    FloatCoordinate bottom = top -
                             _display->toFloatCoordinate( exposeEvent.height );
    _canvas->redraw( left, bottom, right, top );

  }

}


// Handle an X ConfigureNotify event.  If the window has been configured
// once already, then only resize it if the new size is different.
// If it hasn't been configured once, then note the size and we'll take
// care of it when the first expose event is handled.
void gkg::X11WindowImplementation::configureNotify(
                                         const XConfigureEvent& configureEvent )
{

  _moved = true;
  if ( _resized )
  {

    if ( ( configureEvent.width != _canvas->getPixelWidth() ) || 
         ( configureEvent.height != _canvas->getPixelHeight() ) )
    {

      resizeInPixelCoordinate( configureEvent.width, configureEvent.height );

    }

  }
  else
  {

    _canvas->setPixelSize( configureEvent.width, configureEvent.height );
    _needsResize = true;

  }

}


XWindow gkg::X11WindowImplementation::getXWindow() const
{

  return _xWindow;

}


XWindow gkg::X11WindowImplementation::getXTopLevelWindow() const
{

  return _xTopLevelWindow;

}


gkg::Window* 
gkg::X11WindowImplementation::find( XWindow xWindow,
                              const std::map< XWindow, gkg::Window* >& windows )
{

  std::map< XWindow, gkg::Window* >::const_iterator
    w = windows.find( xWindow );
  if ( w != windows.end() )
  {

    gkg::Window* window = ( *w ).second;

    gkg::X11WindowImplementation* x11WindowImpl =
      gkg::X11WindowImplementation::getImplementation( window );

    std::map< XWindow, gkg::Window* >::const_iterator
      tw = windows.find( x11WindowImpl->getXTopLevelWindow() );

    if ( tw != windows.end() )
    {

      gkg::Window* topLevelWindow = ( *tw ).second;
      
      if ( topLevelWindow == x11WindowImpl->getTopLevelWindow() )
      {

        return window;

      }

    }

  }
  return 0;

}


gkg::X11WindowImplementation* 
gkg::X11WindowImplementation::getImplementation( gkg::Window* window )
{

  return static_cast< gkg::X11WindowImplementation* >(
                                                  window->getImplementation() );

}


void gkg::X11WindowImplementation::setAttributes()
{

  if ( !_visual )
  {

    _visual = gkg::X11Visual::findVisual( _display, _style );

  }

  _xAttributeMask |= CWBackPixmap;
  _xAttributes.background_pixmap = None;

  // It is necessary to set the border pixel to avoid trying
  // to use the parent's border.  The problem is the parent
  // might have a different visual.  Of course, none of this really
  // matters because we always use a border width of zero!
  _xAttributeMask |= CWBorderPixel;
  _xAttributes.border_pixel = 0;

  if ( _style->getValueIsOn( "backingStore" ) )
  {

    _xAttributeMask |= CWBackingStore;
    _xAttributes.backing_store = WhenMapped;

  }

  if ( _style->getValueIsOn( "saveUnder" ) )
  {

    _xAttributeMask |= CWSaveUnder;
    _xAttributes.save_under = true;

  }

  _xAttributeMask |= CWEventMask;
  _xAttributes.event_mask = KeyPressMask | KeyReleaseMask |
                            ButtonPressMask | ButtonReleaseMask |
                            PointerMotionMask | PointerMotionHintMask |
                            EnterWindowMask | LeaveWindowMask |
                            ExposureMask |
                            StructureNotifyMask |
                            FocusChangeMask |
                            OwnerGrabButtonMask;

  // These events are caught at the top-level and not propagated
  // out to the root window (meaning the window manager).
  gkg::X11Visual* x11Visual = gkg::X11Visual::getImplementation( _visual );

  _xAttributeMask |= CWDontPropagate;
  _xAttributes.do_not_propagate_mask = KeyPressMask | KeyReleaseMask |
                                       ButtonPressMask | ButtonReleaseMask |
                                       PointerMotionMask;

  _xAttributeMask |= CWColormap;
  _xAttributes.colormap = x11Visual->getColormap();

  if ( _cursor )
  {

    X11CursorImplementation* x11CursorImpl =
      gkg::X11CursorImplementation::getImplementation( _cursor );
    _xAttributeMask |= CWCursor;
    _xAttributes.cursor = x11CursorImpl->getXId( _display, _visual );

  }

}


void gkg::X11WindowImplementation::doMap()
{

  X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( _display );
  gkg::X11Visual* x11Visual = gkg::X11Visual::getImplementation(_visual);
  gkg::X11Visual* x11Visual2 = 
    gkg::X11Visual::getImplementation( x11DisplayImpl->getDefaultVisual() );

  XColormap colormap = x11Visual->getColormap();
  XColormap colormap2 = x11Visual2->getColormap();
  if ( ( ( _xAttributeMask & CWOverrideRedirect ) != 0 ) &&
       ( _xAttributes.override_redirect && colormap != colormap2 ) )
  {

    XInstallColormap( x11DisplayImpl->getXDisplay(), colormap );

  }
  XMapRaised( x11DisplayImpl->getXDisplay(), _xWindow );
  _mapPending = true;

}


void gkg::X11WindowImplementation::doUnmap()
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XUnmapWindow( xDisplay, _xWindow );

}


uint32_t gkg::X11WindowImplementation::getXAttributeMask() const
{

  return _xAttributeMask;

}


void gkg::X11WindowImplementation::setXAttributeMask( uint32_t mask )
{

  _xAttributeMask = mask;

}


XSetWindowAttributes& gkg::X11WindowImplementation::getXAttributes()
{

  return _xAttributes;

}


const XSetWindowAttributes& gkg::X11WindowImplementation::getXAttributes() const
{

  return _xAttributes;

}


void gkg::X11WindowImplementation::forceXWindow( XWindow xWindow )
{

  _xWindow = xWindow;

}


void gkg::X11WindowImplementation::forceXTopLevelWindow( 
                                                       XWindow xTopLevelWindow )
{

  _xTopLevelWindow = xTopLevelWindow;

}
