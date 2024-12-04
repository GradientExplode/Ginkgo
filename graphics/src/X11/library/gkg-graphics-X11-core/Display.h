#ifndef _gkg_graphics_X11_core_Display_h_
#define _gkg_graphics_X11_core_Display_h_


#include <string>
#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Style;
class DisplayImplementation;
class Event;
class Window;
class EventHandler;
class SelectionManager;


class Display
{

  public:

    Display( const std::string& device = "" );
    virtual ~Display();

    virtual void close();

    virtual int32_t getFd() const;

    virtual FloatCoordinate getWidth() const;
    virtual FloatCoordinate getHeight() const;
    virtual PixelCoordinate getPixelWidth() const;
    virtual PixelCoordinate getPixelHeight() const;

    virtual FloatCoordinate getAWidth() const;
    virtual FloatCoordinate getAHeight() const;

    PixelCoordinate toPixelCoordinate( FloatCoordinate floatCoordinate ) const;
    FloatCoordinate toFloatCoordinate( PixelCoordinate pixelCoordinate ) const;

    virtual bool getDefaults( std::string& defaults ) const;
    virtual void setStyle( Style* style );
    virtual Style* getStyle() const;

    virtual void setScreen( int32_t screen );

    virtual void repair();
    virtual void flush();
    virtual void sync();

    virtual bool getEvent( Event& event );
    virtual void putEvent( const Event& event );
    virtual bool isClosed();

    virtual void grab( Window* window, EventHandler* eventHandler );
    virtual void ungrab( EventHandler* eventHandler, bool all = false );
    virtual EventHandler* getGrabber() const;
    virtual bool isGrabbing( EventHandler* eventHandler ) const;

    virtual void ringBell( int32_t volume );
    virtual void setKeyClick( int32_t percent );
    virtual void setAutoRepeat( bool value );
    virtual void setPointerFeedback( int32_t thresh, int32_t scale );
    virtual void movePointer( FloatCoordinate x, FloatCoordinate y );

    virtual SelectionManager* getPrimarySelection();
    virtual SelectionManager* getSecondarySelection();
    virtual SelectionManager* getClipboardSelection();
    virtual SelectionManager* findSelection( const std::string& name );

    DisplayImplementation* getImplementation() const;


  private:

    DisplayImplementation* _displayImplementation;

};


}


#endif
