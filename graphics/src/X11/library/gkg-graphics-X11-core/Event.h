#ifndef _gkg_graphics_X11_core_Event_h_
#define _gkg_graphics_X11_core_Event_h_


#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Display;
class Window;
class EventHandler;
class EventImplementation;


class Event
{

  public:


    enum Type
    {

      Undefined,
      Motion,
      Down,
      Up,
      Key,
      OtherEvent

    };

    enum Button
    {

      ButtonNone,
      Any,
      Left,
      Middle,
      Right,
      WheelDown,
      WheelUp,
      OtherButton

    };

    Event();
    Event( const Event& other );
    virtual ~Event();

    virtual void setDisplay( Display* display );
    virtual Display* getDisplay() const;

    virtual void setWindow( Window* window );
    virtual Window* getWindow() const;

    virtual bool pending() const;
    virtual void read();
    virtual bool read( int32_t sec, int32_t usec );
    virtual void unread();
    virtual void poll();

    virtual EventHandler* getEventHandler() const;
    virtual void handle();
    virtual void grab( EventHandler* eventHandler ) const;
    virtual void ungrab( EventHandler* eventHandler ) const;
    virtual EventHandler* getGrabber() const;
    virtual bool isGrabbing( EventHandler* eventHandler ) const;

    virtual Type getType() const;
    virtual uint32_t getTime() const;
    virtual FloatCoordinate getPointerX() const;
    virtual FloatCoordinate getPointerY() const;
    virtual FloatCoordinate getPointerRootX() const;
    virtual FloatCoordinate getPointerRootY() const;
    virtual Button getPointerButton() const;
    virtual uint32_t getKeyMask() const;
    virtual bool controlIsDown() const;
    virtual bool metaIsDown() const;
    virtual bool shiftIsDown() const;
    virtual bool capslockIsDown() const;
    virtual bool leftIsDown() const;
    virtual bool middleIsDown() const;
    virtual bool rightIsDown() const;
    virtual unsigned char getKeyCode() const;
    virtual uint32_t getKeySym() const;
    virtual uint32_t getMapKey( char* buffer, uint32_t length ) const;

    EventImplementation* getImplementation() const;

  private:

    EventImplementation* _eventImplementation;

};


}


#endif
