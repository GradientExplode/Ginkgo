#ifndef _gkg_graphics_X11_core_BitmapImplementation_h_
#define _gkg_graphics_X11_core_BitmapImplementation_h_


#include <string>
#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Bitmap;
class Display;
class Font;


class BitmapImplementation
{

  public:

    virtual ~BitmapImplementation();

    virtual  Bitmap* getTarget() const;

    virtual Display* getDisplay() const;

    virtual void poke( bool setting, PixelCoordinate x, PixelCoordinate y ) = 0;
    virtual bool peek( PixelCoordinate x, PixelCoordinate y ) = 0;

    virtual FloatCoordinate getWidth() const;
    virtual FloatCoordinate getHeight() const;
    virtual FloatCoordinate getLeft() const;
    virtual FloatCoordinate getBottom() const;
    virtual FloatCoordinate getRight() const;
    virtual FloatCoordinate getTop() const;

    virtual PixelCoordinate getPixelWidth() const;
    virtual PixelCoordinate getPixelHeight() const;

    virtual FloatCoordinate getLeftBearing() const;
    virtual FloatCoordinate getRightBearing() const;
    virtual FloatCoordinate getAscent() const;
    virtual FloatCoordinate getDescent() const;

    virtual void flush() = 0;

  protected:

    BitmapImplementation( Bitmap* bitmap, const void* data,
                          PixelCoordinate pixelWidth,
                          PixelCoordinate pixelHeight,
                          PixelCoordinate x0,
                          PixelCoordinate y0 );
    BitmapImplementation( Bitmap* bitmap, const Font* font,
                          int32_t character, float scale );
    BitmapImplementation( Bitmap* bitmap, BitmapImplementation* other );
    BitmapImplementation( Bitmap* bitmap, const std::string& filename );
    BitmapImplementation( Bitmap* bitmap,
                          Display* display,
                          FloatCoordinate left,
                          FloatCoordinate bottom,
                          FloatCoordinate right,
                          FloatCoordinate top,
                          FloatCoordinate width,
                          FloatCoordinate height,
                          PixelCoordinate pixelWidth,
                          PixelCoordinate pixelHeight );

    Bitmap* _target;

    Display* _display;
    FloatCoordinate _left;
    FloatCoordinate _bottom;
    FloatCoordinate _right;
    FloatCoordinate _top;
    FloatCoordinate _width;
    FloatCoordinate _height;
    PixelCoordinate _pixelWidth;
    PixelCoordinate _pixelHeight;
    bool _modified;

};


}


#endif
