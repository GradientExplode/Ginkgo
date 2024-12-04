#ifndef _gkg_graphics_X11_core_CanvasImplementation_h_
#define _gkg_graphics_X11_core_CanvasImplementation_h_


#include <list>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-core/CanvasDamage.h>
#include <functional>
#include <string>
#include <map>


namespace gkg
{


class Font;


//
// font char hash-table
//

struct FontCharEntry
{

  FontCharEntry( const Font* theFont, int32_t theCh ) : font( theFont ),
                                                        ch( theCh ) {}

  const Font* font;
  int32_t ch;

};


struct FontCharEntryCompare /*: public std::binary_function< FontCharEntry,
                                                           FontCharEntry,
                                                           bool >
                              deprecated in c++17 */
{

  typedef FontCharEntry first_argument_type;
  typedef FontCharEntry second_argument_type;
  typedef bool result_type;

  bool operator()( const FontCharEntry& e1, const FontCharEntry& e2 ) const
  {

    return ( ( uint64_t )e1.font < ( uint64_t)e2.font ) ||
           ( ( ( uint64_t )e1.font == ( uint64_t )e2.font  ) &&
             ( e1.ch < e2.ch ) );
  }

};


}



namespace gkg
{


class Display;
class Transformer;
class Bitmap;
class Canvas;
class Window;
class Color;
class Brush;
class Raster;
class GeometryExtension;



//
// class CanvasImplementation
//

class CanvasImplementation
{

  public:

    typedef std::map< FontCharEntry, const Bitmap*, FontCharEntryCompare >
      FontCharTable;

    virtual ~CanvasImplementation();

    virtual Canvas* getTarget() const;

    virtual Display* getDisplay() const;
    virtual Window* getWindow() const;

    virtual void setDisplay( Display* display );

    virtual void setWidth( FloatCoordinate width );
    virtual void setHeight( FloatCoordinate height );
    virtual void setSize( FloatCoordinate width, FloatCoordinate height );
    virtual FloatCoordinate getWidth() const;
    virtual FloatCoordinate getHeight() const;

    virtual void setPixelWidth( PixelCoordinate pixelWidth );
    virtual void setPixelHeight( PixelCoordinate pixelHeight );
    virtual void setPixelSize( PixelCoordinate pixelWidth,
                               PixelCoordinate pixelHeight );
    virtual PixelCoordinate getPixelWidth() const;
    virtual PixelCoordinate getPixelHeight() const;

    virtual PixelCoordinate
            toPixelCoordinate( FloatCoordinate floatCoordinate ) const;
    virtual FloatCoordinate
            toFloatCoordinate( PixelCoordinate pixelCoordinate ) const;
    virtual FloatCoordinate
            toPixelThenFloatCoordinate( FloatCoordinate floatCoordinate ) const;

    virtual void newPath() = 0;
    virtual void moveTo( FloatCoordinate x, FloatCoordinate y ) = 0;
    virtual void lineTo( FloatCoordinate x, FloatCoordinate y ) = 0;
    virtual void curveTo( FloatCoordinate x, FloatCoordinate y,
                          FloatCoordinate x1, FloatCoordinate y1,
                          FloatCoordinate x2, FloatCoordinate y2 ) = 0;
    virtual void closePath() = 0;
    virtual void stroke( const Color* color, const Brush* brush ) = 0;

    virtual void fill( const Color* color ) = 0;

    virtual void character( const Font* font, int32_t ch, FloatCoordinate width,
                            const Color* color,
                            FloatCoordinate x, FloatCoordinate y ) = 0;
    virtual void string( const Font* font, const std::string& s,
                         const Color* color,
                         FloatCoordinate x, FloatCoordinate y );
    virtual void stencil( const Bitmap* bitmap, const Color* color,
                          FloatCoordinate x, FloatCoordinate y ) = 0;
    virtual void image( const Raster* raster,
                        FloatCoordinate x, FloatCoordinate y ) = 0;

    virtual void pushTransform();
    virtual void transform( const Transformer& transformer );
    virtual void popTransform();

    virtual void setTransformer( const Transformer& transformer );
    virtual const Transformer& getTransformer() const;

    virtual void pushClipping() = 0;
    virtual void clip() = 0;
    virtual void popClipping() = 0;

    virtual void frontBuffer() = 0;
    virtual void backBuffer() = 0;

    virtual void setDamage( const GeometryExtension& extension );
    virtual void setDamage( FloatCoordinate left, FloatCoordinate bottom,
                            FloatCoordinate right, FloatCoordinate top );
    virtual void setDamageAll();
    virtual bool isDamaged( const GeometryExtension& extension ) const;
    virtual bool isDamaged( FloatCoordinate left, FloatCoordinate bottom,
                            FloatCoordinate right, FloatCoordinate top ) const;
    virtual bool hasAnyDamage() const;
    virtual void getDamageArea( GeometryExtension& extension ) const;
    virtual void restrictDamage( const GeometryExtension& extension );
    virtual void restrictDamage( FloatCoordinate left, FloatCoordinate bottom,
                                 FloatCoordinate right, FloatCoordinate top );
    virtual void newDamage();
    virtual void clearDamage();

    virtual void redraw( FloatCoordinate left, FloatCoordinate bottom,
                         FloatCoordinate right, FloatCoordinate top ) = 0;

    virtual bool startRepair() = 0;
    virtual void repair();
    virtual void finishRepair();

    virtual void bind( bool doubleBuffered ) = 0;
    virtual void unbind() = 0;

    virtual void flush() = 0;
    virtual void swapBuffers() = 0;
    virtual void setColor( const Color* color ) = 0;
    virtual void setBrush( const Brush* brush ) = 0;
    virtual void setFont( const Font* font ) = 0;

  protected:

    CanvasImplementation( Canvas* canvas, Window* window  );
    
    virtual void referenceColor( const Color* color );
    virtual void referenceBrush( const Brush* brush );
    virtual void referenceFont( const Font* font );
    virtual void unreferenceAll();

    static bool isRectangular( int32_t x0, int32_t y0,
                               int32_t x1, int32_t y1,
                               int32_t x2, int32_t y2,
                               int32_t x3, int32_t y3 );
    static const Bitmap* getCharBitmap( Display* display,
                                        const gkg::Font* font,
                                        int32_t ch );

    Canvas* _target;

    Display* _display;
    Window* _window;
    FloatCoordinate _width;
    FloatCoordinate _height;
    PixelCoordinate _pixelWidth;
    PixelCoordinate _pixelHeight;
    const Brush* _brush;
    const Color* _color;
    const Font* _font;

    std::list< Transformer* > _transformers;
    bool _transformed;

    CanvasDamage _damage;
    bool _damaged;
    bool _onDamageList;
    bool _repairing;

    static FontCharTable _charBitmaps;

};


}


#endif
