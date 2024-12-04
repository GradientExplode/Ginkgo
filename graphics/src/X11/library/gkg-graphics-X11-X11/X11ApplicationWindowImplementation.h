#ifndef _gkg_graphics_X11_X11_X11ApplicationWindowImplementation_h_
#define _gkg_graphics_X11_X11_X11ApplicationWindowImplementation_h_


#include <gkg-graphics-X11-core/ApplicationWindowImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11ApplicationWindowImplementation :public ApplicationWindowImplementation

{

  public:

    X11ApplicationWindowImplementation( ApplicationWindow* applicationWindow );
    ~X11ApplicationWindowImplementation();

    void computeGeometry();
    void setProperties();

    static X11ApplicationWindowImplementation* 
      getImplementation( ApplicationWindow* ApplicationWindow );

};


}


#endif
