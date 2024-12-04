#include <gkg-graphics-X11-X11/X11ManagedWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11BitmapImplementation.h>
#include <gkg-graphics-X11-core/ManagedWindow.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/CanvasImplementation.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-communication-tcpip/Host.h>
#include <X11/Xatom.h>


gkg::X11ManagedWindowImplementation::X11ManagedWindowImplementation( 
                                             gkg::ManagedWindow* managedWindow )
                             : gkg::ManagedWindowImplementation( managedWindow )
{
}


gkg::X11ManagedWindowImplementation::~X11ManagedWindowImplementation()
{
}


void gkg::X11ManagedWindowImplementation::iconify()
{

  gkg::X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation(_target);

  XWindow xWindow = x11WindowImpl->getXWindow();

  if ( xWindow != gkg::X11WindowImplementation::Unbound )
  {

    static Atom atom = None;

    gkg::Display*
      display = x11WindowImpl->getCanvas()->getImplementation()->getDisplay();

    gkg::X11DisplayImplementation*
      x11DisplayImpl = gkg::X11DisplayImplementation::getImplementation(
                                                                   display );
    XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
    XWindow xRoot = x11DisplayImpl->getXRoot();

    if ( atom == None )
    {

      atom = XInternAtom( xDisplay, "WM_CHANGE_STATE", False );

    }

    XEvent xEvent;
    xEvent.type = ClientMessage;
    xEvent.xclient.type = ClientMessage;
    xEvent.xclient.display = xDisplay;
    xEvent.xclient.window = xWindow;
    xEvent.xclient.message_type = atom;
    xEvent.xclient.format = 32;
    xEvent.xclient.data.l[ 0 ] = IconicState;
    XSendEvent( xDisplay, xRoot, False,
                SubstructureRedirectMask | SubstructureNotifyMask, &xEvent );

  }

}


void gkg::X11ManagedWindowImplementation::deiconify()
{

  gkg::X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation(_target);

  XWindow xWindow = x11WindowImpl->getXWindow();

  if ( xWindow != gkg::X11WindowImplementation::Unbound )
  {

    gkg::Display*
      display = x11WindowImpl->getCanvas()->getImplementation()->getDisplay();

    gkg::X11DisplayImplementation*
      x11DisplayImpl = gkg::X11DisplayImplementation::getImplementation(
                                                                      display );
    XDisplay* xDisplay = x11DisplayImpl->getXDisplay();

    XMapWindow( xDisplay, xWindow );

  }

}


void gkg::X11ManagedWindowImplementation::computeGeometry()
{

  gkg::CanvasImplementation* 
    canvasImpl = _target->getCanvas()->getImplementation();

  if ( canvasImpl->getPixelWidth() <= 0 )
  {

    canvasImpl->setWidth( 72 );

  }
  if ( canvasImpl->getPixelHeight() <= 0 )
  {

    canvasImpl->setHeight( 72 );

  }

}


void gkg::X11ManagedWindowImplementation::setProperties()
{

  setWmNormalHints();
  setWmName();
  setWmClass();
  setWmProtocols();
  setWmColormapWindows();
  setWmHints();

}


void gkg::X11ManagedWindowImplementation::doSetInfoIcon()
{

  doSet( &gkg::X11ManagedWindowImplementation::setInfoIcon );


}


void gkg::X11ManagedWindowImplementation::doSetInfoIconBitmap()
{

  doSet( &gkg::X11ManagedWindowImplementation::setInfoIconBitmap );

}


void gkg::X11ManagedWindowImplementation::doSetInfoIconMask()
{

  doSet( &gkg::X11ManagedWindowImplementation::setInfoIconMask );

}


void gkg::X11ManagedWindowImplementation::doSet(
             gkg::X11ManagedWindowImplementation::HintFunction hintFunction )
{

  gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo info;

  gkg::X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation(_target );

  info.xWindow = x11WindowImpl->getXWindow();

  if ( info.xWindow != gkg::X11WindowImplementation::Unbound )
  {

    info.style = x11WindowImpl->getStyle();
    info.xDisplay = x11WindowImpl->getXDisplay();
    info.hints = XGetWMHints( info.xDisplay, info.xWindow );
    if ( !info.hints )
    {

      info.hints = XAllocWMHints();

    }
    info.pixelWidth = x11WindowImpl->getCanvas()->getPixelWidth();
    info.pixelHeight = x11WindowImpl->getCanvas()->getPixelHeight();
    info.display = x11WindowImpl->getDisplay();
    if ( ( this->*hintFunction )( info ) )
    {

      XSetWMHints( info.xDisplay, info.xWindow, info.hints );

    }
    XFree( ( char* )info.hints );

  }

}


bool gkg::X11ManagedWindowImplementation::setInfoName(
              gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo& info )
{

  if ( info.style )
  {

    gkg::Style& style = *info.style;
    std::string s;
    if ( style.findAttribute( "name", s ) ||
         style.findAttribute( "title", s ) )
    {

      s += '\0';  // to be sure the last character is nul
      XStoreName( info.xDisplay, info.xWindow, s.c_str() );

    }

  }
  return false;

}


bool gkg::X11ManagedWindowImplementation::setInfoGeometry(
                   gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo& )
{

  // unimplemented: should configure mapped windows
  return false;

}


bool gkg::X11ManagedWindowImplementation::setInfoGroupLeader(
              gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo& info )
{

  if ( !_groupLeader )
  {

    info.hints->flags &= ~WindowGroupHint;
    info.hints->window_group = None;
    return true;

  }
  else
  {

    gkg::X11WindowImplementation* x11WindowImpl =
      gkg::X11WindowImplementation::getImplementation( _groupLeader );

    XWindow xWindow = x11WindowImpl->getXWindow();
    if ( xWindow != gkg::X11WindowImplementation::Unbound )
    {

      info.hints->flags |= WindowGroupHint;
      info.hints->window_group = xWindow;
      return true;

    }

  }
  return false;

}


bool gkg::X11ManagedWindowImplementation::setInfoTransientFor(
              gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo& info )
{

  if ( _transientFor )
  {

    gkg::X11WindowImplementation* x11WindowImpl =
      gkg::X11WindowImplementation::getImplementation( _transientFor );

    XDrawable td = x11WindowImpl->getXWindow();
    if ( td != gkg::X11WindowImplementation::Unbound )
    {

      XSetTransientForHint( info.xDisplay, info.xWindow, td );
      return true;

    }

  }
  return false;

}


bool gkg::X11ManagedWindowImplementation::setInfoIcon(
              gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo& info )
{

  if ( !_icon )
  {

    info.hints->flags &= ~IconWindowHint;
    info.hints->icon_window = None;
    return true;

  }
  else
  {

    gkg::X11WindowImplementation* x11WindowImpl =
      gkg::X11WindowImplementation::getImplementation( _icon );

    XWindow xWindow = x11WindowImpl->getXWindow();
    if ( xWindow != gkg::X11WindowImplementation::Unbound )
    {

      info.hints->flags |= IconWindowHint;
      info.hints->icon_window = xWindow;
      return true;

    }

  }
  return false;

}


bool gkg::X11ManagedWindowImplementation::setInfoIconName(
              gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo& info )
{

  if ( info.style )
  {

    gkg::Style& style = *info.style;
    std::string s;
    if ( style.findAttribute( "iconName", s ) ||
         style.findAttribute( "name", s ) )
    {

      s += '\0';  // to be sure the last character is nul
      XSetIconName( info.xDisplay, info.xWindow, s.c_str() );

    }

  }
  return false;

}


bool gkg::X11ManagedWindowImplementation::setInfoIconGeometry(
              gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo& info )
{

  info.hints->flags &= ~IconPositionHint;
  std::string s;
  if ( !info.style->findAttribute( "iconGeometry", s ) )
  {

    if ( !_icon )
    {

      return false;

    }
    gkg::Style* style = _icon->getStyle();
    if ( !style || !style->findAttribute( "geometry", s ) )
    {

      return false;

    }

  }
  s += '\0';  // to be sure the last character is nul

  int32_t x = 0, y = 0;
  uint32_t w = info.pixelWidth;
  uint32_t h = info.pixelHeight;
  if ( _iconBitmap )
  {

    w = _iconBitmap->getPixelWidth();
    h = _iconBitmap->getPixelHeight();

  }
  if ( _icon )
  {

    w = _icon->getCanvas()->getPixelWidth();
    h = _icon->getCanvas()->getPixelHeight();

  }
  uint32_t p = XParseGeometry( s.c_str(), &x, &y, &w, &h );

  gkg::Display& display = *info.display;

  if ( p & XNegative )
  {

    x = display.getPixelWidth() + x - w;

  }
  if ( p & YNegative )
  {

    y = display.getPixelHeight() + y - h;

  }
  if ( p & ( XValue | YValue ) )
  {

    info.hints->flags |= IconPositionHint;
    info.hints->icon_x = x;
    info.hints->icon_y = y;
    return true;

  }
  return false;

}



bool gkg::X11ManagedWindowImplementation::setInfoIconBitmap(
              gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo& info )
{

  if ( !_iconBitmap ) 
  {

    info.hints->flags &= ~IconPixmapHint;
    info.hints->icon_pixmap = None;

  }
  else
  {

    gkg::X11BitmapImplementation* x11BitmapImpl =
      gkg::X11BitmapImplementation::getImplementation( _iconBitmap );
    info.hints->flags |= IconPixmapHint;
    info.hints->icon_pixmap = x11BitmapImpl->getPixmap();

  }
  return true;

}


bool gkg::X11ManagedWindowImplementation::setInfoIconMask(
              gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo& info )
{

  if ( !_iconMask )
  {

    info.hints->flags &= ~IconMaskHint;
    info.hints->icon_mask = None;

  }
  else
  {

    gkg::X11BitmapImplementation* x11BitmapImpl =
      gkg::X11BitmapImplementation::getImplementation( _iconMask );
    info.hints->flags |= IconMaskHint;
    info.hints->icon_mask = x11BitmapImpl->getPixmap();

  }
  return true;

}


bool gkg::X11ManagedWindowImplementation::setInfoAll(
              gkg::X11ManagedWindowImplementation::ManagedWindowHintInfo& info )
{

  gkg::Style* style = info.style;
  XWMHints& hints = *info.hints;
  hints.flags = InputHint;
  hints.input = True;
  hints.flags |= StateHint;
  hints.initial_state = ( style && style->getValueIsOn( "iconic" ) ) ?
                        IconicState : NormalState;
  setInfoName( info );
  setInfoGeometry( info );
  setInfoGroupLeader( info );
  setInfoTransientFor( info );
  setInfoIconName( info );
  setInfoIconGeometry( info );
  setInfoIcon( info );
  setInfoIconBitmap( info );
  setInfoIconMask( info );
  return true;

}


void gkg::X11ManagedWindowImplementation::setWmNormalHints()
{

  gkg::X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation(_target);

  gkg::Display* display = x11WindowImpl->getDisplay();

  uint32_t cPixelWidth = x11WindowImpl->getCanvas()->getPixelWidth();
  uint32_t cPixelHeight = x11WindowImpl->getCanvas()->getPixelHeight();

  XSizeHints xSizeHints;
  if ( x11WindowImpl->isPlaced() )
  {

    xSizeHints.flags = USPosition | USSize;

  }
  else
  {

    xSizeHints.flags = PSize | PBaseSize;

  }

  // obsolete as of R4, but kept for backward compatibility
  xSizeHints.x = x11WindowImpl->getXPosition();
  xSizeHints.y = x11WindowImpl->getYPosition();
  xSizeHints.width = cPixelWidth;
  xSizeHints.height = cPixelHeight;

  xSizeHints.base_width = cPixelWidth;
  xSizeHints.base_height = cPixelHeight;

  const gkg::FloatCoordinate smallest = display->toFloatCoordinate( 2 );
  const gkg::FloatCoordinate xLargest = display->getWidth();
  const gkg::FloatCoordinate yLargest = display->getHeight();
  const gkg::GeometryRequirement& 
    rx = x11WindowImpl->getGeometryShape().getRequirement( gkg::DimensionX );
  const gkg::GeometryRequirement&
    ry = x11WindowImpl->getGeometryShape().getRequirement( gkg::DimensionY );

  gkg::FloatCoordinate minWidth = std::min( xLargest, 
                                          std::max( smallest,rx.getNatural() -
                                                             rx.getShrink() ) );
  gkg::FloatCoordinate minHeight = std::min( yLargest, 
                                          std::max( smallest,ry.getNatural() -
                                                             ry.getShrink() ) );

  xSizeHints.flags |= PMinSize;
  xSizeHints.min_width = display->toPixelCoordinate( minWidth );
  xSizeHints.min_height = display->toPixelCoordinate( minHeight );

  gkg::FloatCoordinate maxWidth = std::max( smallest, rx.getNatural() +
                                                    rx.getStretch() );
  gkg::FloatCoordinate maxHeight = std::max( smallest, ry.getNatural() +
                                                     ry.getStretch() );
  if ( ( maxWidth < xLargest ) || ( maxHeight < yLargest ) )
  {

    xSizeHints.flags |= PMaxSize;
    xSizeHints.max_width = display->toPixelCoordinate( std::min( maxWidth,
                                                                 xLargest ) );
    xSizeHints.max_height = display->toPixelCoordinate( std::min( maxHeight,
                                                                  yLargest ) );

  }

  XSetNormalHints( x11WindowImpl->getXDisplay(),
                   x11WindowImpl->getXWindow(),
                   &xSizeHints );

}


void gkg::X11ManagedWindowImplementation::setWmName()
{

  gkg::X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation(_target);

  gkg::Style* style = x11WindowImpl->getStyle();
  std::string s;
  if ( !style->findAttribute( "name", s ) &&
       !style->findAttribute( "title", s ) )
  {

    style->addAttribute( "name", gkg::Session::getInstance().getName() );

  }
  std::string hostName( gkg::Host::getName() );
  XChangeProperty( x11WindowImpl->getXDisplay(),
                   x11WindowImpl->getXWindow(),
                   XA_WM_CLIENT_MACHINE, XA_STRING, 8, PropModeReplace,
                  ( unsigned char* )hostName.c_str(), hostName.length() );

}


void gkg::X11ManagedWindowImplementation::setWmClass()
{

  gkg::X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation(_target);

  gkg::Style* style = x11WindowImpl->getStyle();
  XClassHint xClassHint;
  std::string s( "Noname" );
  if ( !style->findAttribute( "name", s ) )
  {

    style->findAttribute( "title", s );

  }
  s += '\0';
  xClassHint.res_name = ( char* )s.c_str();
  xClassHint.res_class = 
    ( char* )gkg::Session::getInstance().getClassName().c_str();
  XSetClassHint( x11WindowImpl->getXDisplay(),
                 x11WindowImpl->getXWindow(),
                 &xClassHint );

}


void gkg::X11ManagedWindowImplementation::setWmProtocols()
{

  gkg::X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation(_target);

  Atom atom = x11WindowImpl->getWmDeleteAtom();
  XSetWMProtocols( x11WindowImpl->getXDisplay(),
                   x11WindowImpl->getXWindow(),
                   &atom, 1 );

}


void gkg::X11ManagedWindowImplementation::setWmColormapWindows()
{

  // we do not currently manipulate colormaps

}


void gkg::X11ManagedWindowImplementation::setWmHints()
{

  doSet( &gkg::X11ManagedWindowImplementation::setInfoAll );

}

gkg::X11ManagedWindowImplementation* 
gkg::X11ManagedWindowImplementation::getImplementation( 
                                             gkg::ManagedWindow* managedWindow )
{

  return static_cast< X11ManagedWindowImplementation* >(
                                           managedWindow->getImplementation() );

}


