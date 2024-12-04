#include <cctype>
#include <gkg-graphics-X11-X11/X11CanvasImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11ColorImplementation.h>
#include <gkg-graphics-X11-X11/X11BitmapImplementation.h>
#include <gkg-graphics-X11-X11/X11RasterImplementation.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-X11/X11BrushImplementation.h>
#include <gkg-graphics-X11-X11/X11FontImplementation.h>
#include <gkg-graphics-X11-X11/X11Visual.h>
#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-core/Raster.h>
#include <gkg-graphics-X11-core/Brush.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-processing-algobase/Math.h>

//
// Adobe table of characters for ISO8859 standard
//
static const int32_t adobe_to_iso8859[] =
{

  0000, 0001, 0002, 0003, 0004, 0005, 0006, 0007,
  0010, 0011, 0012, 0013, 0014, 0015, 0016, 0017,
  0020, 0021, 0022, 0023, 0024, 0025, 0026, 0027,
  0030, 0031, 0032, 0033, 0034, 0035, 0036, 0037,
  0040, 0041, 0042, 0043, 0044, 0045, 0046, 0047,
  0050, 0051, 0052, 0053, 0054, 0055, 0056, 0057,
  0060, 0061, 0062, 0063, 0064, 0065, 0066, 0067,
  0070, 0071, 0072, 0073, 0074, 0075, 0076, 0077,

  0100, 0101, 0102, 0103, 0104, 0105, 0106, 0107,
  0110, 0111, 0112, 0113, 0114, 0115, 0116, 0117,
  0120, 0121, 0122, 0123, 0124, 0125, 0126, 0127,
  0130, 0131, 0132, 0133, 0134, 0135, 0136, 0137,
  0140, 0141, 0142, 0143, 0144, 0145, 0146, 0147,
  0150, 0151, 0152, 0153, 0154, 0155, 0156, 0157,
  0160, 0161, 0162, 0163, 0164, 0165, 0166, 0167,
  0170, 0171, 0172, 0173, 0174, 0175, 0176, 0177,

  0200, 0201, 0202, 0203, 0204, 0205, 0206, 0207,
  0210, 0211, 0212, 0213, 0214, 0215, 0216, 0217,
  0220, 0221, 0222, 0223, 0224, 0225, 0226, 0227,
  0230, 0231, 0232, 0233, 0234, 0235, 0236, 0237,
  0240, 0241, 0242, 0243, 0057, 0245, 0000, 0247,
  0244, 0264, 0042, 0253, 0000, 0000, 0000, 0000,
  0000, 0055, 0000, 0000, 0267, 0000, 0266, 0052,
  0000, 0000, 0042, 0273, 0137, 0000, 0000, 0277,

  0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
  0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
  0055, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
  0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
  0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
  0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
  0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000,
  0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000

};


//
// X11CanvasImplementation
//

gkg::X11CanvasImplementation::BitmapTable 
gkg::X11CanvasImplementation::_x11BitmapImplementations;

gkg::X11CanvasImplementation::RasterTable 
gkg::X11CanvasImplementation::_x11RasterImplementations;

gkg::X11TextRenderInfo 
gkg::X11CanvasImplementation::_text = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

gkg::X11PathRenderInfo 
gkg::X11CanvasImplementation::_path = { 0, 0, 0, 0, 0 };


gkg::X11CanvasImplementation::X11CanvasImplementation( gkg::Canvas* canvas,
                                                       gkg::Window* window )
                          : gkg::CanvasImplementation( canvas, window ),
                            _xDrawable( gkg::X11CanvasImplementation::Unbound ),
                            _drawBuffer( gkg::X11CanvasImplementation::Unbound ),
                            _copyBuffer( gkg::X11CanvasImplementation::Unbound ),
                            _clipping( XCreateRegion() ),
                            _empty( XCreateRegion() ),
                            _drawGC( 0 ),
                            _copyGC( 0 ),
                            _stipple( 0 ),
                            _pixel( 0 ),
                            _brushWidth( 0 ),
                            _dashList( 0),
                            _dashCount( 0 ),
                            _textTwoByte( false ),
                            _textReencode( false )
{

  if ( !_text.text )
  {

    _text.text = new char[ 1000 ];
    _text.currentText = _text.text;
    _text.items = new XTextItem[ 100 ];

  }
  if ( !_path.point )
  {

    _path.point = new XPoint[ 10 ];
    _path.currentPoint = _path.point;
    _path.endPoint = _path.point + 10;

  }

}


gkg::X11CanvasImplementation::~X11CanvasImplementation()
{

  unbind();
  XDestroyRegion( _clipping );
  XDestroyRegion( _empty );
  _clippers.clear();

}


void gkg::X11CanvasImplementation::newPath()
{

  _path.currentX = 0;
  _path.currentY = 0;
  XPoint* xPoint = _path.point;
  xPoint->x = 0;
  xPoint->y = 0;
  _path.currentPoint = xPoint;

}


void gkg::X11CanvasImplementation::moveTo( gkg::FloatCoordinate x,
                                           gkg::FloatCoordinate y )
{

  _path.currentX = x;
  _path.currentY = y;
  gkg::FloatCoordinate tx, ty;
  if ( _transformed )
  {

    getTransformer().transform( x, y, tx, ty );

  }
  else
  {

    tx = x;
    ty = y;

  }
  XPoint* xPoint = _path.point;
  xPoint->x = _display->toPixelCoordinate( tx );
  xPoint->y = _pixelHeight - _display->toPixelCoordinate( ty );
  _path.currentPoint = xPoint + 1;

}


void gkg::X11CanvasImplementation::lineTo( gkg::FloatCoordinate x,
                                           gkg::FloatCoordinate y )
{

  _path.currentX = x;
  _path.currentY = y;
  gkg::FloatCoordinate tx, ty;
  if ( _transformed )
  {

    getTransformer().transform( x, y, tx, ty );

  }
  else
  {

    tx = x;
    ty = y;

  }
  XPoint* xPoint = nextPoint();
  xPoint->x = _display->toPixelCoordinate( tx );
  xPoint->y = _pixelHeight - _display->toPixelCoordinate( ty );

}


void gkg::X11CanvasImplementation::curveTo( 
                              gkg::FloatCoordinate x, gkg::FloatCoordinate y,
                              gkg::FloatCoordinate x1, gkg::FloatCoordinate y1,
                              gkg::FloatCoordinate x2, gkg::FloatCoordinate y2 )
{

  gkg::FloatCoordinate px = _path.currentX;
  gkg::FloatCoordinate py = _path.currentY;

  if ( _transformers.front()->isStraight( px, py, x1, y1, x2, y2, x, y ) )
  {

    lineTo( x, y );

  }
  else 
  {

    gkg::FloatCoordinate xx = gkg::middle( x1, x2 );
    gkg::FloatCoordinate yy = gkg::middle( y1, y2 );
    gkg::FloatCoordinate x11 = gkg::middle( px, x1 );
    gkg::FloatCoordinate y11 = gkg::middle( py, y1 );
    gkg::FloatCoordinate x22 = gkg::middle( x2, x );
    gkg::FloatCoordinate y22 = gkg::middle( y2, y );
    gkg::FloatCoordinate x12 = gkg::middle( x11, xx );
    gkg::FloatCoordinate y12 = gkg::middle( y11, yy );
    gkg::FloatCoordinate x21 = gkg::middle( xx, x22 );
    gkg::FloatCoordinate y21 = gkg::middle( yy, y22 );
    gkg::FloatCoordinate cx = gkg::middle( x12, x21 );
    gkg::FloatCoordinate cy = gkg::middle( y12, y21 );

    curveTo( cx, cy, x11, y11, x12, y12 );
    curveTo( x, y, x21, y21, x22, y22 );

  }

}


void gkg::X11CanvasImplementation::closePath()
{

  XPoint* startPoint = _path.point;
  XPoint* xPoint = nextPoint();
  xPoint->x = startPoint->x;
  xPoint->y = startPoint->y;

}


void gkg::X11CanvasImplementation::stroke( const gkg::Color* color,
                                           const gkg::Brush* brush )
{

  int32_t n = _path.currentPoint - _path.point;
  if ( n < 2 )
  {

    return;

  }

  flush();
  setColor( color );
  setBrush( brush );

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();

  XPoint* point = _path.point;
  if ( n == 2 )
  {

    XDrawLine( xDisplay, _drawBuffer, _drawGC,
               point[ 0 ].x, point[ 0 ].y,
               point[ 1 ].x, point[ 1 ].y );

  }
  else if ( isXRectangular( point, n ) )
  {

    int32_t x = std::min( point[ 0 ].x, point[ 2 ].x );
    int32_t y = std::min( point[ 0 ].y, point[ 2 ].y );
    int32_t w = gkg::abs( point[ 0 ].x - point[ 2 ].x );
    int32_t h = gkg::abs( point[ 0 ].y - point[ 2 ].y );
    XDrawRectangle( xDisplay, _drawBuffer, _drawGC, x, y, w, h);

  }
  else
  {

    XDrawLines( xDisplay, _drawBuffer, _drawGC, point, n, CoordModeOrigin );

  }

}


void gkg::X11CanvasImplementation::fill( const gkg::Color* color )
{

  int32_t n = _path.currentPoint - _path.point;
  if ( n <= 2 )
  {

    return;

  }
  flush();
  setColor( color );
  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XPoint* point = _path.point;
  if ( isXRectangular( point, n ) )
  {

    int32_t x = std::min( point[ 0 ].x, point[ 2 ].x );
    int32_t y = std::min( point[ 0 ].y, point[ 2 ].y );
    int32_t w = gkg::abs( point[ 0 ].x - point[ 2 ].x );
    int32_t h = gkg::abs( point[ 0 ].y - point[ 2 ].y );
    XFillRectangle( xDisplay, _drawBuffer, _drawGC, x, y, w, h );

  }
  else
  {

    XFillPolygon( xDisplay, _drawBuffer, _drawGC, point, n,
                  Complex, CoordModeOrigin );

  }

}


void gkg::X11CanvasImplementation::character( const gkg::Font* font, int32_t ch,
                                              gkg::FloatCoordinate width,
                                              const gkg::Color* color,
                                              gkg::FloatCoordinate x,
                                              gkg::FloatCoordinate y )
{

  bool isFlush = !isprint( ( int32_t )ch );
  if ( font && font != _font )
  {

    flush();
    setFont( font );

  }
  if ( color && color != _color )
  {

    flush();
    setColor( color );

  }
  const gkg::Transformer& transformer = getTransformer();
  if ( !_fontIsScaled && 
       ( !_transformed || transformer.getKey( width, width ) == 0 ) )
  {

    if ( isFlush || ( y != _text.currentY ) ||
         !gkg::equal( x, _text.currentX, 0.1f ) )
    {

      flush();

    }
    char* cp = _text.currentText;
    if ( cp == _text.text )
    {

      gkg::FloatCoordinate tx = x;
      gkg::FloatCoordinate ty = y;
      if ( _transformed )
      {

        transformer.transform( tx, ty );

      }
      _text.canvas = this;
      _text.drawGC = _drawGC;
      _text.x0 = _display->toPixelCoordinate( tx );
      _text.y0 = _pixelHeight - _display->toPixelCoordinate( ty );
      _text.width = 0;

    }
    _text.width += width;
    _text.currentX = x + width;
    _text.currentY = y;
    if ( _textTwoByte )
    {

      *cp++ = ( char )( ( ch & 0xff00 ) >> 8 );
      *cp++ = ( char )( ch & 0xff );

    } 
    else if ( _textReencode )
    {

      *cp++ = adobe_to_iso8859[ ch & 0xff ];

    }
    else
    {

      *cp++ = ( char )( ch & 0xff );

    }
    _text.currentText = cp;
    if ( ch == ' ' )
    {

      _text.spaces ++;
      if ( cp > _text.text + 1 )
      {

        flush();

      }

    }
    if ( isFlush )
    {

      flush();

    }

  } 
  else if ( ch != ' ' )
  {

    flush();
    stencil( gkg::CanvasImplementation::getCharBitmap( _display, font, ch ),
             color, x, y );

  }

}


void gkg::X11CanvasImplementation::stencil( const gkg::Bitmap* bitmap,
                                            const gkg::Color* color,
                                            gkg::FloatCoordinate x,
                                            gkg::FloatCoordinate y )
{

  flush();

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  gkg::X11ColorImplementation*
    x11ColorImpl = gkg::X11ColorImplementation::getImplementation(
                                    ( gkg::Color* )color,
                                    _window->getImplementation()->getVisual() );
  // hack to be suppressed

  const gkg::Transformer& transformer = getTransformer();
  bitmap->flush();

  const gkg::X11BitmapImplementation* 
    x11BitmapImpl = getBitmapImplementation( ( gkg::Bitmap* )bitmap,
                                             transformer );
  // hack to be suppressed
  
  gkg::FloatCoordinate tx;
  gkg::FloatCoordinate ty;
  if ( _transformed )
  {

    transformer.transform( x, y, tx, ty );

  }
  else
  {

    tx = x;
    ty = y;

  }

  int32_t pixelLeft = _display->toPixelCoordinate( tx + 
                                                   x11BitmapImpl->getLeft() );
  int32_t pixelTop = _pixelHeight - 
                     _display->toPixelCoordinate( ty + 
                                                  x11BitmapImpl->getTop() );

  XGCValues gcValues;
  uint32_t valueMask = 0;

  valueMask |= GCFunction;
  gcValues.function = GXand;
  valueMask |= GCForeground;
  gcValues.foreground = 0;
  valueMask |= GCBackground;
  gcValues.background = AllPlanes;
  valueMask |= GCGraphicsExposures;
  gcValues.graphics_exposures = False;

  GC xgc = XCreateGC( xDisplay, _drawBuffer, valueMask, &gcValues );
  XCopyGC( xDisplay, _drawGC, GCClipMask, xgc );

  XCopyPlane( xDisplay, x11BitmapImpl->getPixmap(), _drawBuffer, xgc,
              0, 0,
              x11BitmapImpl->getPixelWidth(), x11BitmapImpl->getPixelHeight(),
              pixelLeft, pixelTop, 1 );

  gcValues.function = GXxor;
  gcValues.foreground = x11ColorImpl->getXColor().pixel;
  gcValues.background = 0;
  valueMask &= ~GCGraphicsExposures;
  XChangeGC( xDisplay, xgc, valueMask, &gcValues );
  XCopyPlane( xDisplay, x11BitmapImpl->getPixmap(), _drawBuffer, xgc,
              0, 0,
              x11BitmapImpl->getPixelWidth(), x11BitmapImpl->getPixelHeight(),
              pixelLeft, pixelTop, 1 );
  XFreeGC( xDisplay, xgc );

}


void gkg::X11CanvasImplementation::image( const gkg::Raster* raster,
                                          gkg::FloatCoordinate x,
                                          gkg::FloatCoordinate y )
{

  flush();

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();

  const gkg::Transformer& transformer = getTransformer();
  raster->flush();

  const X11RasterImplementation* 
    x11RasterImpl = getRasterImplementation( ( gkg::Raster* )raster,
                                             transformer );

  gkg::FloatCoordinate tx;
  gkg::FloatCoordinate ty;
  if ( _transformed )
  {

    transformer.transform( x, y, tx, ty );

  }
  else
  {

    tx = x;
    ty = y;

  }

  int32_t pixelLeft = _display->toPixelCoordinate( tx + 
                                                   x11RasterImpl->getLeft() );
  int32_t pixelTop = _pixelHeight - 
                     _display->toPixelCoordinate( ty + 
                                                  x11RasterImpl->getTop() );

  // we assume that graphics exposures are off in the gc.
  XCopyArea( xDisplay, x11RasterImpl->getPixmap(), _drawBuffer, _drawGC,
             0, 0,
             x11RasterImpl->getPixelWidth(), x11RasterImpl->getPixelHeight(),
             pixelLeft, pixelTop );

}


// There doesn't appear to be any way to copy a region other than
// to union it with an empty region.
void gkg::X11CanvasImplementation::pushClipping()
{

  flush();
  Region oldClip = _clipping;
  Region newClip = XCreateRegion();
  XUnionRegion( oldClip, _empty, newClip );
  _clippers.push_back( oldClip );
  _clipping = newClip;

}


void gkg::X11CanvasImplementation::clip()
{

  flush();

  int32_t n = _path.currentPoint - _path.point;
  if ( n <= 2 )
  {

    return;

  }

  Region clip;
  XPoint* point = _path.point;
  if ( isXRectangular( point, n ) )
  {

    XRectangle xr;
    xr.x = std::min( point[ 0 ].x, point[ 2 ].x );
    xr.y = std::min( point[ 0 ].y, point[ 2 ].y );
    xr.width = gkg::abs( point[ 0 ].x - point[ 2 ].x );
    xr.height = gkg::abs( point[ 0 ].y - point[ 2 ].y );
    clip = XCreateRegion();
    XUnionRectWithRegion( &xr, _empty, clip );

  }
  else
  {

    clip = XPolygonRegion( point, n, EvenOddRule );

  }

  if ( !XEmptyRegion( _clipping ) )
  {

    Region intersect = XCreateRegion();
    XIntersectRegion( _clipping, clip, intersect );
    XDestroyRegion( clip );
    clip = intersect;

  }
  XDestroyRegion( _clipping );
  _clipping = clip;
  X11DisplayImplementation* x11DisplayImpl =
                  gkg::X11DisplayImplementation::getImplementation( _display );
  XSetRegion( x11DisplayImpl->getXDisplay(),
              _drawGC, clip );

}


void gkg::X11CanvasImplementation::popClipping()
{

  flush();

  // stack underflow--should raise exception
  if ( _clippers.empty() )
  {

    return;

  }

  XDestroyRegion( _clipping );
  _clipping = _clippers.back();
  _clippers.pop_back();

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();

  if ( XEmptyRegion( _clipping ) )
  {

    XSetClipMask( xDisplay, _drawGC, None );

  }
  else
  {

    XSetRegion( xDisplay, _drawGC, _clipping );

  }

}


void gkg::X11CanvasImplementation::frontBuffer()
{

  if ( _copyBuffer != gkg::X11CanvasImplementation::Unbound )
  {

    _drawBuffer = _copyBuffer;

  }

}


void gkg::X11CanvasImplementation::backBuffer()
{

  if ( _copyBuffer != gkg::X11CanvasImplementation::Unbound )
  {

    _drawBuffer = _xDrawable;

  }

}


void gkg::X11CanvasImplementation::redraw( gkg::FloatCoordinate left,
                                           gkg::FloatCoordinate bottom,
                                           gkg::FloatCoordinate right,
                                           gkg::FloatCoordinate top )
{

  // Force a portion of the canvas to be redrawn.  This is typically caused
  // by an X expose event.  If there is no damage and we have a buffer
  // with a copy of the canvas, then we can just copy it to the draw buffer.
  // Otherwise, we just damage the area.

  if ( !_damaged && ( _copyBuffer != gkg::X11CanvasImplementation::Unbound ) )
  {

    int32_t pixelTop = _display->toPixelCoordinate( top );
    int32_t x = _display->toPixelCoordinate( left );
    int32_t y = _pixelHeight - pixelTop;
    uint32_t w = _display->toPixelCoordinate( right ) - x;
    uint32_t h = pixelTop - _display->toPixelCoordinate( bottom );

    XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
    XCopyArea( xDisplay, _drawBuffer, _copyBuffer, _copyGC, x, y, w, h, x, y );

  }
  else
  {

    setDamage( left, bottom, right, top );

  }

}


bool gkg::X11CanvasImplementation::startRepair()
{

  if ( !_damaged )
  {

    return false;

  }

  int32_t left = _display->toPixelCoordinate( _damage.left );
  int32_t bottom = _display->toPixelCoordinate( _damage.bottom );
  int32_t right = _display->toPixelCoordinate( _damage.right );
  int32_t top = _display->toPixelCoordinate( _damage.top );

  if ( left < 0 )
  {

    left = 0;

  }
  else if ( left > _pixelWidth )
  {

    left = _pixelWidth;

  }

  if ( bottom < 0 )
  {

    bottom = 0;

  }
  else if ( bottom > _pixelHeight )
  {

    bottom = _pixelHeight;

  }

  if ( right < 0 )
  {

    right = 0;

  }
  else if ( right > _pixelWidth )
  {

    right = _pixelWidth;

  }

  if ( top < 0 )
  {

    top = 0;

  }
  else if ( top > _pixelHeight )
  {

    top = _pixelHeight;

  }

  _clip.x = left;
  _clip.y = _pixelHeight - top;
  _clip.width = right - left;
  _clip.height = top - bottom;

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();

  XUnionRectWithRegion( &_clip, _empty, _clipping );
  XSetClipRectangles( xDisplay, _drawGC, 0, 0, &_clip, 1, YXBanded );
  _repairing = true;

  return true;

}


// We must (re)bind the drawbuffer to the canvas' drawable whenever
// it changes size or is set to a new xid.  This allows us
// to (re)allocate a backbuffer pixmap when double-buffering.
// We set the canvas' xdrawable to the backbuffer.
void gkg::X11CanvasImplementation::bind( bool doubleBuffered )
{

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation(_window);
  gkg::X11Visual* x11Visual = 
    gkg::X11Visual::getImplementation( x11WindowImpl->getVisual() );

  XGCValues gcValues;
  gcValues.graphics_exposures = False;

  if ( doubleBuffered )
  {

    _drawBuffer = XCreatePixmap( xDisplay, _xDrawable,
                                 _pixelWidth, _pixelHeight,
                                 x11Visual->getDepth() );
    _copyBuffer = _xDrawable;
    _copyGC = XCreateGC( xDisplay, _copyBuffer, GCGraphicsExposures, &gcValues);
    _xDrawable = _drawBuffer;

  }
  else
  {

    _drawBuffer = _xDrawable;
    _copyBuffer = gkg::X11CanvasImplementation::Unbound;

  }
  _drawGC = XCreateGC( xDisplay, _drawBuffer, GCGraphicsExposures, &gcValues );

}


// If double-buffering, free the backbuffer and set the canvas' xdrawable
// back to the frontbuffer.
void gkg::X11CanvasImplementation::unbind()
{

  if ( _display )
  {

    XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
    if ( _copyBuffer != gkg::X11CanvasImplementation::Unbound )
    {

      XFreePixmap( xDisplay, _drawBuffer );
      _xDrawable = _copyBuffer;
      _copyBuffer = gkg::X11CanvasImplementation::Unbound;
      if ( _copyGC )
      {

        XFreeGC( xDisplay, _copyGC );
        _copyGC = 0;

      }

    }
    if ( _drawGC )
    {

      XFreeGC( xDisplay, _drawGC );
      _drawGC = 0;

    }

  }
  _drawBuffer = gkg::X11CanvasImplementation::Unbound;
  unreferenceAll();

}


void gkg::X11CanvasImplementation::flush()
{

  uint32_t charCount = _text.currentText - _text.text;
  if ( charCount )
  {

    XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();

    if ( ( _text.spaces == 0 ) || _textTwoByte )
    {

      if ( _textTwoByte )
      {

        XDrawString16( xDisplay, _drawBuffer, _drawGC,
                       _text.x0, _text.y0,
                       ( XChar2b* )_text.text, charCount / 2 );

      }
      else
      {

        XDrawString( xDisplay, _drawBuffer, _drawGC,
                     _text.x0, _text.y0,
                     _text.text, charCount );

      }

    }
    else
    {

      int32_t width = XTextWidth( _xFontStruct, _text.text, charCount );
      int32_t delta = _display->toPixelCoordinate( _text.width ) - width;
      int32_t item = 0;
      int32_t count = 0;
      _text.items[ item ].chars = _text.text;
      _text.items[ item ].delta = 0;
      _text.items[ item ].font = None;
      char* textEnd = _text.currentText;
      for ( char* cp = _text.text; cp < textEnd; cp++ )
      {

        if ( *cp == ' ' )
        {

          int32_t d = delta / ( _text.spaces - item );
          delta -= d;
          _text.items[ item ].nchars = count;
          ++ item;
          _text.items[ item ].chars = cp;
          _text.items[ item ].delta = d;
          _text.items[ item ].font = None;
          count = 0;

        }
        ++ count;

      }
      _text.items[ item ].nchars = count;
      XDrawText( xDisplay, _drawBuffer, _drawGC,
                 _text.x0, _text.y0,
                 _text.items, item + 1 );

    }
    _text.currentText = _text.text;
    _text.spaces = 0;

  }

}


void gkg::X11CanvasImplementation::swapBuffers()
{

  // not double-buffering
  if ( _copyBuffer == gkg::X11CanvasImplementation::Unbound )
  {

    return;

  }

  XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
  XCopyArea( xDisplay, _drawBuffer, _copyBuffer, _copyGC,
             _clip.x, _clip.y, _clip.width, _clip.height, _clip.x, _clip.y );

}


void gkg::X11CanvasImplementation::setColor( const gkg::Color* color )
{

  if ( color && color != _color )
  {

    referenceColor( color );

    XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();

    gkg::X11WindowImplementation* x11WindowImpl =
      gkg::X11WindowImplementation::getImplementation( _window );

    gkg::X11Visual* x11Visual = 
      gkg::X11Visual::getImplementation( x11WindowImpl->getVisual() );

    gkg::X11ColorImplementation*
      x11ColorImpl = gkg::X11ColorImplementation::getImplementation( 
                                   ( gkg::Color* ) color, x11Visual );
    // hack to be suppressed

    _pixel = x11ColorImpl->getXColor().pixel;
    _operation = x11ColorImpl->getOperation();
    _stipple = x11ColorImpl->getStipple();

    if ( x11ColorImpl->isMasking() )
    {

      XSetForeground( xDisplay, _drawGC, 1 );

    }
    else if ( _operation == GXxor )
    {

      XSetForeground( xDisplay, _drawGC, 
                      x11Visual->getXor( _window->getStyle() ) );

    }
    else
    {

      XSetForeground( xDisplay, _drawGC, _pixel );

    }

    XSetFunction( xDisplay, _drawGC, _operation );
    if ( _stipple )
    {

      XSetStipple( xDisplay, _drawGC, _stipple );
      XSetFillStyle( xDisplay, _drawGC, FillStippled );

    }
    else
    {

      XSetFillStyle( xDisplay, _drawGC, FillSolid );

    }

  }

}


void gkg::X11CanvasImplementation::setBrush( const gkg::Brush* brush )
{

  if ( brush && ( brush != _brush ) )
  {

    referenceBrush( brush );

    XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();

    X11BrushImplementation* 
    x11BrushImpl = gkg::X11BrushImplementation::getImplementation(
                                   ( gkg::Brush* )brush, _display );

    _brushWidth = x11BrushImpl->getPixelWidth();
    _dashList = x11BrushImpl->getDashList();
    _dashCount = x11BrushImpl->getDashCount();

    if ( !_dashList )
    {

      XSetLineAttributes( xDisplay, _drawGC, _brushWidth, LineSolid,
                          CapButt, JoinMiter );

    }
    else 
    {

      XSetLineAttributes( xDisplay, _drawGC, _brushWidth, LineOnOffDash,
                          CapButt, JoinMiter );
      XSetDashes( xDisplay, _drawGC, 0, _dashList, _dashCount );

    }

  }

}


void gkg::X11CanvasImplementation::setFont( const gkg::Font* font )
{

  if ( font && font != _font )
  {

    referenceFont( font );

    XDisplay* xDisplay = gkg::X11DisplayImplementation::getImplementation(
                                                      _display )->getXDisplay();
    gkg::X11FontImplementation*
      x11FontImpl = gkg::X11FontImplementation::getImplementation(
                                    ( gkg::Font* )_font, _display );

    _xFontStruct = x11FontImpl->getXFontStruct();
    _textTwoByte = _xFontStruct->min_byte1 > 0 || _xFontStruct->max_byte1 > 0;
    std::string coding = x11FontImpl->getEncoding();
    _textReencode = ( !coding.empty() && ( coding != "ISO8859" ) );
    if ( x11FontImpl->isUnscaled() )
    {

      _fontIsScaled = false;

    }
    else
    {

      _fontIsScaled = true;
      float tolerance = 0.15;
      if ( _window )
      {

        gkg::Style* style = _window->getStyle();
        if ( style )
        {

          style->findAttribute( "fontScaleTolerance", tolerance );

        }

      }
      _fontIsScaled = (	x11FontImpl->getScale() < ( 1 - tolerance ) ||
                        x11FontImpl->getScale() > ( 1 + tolerance ) );

    }
    XSetFont( xDisplay, _drawGC, _xFontStruct->fid );

  }

}


void gkg::X11CanvasImplementation::setXDrawable( XDrawable xDrawable )
{

  _xDrawable = xDrawable;

}


gkg::X11CanvasImplementation* 
gkg::X11CanvasImplementation::getImplementation( gkg::Canvas* canvas )
{

  return static_cast< X11CanvasImplementation* >( canvas->getImplementation() );

}


XPoint* gkg::X11CanvasImplementation::nextPoint()
{

  if ( _path.currentPoint == _path.endPoint )
  {

    int32_t oldSize = _path.currentPoint - _path.point;
    int32_t newSize = 2 * oldSize;
    XPoint* theNewPath = new XPoint[ newSize ];
    for ( int32_t i = 0; i < oldSize; i++ )
    {

      theNewPath[ i ] = _path.point[ i ];

    }
    delete _path.point;
    _path.point = theNewPath;
    _path.currentPoint = _path.point + oldSize;
    _path.endPoint = _path.point + newSize;

  }
  XPoint* xPoint = _path.currentPoint;
  _path.currentPoint = xPoint + 1;
  return xPoint;

}


bool 
gkg::X11CanvasImplementation::isXRectangular( const XPoint* point, int32_t n )
{

  return ( n == 5 ) &&
         ( point[ 0 ].x == point[ 4 ].x ) &&
         ( point[ 0 ].y == point[ 4 ].y ) &&
	 gkg::CanvasImplementation::isRectangular(
                                                   point[ 0 ].x, point[ 0 ].y,
                                                   point[ 1 ].x, point[ 1 ].y,
                                                   point[ 2 ].x, point[ 2 ].y,
                                                   point[ 3 ].x, point[ 3 ].y );

}


const gkg::X11BitmapImplementation* 
gkg::X11CanvasImplementation::getBitmapImplementation(
                                           const gkg::Bitmap* bitmap,
                                           const gkg::Transformer& transformer )
{

  int32_t key = transformer.getKey( bitmap->getWidth(), bitmap->getHeight() );
  if ( key == 0 )
  {

    return gkg::X11BitmapImplementation::getImplementation( 
                                                       ( gkg::Bitmap* )bitmap );

  }

  const gkg::X11BitmapImplementation* x11BitmapImpl = 0;

  gkg::BitmapEntry entry( bitmap, key );
  gkg::X11CanvasImplementation::BitmapTable::const_iterator 
    b = _x11BitmapImplementations.find( entry );

  if ( b != _x11BitmapImplementations.end() )
  {

    x11BitmapImpl = ( *b ).second;

  }
  else
  {

    gkg::Display* display = bitmap->getImplementation()->getDisplay();
    gkg::Transformer t( transformer );

    gkg::FloatCoordinate x0, y0;
    t.transform( 0, 0, x0, y0 );
    t.translate( -x0, -y0 );

    gkg::FloatCoordinate x1, x2, x3, x4;
    gkg::FloatCoordinate y1, y2, y3, y4;

    t.transform( -bitmap->getLeftBearing(), -bitmap->getDescent(), x1, y1 );
    t.transform( -bitmap->getLeftBearing(), bitmap->getAscent(), x2, y2 );
    t.transform( bitmap->getRightBearing(), bitmap->getAscent(), x3, y3 );
    t.transform( bitmap->getRightBearing(), -bitmap->getDescent(), x4, y4 );

    gkg::FloatCoordinate xMax = gkg::max( x1, x2, x3, x4 );
    gkg::FloatCoordinate xMin = gkg::min( x1, x2, x3, x4 );
    gkg::FloatCoordinate yMax = gkg::max( y1, y2, y3, y4 );
    gkg::FloatCoordinate yMin = gkg::min( y1, y2, y3, y4 );

    int32_t width = display->toPixelCoordinate( xMax ) -
                    display->toPixelCoordinate( xMin );
    int32_t height = display->toPixelCoordinate( yMax ) -
                     display->toPixelCoordinate( yMin );
    if ( width <= 0 )
    {

      width = 1;

    }
    if ( height <= 0 )
    {

      height = 1;

    }

    X11DisplayImplementation*
      x11DisplayImpl = gkg::X11DisplayImplementation::getImplementation(
                                                                      display );
    XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
    X11BitmapImplementation* x11BitmapImplSrc =
      gkg::X11BitmapImplementation::getImplementation(
                               ( gkg::Bitmap* )bitmap );

    XImage* source = XGetImage( xDisplay,
                                x11BitmapImplSrc->getPixmap(),
                                0, 0,
                                x11BitmapImplSrc->getPixelWidth(),
                                x11BitmapImplSrc->getPixelHeight(),
                                0x01, ZPixmap );

    Pixmap pixmap = XCreatePixmap( xDisplay,
                                   x11DisplayImpl->getXRoot(),
                                   width, height, 1 );
    GC xGc = XCreateGC( xDisplay, pixmap, 0, 0 );
    XSetForeground( xDisplay, xGc, 0 );
    XFillRectangle( xDisplay, pixmap, xGc, 0, 0, width, height );
    XImage* dest = XGetImage( xDisplay,
                              pixmap,
                              0, 0,
                              width, height,
                              0x01, ZPixmap );

    int32_t dx0 = display->toPixelCoordinate( -xMin );
    int32_t dy0 = display->toPixelCoordinate( -yMin );
    int32_t sx0 = display->toPixelCoordinate( bitmap->getLeftBearing() );
    int32_t sy0 = display->toPixelCoordinate( bitmap->getDescent() );
    for ( int32_t dy = 0; dy < height; dy++ )
    {

      gkg::FloatCoordinate tx1, ty1, tx2, ty2;
      t.inverseTransform( -dx0, dy - dy0, tx1, ty1 );
      t.inverseTransform( width - dx0, dy - dy0, tx2, ty2 );
      float deltaX = ( tx2 - tx1 ) / width;
      float deltaY = ( ty2 - ty1 ) / width;
      int32_t sx, sy;
      for ( int32_t dx = 0; dx < width; dx++ )
      {

        sx = ( int32_t )tx1 + sx0;
        sy = ( int32_t )ty1 + sy0;
        if ( ( sx >= 0 ) && ( sx < x11BitmapImplSrc->getPixelWidth() ) &&
             ( sy >= 0 ) && ( sy < x11BitmapImplSrc->getPixelHeight() ) )
        {

          XPutPixel( dest, dx, height - 1 - dy,
                     XGetPixel( source, sx,
                                x11BitmapImplSrc->getPixelHeight() - 1 - sy ) );

        }
        tx1 += deltaX;
        ty1 += deltaY;

      }

    }

    XPutImage( xDisplay, pixmap, xGc, dest, 0, 0, 0, 0, width, height );
    XFreeGC( xDisplay, xGc );
    XDestroyImage( source );
    XDestroyImage( dest );

    x11BitmapImpl = new gkg::X11BitmapImplementation( ( gkg::Bitmap* )bitmap,
                                                      display,
                                                      xMin,
                                                      yMin,
                                                      xMax,
                                                      yMax,
                                                      xMax - xMin,
                                                      yMax - yMin,
                                                      width,
                                                      height,
                                                      pixmap );
    // hack to be suppressed
    _x11BitmapImplementations[ entry ] = x11BitmapImpl;

  }
  return x11BitmapImpl;

}


const gkg::X11RasterImplementation* 
gkg::X11CanvasImplementation::getRasterImplementation(
                                           const gkg::Raster* raster,
                                           const gkg::Transformer& transformer )
{


  int32_t key = transformer.getKey( raster->getWidth(), raster->getHeight() );
  if ( key == 0 )
  {

    return gkg::X11RasterImplementation::getImplementation(
                                                       ( gkg::Raster* )raster );

  }

  const gkg::X11RasterImplementation* x11RasterImpl = 0;

  gkg::RasterEntry entry( raster, key );
  gkg::X11CanvasImplementation::RasterTable::const_iterator
    r = _x11RasterImplementations.find( entry );

  if ( r != _x11RasterImplementations.end() )
  {

    x11RasterImpl = ( *r ).second;

    gkg::Display* display = raster->getImplementation()->getDisplay();
    gkg::Transformer t( transformer );

    gkg::FloatCoordinate x0, y0;
    t.transform( 0, 0, x0, y0 );
    t.translate( -x0, -y0 );

    gkg::FloatCoordinate x1, x2, x3, x4;
    gkg::FloatCoordinate y1, y2, y3, y4;

    t.transform( -raster->getLeftBearing(), -raster->getDescent(), x1, y1 );
    t.transform( -raster->getLeftBearing(), raster->getAscent(), x2, y2 );
    t.transform( raster->getRightBearing(), raster->getAscent(), x3, y3 );
    t.transform( raster->getRightBearing(), -raster->getDescent(), x4, y4 );

    gkg::FloatCoordinate xMax = gkg::max( x1, x2, x3, x4 );
    gkg::FloatCoordinate xMin = gkg::min( x1, x2, x3, x4 );
    gkg::FloatCoordinate yMax = gkg::max( y1, y2, y3, y4 );
    gkg::FloatCoordinate yMin = gkg::min( y1, y2, y3, y4 );

    int32_t width = display->toPixelCoordinate( xMax ) -
                    display->toPixelCoordinate( xMin );
    int32_t height = display->toPixelCoordinate( yMax ) -
                     display->toPixelCoordinate( yMin );
    if ( width <= 0 )
    {

      width = 1;

    }
    if ( height <= 0 )
    {

      height = 1;

    }

    X11DisplayImplementation*
      x11DisplayImpl = gkg::X11DisplayImplementation::getImplementation(
                                                                   display );
    XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
    X11RasterImplementation* x11RasterImplSrc =
      gkg::X11RasterImplementation::getImplementation( 
                               ( gkg::Raster* )raster );

    XImage* source = XGetImage( xDisplay,
                                x11RasterImplSrc->getPixmap(),
                                0, 0,
                                x11RasterImplSrc->getPixelWidth(),
                                x11RasterImplSrc->getPixelHeight(),
                                AllPlanes, ZPixmap );

    GC xGc = XCreateGC( xDisplay, x11RasterImpl->getPixmap(), 0, 0 );

    XImage* dest = XGetImage( xDisplay,
                              x11RasterImpl->getPixmap(),
                              0, 0,
                              width, height,
                              AllPlanes, ZPixmap );

    int32_t dx0 = display->toPixelCoordinate( -xMin );
    int32_t dy0 = display->toPixelCoordinate( -yMin );
    int32_t sx0 = display->toPixelCoordinate( raster->getLeftBearing() );
    int32_t sy0 = display->toPixelCoordinate( raster->getDescent() );
    for ( int32_t dy = 0; dy < height; dy++ )
    {

      gkg::FloatCoordinate tx1, ty1, tx2, ty2;
      t.inverseTransform( -dx0, dy - dy0, tx1, ty1 );
      t.inverseTransform( width - dx0, dy - dy0, tx2, ty2 );
      float deltaX = ( tx2 - tx1 ) / width;
      float deltaY = ( ty2 - ty1 ) / width;
      int32_t sx, sy;
      for ( int32_t dx = 0; dx < width; dx++ )
      {

        sx = ( int32_t )tx1 + sx0;
        sy = ( int32_t )ty1 + sy0;
        if ( ( sx >= 0 ) && ( sx < x11RasterImplSrc->getPixelWidth() ) &&
             ( sy >= 0 ) && ( sy < x11RasterImplSrc->getPixelHeight() ) )
        {

          XPutPixel( dest, dx, height - 1 - dy,
                     XGetPixel( source, sx,
                                x11RasterImplSrc->getPixelHeight() - 1 - sy ) );

        }
        tx1 += deltaX;
        ty1 += deltaY;

      }

    }

    XPutImage( xDisplay, x11RasterImpl->getPixmap(), xGc, dest,
               0, 0, 0, 0, width, height );
    XFreeGC( xDisplay, xGc );
    XDestroyImage( source );
    XDestroyImage( dest );

  }
  else
  {

    gkg::Display* display = raster->getImplementation()->getDisplay();
    gkg::Transformer t( transformer );

    gkg::FloatCoordinate x0, y0;
    t.transform( 0, 0, x0, y0 );
    t.translate( -x0, -y0 );

    gkg::FloatCoordinate x1, x2, x3, x4;
    gkg::FloatCoordinate y1, y2, y3, y4;

    t.transform( -raster->getLeftBearing(), -raster->getDescent(), x1, y1 );
    t.transform( -raster->getLeftBearing(), raster->getAscent(), x2, y2 );
    t.transform( raster->getRightBearing(), raster->getAscent(), x3, y3 );
    t.transform( raster->getRightBearing(), -raster->getDescent(), x4, y4 );

    gkg::FloatCoordinate xMax = gkg::max( x1, x2, x3, x4 );
    gkg::FloatCoordinate xMin = gkg::min( x1, x2, x3, x4 );
    gkg::FloatCoordinate yMax = gkg::max( y1, y2, y3, y4 );
    gkg::FloatCoordinate yMin = gkg::min( y1, y2, y3, y4 );

    int32_t width = display->toPixelCoordinate( xMax ) -
                    display->toPixelCoordinate( xMin );
    int32_t height = display->toPixelCoordinate( yMax ) -
                     display->toPixelCoordinate( yMin );
    if ( width <= 0 )
    {

      width = 1;

    }
    if ( height <= 0 )
    {

      height = 1;

    }

    X11DisplayImplementation*
      x11DisplayImpl = gkg::X11DisplayImplementation::getImplementation(
                                                                   display );
    XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
    X11RasterImplementation* x11RasterImplSrc =
      gkg::X11RasterImplementation::getImplementation( 
                               ( gkg::Raster* )raster );
    int32_t depth = gkg::X11Visual::getImplementation(
                               x11DisplayImpl->getDefaultVisual() )->getDepth();

    XImage* source = XGetImage( xDisplay,
                                x11RasterImplSrc->getPixmap(),
                                0, 0,
                                x11RasterImplSrc->getPixelWidth(),
                                x11RasterImplSrc->getPixelHeight(),
                                AllPlanes, ZPixmap );

    Pixmap pixmap = XCreatePixmap( xDisplay,
                                   x11DisplayImpl->getXRoot(),
                                   width, height,
                                   depth );
    GC xGc = XCreateGC( xDisplay, pixmap, 0, 0 );
    XSetForeground( xDisplay, xGc, 0 );
    XFillRectangle( xDisplay, pixmap, xGc, 0, 0, width, height );
    XImage* dest = XGetImage( xDisplay,
                              pixmap,
                              0, 0,
                              width, height,
                              AllPlanes, ZPixmap );

    int32_t dx0 = display->toPixelCoordinate( -xMin );
    int32_t dy0 = display->toPixelCoordinate( -yMin );
    int32_t sx0 = display->toPixelCoordinate( raster->getLeftBearing() );
    int32_t sy0 = display->toPixelCoordinate( raster->getDescent() );
    for ( int32_t dy = 0; dy < height; dy++ )
    {

      gkg::FloatCoordinate tx1, ty1, tx2, ty2;
      t.inverseTransform( -dx0, dy - dy0, tx1, ty1 );
      t.inverseTransform( width - dx0, dy - dy0, tx2, ty2 );
      float deltaX = ( tx2 - tx1 ) / width;
      float deltaY = ( ty2 - ty1 ) / width;
      int32_t sx, sy;
      for ( int32_t dx = 0; dx < width; dx++ )
      {

        sx = ( int32_t )tx1 + sx0;
        sy = ( int32_t )ty1 + sy0;
        if ( ( sx >= 0 ) && ( sx < x11RasterImplSrc->getPixelWidth() ) &&
             ( sy >= 0 ) && ( sy < x11RasterImplSrc->getPixelHeight() ) )
        {

          XPutPixel( dest, dx, height - 1 - dy,
                     XGetPixel( source, sx,
                                x11RasterImplSrc->getPixelHeight() - 1 - sy ) );

        }
        tx1 += deltaX;
        ty1 += deltaY;

      }

    }

    XPutImage( xDisplay, pixmap, xGc, dest, 0, 0, 0, 0, width, height );
    XFreeGC( xDisplay, xGc );
    XDestroyImage( source );
    XDestroyImage( dest );

    x11RasterImpl = new gkg::X11RasterImplementation( ( gkg::Raster* )raster,
                                                       display,
                                                       xMin,
                                                       yMin,
                                                       xMax,
                                                       yMax,
                                                       xMax - xMin,
                                                       yMax - yMin,
                                                       width,
                                                       height,
                                                       pixmap );
    // hack to be suppressed
    _x11RasterImplementations[ entry ] = x11RasterImpl;

  }

  return x11RasterImpl;

}
