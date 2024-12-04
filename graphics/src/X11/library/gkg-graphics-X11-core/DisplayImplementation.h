#ifndef _gkg_graphics_X11_core_DisplayImplementation_h_
#define _gkg_graphics_X11_core_DisplayImplementation_h_


#include <string>
#include <list>
#include <gkg-graphics-X11-base/Coordinates.h>



namespace gkg
{


class Visual;
class Style;
class Window;
class SelectionManager;
class Display;
class Event;
class EventHandler;
class Window;
struct GrabInfo;


class DisplayImplementation
{

  public:

    virtual ~DisplayImplementation();

    Display* getTarget() const;

    const std::list< Visual* >& getVisuals() const;
    void addVisual( Visual* visual );
    Visual* getDefaultVisual() const;

    virtual void close() = 0;

    virtual int32_t getFd() const = 0;

    virtual FloatCoordinate getWidth() const;
    virtual FloatCoordinate getHeight() const;
    virtual PixelCoordinate getPixelWidth() const;
    virtual PixelCoordinate getPixelHeight() const;

    virtual FloatCoordinate getAWidth() const = 0;
    virtual FloatCoordinate getAHeight() const = 0;

    PixelCoordinate toPixelCoordinate( FloatCoordinate floatCoordinate ) const;
    FloatCoordinate toFloatCoordinate( PixelCoordinate pixelCoordinate ) const;

    virtual bool getDefaults( std::string& defaults ) const = 0;
    virtual void setStyle( Style* style ) = 0;
    virtual Style* getStyle() const;

    virtual void setScreen( int32_t screen ) = 0;

    virtual void repair();
    virtual void flush() = 0;
    virtual void sync() = 0;

    virtual bool getEvent( Event& event ) = 0;
    virtual void putEvent( const Event& event ) = 0;
    virtual bool isClosed() = 0;

    virtual void grab( Window* window, EventHandler* eventHandler );
    virtual void ungrab( EventHandler* eventHandler, bool all );
    virtual EventHandler* getGrabber() const;
    virtual bool isGrabbing( EventHandler* eventHandler ) const;

    virtual void ringBell( int32_t volume ) = 0;
    virtual void setKeyClick( int32_t percent ) = 0;
    virtual void setAutoRepeat( bool value ) = 0;
    virtual void setPointerFeedback( int32_t thresh, int32_t scale ) = 0;
    virtual void movePointer( FloatCoordinate x, FloatCoordinate y ) = 0;

    virtual SelectionManager* getPrimarySelection();
    virtual SelectionManager* getSecondarySelection();
    virtual SelectionManager* getClipboardSelection();
    virtual SelectionManager* findSelection( const std::string& name );


    virtual void addWindow( Window* window );
    virtual void removeWindow( Window* window );

    void setDpi();
    void needsRepair( Window* window );

    void removeFromLists( Window* window );

  protected:

    DisplayImplementation( Display* display, const std::string& device );

    virtual void referenceStyle( Style* style );

    Display* _target;

    std::list< Visual* > _visuals;
    Visual* _defaultVisual;

    FloatCoordinate _pixel;
    FloatCoordinate _point;

    FloatCoordinate _width;
    FloatCoordinate _height;
    uint32_t _pixelWidth;
    uint32_t _pixelHeight;
    Style* _style;

    std::list< GrabInfo > _grabbers;
    std::list< Window* > _damagedWindows;
    std::list< SelectionManager* > _selections;

};


}


#endif
