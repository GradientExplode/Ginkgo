#ifndef _gkg_graphics_X11_core_IconWindow_h_
#define _gkg_graphics_X11_core_IconWindow_h_


#include <gkg-graphics-X11-core/ManagedWindow.h>


namespace gkg
{


class Glyph;
class IconWindowImplementation;


class IconWindow : public ManagedWindow
{

  public:

    IconWindow( Glyph* glyph );
    virtual ~IconWindow();

    virtual void doMap();

    IconWindowImplementation* getImplementation() const;

  private:

    IconWindowImplementation* _iconWindowImplementation;

};


}


#endif
