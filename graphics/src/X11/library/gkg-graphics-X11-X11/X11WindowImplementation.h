#ifndef _gkg_graphics_X11_X11_X11WindowImplementation_h_
#define _gkg_graphics_X11_X11_X11WindowImplementation_h_


#include <gkg-graphics-X11-core/WindowImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>
#include <map>


namespace gkg
{


class X11WindowImplementation : public WindowImplementation
{

  public:

    enum
    {

      Unbound = 0

    };

    X11WindowImplementation( Window* window, Glyph* glyph );
    ~X11WindowImplementation();

    void setCursor( Cursor* cursor );

    void bind();
    void unbind();
    bool isBound() const;

    void raise();
    void lower();
    void move( FloatCoordinate left, FloatCoordinate bottom );
    void resize();

    void receive( const Event& event );
    EventHandler* getTarget( const Event& event ) const;
    void getGrabPointer( Cursor* cursor = 0 ) const;
    void ungrabPointer() const;

    void clearMappingInfo();

    void checkPosition();

    XDisplay* getXDisplay() const;
    Atom getWmDeleteAtom() const;
    Atom getWmProtocolsAtom() const;
    void mapNotify( const XMapEvent& );
    void unmapNotify( const XUnmapEvent& );
    void expose( const XExposeEvent& exposeEvent );
    void configureNotify( const XConfigureEvent& configureEvent );

    XWindow getXWindow() const;
    XWindow getXTopLevelWindow() const;

    static Window* find( XWindow xWindow,
                         const std::map< XWindow, Window* >& windows );
    static X11WindowImplementation* getImplementation( Window* window );

    virtual void setAttributes();
    virtual void doMap();
    virtual void doUnmap();

    uint32_t getXAttributeMask() const;
    void setXAttributeMask( uint32_t mask );

    XSetWindowAttributes& getXAttributes();
    const XSetWindowAttributes& getXAttributes() const;

    // forceXWindow and forceXTopLevelWindow should only be used for the
    // creation of unvisible window(s)
    void forceXWindow( XWindow xWindow ); 
    void forceXTopLevelWindow( XWindow xTopLevelWindow ); 

  private:

    static Atom _wmDeleteAtom;
    static Atom _wmProtocolsAtom;

    XWindow _xWindow;
    XSetWindowAttributes _xAttributes;
    uint32_t _xAttributeMask;
    uint32_t _xClass;

    XWindow _xTopLevelWindow;

};


}


#endif
