#ifndef _gkg_graphics_X11_X11_X11TransientWindowImplementation_h_
#define _gkg_graphics_X11_X11_X11TransientWindowImplementation_h_


#include <gkg-graphics-X11-core/TransientWindowImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11TransientWindowImplementation :public TransientWindowImplementation

{

  public:

    X11TransientWindowImplementation( TransientWindow* TransientWindow );
    ~X11TransientWindowImplementation();

    void setTransientFor( Window* window );

    static X11TransientWindowImplementation* 
      getImplementation( TransientWindow* transientWindow );

};


}


#endif
