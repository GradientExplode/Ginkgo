#ifndef _gkg_graphics_X11_X11_X11TopLevelWindowImplementation_h_
#define _gkg_graphics_X11_X11_X11TopLevelWindowImplementation_h_


#include <gkg-graphics-X11-core/TopLevelWindowImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11TopLevelWindowImplementation :public TopLevelWindowImplementation

{

  public:

    X11TopLevelWindowImplementation( TopLevelWindow* topLevelWindow );
    ~X11TopLevelWindowImplementation();

    void setGroupLeader( Window* window );

    static X11TopLevelWindowImplementation* 
      getImplementation( TopLevelWindow* topLevelWindow );

};


}


#endif
