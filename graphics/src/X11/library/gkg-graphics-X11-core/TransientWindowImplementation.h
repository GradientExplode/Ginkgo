#ifndef _gkg_graphics_X11_core_TransientWindowImplementation_h_
#define _gkg_graphics_X11_core_TransientWindowImplementation_h_


namespace gkg
{


class TransientWindow;
class Window;


class TransientWindowImplementation
{

  public:

    virtual ~TransientWindowImplementation();

    TransientWindow* getTarget() const;

    virtual void setTransientFor( Window* window ) = 0;
    virtual Window* getTransientFor() const;

    virtual void configure();
    virtual void setAttributes();

  protected:

    TransientWindowImplementation( TransientWindow* transientWindow );

    TransientWindow* _target;

};


}


#endif
