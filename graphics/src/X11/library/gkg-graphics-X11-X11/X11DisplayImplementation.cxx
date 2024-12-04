#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11EventImplementation.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-X11/X11Visual.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Display.h>
#include <sys/ioctl.h>

//#if !( defined(__APPLE__) && defined(__MACH__) )

//// stropts.h does not exist on MacOS
//#include <stropts.h>

//#endif

gkg::X11DisplayImplementation::X11DisplayImplementation(
                                                     gkg::Display* display,
                                                     const std::string& device )
                              : gkg::DisplayImplementation( display, device ),
                                _xRoot( 0 ),
                                _screen( 0 )
{

  if ( device.empty() )
  {

    _xDisplay = XOpenDisplay( 0 );

  }
  else
  {

    _xDisplay = XOpenDisplay( device.c_str() );

  }
  _screen = DefaultScreen( _xDisplay );

}


gkg::X11DisplayImplementation::~X11DisplayImplementation()
{

  _windows.clear();

}


void gkg::X11DisplayImplementation::close()
{

  XCloseDisplay( _xDisplay );

}


int32_t gkg::X11DisplayImplementation::getFd() const
{

  return ConnectionNumber( _xDisplay );

}


gkg::FloatCoordinate gkg::X11DisplayImplementation::getAWidth() const
{

  return ( gkg::FloatCoordinate )millimetersToPoints( 
                               ( double )DisplayWidthMM( _xDisplay, _screen ) );

}


gkg::FloatCoordinate gkg::X11DisplayImplementation::getAHeight() const
{

  return ( gkg::FloatCoordinate )millimetersToPoints( 
                              ( double )DisplayHeightMM( _xDisplay, _screen ) );

}


double gkg::X11DisplayImplementation::millimetersToPoints( double millimeters )
{

  // convert millimeters to points; we use 72.0 pts/in and 25.4 mm/in.
  return ( 72.0 / 25.4 ) * millimeters;

}


bool gkg::X11DisplayImplementation::getDefaults( std::string& defaults ) const
{

  const char* charDefaults = XResourceManagerString( _xDisplay );

  if ( charDefaults )
  {

    defaults = std::string( charDefaults );
    return true;

  }
  return false;

}


void gkg::X11DisplayImplementation::setStyle( gkg::Style* style )
{

  referenceStyle( style );
  setScreen( _screen );
  if ( style->getValueIsOn( "synchronous" ) )
  {

    XSynchronize( _xDisplay, True );

  }

}


void gkg::X11DisplayImplementation::setScreen( int screen )
{

  if ( ( screen < 0 ) || ( screen >= ScreenCount( _xDisplay ) ) )
  {

    return;

  }

  _screen = screen;
  _xRoot = RootWindow( _xDisplay, screen );
  _defaultVisual = gkg::X11Visual::findVisual( _target, _style );
  _pixelWidth = DisplayWidth( _xDisplay, screen );
  _pixelHeight = DisplayHeight( _xDisplay, screen );
  setDpi();
  _point = 1 / _pixel;
  _width = toFloatCoordinate( _pixelWidth );
  _height = toFloatCoordinate( _pixelHeight );

}


void gkg::X11DisplayImplementation::flush()
{

  XFlush( _xDisplay );

}


void gkg::X11DisplayImplementation::sync()
{

  XSync( _xDisplay, 0 );

}


// Read the next event if one is pending.  Otherwise, return false.
// Window::receive will be called on the target window for the event,
// if the window is known and is valid.  Because we don't keep track
// of subwindows, it is possible to get an event for a subwindow after
// the main window has been unmapped.  We must ignore such events.
bool gkg::X11DisplayImplementation::getEvent( gkg::Event& event )
{

  X11EventImplementation*
    x11EventImpl = X11EventImplementation::getImplementation( &event );

  x11EventImpl->setDisplay( _target );
  const XEvent& xEvent = x11EventImpl->getXEvent();

  if ( _damagedWindows.size() && ( QLength( _xDisplay ) == 0 ) )
  {

    repair();

  }

  if ( !XPending( _xDisplay ) )
  {

    return false;

  }

  x11EventImpl->setNextXEvent();

  event.setWindow( X11WindowImplementation::find( xEvent.xany.window,
                                                  _windows ) );
  if ( event.getWindow() )
  {

    event.getWindow()->receive( event );

  }
  return true;

}


void gkg::X11DisplayImplementation::putEvent( const gkg::Event& event )
{

  X11EventImplementation*
    x11EventImpl = X11EventImplementation::getImplementation( 
                                                        ( gkg::Event* )&event );
  // hack to be suppressed

  x11EventImpl->putBackXEvent();

}


#if !defined(FIONREAD) && defined(sun) && defined(__svr4__)

#define FIONREAD I_NREAD

#endif

bool gkg::X11DisplayImplementation::isClosed()
{

  if ( XEventsQueued( _xDisplay, QueuedAfterReading ) == 0 )
  {

    // need to detect whether partial event or connection closed
    int32_t fd = ConnectionNumber( _xDisplay );
    int32_t pending = 0;
    if ( ( ioctl( fd, FIONREAD, ( char* )&pending ) < 0 ) || ( pending == 0 ) )
    {

      return true;

    }

  }
  return false;

}


void gkg::X11DisplayImplementation::ringBell( int volume )
{

  if ( volume > 100 )
  {

    XBell( _xDisplay, 100 );

  }
  else if ( volume >= 0 )
  {

    XBell( _xDisplay, volume );

  }

}


void gkg::X11DisplayImplementation::setKeyClick( int32_t percent )
{

  XKeyboardControl k;
  k.key_click_percent = percent;
  XChangeKeyboardControl( _xDisplay, KBKeyClickPercent, &k );

}


void gkg::X11DisplayImplementation::setAutoRepeat( bool value )
{

  if ( value )
  {

    XAutoRepeatOn( _xDisplay );

  }
  else
  {

    XAutoRepeatOff( _xDisplay );

  }

}


void gkg::X11DisplayImplementation::setPointerFeedback( int32_t thresh, 
                                                        int32_t scale )
{

  XChangePointerControl( _xDisplay, True, True, scale, 1, thresh );

}


void gkg::X11DisplayImplementation::movePointer( gkg::FloatCoordinate x,
                                                 gkg::FloatCoordinate y )
{

  XWarpPointer( _xDisplay, None, _xRoot, 0, 0, 0, 0,
                toPixelCoordinate( x ), _pixelHeight - toPixelCoordinate( y ) );

}


XDisplay* gkg::X11DisplayImplementation::getXDisplay() const
{

  return _xDisplay;

}


XWindow gkg::X11DisplayImplementation::getXRoot() const
{

  return _xRoot;

}


uint32_t gkg::X11DisplayImplementation::getScreen() const
{

  return _screen;

}


void gkg::X11DisplayImplementation::addWindow( gkg::Window* window )
{

  X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation( window);
  _windows[ x11WindowImpl->getXWindow() ] = window;

}


void gkg::X11DisplayImplementation::removeWindow( gkg::Window* window )
{

  X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation( window);
  std::map< XWindow, gkg::Window* >::iterator
     w = _windows.find( x11WindowImpl->getXWindow() );
  if ( w != _windows.end() )
  {

    _windows.erase( w );

  }
  removeFromLists( window );


}


const std::map< XWindow, gkg::Window* >& 
gkg::X11DisplayImplementation::getWindows() const
{

  return _windows;

}


gkg::X11DisplayImplementation* 
gkg::X11DisplayImplementation::getImplementation( gkg::Display* display )
{

  return static_cast<X11DisplayImplementation*>( display->getImplementation() );

}



