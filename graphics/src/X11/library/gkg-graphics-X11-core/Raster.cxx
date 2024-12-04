#include <gkg-graphics-X11-core/Raster.h>
#include <gkg-graphics-X11-core/RasterImplementation.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::Raster::Raster( PixelCoordinate pixelWidth, PixelCoordinate pixelHeight )
            : gkg::RCGUIObject()
{

  _rasterImplementation = 
    gkg::getGUIImplementationFactory()->createRasterImplementation( this,
                                                      pixelWidth, pixelHeight );

}


gkg::Raster::Raster( const gkg::Raster& other )
            : gkg::RCGUIObject()
{

  _rasterImplementation = 
    gkg::getGUIImplementationFactory()->createRasterImplementation( this,
                                                    other.getImplementation() );

}


gkg::Raster::~Raster()
{

  delete _rasterImplementation;

}


void gkg::Raster::poke( gkg::PixelCoordinate x, gkg::PixelCoordinate y,
                        gkg::FloatColorIntensity r,
                        gkg::FloatColorIntensity g,
                        gkg::FloatColorIntensity b,
                        float alpha )
{

  _rasterImplementation->poke( x, y, r, g, b, alpha );

}


void gkg::Raster::peek( gkg::PixelCoordinate x, gkg::PixelCoordinate y,
                           gkg::FloatColorIntensity& r,
                           gkg::FloatColorIntensity& g,
                           gkg::FloatColorIntensity& b,
                           float& alpha ) const
{

  _rasterImplementation->peek( x, y, r, g, b, alpha );

}


gkg::FloatCoordinate gkg::Raster::getWidth() const
{

  return _rasterImplementation->getWidth();

}


gkg::FloatCoordinate gkg::Raster::getHeight() const
{

  return _rasterImplementation->getHeight();

}


gkg::PixelCoordinate gkg::Raster::getPixelWidth() const
{

  return _rasterImplementation->getPixelWidth();

}


gkg::PixelCoordinate gkg::Raster::getPixelHeight() const
{

  return _rasterImplementation->getPixelHeight();

}


gkg::FloatCoordinate gkg::Raster::getLeftBearing() const
{

  return _rasterImplementation->getLeftBearing();

}


gkg::FloatCoordinate gkg::Raster::getRightBearing() const
{

  return _rasterImplementation->getRightBearing();

}


gkg::FloatCoordinate gkg::Raster::getAscent() const
{

  return _rasterImplementation->getAscent();

}


gkg::FloatCoordinate gkg::Raster::getDescent() const
{

  return _rasterImplementation->getDescent();

}


void gkg::Raster::flush() const
{

  _rasterImplementation->flush();

}


gkg::RasterImplementation* gkg::Raster::getImplementation() const
{

  return _rasterImplementation;

}
