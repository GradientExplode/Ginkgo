#ifndef _gkg_graphics_X11_core_PopupWindow_h_
#define _gkg_graphics_X11_core_PopupWindow_h_


#include <gkg-graphics-X11-core/Window.h>


namespace gkg
{


class Glyph;
class PopupWindowImplementation;


class PopupWindow : public Window
{

  public:

    PopupWindow( Glyph* glyph );
    virtual ~PopupWindow();

    virtual void setAttributes();

    PopupWindowImplementation* getImplementation() const;

  private:

    PopupWindowImplementation* _popupWindowImplementation;

};


}


#endif
