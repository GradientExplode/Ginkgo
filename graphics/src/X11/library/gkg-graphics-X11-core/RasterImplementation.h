#ifndef _gkg_graphics_X11_core_RasterImplementation_h_
#define _gkg_graphics_X11_core_RasterImplementation_h_


#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-base/Color.h>


namespace gkg
{


class Raster;
class Display;


class RasterImplementation
{

  public:

    virtual ~RasterImplementation();

    virtual  Raster* getTarget() const;

    virtual Display* getDisplay() const;

    virtual void poke( PixelCoordinate x, PixelCoordinate y,
                       FloatColorIntensity r,
                       FloatColorIntensity g,
                       FloatColorIntensity b,
                       float alpha ) = 0;
    virtual void peek( PixelCoordinate x, PixelCoordinate y,
                       FloatColorIntensity& r,
                       FloatColorIntensity& g,
                       FloatColorIntensity& b,
                       float& alpha ) = 0;

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

    RasterImplementation( Raster* Raster,
                          PixelCoordinate pixelWidth,
                          PixelCoordinate pixelHeight );
    RasterImplementation( Raster* raster, RasterImplementation* other );
    RasterImplementation( Raster* raster,
                          Display* display,
                          FloatCoordinate left,
                          FloatCoordinate bottom,
                          FloatCoordinate right,
                          FloatCoordinate top,
                          FloatCoordinate width,
                          FloatCoordinate height,
                          PixelCoordinate pixelWidth,
                          PixelCoordinate pixelHeight );

    Raster* _target;

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
