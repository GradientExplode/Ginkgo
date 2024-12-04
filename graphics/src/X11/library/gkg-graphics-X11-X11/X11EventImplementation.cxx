#include <gkg-graphics-X11-X11/X11EventImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-X11/X11Drag.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <X11/keysymdef.h>


gkg::X11EventImplementation::X11EventImplementation( gkg::Event* event )
                            : gkg::EventImplementation( event )
{

  _xEvent.type = LASTEvent;

}


gkg::X11EventImplementation::X11EventImplementation( 
                                            gkg::Event* event,
                                            gkg::X11EventImplementation* other )
                            : gkg::EventImplementation( event, other ),
                              _xEvent( other->_xEvent )
{
}


gkg::X11EventImplementation::~X11EventImplementation()
{
}


void gkg::X11EventImplementation::poll()
{

  if ( !_display )
  {

    if ( !_window )
    {

      _display = gkg::Session::getInstance().getDefaultDisplay();

    }
    else
    {

      _display = _window->getDisplay();

    }

  }

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( _display );
  gkg::X11WindowImplementation* x11WindowImpl =
    gkg::X11WindowImplementation::getImplementation( _window );

  XMotionEvent& m = _xEvent.xmotion;
  if ( !_window )
  {

    m.window = x11DisplayImpl->getXRoot();

  }
  else
  {

    m.window = x11WindowImpl->getXWindow();

  }

  XQueryPointer( x11DisplayImpl->getXDisplay(),
                 m.window, &m.root, &m.subwindow,
                 &m.x_root, &m.y_root, &m.x, &m.y, &m.state );
  m.type = MotionNotify;
  clear();

}


gkg::Event::Type gkg::X11EventImplementation::getType() const
{

  switch ( _xEvent.type )
  {

    case MotionNotify:
    case EnterNotify:
    case LeaveNotify:

      return gkg::Event::Motion;

    case ButtonPress:

      return gkg::Event::Down;

    case ButtonRelease:

      return gkg::Event::Up;

    case KeyPress:

      return gkg::Event::Key;

    default:

      return gkg::Event::OtherEvent;

  }

}


uint32_t gkg::X11EventImplementation::getTime() const
{

  switch ( _xEvent.type )
  {

    case MotionNotify:
    case EnterNotify:
    case LeaveNotify:

      return _xEvent.xmotion.time;

    case ButtonPress:
    case ButtonRelease:

      return _xEvent.xbutton.time;

    case KeyPress:

      return _xEvent.xkey.time;

    default:

      return CurrentTime;

  }

}


gkg::Event::Button gkg::X11EventImplementation::getPointerButton() const
{

  switch ( _xEvent.type )
  {

    case ButtonPress:
    case ButtonRelease:

      switch ( _xEvent.xbutton.button )
      {

	case Button1:

          return gkg::Event::Left;

	case Button2:

          return gkg::Event::Middle;

	case Button3:

          return gkg::Event::Right;

	case Button4:

          return gkg::Event::WheelDown;

	case Button5:

          return gkg::Event::WheelUp;

	default:

          return gkg::Event::OtherButton;

      }

    default:

      return gkg::Event::ButtonNone;

  }

}


uint32_t gkg::X11EventImplementation::getKeyMask() const
{

  switch ( _xEvent.type )
  {

    case MotionNotify:

      return _xEvent.xmotion.state;

    case ButtonPress:
    case ButtonRelease:

      return _xEvent.xbutton.state;

    case KeyPress:

      return _xEvent.xkey.state;

    case EnterNotify:
    case LeaveNotify:

      return _xEvent.xcrossing.state;

    default:

      // is this really correct?
      return 0;

  }

}


bool gkg::X11EventImplementation::controlIsDown() const
{

  return ( getKeyMask() & ControlMask ) != 0;

}


bool gkg::X11EventImplementation::metaIsDown() const
{

  return ( getKeyMask() & Mod1Mask ) != 0;

}


bool gkg::X11EventImplementation::shiftIsDown() const
{

  return ( getKeyMask() & ShiftMask ) != 0;

}


bool gkg::X11EventImplementation::capslockIsDown() const
{

  return ( getKeyMask() & LockMask ) != 0;

}


bool gkg::X11EventImplementation::leftIsDown() const
{

  return ( getKeyMask() & Button1Mask ) != 0;

}


bool gkg::X11EventImplementation::middleIsDown() const
{

  return ( getKeyMask() & Button2Mask ) != 0;

}


bool gkg::X11EventImplementation::rightIsDown() const
{

  return ( getKeyMask() & Button3Mask ) != 0;

}



unsigned char gkg::X11EventImplementation::getKeyCode() const
{

  if ( _xEvent.type == KeyPress )
  {

    return _xEvent.xkey.keycode;

  }
  return 0;

}


uint32_t gkg::X11EventImplementation::getKeySym() const
{

  if ( _xEvent.type == KeyPress )
  {

    int32_t keyIndex = shiftIsDown();
    return XLookupKeysym( (  XKeyEvent* )&_xEvent.xkey, keyIndex );

  }
  return XK_VoidSymbol;

}


uint32_t gkg::X11EventImplementation::getMapKey( char* buffer,
                                                 uint32_t length ) const
{

  uint32_t n = 0;
  if ( _xEvent.type == KeyPress )
  {

    n = XLookupString( ( XKeyEvent* )&_xEvent.xkey, buffer, length, 0, 0 );
    // R5 internationalization might make this superfluous
    if ( metaIsDown() )
    {

      for ( uint32_t i = 0; i < n; i++ )
      {

        buffer[ i ] |= 0200;

      }

    }

  }
  return n;

}


const XEvent& gkg::X11EventImplementation::getXEvent() const
{

  return _xEvent;

}


void gkg::X11EventImplementation::setNextXEvent()
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XNextEvent( xDisplay, &_xEvent );
  clear();

}


void gkg::X11EventImplementation::putBackXEvent()
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XPutBackEvent( xDisplay, &_xEvent );

}


void gkg::X11EventImplementation::locate()
{

  if ( !_locationValid && _window )
  {

    gkg::PixelCoordinate x, y, rootX = 0, rootY = 0;
    bool hasRootLocation = true;
    switch ( _xEvent.type )
    {

      case MotionNotify:

        x = _xEvent.xmotion.x;
        y = _xEvent.xmotion.y;
        rootX = _xEvent.xmotion.x_root;
        rootY = _xEvent.xmotion.y_root;
        break;

      case ButtonPress:
      case ButtonRelease:

        x = _xEvent.xbutton.x;
        y = _xEvent.xbutton.y;
        rootX = _xEvent.xbutton.x_root;
        rootY = _xEvent.xbutton.y_root;
        break;

      case KeyPress:

        x = _xEvent.xkey.x;
        y = _xEvent.xkey.y;
        rootX = _xEvent.xkey.x_root;
        rootY = _xEvent.xkey.y_root;
        break;

      case EnterNotify:
      case LeaveNotify:

        x = _xEvent.xcrossing.x;
        y = _xEvent.xcrossing.y;
        rootX = _xEvent.xcrossing.x_root;
        rootY = _xEvent.xcrossing.y_root;
        break;

      case ClientMessage: // drag & drop

        if ( !gkg::X11Drag::isDrag( _xEvent ) )
        {

          _hasPointerLocation = false;
          return;

        }
        gkg::X11Drag::locate( _xEvent, x, y );
        hasRootLocation = false;
        break;

      default:

        _hasPointerLocation = false;
        return;

    }
    _hasPointerLocation = true;
    _pointerX = _display->toFloatCoordinate( x );
    _pointerY = _display->toFloatCoordinate(
                  _window->getCanvas()->getPixelHeight() - y );
    _pointerRootX = _display->toFloatCoordinate( rootX );
    _pointerRootY = _display->toFloatCoordinate(
                      _display->getPixelHeight()- rootY );
    _locationValid = true;

    // as a side effect, the pointer location tells us the root-relative
    // location of the window.
    if ( hasRootLocation )
    {

      _window->getImplementation()->moveInPixelCoordinate( rootX - x,
                                                           rootY - y );

    }

  }

}


void gkg::X11EventImplementation::acknowledgeMotion()
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();

  XMotionEvent& m = _xEvent.xmotion;
  XQueryPointer( xDisplay,
                 m.window,
                 &m.root,
                 &m.subwindow,
                 &m.x_root, &m.y_root,
                 &m.x, &m.y,
                 &m.state );

}


gkg::X11EventImplementation* 
gkg::X11EventImplementation::getImplementation( gkg::Event* event )
{

  return static_cast< X11EventImplementation* >( event->getImplementation() );

}


