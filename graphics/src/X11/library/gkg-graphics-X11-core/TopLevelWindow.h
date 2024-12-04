#ifndef _gkg_graphics_X11_core_TopLevelWindow_h_
#define _gkg_graphics_X11_core_TopLevelWindow_h_


#include <gkg-graphics-X11-core/ManagedWindow.h>


namespace gkg
{


class Glyph;
class TopLevelWindowImplementation;


class TopLevelWindow : public ManagedWindow
{

  public:

    TopLevelWindow( Glyph* glyph );
    virtual ~TopLevelWindow();

    virtual void setGroupLeader( Window* window );
    virtual Window* getGroupLeader() const;

    virtual void setProperties();

    TopLevelWindowImplementation* getImplementation() const;

  private:

    TopLevelWindowImplementation* _topLevelWindowImplementation;

};


}


#endif
