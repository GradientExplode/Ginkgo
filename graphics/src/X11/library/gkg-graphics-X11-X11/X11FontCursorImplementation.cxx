#include <gkg-graphics-X11-X11/X11FontCursorImplementation.h>
#include <gkg-graphics-X11-X11/X11CursorImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11ColorImplementation.h>
#include <gkg-graphics-X11-X11/X11FontImplementation.h>
#include <gkg-graphics-X11-core/FontCursor.h>


gkg::X11FontCursorImplementation::X11FontCursorImplementation(
                                                    gkg::FontCursor* fontCursor,
                                                    const gkg::Font* font,
                                                    int32_t pattern,
                                                    int32_t mask )
                                 : gkg::FontCursorImplementation( fontCursor,
                                                                  font,
                                                                  pattern,
                                                                  mask )
{
}


gkg::X11FontCursorImplementation::~X11FontCursorImplementation()
{
}


void 
gkg::X11FontCursorImplementation::makeCursor( gkg::Display* display,
                                              gkg::Visual* visual )
{

  gkg::X11CursorImplementation* x11CursorImpl =
    gkg::X11CursorImplementation::getImplementation( _target );

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                       display )->getXDisplay();

  X11FontImplementation* x11FontImpl =
    gkg::X11FontImplementation::getImplementation( ( gkg::Font* )_font,
                                                   display );

  XFontStruct* xFontStruct = x11FontImpl->getXFontStruct();

  gkg::X11ColorImplementation* x11FgColorImpl = 
    gkg::X11ColorImplementation::getImplementation(
                ( gkg::Color* )x11CursorImpl->getForeground(), visual );
  gkg::X11ColorImplementation* x11BkColorImpl = 
    gkg::X11ColorImplementation::getImplementation(
                ( gkg::Color* )x11CursorImpl->getBackground(), visual );
  // hack to be suppressed

  x11CursorImpl->setXCursor( 
          XCreateGlyphCursor( xDisplay,
                              xFontStruct->fid, xFontStruct->fid,
                              _pattern, _mask,
                              ( XColor* )&x11FgColorImpl->getXColor(),
                              ( XColor* )&x11BkColorImpl->getXColor() ) );

}


gkg::X11FontCursorImplementation* 
gkg::X11FontCursorImplementation::getImplementation( 
                                                   gkg::FontCursor* fontCursor )
{

  return static_cast< X11FontCursorImplementation* >( 
                                              fontCursor->getImplementation() );

}
