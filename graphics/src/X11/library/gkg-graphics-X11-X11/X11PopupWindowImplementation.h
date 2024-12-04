#ifndef _gkg_graphics_X11_X11_X11PopupWindowImplementation_h_
#define _gkg_graphics_X11_X11_X11PopupWindowImplementation_h_


#include <gkg-graphics-X11-core/PopupWindowImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11PopupWindowImplementation :public PopupWindowImplementation

{

  public:

    X11PopupWindowImplementation( PopupWindow* popupWindow );
    ~X11PopupWindowImplementation();

    void setAttributes();

    static X11PopupWindowImplementation* 
      getImplementation( PopupWindow* popupWindow );

};


}


#endif
