#ifndef _gkg_graphics_X11_X11_X11CursorImplementation_h_
#define _gkg_graphics_X11_X11_X11CursorImplementation_h_


#include <gkg-graphics-X11-core/CursorImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class Visual;


class X11CursorImplementation : public CursorImplementation
{

  public:

    X11CursorImplementation( Cursor* cursor,
                             const Color* foreground,
                             const Color* background );
    ~X11CursorImplementation();

    void setXCursor( XCursor xCursor );
    XCursor getXId( Display* display, Visual* visual );

    static X11CursorImplementation*
      getImplementation( Cursor* cursor );

  protected:

    XCursor _xCursor;    

};


}


#endif
