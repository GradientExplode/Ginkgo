#ifndef _gkg_graphics_X11_X11_X11DataCursorImplementation_h_
#define _gkg_graphics_X11_X11_X11DataCursorImplementation_h_


#include <gkg-graphics-X11-core/DataCursorImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>


namespace gkg
{


class Visual;


class X11DataCursorImplementation : public DataCursorImplementation
{

  public:

    X11DataCursorImplementation( DataCursor* dataCursor,
                                 int16_t xHot,
                                 int16_t yHot,
                                 const int32_t* pattern,
                                 const int32_t* mask );
    ~X11DataCursorImplementation();

    void makeCursor( Display* display, Visual* visual );

    static X11DataCursorImplementation*
      getImplementation( DataCursor* dataCursor );

  private:

    Pixmap makeCursorPixmap( XDisplay* xDisplay,
                             XWindow root,
                             const int32_t* scanline );

};


}


#endif
