#ifndef _gkg_graphics_X11_X11_X11BitmapImplementation_h_
#define _gkg_graphics_X11_X11_X11BitmapImplementation_h_


#include <gkg-graphics-X11-core/BitmapImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>


namespace gkg
{


class X11BitmapImplementation : public BitmapImplementation
{

  public:

    X11BitmapImplementation( Bitmap* bitmap,
                             const void* data,
                             PixelCoordinate pixelWidth,
                             PixelCoordinate pixelHeight,
                             PixelCoordinate x0,
                             PixelCoordinate y0 );
    X11BitmapImplementation( Bitmap* bitmap,
                             const Font* font,
                             int32_t character,
                             float scale );
    X11BitmapImplementation( Bitmap* bitmap,
                             X11BitmapImplementation* other );
    X11BitmapImplementation( Bitmap* bitmap,
                             const std::string& filename );
    X11BitmapImplementation( Bitmap* bitmap,
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
    virtual ~X11BitmapImplementation();

    void poke( bool setting, PixelCoordinate x, PixelCoordinate y );
    bool peek( PixelCoordinate x, PixelCoordinate y );

    void flush();
    void fill();

    XImage* getXImage() const;
    Pixmap getPixmap() const;

    static X11BitmapImplementation* getImplementation( Bitmap* bitmap );

  private:

    XImage* _xImage;
    Pixmap _pixmap;

};


}


#endif
