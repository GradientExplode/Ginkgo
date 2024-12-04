#ifndef _gkg_graphics_X11_core_WindowImplementation_h_
#define _gkg_graphics_X11_core_WindowImplementation_h_


#include <list>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-core/GeometryRequisition.h>
#include <gkg-graphics-X11-core/GeometryAllocation.h>


namespace gkg
{


class Glyph;
class Window;
class Style;
class Display;
class Canvas;
class Cursor;
class EventHandler;
class Visual;
class Event;


class WindowImplementation
{

  public:

    virtual ~WindowImplementation();

    Window* getTarget() const;

    virtual Glyph* getGlyph() const;

    virtual void setStyle( Style* style );
    virtual Style* getStyle() const;

    virtual void setDisplay( Display* display );
    virtual Display* getDisplay() const;
    virtual void forceDisplay( Display* display ); // should only be used
                                                   // for unvisible window

    virtual Visual* getVisual() const;

    virtual Canvas* getCanvas() const;

    virtual void setCursor( Cursor* cursor ) = 0;
    virtual Cursor* getCursor() const;
    virtual void pushCursor();
    virtual void popCursor();

    virtual void setPlace( FloatCoordinate left, FloatCoordinate bottom );
    virtual void setPixelPlace( IntegerCoordinate left,
                                IntegerCoordinate bottom );
    virtual void align( float x, float y );
    virtual FloatCoordinate getLeft();
    virtual FloatCoordinate getBottom();

    virtual FloatCoordinate getWidth() const;
    virtual FloatCoordinate getHeight() const;

    virtual void map();
    virtual void unmap();
    virtual bool isMapped() const;

    virtual void bind() = 0;
    virtual void unbind() = 0;
    virtual bool isBound() const = 0;

    virtual void raise() = 0;
    virtual void lower() = 0;
    virtual void move( FloatCoordinate left, FloatCoordinate bottom ) = 0;
    virtual void resize() = 0;

    virtual void receive( const Event& event ) = 0;
    virtual EventHandler* getTarget( const Event& event ) const = 0;
    virtual void getGrabPointer( Cursor* cursor = 0 ) const = 0;
    virtual void ungrabPointer() const = 0;

    virtual void repair();

    virtual void clearMappingInfo() = 0;

    void moveInPixelCoordinate( int32_t x, int32_t y );
    void resizeInPixelCoordinate( PixelCoordinate width,
                                  PixelCoordinate height );
    virtual void checkPosition() = 0;

    void checkBinding();
    void initRenderer();

    virtual void setFocusEvent( EventHandler* in, EventHandler* out );
    virtual void setWmDelete( EventHandler* eventHandler );

    virtual void configure();
    virtual void setDefaultGeometry();
    virtual void computeGeometry();
    virtual void setAttributes() = 0;
    virtual void setProperties();
    virtual void doMap() = 0;
    virtual void doUnmap() = 0;

    virtual void setPlaced( bool value );
    virtual bool isPlaced() const;

    virtual void setXPosition( int32_t xPosition );
    virtual void setYPosition( int32_t yPosition );

    virtual int32_t getXPosition() const;
    virtual int32_t getYPosition() const;

    virtual Window* getTopLevelWindow() const;

    virtual const GeometryRequisition& getGeometryShape() const;

  protected:

    WindowImplementation( Window* window, Glyph* glyph );

    Window* _target;

    Glyph* _glyph;
    Style* _style;
    Display* _display;
    Visual* _visual;
    Canvas* _canvas;
    GeometryRequisition _geometryShape;
    GeometryAllocation _geometryAllocation;
    Cursor* _cursor;
    std::list< Cursor* > _cursors;
    FloatCoordinate _left;
    FloatCoordinate _bottom;
    float _xAlign;
    float _yAlign;

    EventHandler* _focusIn;
    EventHandler* _focusOut;
    EventHandler* _wmDelete;

    int32_t _xPosition;
    int32_t _yPosition;

    Window* _topLevelWindow;

    bool _placed;
    bool _aligned;
    bool _needsResize;
    bool _resized;
    bool _moved;
    bool _unmapped;
    bool _wmMapped;
    bool _mapPending;

};


}


#endif
