#ifndef _gkg_graphics_X11_core_UnvisibleWindow_h_
#define _gkg_graphics_X11_core_UnvisibleWindow_h_


#include <gkg-graphics-X11-core/Window.h>


namespace gkg
{


class Display;
class UnvisibleWindowImplementation;


class UnvisibleWindow : public Window
{

  public:

    UnvisibleWindow( Display* display );
    virtual ~UnvisibleWindow();

    virtual void setAttributes();

    UnvisibleWindowImplementation* getImplementation() const;

  private:

    UnvisibleWindowImplementation* _unvisibleWindowImplementation;

};


}


#endif
