#include <gkg-graphics-X11-core/BitmapCursor.h>
#include <gkg-graphics-X11-core/BitmapCursorImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::BitmapCursor::BitmapCursor( const gkg::Bitmap* pattern,
                                 const gkg::Bitmap* mask,
                                 const gkg::Color* foreground,
                                 const gkg::Color* background )
                  : gkg::Cursor( foreground, background )
{

  _bitmapCursorImplementation =
    gkg::getGUIImplementationFactory()->createBitmapCursorImplementation( this,
                                                                pattern, mask );

}


gkg::BitmapCursor::~BitmapCursor()
{

  delete _bitmapCursorImplementation;

}


void gkg::BitmapCursor::makeCursor( gkg::Display* display, gkg::Visual* visual )
{

  _bitmapCursorImplementation->makeCursor( display, visual );

}


gkg::BitmapCursorImplementation*
gkg::BitmapCursor::getImplementation() const
{

  return _bitmapCursorImplementation;

}
