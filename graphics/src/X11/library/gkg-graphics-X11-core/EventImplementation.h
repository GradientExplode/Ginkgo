#ifndef _gkg_graphics_X11_core_EventImplementation_h_
#define _gkg_graphics_X11_core_EventImplementation_h_


#include <gkg-graphics-X11-core/Event.h>


namespace gkg
{


class Display;
class Window;
class EventHandler;

class EventImplementation
{

  public:

    virtual ~EventImplementation();

    virtual void setDisplay( Display* display );
    virtual Display* getDisplay() const;

    virtual void setWindow( Window* window );
    virtual Window* getWindow() const;

    virtual bool pending() const;
    virtual void unread();
    virtual void poll() = 0;

    virtual EventHandler* getEventHandler() const;
    virtual void handle();
    virtual void grab( EventHandler* eventHandler ) const;
    virtual void ungrab( EventHandler* eventHandler ) const;
    virtual EventHandler* getGrabber() const;
    virtual bool isGrabbing( EventHandler* eventHandler ) const;

    virtual Event::Type getType() const = 0;
    virtual uint32_t getTime() const = 0;
    virtual FloatCoordinate getPointerX();
    virtual FloatCoordinate getPointerY();
    virtual FloatCoordinate getPointerRootX();
    virtual FloatCoordinate getPointerRootY();
    virtual Event::Button getPointerButton() const = 0;
    virtual uint32_t getKeyMask() const = 0;
    virtual bool controlIsDown() const = 0;
    virtual bool metaIsDown() const = 0;
    virtual bool shiftIsDown() const = 0;
    virtual bool capslockIsDown() const = 0;
    virtual bool leftIsDown() const = 0;
    virtual bool middleIsDown() const = 0;
    virtual bool rightIsDown() const = 0;
    virtual unsigned char getKeyCode() const = 0;
    virtual uint32_t getKeySym() const = 0;
    virtual uint32_t getMapKey( char* buffer,
                                uint32_t length ) const = 0;

    virtual void clear();
    virtual void locate() = 0;
    virtual bool hasPointerLocation();
    virtual void acknowledgeMotion() = 0;

  protected:

    EventImplementation( Event* event );
    EventImplementation( Event* event, EventImplementation* other );

    Event* _target;

    Display* _display;
    Window* _window;
    FloatCoordinate _pointerX;
    FloatCoordinate _pointerY;
    FloatCoordinate _pointerRootX;
    FloatCoordinate _pointerRootY;
    bool _locationValid;
    bool _hasPointerLocation;

};


}


#endif
