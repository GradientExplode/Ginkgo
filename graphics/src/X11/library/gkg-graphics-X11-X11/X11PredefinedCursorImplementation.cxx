#include <gkg-graphics-X11-X11/X11PredefinedCursorImplementation.h>
#include <gkg-graphics-X11-X11/X11CursorImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11ColorImplementation.h>
#include <gkg-graphics-X11-core/PredefinedCursor.h>


gkg::X11PredefinedCursorImplementation::X11PredefinedCursorImplementation(
                                        gkg::PredefinedCursor* predefinedCursor,
                                        int32_t code )
                                       : gkg::PredefinedCursorImplementation(
                                                               predefinedCursor,
                                                               code )
{
}


gkg::X11PredefinedCursorImplementation::~X11PredefinedCursorImplementation()
{
}


void 
gkg::X11PredefinedCursorImplementation::makeCursor( gkg::Display* display,
                                                    gkg::Visual* visual )
{

  gkg::X11CursorImplementation* x11CursorImpl =
    gkg::X11CursorImplementation::getImplementation( _target );

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                       display )->getXDisplay();

  gkg::X11ColorImplementation* x11FgColorImpl = 
    gkg::X11ColorImplementation::getImplementation(
                ( gkg::Color* )x11CursorImpl->getForeground(), visual );
  gkg::X11ColorImplementation* x11BkColorImpl = 
    gkg::X11ColorImplementation::getImplementation(
                        ( gkg::Color* )x11CursorImpl->getBackground(), visual );
  // hack to be suppressed

  XCursor xCursor = XCreateFontCursor( xDisplay, _code );
  x11CursorImpl->setXCursor( xCursor );
  XRecolorCursor( xDisplay, xCursor,
                  ( XColor* )&x11FgColorImpl->getXColor(),
                  ( XColor* )&x11BkColorImpl->getXColor() );

}


gkg::X11PredefinedCursorImplementation* 
gkg::X11PredefinedCursorImplementation::getImplementation(
                                       gkg::PredefinedCursor* predefinedCursor )
{

  return static_cast< X11PredefinedCursorImplementation* >( 
                                        predefinedCursor->getImplementation() );

}
