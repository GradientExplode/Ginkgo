#ifndef _gkg_graphics_X11_core_PopupWindowImplementation_h_
#define _gkg_graphics_X11_core_PopupWindowImplementation_h_


namespace gkg
{


class PopupWindow;


class PopupWindowImplementation
{

  public:

    virtual ~PopupWindowImplementation();

    PopupWindow* getTarget() const;

    virtual void setAttributes() = 0;

  protected:

    PopupWindowImplementation( PopupWindow* popupWindow );

    PopupWindow* _target;

};


}


#endif
