#ifndef _gkg_graphics_X11_X11_X11RasterImplementation_h_
#define _gkg_graphics_X11_X11_X11RasterImplementation_h_


#include <gkg-graphics-X11-core/RasterImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11RasterImplementation : public RasterImplementation
{

  public:

    X11RasterImplementation( Raster* raster,
                             PixelCoordinate pixelWidth,
                             PixelCoordinate pixelHeight );
    X11RasterImplementation( Raster* raster,
                             X11RasterImplementation* other );
    X11RasterImplementation( Raster* raster,
                             Display* display,
                             FloatCoordinate left,
                             FloatCoordinate bottom,
                             FloatCoordinate right,
                             FloatCoordinate top,
                             FloatCoordinate width,
                             FloatCoordinate height,
                             PixelCoordinate pixelWidth,
                             PixelCoordinate pixelHeight,
                             Pixmap pixmap );
    virtual ~X11RasterImplementation();

    void poke( PixelCoordinate x,
               PixelCoordinate y,
               FloatColorIntensity r,
               FloatColorIntensity g,
               FloatColorIntensity b,
               float alpha );
    void peek( PixelCoordinate x,
               PixelCoordinate y,
               FloatColorIntensity& r,
               FloatColorIntensity& g,
               FloatColorIntensity& b,
               float& alpha );

    void flush();
    void fill();

    XImage* getXImage() const;
    Pixmap getPixmap() const;
    GC getGC() const;

    static X11RasterImplementation* getImplementation( Raster* raster );

  private:

    XImage* _xImage;
    Pixmap _pixmap;
    GC _gc;

};


}


#endif
