#include <gkg-graphics-X11-X11/X11DataCursorImplementation.h>
#include <gkg-graphics-X11-X11/X11CursorImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11ColorImplementation.h>
#include <gkg-graphics-X11-core/DataCursor.h>


gkg::X11DataCursorImplementation::X11DataCursorImplementation(
                                                    gkg::DataCursor* dataCursor,
                                                    int16_t xHot,
                                                    int16_t yHot,
                                                    const int32_t* pattern,
                                                    const int32_t* mask )
                                 : gkg::DataCursorImplementation( dataCursor,
                                                                  xHot,
                                                                  yHot,
                                                                  pattern,
                                                                  mask )
{
}


gkg::X11DataCursorImplementation::~X11DataCursorImplementation()
{
}


void gkg::X11DataCursorImplementation::makeCursor( gkg::Display* display,
                                                   gkg::Visual* visual )
{

  gkg::X11DisplayImplementation* x11DisplayImpl =
       gkg::X11DisplayImplementation::getImplementation( display );
  XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
  XWindow xRoot = x11DisplayImpl->getXRoot();

  Pixmap pattern = makeCursorPixmap( xDisplay, xRoot, _pattern );
  Pixmap mask = makeCursorPixmap( xDisplay, xRoot, _mask );


  gkg::X11CursorImplementation* x11CursorImpl =
    gkg::X11CursorImplementation::getImplementation( _target );

  gkg::X11ColorImplementation* x11FgColorImpl = 
    gkg::X11ColorImplementation::getImplementation(
                ( gkg::Color* )x11CursorImpl->getForeground(), visual );
  gkg::X11ColorImplementation* x11BkColorImpl = 
    gkg::X11ColorImplementation::getImplementation(
                ( gkg::Color* )x11CursorImpl->getBackground(), visual );

  x11CursorImpl->setXCursor( 
                   XCreatePixmapCursor( xDisplay, pattern, mask,
                                        ( XColor* )&x11FgColorImpl->getXColor(),
                                        ( XColor* )&x11BkColorImpl->getXColor(),
                                        _x, gkg::cursorHeight - 1 - _y ) );

  XFreePixmap( xDisplay, pattern );
  XFreePixmap( xDisplay, mask );

}


// create the pixmap for a cursor; these are always 16x16, unlike fill patterns,
// which are 32x32.
Pixmap gkg::X11DataCursorImplementation::makeCursorPixmap( 
                                                       XDisplay* xDisplay,
                                                       XWindow xRoot,
                                                       const int32_t* scanline )
{

  Pixmap dst = XCreatePixmap( xDisplay, xRoot,
                              gkg::cursorWidth,
                              gkg::cursorHeight,
                              1 );
  GC gc = XCreateGC( xDisplay, dst, 0, 0 );
  XSetForeground( xDisplay, gc, 0 );
  XSetFillStyle( xDisplay, gc, FillSolid );
  XFillRectangle( xDisplay, dst, gc, 0, 0,
                  gkg::cursorWidth, gkg::cursorHeight );
  XSetForeground( xDisplay, gc, 1 );

  int32_t i, j;
  uint32_t s1, s2;
  for ( i = 0; i < gkg::cursorHeight; i++ )
  {

    s1 = scanline[ i ];
    s2 = 1;
    for ( j = 0; j < gkg::cursorWidth; j++ )
    {

      if ( ( s1 & s2 ) != 0 )
        XDrawPoint( xDisplay, dst, gc, gkg::cursorWidth - 1 - j, i );
      s2 <<= 1;

    }

  }
  XFreeGC( xDisplay, gc );
  return dst;

}


gkg::X11DataCursorImplementation* 
gkg::X11DataCursorImplementation::getImplementation( 
                                                   gkg::DataCursor* dataCursor )
{

  return static_cast< X11DataCursorImplementation* >( 
                                              dataCursor->getImplementation() );

}
