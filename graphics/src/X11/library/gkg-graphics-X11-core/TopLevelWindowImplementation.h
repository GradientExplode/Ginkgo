#ifndef _gkg_graphics_X11_core_TopLevelWindowImplementation_h_
#define _gkg_graphics_X11_core_TopLevelWindowImplementation_h_


namespace gkg
{


class TopLevelWindow;
class Window;


class TopLevelWindowImplementation
{

  public:

    virtual ~TopLevelWindowImplementation();

    TopLevelWindow* getTarget() const;

    virtual void setGroupLeader( Window* window ) = 0;
    virtual Window* getGroupLeader() const;

    virtual void setProperties();

  protected:

    TopLevelWindowImplementation( TopLevelWindow* topLevelWindow );

    TopLevelWindow* _target;

};


}


#endif
