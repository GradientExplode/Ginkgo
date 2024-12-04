#ifndef _gkg_graphics_X11_X11_X11DragAtoms_h_
#define _gkg_graphics_X11_X11_X11DragAtoms_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <X11/Xatom.h>


namespace gkg
{


class Event;


class X11DragAtoms : public Singleton< X11DragAtoms >
{

  public:

    virtual ~X11DragAtoms();

    bool enter( const XEvent& xEvent );
    bool motion( const XEvent& xEvent );
    bool leave( const XEvent& xEvent );
    bool drop( const XEvent& xEvent );

    bool enter( const Event& event );
    bool motion( const Event& event );
    bool leave( const Event& event );
    bool drop( const Event& event );

    Atom enter( XDisplay* xDisplay );
    Atom motion( XDisplay* xDisplay );
    Atom leave( XDisplay* xDisplay );
    Atom drop( XDisplay* xDisplay );
    Atom drag( XDisplay* xDisplay );

    const char* getEnterName() const;
    const char* getMotionName() const;
    const char* getLeaveName() const;
    const char* getDropName() const;
    const char* getDragName() const;
    int32_t getDropUid() const;
    void incDropUid();

  protected:

    friend class Singleton< X11DragAtoms >;

    X11DragAtoms();

    void cache( XDisplay* xDisplay );

    int32_t _dropUid;
    XDisplay* _xDisplay;
    Atom _enter;
    Atom _motion;
    Atom _leave;
    Atom _drop;
    Atom _drag;

};


}


#endif
