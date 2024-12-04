#ifndef _gkg_graphics_X11_core_TransientWindow_h_
#define _gkg_graphics_X11_core_TransientWindow_h_


#include <gkg-graphics-X11-core/TopLevelWindow.h>


namespace gkg
{


class Glyph;
class TransientWindowImplementation;


class TransientWindow : public TopLevelWindow
{

  public:

    TransientWindow( Glyph* glyph );
    virtual ~TransientWindow();

    virtual void setTransientFor( Window* window );
    virtual Window* getTransientFor() const;

    virtual void configure();
    virtual void setAttributes();

    TransientWindowImplementation* getImplementation() const;

  private:

    TransientWindowImplementation* _transientWindowImplementation;

};


}


#endif
