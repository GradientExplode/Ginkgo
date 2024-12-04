#include <gkg-graphics-X11-X11/X11BitmapCursorImplementation.h>
#include <gkg-graphics-X11-X11/X11BitmapImplementation.h>
#include <gkg-graphics-X11-X11/X11CursorImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11ColorImplementation.h>
#include <gkg-graphics-X11-core/BitmapCursor.h>
#include <gkg-graphics-X11-core/Display.h>


gkg::X11BitmapCursorImplementation::X11BitmapCursorImplementation(
                                                gkg::BitmapCursor* bitmapCursor,
                                                const gkg::Bitmap* pattern,
                                                const gkg::Bitmap* mask )
                                   : gkg::BitmapCursorImplementation(
                                                                   bitmapCursor,
                                                                   pattern,
                                                                   mask )
{
}


gkg::X11BitmapCursorImplementation::~X11BitmapCursorImplementation()
{
}


void 
gkg::X11BitmapCursorImplementation::makeCursor( gkg::Display* display,
                                                gkg::Visual* visual )
{

  gkg::X11CursorImplementation* x11CursorImpl =
    gkg::X11CursorImplementation::getImplementation( _target );

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                       display )->getXDisplay();

  gkg::X11BitmapImplementation* x11PatternBitmapImpl =
    gkg::X11BitmapImplementation::getImplementation(
                                                  ( gkg::Bitmap* )_pattern );
  gkg::X11BitmapImplementation* x11MaskBitmapImpl =
    gkg::X11BitmapImplementation::getImplementation(
                                                     ( gkg::Bitmap* )_mask );

  gkg::X11ColorImplementation* x11FgColorImpl = 
    gkg::X11ColorImplementation::getImplementation(
                        ( gkg::Color* )x11CursorImpl->getForeground(), visual );
  gkg::X11ColorImplementation* x11BkColorImpl = 
    gkg::X11ColorImplementation::getImplementation(
                        ( gkg::Color* )x11CursorImpl->getBackground(), visual );
  // hack to be suppressed

  x11CursorImpl->setXCursor( 
    XCreatePixmapCursor(
             xDisplay,
             x11PatternBitmapImpl->getPixmap(),
             x11MaskBitmapImpl->getPixmap(),
             ( XColor* )&x11FgColorImpl->getXColor(),
             ( XColor* )&x11BkColorImpl->getXColor(),
             display->toPixelCoordinate( -x11PatternBitmapImpl->getLeft() ),
             display->toPixelCoordinate( x11PatternBitmapImpl->getHeight() - 1 +
                                        x11PatternBitmapImpl->getBottom() ) ) );

}


gkg::X11BitmapCursorImplementation* 
gkg::X11BitmapCursorImplementation::getImplementation(
                                               gkg::BitmapCursor* bitmapCursor )
{

  return static_cast< X11BitmapCursorImplementation* >( 
                                            bitmapCursor->getImplementation() );

}
