#ifndef _gkg_graphics_X11_core_ManagedWindowImplementation_h_
#define _gkg_graphics_X11_core_ManagedWindowImplementation_h_


namespace gkg
{


class ManagedWindow;
class Bitmap;
class EventHandler;
class Window;


class ManagedWindowImplementation
{

  public:

    virtual ~ManagedWindowImplementation();

    ManagedWindow* getTarget() const;

    virtual void setIcon( ManagedWindow* managedWindow );
    virtual ManagedWindow* getIcon() const;

    virtual void setIconBitmap( Bitmap* bitmap );
    virtual Bitmap* getIconBitmap() const;

    virtual void setIconMask( Bitmap* bitmap );
    virtual Bitmap* getIconMask() const;

    virtual void iconify() = 0;
    virtual void deiconify() = 0;

    virtual void setFocusEvent( EventHandler* in, EventHandler* out );
    virtual void setWmDelete( EventHandler* eventHandler );
    virtual void setWmNormalHints() = 0;

    virtual void resize();

    virtual void computeGeometry() = 0;
    virtual void setProperties() = 0;

    virtual void doSetInfoIcon() = 0;
    virtual void doSetInfoIconBitmap() = 0;
    virtual void doSetInfoIconMask() = 0;

    virtual void setGroupLeader( Window* window );
    virtual Window* getGroupLeader() const;

    virtual void setTransientFor( Window* window );
    virtual Window* getTransientFor() const;

  protected:

    ManagedWindowImplementation( ManagedWindow* managedWindow );

    ManagedWindow* _target;

    ManagedWindow* _icon;
    Bitmap* _iconBitmap;
    Bitmap* _iconMask;
    Window* _groupLeader;
    Window* _transientFor;

};


}


#endif
