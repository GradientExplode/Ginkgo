#ifndef _gkg_graphics_X11_core_DataCursor_h_
#define _gkg_graphics_X11_core_DataCursor_h_


#include <gkg-graphics-X11-core/Cursor.h>


namespace gkg
{


class DataCursorImplementation;


class DataCursor : public Cursor
{

  public:

    DataCursor( int16_t xOffset, int16_t yOffset,
                const int32_t* pattern, const int32_t* mask,
                const Color* foreground = 0,
                const Color* background = 0 );
    virtual ~DataCursor();

    virtual void makeCursor( Display* display, Visual* visual );

    DataCursorImplementation* getImplementation() const;

  private:

    DataCursorImplementation* _dataCursorImplementation;

};


}


#endif
