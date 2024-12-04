#ifndef _gkg_graphics_X11_core_UnvisibleWindowImplementation_h_
#define _gkg_graphics_X11_core_UnvisibleWindowImplementation_h_


namespace gkg
{


class UnvisibleWindow;


class UnvisibleWindowImplementation
{

  public:

    virtual ~UnvisibleWindowImplementation();

    UnvisibleWindow* getTarget() const;

    virtual void setAttributes() = 0;

  protected:

    UnvisibleWindowImplementation( UnvisibleWindow* UnvisibleWindow );

    UnvisibleWindow* _target;

};


}


#endif
