#ifndef _gkg_graphics_X11_core_ApplicationWindowImplementation_h_
#define _gkg_graphics_X11_core_ApplicationWindowImplementation_h_


namespace gkg
{


class ApplicationWindow;


class ApplicationWindowImplementation
{

  public:

    virtual ~ApplicationWindowImplementation();

    ApplicationWindow* getTarget() const;

    virtual void computeGeometry() = 0;
    virtual void setProperties() = 0;

  protected:

    ApplicationWindowImplementation( ApplicationWindow* applicationWindow );

    ApplicationWindow* _target;

};


}


#endif
