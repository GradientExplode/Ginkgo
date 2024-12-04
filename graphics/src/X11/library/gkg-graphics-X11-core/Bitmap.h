#ifndef _gkg_graphics_X11_core_Bitmap_h_
#define _gkg_graphics_X11_core_Bitmap_h_


#include <string>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class Font;
class BitmapImplementation;


class Bitmap : public RCGUIObject
{

  public:

    Bitmap( const void* data,
            PixelCoordinate pixelWidth, PixelCoordinate pixelHeight,
            PixelCoordinate x0 = -1, PixelCoordinate y0 = -1 );
    Bitmap( const Font* font, int32_t character, float scale = 1.0 );
    Bitmap( const Bitmap& other );
    Bitmap( const std::string& filename );
    virtual ~Bitmap();

    virtual void poke( bool setting, PixelCoordinate x, PixelCoordinate y );
    virtual bool peek( PixelCoordinate x, PixelCoordinate y ) const;

    virtual FloatCoordinate getWidth() const;
    virtual FloatCoordinate getHeight() const;

    virtual PixelCoordinate getPixelWidth() const;
    virtual PixelCoordinate getPixelHeight() const;

    virtual FloatCoordinate getLeftBearing() const;
    virtual FloatCoordinate getRightBearing() const;
    virtual FloatCoordinate getAscent() const;
    virtual FloatCoordinate getDescent() const;

    virtual void flush() const;

    BitmapImplementation* getImplementation() const;

  protected:

    Bitmap();

  private:

    BitmapImplementation* _bitmapImplementation;

};


}


#endif
