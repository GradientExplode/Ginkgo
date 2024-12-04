#ifndef _gkg_graphics_X11_core_Raster_h_
#define _gkg_graphics_X11_core_Raster_h_


#include <string>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-base/Color.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class RasterImplementation;


class Raster : public RCGUIObject
{

  public:

    Raster( PixelCoordinate pixelWidth, PixelCoordinate pixelHeight );
    Raster( const Raster& other );
    virtual ~Raster();

    virtual void poke( PixelCoordinate x, PixelCoordinate y,
                       FloatColorIntensity r,
                       FloatColorIntensity g,
                       FloatColorIntensity b,
                       float alpha );
    virtual void peek( PixelCoordinate x, PixelCoordinate y,
                       FloatColorIntensity& r,
                       FloatColorIntensity& g,
                       FloatColorIntensity& b,
                       float& alpha ) const;

    virtual FloatCoordinate getWidth() const;
    virtual FloatCoordinate getHeight() const;

    virtual PixelCoordinate getPixelWidth() const;
    virtual PixelCoordinate getPixelHeight() const;

    virtual FloatCoordinate getLeftBearing() const;
    virtual FloatCoordinate getRightBearing() const;
    virtual FloatCoordinate getAscent() const;
    virtual FloatCoordinate getDescent() const;

    virtual void flush() const;

    RasterImplementation* getImplementation() const;

  protected:

    Raster();

  private:

    RasterImplementation* _rasterImplementation;

};


}


#endif
