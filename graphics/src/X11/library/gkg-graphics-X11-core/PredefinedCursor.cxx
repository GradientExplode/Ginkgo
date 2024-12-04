#include <gkg-graphics-X11-core/PredefinedCursor.h>
#include <gkg-graphics-X11-core/PredefinedCursorImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::PredefinedCursor::PredefinedCursor( int32_t code,
                                         const gkg::Color* foreground,
                                         const gkg::Color* background )
                      : gkg::Cursor( foreground, background )
{

  _predefinedCursorImplementation =
    gkg::getGUIImplementationFactory()->createPredefinedCursorImplementation(
                                                                  this, code );

}


gkg::PredefinedCursor::~PredefinedCursor()
{

  delete _predefinedCursorImplementation;

}


void gkg::PredefinedCursor::makeCursor( gkg::Display* display,
                                        gkg::Visual* visual )
{

  _predefinedCursorImplementation->makeCursor( display, visual );

}


gkg::PredefinedCursorImplementation*
gkg::PredefinedCursor::getImplementation() const
{

  return _predefinedCursorImplementation;

}
