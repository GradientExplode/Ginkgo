#ifndef _gkg_graphics_X11_X11_X11UnvisibleWindowImplementation_h_
#define _gkg_graphics_X11_X11_X11UnvisibleWindowImplementation_h_


#include <gkg-graphics-X11-core/UnvisibleWindowImplementation.h>


namespace gkg
{


class Display;


class X11UnvisibleWindowImplementation :public UnvisibleWindowImplementation

{

  public:

    X11UnvisibleWindowImplementation( UnvisibleWindow* UnvisibleWindow,
                                      Display* display );
    ~X11UnvisibleWindowImplementation();

    void setAttributes();

    static X11UnvisibleWindowImplementation* 
      getImplementation( UnvisibleWindow* UnvisibleWindow );

};


}


#endif
