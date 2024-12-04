#ifndef _gkg_graphics_X11_core_ApplicationWindow_h_
#define _gkg_graphics_X11_core_ApplicationWindow_h_


#include <gkg-graphics-X11-core/ManagedWindow.h>


namespace gkg
{


class Glyph;
class ApplicationWindowImplementation;


class ApplicationWindow : public ManagedWindow
{

  public:

    ApplicationWindow( Glyph* glyph );
    virtual ~ApplicationWindow();

    virtual void computeGeometry();
    virtual void setProperties();

    ApplicationWindowImplementation* getImplementation() const;

  private:

    ApplicationWindowImplementation* _applicationWindowImplementation;

};


}


#endif
