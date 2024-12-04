#ifndef _gkg_graphics_X11_X11_X11PredefinedCursorImplementation_h_
#define _gkg_graphics_X11_X11_X11PredefinedCursorImplementation_h_


#include <gkg-graphics-X11-core/PredefinedCursorImplementation.h>


namespace gkg
{


class Visual;


class X11PredefinedCursorImplementation : public PredefinedCursorImplementation
{

  public:

    X11PredefinedCursorImplementation( PredefinedCursor* predefinedCursor,
                                       int32_t code );
    ~X11PredefinedCursorImplementation();

    void makeCursor( Display* display, Visual* visual );

    static X11PredefinedCursorImplementation* 
      getImplementation( PredefinedCursor* predefinedCursor );

};


}


#endif
