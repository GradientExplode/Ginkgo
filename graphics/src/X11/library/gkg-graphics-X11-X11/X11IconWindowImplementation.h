#ifndef _gkg_graphics_X11_X11_X11IconWindowImplementation_h_
#define _gkg_graphics_X11_X11_X11IconWindowImplementation_h_


#include <gkg-graphics-X11-core/IconWindowImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11IconWindowImplementation :public IconWindowImplementation

{

  public:

    X11IconWindowImplementation( IconWindow* iconWindow );
    ~X11IconWindowImplementation();

    void doMap();

    static X11IconWindowImplementation* 
      getImplementation( IconWindow* IconWindow );

};


}


#endif
