#ifndef _gkg_graphics_X11_X11_X11FontCursorImplementation_h_
#define _gkg_graphics_X11_X11_X11FontCursorImplementation_h_


#include <gkg-graphics-X11-core/FontCursorImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class Visual;


class X11FontCursorImplementation : public FontCursorImplementation
{

  public:

    X11FontCursorImplementation( FontCursor* fontCursor,
                                 const Font* font,
                                 int32_t pattern,
                                 int32_t mask );
    ~X11FontCursorImplementation();

    void makeCursor( Display* display, Visual* visual );

    static X11FontCursorImplementation* 
      getImplementation( FontCursor* fontCursor );

};


}


#endif
