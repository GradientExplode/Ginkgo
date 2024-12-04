#ifndef _gkg_graphics_X11_X11_X11Drag_h_
#define _gkg_graphics_X11_X11_X11Drag_h_


#include <gkg-graphics-X11-X11/Xlib.h>


namespace gkg
{


struct X11Drag
{

  // these are used by xevent to detect and interpret a drag client message.
  static bool isDrag( const XEvent& xEvent );
  static void locate( const XEvent& xEvent, int32_t& x, int32_t& y );

};


}


#endif
