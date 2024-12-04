#include <gkg-graphics-X11-core/FontCursor.h>
#include <gkg-graphics-X11-core/FontCursorImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::FontCursor::FontCursor( const gkg::Font* font, 
                             int32_t pattern, int32_t mask,
                             const gkg::Color* foreground,
                             const gkg::Color* background )
                : gkg::Cursor( foreground, background )
{

  _fontCursorImplementation =
    gkg::getGUIImplementationFactory()->createFontCursorImplementation( this,
                                                          font, pattern, mask );

}


gkg::FontCursor::~FontCursor()
{

  delete _fontCursorImplementation;

}


void gkg::FontCursor::makeCursor( gkg::Display* display, gkg::Visual* visual )
{

  _fontCursorImplementation->makeCursor( display, visual );

}


gkg::FontCursorImplementation* gkg::FontCursor::getImplementation() const
{

  return _fontCursorImplementation;

}
