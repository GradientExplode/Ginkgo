#include <cassert>
#include <gkg-graphics-X11-X11/X11RasterImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11Visual.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Raster.h>


gkg::X11RasterImplementation::X11RasterImplementation( 
                                              gkg::Raster* raster,
                                              gkg::PixelCoordinate pixelWidth,
                                              gkg::PixelCoordinate pixelHeight )
                             : gkg::RasterImplementation( raster,
                                                          pixelWidth,
                                                          pixelHeight )
{

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( _display );
  gkg::X11Visual* x11Visual = 
    gkg::X11Visual::getImplementation( x11DisplayImpl->getDefaultVisual() );
  XDisplay* xDisplay = x11DisplayImpl->getXDisplay();

  _pixmap = XCreatePixmap( xDisplay,
                           x11DisplayImpl->getXRoot(),
                           _pixelWidth, _pixelHeight,
                           x11Visual->getDepth() );
  _gc = XCreateGC( xDisplay, _pixmap, 0, 0 );
  _xImage = XGetImage( xDisplay, _pixmap, 0, 0, _pixelWidth, _pixelHeight,
                       AllPlanes, ZPixmap );

}


gkg::X11RasterImplementation::X11RasterImplementation( 
                                                gkg::Raster* raster,
                                                X11RasterImplementation* other )
                             : gkg::RasterImplementation( raster, other ),
                               _xImage( 0 )
{

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( _display );
  gkg::X11Visual* x11Visual = 
    gkg::X11Visual::getImplementation( x11DisplayImpl->getDefaultVisual() );
  XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
  XWindow xRoot = x11DisplayImpl->getXRoot();

  other->flush();
  _pixmap = XCreatePixmap( xDisplay, xRoot, _pixelWidth, _pixelHeight,
                           x11Visual->getDepth() );
  _gc = XCreateGC( xDisplay, _pixmap, 0, 0 );
  XCopyArea( xDisplay, other->getPixmap(), _pixmap, _gc, 0, 0,
             _pixelWidth, _pixelHeight, 0, 0 );
  _xImage = XGetImage( xDisplay, _pixmap, 0, 0, _pixelWidth, _pixelHeight,
                       AllPlanes, ZPixmap );

}


gkg::X11RasterImplementation::X11RasterImplementation( 
                                               gkg::Raster* raster,
                                               gkg::Display* display,
                                               gkg::FloatCoordinate left,
                                               gkg::FloatCoordinate bottom,
                                               gkg::FloatCoordinate right,
                                               gkg::FloatCoordinate top,
                                               gkg::FloatCoordinate width,
                                               gkg::FloatCoordinate height,
                                               gkg::PixelCoordinate pixelWidth,
                                               gkg::PixelCoordinate pixelHeight,
                                               Pixmap pixmap )
                             : gkg::RasterImplementation( raster,
                                                          display,
                                                          left,
                                                          bottom,
                                                          right,
                                                          top,
                                                          width,
                                                          height,
                                                          pixelWidth,
                                                          pixelHeight ),
                               _xImage( 0 ),
                               _pixmap( pixmap ),
                               _gc( 0 )
{
}
                                  

gkg::X11RasterImplementation::~X11RasterImplementation()
{

  if ( _xImage )
  {

    XDestroyImage( _xImage );

  }
  XDisplay* xDisplay =
   gkg::X11DisplayImplementation::getImplementation(_display)->getXDisplay();
  if ( _pixmap )
  {

    XFreePixmap( xDisplay, _pixmap );

  }
  if ( _gc )
  {

    XFreeGC( xDisplay, _gc );

  }

}


void gkg::X11RasterImplementation::poke( gkg::PixelCoordinate x,
                                         gkg::PixelCoordinate y,
                                         gkg::FloatColorIntensity r,
                                         gkg::FloatColorIntensity g,
                                         gkg::FloatColorIntensity b,
                                         float )
{

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( _display );
  gkg::X11Visual* x11Visual = 
    gkg::X11Visual::getImplementation( x11DisplayImpl->getDefaultVisual() );

  uint16_t sr = ( uint16_t )( r * 0xffff );
  uint16_t sg = ( uint16_t )( g * 0xffff );
  uint16_t sb = ( uint16_t )( b * 0xffff );
  XColor xColor;
  x11Visual->findColor( sr, sg, sb, xColor );
  XPutPixel( _xImage, x, _pixelHeight - 1 - y, xColor.pixel );
  _modified = true;

}


void gkg::X11RasterImplementation::peek( gkg::PixelCoordinate x,
                                         gkg::PixelCoordinate y,
                                         gkg::FloatColorIntensity& r,
                                         gkg::FloatColorIntensity& g,
                                         gkg::FloatColorIntensity& b,
                                         float& alpha )
{

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( _display );
  gkg::X11Visual* x11Visual = 
    gkg::X11Visual::getImplementation( x11DisplayImpl->getDefaultVisual() );

  uint32_t pixel = XGetPixel( _xImage, x, _pixelHeight - 1 - y );
  XColor xColor;
  x11Visual->findColor( pixel, xColor );
  r = ( float )xColor.red / 0xffff;  
  g = ( float )xColor.green / 0xffff;  
  b = ( float )xColor.blue / 0xffff;  
  alpha = 1.0f;

}


void gkg::X11RasterImplementation::flush()
{

  if ( _modified )
  {

    _modified = false;
    gkg::X11DisplayImplementation* x11DisplayImpl =
      gkg::X11DisplayImplementation::getImplementation( _display );
    XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
    XPutImage( xDisplay, _pixmap, _gc, _xImage,
               0, 0, 0, 0, _pixelWidth, _pixelHeight );

  }

}


XImage* gkg::X11RasterImplementation::getXImage() const
{

  return _xImage;

}


Pixmap gkg::X11RasterImplementation::getPixmap() const
{

  return _pixmap;

}


GC gkg::X11RasterImplementation::getGC() const
{

  return _gc;

}


gkg::X11RasterImplementation* 
gkg::X11RasterImplementation::getImplementation( gkg::Raster* raster )
{

  return static_cast< X11RasterImplementation* >( raster->getImplementation() );

}


