#ifndef _gkg_graphics_X11_X11_Xutil_h_
#define _gkg_graphics_X11_X11_Xutil_h_


#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-core-cppext/StdInt.h>


extern "C"
{


#include <gkg-graphics-X11-X11/Xdefs.h>
#include <X11/Xutil.h>
#include <gkg-graphics-X11-X11/Xundefs.h>


}


// these are macros that make use of untype-checking pointers to functions
#undef XDestroyImage
#undef XGetPixel
#undef XPutPixel
#undef XSubImage
#undef XAddPixel

inline 
int32_t XDestroyImage( XImage* xImage )
{

  typedef int32_t ( *PF )( XImage* );
  return ( *( ( PF )xImage->f.destroy_image ) )( xImage );
}


inline
uint32_t XGetPixel( XImage* xImage, int32_t x, int32_t y )
{

  typedef unsigned long ( *PF )( XImage*, int32_t, int32_t );
  return uint32_t( ( *( ( PF )xImage->f.get_pixel ) )( xImage, x, y ) );

}


inline
int32_t XPutPixel( XImage* xImage, int32_t x, int32_t y, uint32_t pixel )
{

  typedef int32_t ( *PF )( XImage*, int32_t, int32_t, unsigned long );
  return ( *( ( PF )xImage->f.put_pixel ) )( xImage, x, y, pixel );

}


inline 
XImage* XSubImage( XImage* xImage, int32_t x, int32_t y,
                   uint32_t width, uint32_t height )
{

  typedef XImage* ( *PF )( XImage*, int32_t, int32_t, uint32_t, uint32_t );
  return ( *( ( PF )xImage->f.sub_image ) )( xImage, x, y, width, height );

}


inline
int32_t XAddPixel( XImage* xImage, int32_t value )
{

  typedef int32_t ( *PF )( XImage*, long );
  return ( *( ( PF )xImage->f.add_pixel ) )( xImage, value );

}


#endif


