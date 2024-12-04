#ifndef _gkg_graphics_X11_core_PrinterImplementation_h_
#define _gkg_graphics_X11_core_PrinterImplementation_h_


#include <iostream>
#include <list>
#include <string>
#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Printer;
class Color;
class Brush;
class Font;
class Bitmap;
class Raster;
class Transformer;


class PrinterImplementation
{

  public:

    struct Info
    {

      Info();
      Info( const Color* theColor, const Brush* theBrush, const Font* theFont );
      const Color* color;
      const Brush* brush;
      const Font* font;

    };

    PrinterImplementation( Printer* printer, std::ostream* out );
    virtual ~PrinterImplementation();

    Printer* getTarget() const;

    virtual PixelCoordinate
            toPixelCoordinate( FloatCoordinate floatCoordinate ) const = 0;
    virtual FloatCoordinate
            toFloatCoordinate( PixelCoordinate pixelCoordinate ) const = 0;
    virtual FloatCoordinate
            toPixelThenFloatCoordinate(
                               FloatCoordinate floatCoordinate ) const = 0;

    virtual void resize( FloatCoordinate left,
                         FloatCoordinate bottom,
                         FloatCoordinate right,
                         FloatCoordinate top ) = 0;

    virtual void prolog( const std::string& creator ) = 0;
    virtual void epilog() = 0;

    virtual void setComment( const std::string& comment ) = 0;
    virtual void setPage( const std::string& label ) = 0;

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
    virtual void stencil( const Bitmap* bitmap, const Color* color,
                          FloatCoordinate x, FloatCoordinate y ) = 0;
    virtual void image( const Raster* raster,
                        FloatCoordinate x, FloatCoordinate y ) = 0;

    virtual void pushTransform() = 0;
    virtual void transform( const Transformer& transformer ) = 0;
    virtual void popTransform() = 0;

    virtual void pushClipping() = 0;
    virtual void clip() = 0;
    virtual void popClipping() = 0;

    virtual void flush() = 0;

  protected:

    Printer* _target;

    std::ostream* _out;
    int32_t _page;
    std::list< Info > _infos;

    float _x;
    float _y;
    FloatCoordinate _textCurrentX;
    FloatCoordinate _textCurrentY;
    int32_t _textChars;
    int32_t _textSpaces;

};


}


#endif
