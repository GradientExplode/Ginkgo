#ifndef _gkg_graphics_X11_X11_X11CanvasImplementation_h_
#define _gkg_graphics_X11_X11_X11CanvasImplementation_h_


#include <gkg-graphics-X11-core/CanvasImplementation.h>
#include <gkg-graphics-X11-X11/Xlib.h>
#include <gkg-graphics-X11-X11/Xutil.h>
#include <map>


namespace gkg
{


class X11CanvasImplementation;
class X11BitmapImplementation;
class X11RasterImplementation;


//
// struct X11TextRenderInfo
//

struct X11TextRenderInfo
{

  X11CanvasImplementation* canvas;
  GC drawGC;
  int32_t x0;
  int32_t y0;
  FloatCoordinate width;
  FloatCoordinate currentX;
  FloatCoordinate currentY;
  char* text;
  char* currentText;
  int32_t spaces;
  XTextItem* items;

};


//
// struct X11PathRenderInfo
//

struct X11PathRenderInfo
{

  FloatCoordinate currentX;
  FloatCoordinate currentY;
  XPoint* point;
  XPoint* currentPoint;
  XPoint* endPoint;

};



//
// bitmaps hash-table
//

struct BitmapEntry
{

  BitmapEntry( const Bitmap* theBitmap, int32_t theKey ) : bitmap( theBitmap ),
                                                           key( theKey )
  {
  }

  const Bitmap* bitmap;
  int32_t key;

};


struct BitmapEntryCompare /*: public std::binary_function< BitmapEntry,
                                                         BitmapEntry,
                                                         bool >
                            deprecated in c++17 */
{

  typedef BitmapEntry first_argument_type;
  typedef BitmapEntry second_argument_type;
  typedef bool result_type;

  bool operator()( const BitmapEntry& e1, const BitmapEntry& e2 ) const
  {

    return ( ( uint64_t )e1.bitmap < ( uint64_t)e2.bitmap ) ||
           ( ( ( uint64_t )e1.bitmap == ( uint64_t )e2.bitmap  ) &&
             ( e1.key < e2.key ) );
  }

};


}


namespace gkg
{

//
// rasters hash-table
//

struct RasterEntry
{

  RasterEntry( const Raster* theRaster, int32_t theKey ) : raster( theRaster ),
                                                           key( theKey )
  {
  }

  const Raster* raster;
  int32_t key;

};


struct RasterEntryCompare /*: public std::binary_function< RasterEntry,
                                                         RasterEntry,
                                                         bool >
                            deprecated in c++17 */
{

  typedef RasterEntry first_argument_type;
  typedef RasterEntry second_argument_type;
  typedef bool result_type;

  bool operator()( const RasterEntry& e1, const RasterEntry& e2 ) const
  {

    return ( ( uint64_t )e1.raster < ( uint64_t)e2.raster ) ||
           ( ( ( uint64_t )e1.raster == ( uint64_t )e2.raster  ) &&
             ( e1.key < e2.key ) );
  }

};


//
// class X11CanvasImplementation
//

class X11CanvasImplementation : public CanvasImplementation
{

  public:

    typedef std::map< BitmapEntry,
                      const X11BitmapImplementation*,
                      BitmapEntryCompare > BitmapTable;

    typedef std::map< RasterEntry,
                      const X11RasterImplementation*,
                      RasterEntryCompare > RasterTable;

    enum
    {

      Unbound = 0

    };

    X11CanvasImplementation( Canvas* canvas, Window* window );
    ~X11CanvasImplementation();

    void newPath();
    void moveTo( FloatCoordinate x, FloatCoordinate y );
    void lineTo( FloatCoordinate x, FloatCoordinate y );
    void curveTo( FloatCoordinate x, FloatCoordinate y,
                  FloatCoordinate x1, FloatCoordinate y1,
                  FloatCoordinate x2, FloatCoordinate y2 );
    void closePath();
    void stroke( const Color* color, const Brush* brush );

    void fill( const Color* color );

    void character( const Font* font, int32_t ch, FloatCoordinate width,
                    const Color* color, FloatCoordinate x, FloatCoordinate y );
    void stencil( const Bitmap* bitmap, const Color* color,
                  FloatCoordinate x, FloatCoordinate y );
    void image( const Raster* raster, FloatCoordinate x, FloatCoordinate y );

    void pushClipping();
    void clip();
    void popClipping();

    void frontBuffer();
    void backBuffer();

    void redraw( FloatCoordinate left, FloatCoordinate bottom,
                 FloatCoordinate right, FloatCoordinate top );

    bool startRepair();

    void bind( bool doubleBuffered );
    void unbind();

    void flush();
    void swapBuffers();
    void setColor( const Color* color );
    void setBrush( const Brush* brush );
    void setFont( const Font* font );
    void setXDrawable( XDrawable xDrawable );

    static X11CanvasImplementation* getImplementation( Canvas* canvas );

  private:

    static XPoint* nextPoint();
    static bool isXRectangular( const XPoint* point, int32_t n );
    static const X11BitmapImplementation* 
      getBitmapImplementation( const Bitmap* bitmap,
                               const Transformer& transformer );
    static const X11RasterImplementation* 
      getRasterImplementation( const Raster* raster,
                               const Transformer& transformer );

    XDrawable _xDrawable;

    XDrawable _drawBuffer;
    XDrawable _copyBuffer;

    XRectangle _clip;
    Region _clipping;
    Region _empty;
    GC _drawGC;
    GC _copyGC;
    int32_t _operation;
    Pixmap _stipple;
    uint32_t _pixel;
    int32_t _brushWidth;
    char* _dashList;
    int32_t _dashCount;
    XFontStruct* _xFontStruct;
    bool _textTwoByte;
    bool _textReencode;
    bool _fontIsScaled;
    std::list< _XRegion* > _clippers;

    static BitmapTable _x11BitmapImplementations;
    static RasterTable _x11RasterImplementations;
    static X11TextRenderInfo _text;
    static X11PathRenderInfo _path;

};


}


#endif
