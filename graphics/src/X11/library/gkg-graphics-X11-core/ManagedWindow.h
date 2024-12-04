#ifndef _gkg_graphics_X11_core_ManagedWindow_h_
#define _gkg_graphics_X11_core_ManagedWindow_h_


#include <gkg-graphics-X11-core/Window.h>


namespace gkg
{


class Bitmap;
class Glyph;
class ManagedWindowImplementation;


class ManagedWindow : public Window
{

  public:

    virtual ~ManagedWindow();

    virtual void setIcon( ManagedWindow* managedWindow );
    virtual ManagedWindow* getIcon() const;

    virtual void setIconBitmap( Bitmap* bitmap );
    virtual Bitmap* getIconBitmap() const;

    virtual void setIconMask( Bitmap* bitmap );
    virtual Bitmap* getIconMask() const;

    virtual void iconify();
    virtual void deiconify();

    virtual void setFocusEvent( EventHandler* in, EventHandler* out );
    virtual void setWmDelete( EventHandler* eventHandler );

    virtual void resize();

    virtual void computeGeometry();
    virtual void setProperties();

    ManagedWindowImplementation* getImplementation() const;

  protected:

    ManagedWindow( Glyph* glyph );

  private:

    ManagedWindowImplementation* _managedWindowImplementation;

};


}


#endif
