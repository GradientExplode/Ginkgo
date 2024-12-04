#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/BitmapImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::Bitmap::Bitmap( const void* data,
                     PixelCoordinate pixelWidth, PixelCoordinate pixelHeight,
                     PixelCoordinate x0, PixelCoordinate y0 )
            : gkg::RCGUIObject()
{

  _bitmapImplementation = 
    gkg::getGUIImplementationFactory()->createBitmapImplementation( this,
                                        data, pixelWidth, pixelHeight, x0, y0 );

}


gkg::Bitmap::Bitmap( const gkg::Font* font, int32_t character, float scale )
            : gkg::RCGUIObject()
{

  _bitmapImplementation = 
    gkg::getGUIImplementationFactory()->createBitmapImplementation( this,
                                                       font, character, scale );

}


gkg::Bitmap::Bitmap( const gkg::Bitmap& other )
            : gkg::RCGUIObject()
{

  _bitmapImplementation = 
    gkg::getGUIImplementationFactory()->createBitmapImplementation( this,
                                                    other.getImplementation() );

}


gkg::Bitmap::Bitmap( const std::string& filename )
            : gkg::RCGUIObject()
{

  _bitmapImplementation = 
    gkg::getGUIImplementationFactory()->createBitmapImplementation( this,
                                                                     filename );

}


gkg::Bitmap::~Bitmap()
{

  delete _bitmapImplementation;

}


void gkg::Bitmap::poke( bool setting, 
                        gkg::PixelCoordinate x, gkg::PixelCoordinate y )
{

  _bitmapImplementation->poke( setting, x, y );

}


bool gkg::Bitmap::peek( gkg::PixelCoordinate x, gkg::PixelCoordinate y ) const
{

  return _bitmapImplementation->peek( x, y );

}


gkg::FloatCoordinate gkg::Bitmap::getWidth() const
{

  return _bitmapImplementation->getWidth();

}


gkg::FloatCoordinate gkg::Bitmap::getHeight() const
{

  return _bitmapImplementation->getHeight();

}


gkg::PixelCoordinate gkg::Bitmap::getPixelWidth() const
{

  return _bitmapImplementation->getPixelWidth();

}


gkg::PixelCoordinate gkg::Bitmap::getPixelHeight() const
{

  return _bitmapImplementation->getPixelHeight();

}


gkg::FloatCoordinate gkg::Bitmap::getLeftBearing() const
{

  return _bitmapImplementation->getLeftBearing();

}


gkg::FloatCoordinate gkg::Bitmap::getRightBearing() const
{

  return _bitmapImplementation->getRightBearing();

}


gkg::FloatCoordinate gkg::Bitmap::getAscent() const
{

  return _bitmapImplementation->getAscent();

}


gkg::FloatCoordinate gkg::Bitmap::getDescent() const
{

  return _bitmapImplementation->getDescent();

}


void gkg::Bitmap::flush() const
{

  _bitmapImplementation->flush();

}


gkg::BitmapImplementation* gkg::Bitmap::getImplementation() const
{

  return _bitmapImplementation;

}
