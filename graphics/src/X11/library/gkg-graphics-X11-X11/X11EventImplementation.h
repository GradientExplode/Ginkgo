#ifndef _gkg_graphics_X11_X11_X11EventImplementation_h_
#define _gkg_graphics_X11_X11_X11EventImplementation_h_


#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>
#include <gkg-graphics-X11-core/EventImplementation.h>


namespace gkg
{


class X11EventImplementation : public EventImplementation
{

  public:

    X11EventImplementation( Event* event );
    X11EventImplementation( Event* event, X11EventImplementation* other  );
    ~X11EventImplementation();

    void poll();
    Event::Type getType() const;
    uint32_t getTime() const;
    Event::Button getPointerButton() const;
    uint32_t getKeyMask() const;
    bool controlIsDown() const;
    bool metaIsDown() const;
    bool shiftIsDown() const;
    bool capslockIsDown() const;
    bool leftIsDown() const;
    bool middleIsDown() const;
    bool rightIsDown() const;
    unsigned char getKeyCode() const;
    uint32_t getKeySym() const;
    uint32_t getMapKey( char* buffer, uint32_t length ) const;

    const XEvent& getXEvent() const;
    void setNextXEvent();
    void putBackXEvent();

    void locate();
    void acknowledgeMotion();

    static X11EventImplementation* getImplementation( Event* event );

  private:

    XEvent _xEvent;

};


}


#endif
