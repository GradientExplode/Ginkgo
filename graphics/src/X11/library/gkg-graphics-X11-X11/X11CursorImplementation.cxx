#include <gkg-graphics-X11-X11/X11CursorImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Cursor.h>


gkg::X11CursorImplementation::X11CursorImplementation(
                                                  gkg::Cursor* cursor,
                                                  const gkg::Color* foreground,
                                                  const gkg::Color* background )
                             : gkg::CursorImplementation( cursor,
                                                          foreground,
                                                          background ),
                               _xCursor( 0 )
{
}


gkg::X11CursorImplementation::~X11CursorImplementation()
{

  if ( _xCursor )
  {

    XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
    XFreeCursor( xDisplay, _xCursor );

  }

}


void gkg::X11CursorImplementation::setXCursor( XCursor xCursor )
{

  _xCursor = xCursor;

}


XCursor gkg::X11CursorImplementation::getXId( gkg::Display* display,
                                              gkg::Visual* visual )
{

  if ( _display != display )
  {


    if ( _xCursor )
    {

      XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
      XFreeCursor( xDisplay, _xCursor );

    }
    gkg::Style* style = display->getStyle();
    if ( !_foreground )
    {

      _foreground = makeColor( display, style, "pointerColor",
                               "foreground", "Foreground", "#000000" );

    }
    if ( !_background )
    {

      _background = makeColor( display, style, "pointerColorBackground",
                               "background", "Background", "#ffffff" );

    }
    _target->makeCursor( display, visual );
    _display = display;

  }
  return _xCursor;

}


gkg::X11CursorImplementation* 
gkg::X11CursorImplementation::getImplementation( gkg::Cursor* cursor )
{

  return static_cast< X11CursorImplementation* >( cursor->getImplementation() );

}

