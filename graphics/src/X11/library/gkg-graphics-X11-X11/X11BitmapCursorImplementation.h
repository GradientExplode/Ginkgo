#ifndef _gkg_graphics_X11_X11_X11BitmapCursorImplementation_h_
#define _gkg_graphics_X11_X11_X11BitmapCursorImplementation_h_


#include <gkg-graphics-X11-core/BitmapCursorImplementation.h>


namespace gkg
{


class Visual;


class X11BitmapCursorImplementation : public BitmapCursorImplementation
{

  public:

    X11BitmapCursorImplementation( BitmapCursor* bitmapCursor,
                                   const Bitmap* pattern,
                                   const Bitmap* mask );
    ~X11BitmapCursorImplementation();

    void makeCursor( Display* display, Visual* visual );

    static X11BitmapCursorImplementation*
      getImplementation( BitmapCursor* bitmapCursor );

};


}


#endif
