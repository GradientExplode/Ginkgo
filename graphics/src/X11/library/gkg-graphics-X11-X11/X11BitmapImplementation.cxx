#include <gkg-graphics-X11-X11/X11BitmapImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11FontImplementation.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-core-exception/Exception.h>

#include <cassert>


gkg::X11BitmapImplementation::X11BitmapImplementation( 
                                               gkg::Bitmap* bitmap,
                                               const void* data,
                                               gkg::PixelCoordinate pixelWidth,
                                               gkg::PixelCoordinate pixelHeight,
                                               PixelCoordinate x0,
                                               PixelCoordinate y0 )
                             : gkg::BitmapImplementation( bitmap,
                                                          data,
                                                          pixelWidth,
                                                          pixelHeight,
                                                          x0,
                                                          y0 ),
                               _xImage( 0 )
{

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( _display );
  XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
  
  if ( data )
  {

    _pixmap = XCreateBitmapFromData( x11DisplayImpl->getXDisplay(),
                                     x11DisplayImpl->getXRoot(),
                                     ( char* )data,
                                     _pixelWidth,
                                     _pixelHeight );

  }
  else
  {

    _pixmap = XCreatePixmap( xDisplay,
                             x11DisplayImpl->getXRoot(),
                             _pixelWidth, _pixelHeight, 1 );
    GC gc = XCreateGC( xDisplay,_pixmap, 0, 0 );
    XSetForeground( xDisplay, gc, 0 );
    XFillRectangle( xDisplay, _pixmap, gc, 0, 0, _pixelWidth, _pixelHeight );
    XFreeGC( xDisplay, gc );

  }

}


gkg::X11BitmapImplementation::X11BitmapImplementation( gkg::Bitmap* bitmap,
                                                       const gkg::Font* font,
                                                       int32_t character,
                                                       float scale )
                             : gkg::BitmapImplementation( bitmap,
                                                          font,
                                                          character,
                                                          scale ),
                               _xImage( 0 )
{

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( _display );
  XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
  XWindow xRoot = x11DisplayImpl->getXRoot();

  gkg::X11FontImplementation* x11FontImpl =
    gkg::X11FontImplementation::getImplementation( 
                     ( gkg::Font* )font, _display );

  int32_t w, h;
  int32_t x, y;
  XFontStruct* xFontStruct = x11FontImpl->getXFontStruct();
  if ( ( character >= ( int32_t )xFontStruct->min_char_or_byte2 ) &&
       ( character <= ( int32_t )xFontStruct->max_char_or_byte2 ) &&
       xFontStruct->per_char != 0 )
  {

    int32_t i = character - xFontStruct->min_char_or_byte2;
    w = xFontStruct->per_char[ i ].rbearing -
        xFontStruct->per_char[ i ].lbearing;
    h = xFontStruct->per_char[ i ].ascent + xFontStruct->per_char[ i ].descent;
    x = xFontStruct->per_char[ i ].lbearing;
    y = xFontStruct->per_char[ i ].ascent;

  }
  else
  {

    w = xFontStruct->max_bounds.rbearing - xFontStruct->min_bounds.lbearing;
    h = xFontStruct->max_bounds.ascent + xFontStruct->max_bounds.descent;
    x = xFontStruct->min_bounds.lbearing;
    y = xFontStruct->max_bounds.ascent;

  }

  _pixelWidth = std::max( 1, ( int32_t )( w * scale ) );
  _pixelHeight = std::max( 1, ( int32_t )( h * scale ) );
  w = std::max( 1, w );
  h = std::max( 1, h );

  _pixmap = XCreatePixmap( xDisplay, xRoot, w, h, 1 );
  GC gc = XCreateGC( xDisplay, _pixmap, 0, 0 );
  XSetFont( xDisplay, gc, xFontStruct->fid );
  XSetForeground( xDisplay, gc, 0);
  XFillRectangle( xDisplay, _pixmap, gc, 0, 0, _pixelWidth, _pixelHeight );
  XSetForeground( xDisplay, gc, 1 );
  char ch = ( char )character;
  XDrawString( xDisplay, _pixmap, gc, -x, y, &ch, 1 );
  if ( scale != 1.0 )
  {

    XImage* source = XGetImage( xDisplay, _pixmap, 0, 0, w, h, 0x01, ZPixmap );
    XFreePixmap( xDisplay, _pixmap );
    _pixmap = XCreatePixmap( xDisplay, xRoot, _pixelWidth, _pixelHeight, 1 );
    XFillRectangle( xDisplay, _pixmap, gc, 0, 0, _pixelWidth, _pixelHeight );
    XImage* dest = XGetImage( xDisplay, _pixmap, 0, 0,
                              _pixelWidth, _pixelHeight,
                              0x01, ZPixmap );
    for ( int32_t dy = 0; dy < _pixelHeight; dy++ )
    {

      int32_t sy = ( int32_t )( ( float )dy / scale );
      for ( int32_t dx = 0; dx < _pixelWidth; dx++ )
      {

        int32_t sx = ( int32_t )( ( float )dx / scale );
        XPutPixel( dest, dx, _pixelHeight - 1 - dy,
                   XGetPixel( source, sx, h - 1 - sy ) );

      }

    }
    XPutImage( xDisplay, _pixmap, gc, dest, 0, 0, 0, 0,
               _pixelWidth, _pixelHeight );
    XDestroyImage( source );
    XDestroyImage( dest );

  }
  XFreeGC( xDisplay, gc );

  _width = _display->toFloatCoordinate( _pixelWidth );
  _height = _display->toFloatCoordinate( _pixelHeight );
  _left = _display->toFloatCoordinate( x ) * scale;
  _right = _display->toFloatCoordinate( x + w ) * scale;
  _top = _display->toFloatCoordinate( y ) * scale;
  _bottom = _display->toFloatCoordinate( y - h ) * scale;

}


gkg::X11BitmapImplementation::X11BitmapImplementation( 
                                                gkg::Bitmap* bitmap,
                                                X11BitmapImplementation* other )
                             : gkg::BitmapImplementation( bitmap, other ),
                               _xImage( 0 )
{

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( _display );
  XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
  XWindow xRoot = x11DisplayImpl->getXRoot();

  _pixmap = XCreatePixmap( xDisplay, xRoot, _pixelWidth, _pixelHeight, 1 );
  other->fill();
  fill();

  for ( int32_t bx = 0; bx < _pixelWidth; bx++ )
  {

    for ( int32_t by = 0; by < _pixelHeight; by++ )
    {

      uint32_t pixel;
      if ( ( bx >= 0 ) && ( bx < other->getPixelWidth() ) &&
           ( by >= 0 ) && ( by < other->getPixelHeight() ) )
      {

         pixel = XGetPixel( other->getXImage(), bx,
                            other->getPixelHeight() - by - 1 );

      }
      else
      {

        pixel = 0;

      }
      XPutPixel( _xImage, bx, _pixelHeight - by - 1, pixel );

    }

  }

}


gkg::X11BitmapImplementation::X11BitmapImplementation( 
                                                   gkg::Bitmap* bitmap,
                                                   const std::string& filename )
                             : gkg::BitmapImplementation( bitmap, filename ),
                               _xImage( 0 )
{

  gkg::X11DisplayImplementation* x11DisplayImpl =
    gkg::X11DisplayImplementation::getImplementation( _display );
  XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
  XWindow xRoot = x11DisplayImpl->getXRoot();

  uint32_t w = 0, h = 0;
  int32_t x0 = 0, y0 = 0;
  int32_t status = XReadBitmapFile( xDisplay, xRoot, filename.c_str(),
                                    &w, &h, &_pixmap, &x0, &y0 );

  if ( status != BitmapSuccess )
  {

    throw std::runtime_error( std::string( "Cannot read bitmap file : " ) +
                              filename );

  }

  _pixelWidth = w;
  _pixelHeight = h;
  _width = _display->toFloatCoordinate( w );
  _height = _display->toFloatCoordinate( h );
  if ( ( x0 == -1 ) && ( y0 == -1 ) )
  {

    _left = 0;
    _right = _display->toFloatCoordinate( w );
    _bottom = 0;
    _top = _display->toFloatCoordinate( h );

  }
  else
  {

    _left = _display->toFloatCoordinate( -x0 );
    _right = _display->toFloatCoordinate( w - x0 );
    _bottom = _display->toFloatCoordinate( y0 - h );
    _top = _display->toFloatCoordinate( y0 );

  }

}


gkg::X11BitmapImplementation::X11BitmapImplementation( 
                                               gkg::Bitmap* bitmap,
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
                             : gkg::BitmapImplementation( bitmap,
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
                               _pixmap( pixmap )
{
}
                                  

gkg::X11BitmapImplementation::~X11BitmapImplementation()
{

  if ( _xImage )
  {

    XDestroyImage( _xImage );

  }
  if ( _pixmap )
  {

    gkg::X11DisplayImplementation* x11DisplayImpl =
      gkg::X11DisplayImplementation::getImplementation( _display );

    XFreePixmap( x11DisplayImpl->getXDisplay(), _pixmap );

  }

}


void gkg::X11BitmapImplementation::poke( bool setting,
                                         gkg::PixelCoordinate x,
                                         gkg::PixelCoordinate y )
{

  fill();
  uint32_t p = setting ? 1 : 0;
  XPutPixel( _xImage, x, _pixelHeight - 1 - y, p );
  _modified = true;

}


bool gkg::X11BitmapImplementation::peek( gkg::PixelCoordinate x,
                                         gkg::PixelCoordinate y )
{

  fill();
  uint32_t pixel = XGetPixel( _xImage, x, _pixelHeight - 1 - y );
  return pixel != 0;

}


void gkg::X11BitmapImplementation::flush()
{

  if ( _modified )
  {

    _modified = false;
    gkg::X11DisplayImplementation* x11DisplayImpl =
      gkg::X11DisplayImplementation::getImplementation( _display );
    XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
    GC gc = XCreateGC( xDisplay, _pixmap, 0, 0 );
    XPutImage( xDisplay, _pixmap, gc, _xImage,
               0, 0, 0, 0, _pixelWidth, _pixelHeight );
    XFreeGC( xDisplay, gc );

  }

}


void gkg::X11BitmapImplementation::fill()
{

  if ( !_xImage )
  {

    gkg::X11DisplayImplementation* x11DisplayImpl =
      gkg::X11DisplayImplementation::getImplementation( _display );
    _xImage = XGetImage( x11DisplayImpl->getXDisplay(), _pixmap,
                         0, 0, _pixelWidth, _pixelHeight, 0x01, ZPixmap );

  }

}


XImage* gkg::X11BitmapImplementation::getXImage() const
{

  return _xImage;

}


Pixmap gkg::X11BitmapImplementation::getPixmap() const
{

  return _pixmap;

}


gkg::X11BitmapImplementation* 
gkg::X11BitmapImplementation::getImplementation( gkg::Bitmap* bitmap )
{

  return static_cast< X11BitmapImplementation* >( bitmap->getImplementation() );

}


