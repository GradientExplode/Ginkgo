#include <gkg-graphics-X11-core/DataCursor.h>
#include <gkg-graphics-X11-core/DataCursorImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::DataCursor::DataCursor( int16_t xOffset, int16_t yOffset,
                             const int32_t* pattern, const int32_t* mask,
                             const gkg::Color* foreground,
                             const gkg::Color* background )
                : gkg::Cursor( foreground, background )
{

  _dataCursorImplementation =
    gkg::getGUIImplementationFactory()->createDataCursorImplementation( this,
                                              xOffset, yOffset, pattern, mask );

}


gkg::DataCursor::~DataCursor()
{

  delete _dataCursorImplementation;

}


void gkg::DataCursor::makeCursor( gkg::Display* display, gkg::Visual* visual )
{

  _dataCursorImplementation->makeCursor( display, visual );

}


gkg::DataCursorImplementation* gkg::DataCursor::getImplementation() const
{

  return _dataCursorImplementation;

}
