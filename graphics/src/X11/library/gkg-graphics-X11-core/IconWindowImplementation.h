#ifndef _gkg_graphics_X11_core_IconWindowImplementation_h_
#define _gkg_graphics_X11_core_IconWindowImplementation_h_


namespace gkg
{


class IconWindow;


class IconWindowImplementation
{

  public:

    virtual ~IconWindowImplementation();

    IconWindow* getTarget() const;

    virtual void doMap() = 0;

  protected:

    IconWindowImplementation( IconWindow* iconWindow );

    IconWindow* _target;

};


}


#endif
