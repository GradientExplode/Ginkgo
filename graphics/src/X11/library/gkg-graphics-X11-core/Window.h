#ifndef _gkg_graphics_X11_core_Window_h_
#define _gkg_graphics_X11_core_Window_h_


#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Glyph;
class Style;
class Display;
class Canvas;
class Cursor;
class Event;
class EventHandler;
class WindowImplementation;


class Window
{

  public:

    virtual ~Window();

    virtual Glyph* getGlyph() const;

    virtual void setStyle( Style* style );
    virtual Style* getStyle() const;

    virtual void setDisplay( Display* display );
    virtual Display* getDisplay() const;

    virtual Canvas* getCanvas() const;

    virtual void setCursor( Cursor* cursor );
    virtual Cursor* getCursor() const;
    virtual void pushCursor();
    virtual void popCursor();

    virtual void setPlace( FloatCoordinate left, FloatCoordinate bottom );
    virtual void setPixelPlace( IntegerCoordinate left,
                                IntegerCoordinate bottom );
    virtual void align( float x, float y );
    virtual FloatCoordinate getLeft() const;
    virtual FloatCoordinate getBottom() const;

    virtual FloatCoordinate getWidth() const;
    virtual FloatCoordinate getHeight() const;

    virtual void map();
    virtual void unmap();
    virtual bool isMapped() const;

    virtual void bind();
    virtual void unbind();
    virtual bool isBound() const;

    virtual void raise();
    virtual void lower();
    virtual void move( FloatCoordinate left, FloatCoordinate bottom );
    virtual void resize();

    virtual void receive( const Event& event );
    virtual EventHandler* getTarget( const Event& event ) const;
    virtual void getGrabPointer( Cursor* cursor = 0 ) const;
    virtual void ungrabPointer() const;

    virtual void repair();

    virtual void configure();
    virtual void setDefaultGeometry();
    virtual void computeGeometry();
    virtual void setAttributes();
    virtual void setProperties();
    virtual void doMap();

    WindowImplementation* getImplementation() const;

  protected:

    Window( Glyph* glyph );
    Window( WindowImplementation* windowImplementation );

  private:

    WindowImplementation* _windowImplementation;

};


}


#endif
