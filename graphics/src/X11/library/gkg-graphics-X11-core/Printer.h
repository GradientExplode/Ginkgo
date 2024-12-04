#ifndef _gkg_graphics_X11_core_Printer_h_
#define _gkg_graphics_X11_core_Printer_h_


#include <string>
#include <iostream>
#include <gkg-graphics-X11-core/Canvas.h>


namespace gkg
{


class PrinterImplementation;


// generate output for a printer
class Printer : public Canvas
{

  public:

    Printer( std::ostream* out, const std::string& type = "postscript" );
    virtual ~Printer();

    virtual PixelCoordinate
            toPixelCoordinate( FloatCoordinate floatCoordinate ) const;
    virtual FloatCoordinate
            toFloatCoordinate( PixelCoordinate pixelCoordinate ) const;
    virtual FloatCoordinate
            toPixelThenFloatCoordinate( FloatCoordinate floatCoordinate ) const;

    virtual void resize( FloatCoordinate left,
                         FloatCoordinate bottom,
                         FloatCoordinate right,
                         FloatCoordinate top );

    virtual void prolog( const std::string& creator = "gkg" );
    virtual void epilog();

    virtual void setComment( const std::string& comment );
    virtual void setPage( const std::string& label );

    virtual void newPath();
    virtual void moveTo( FloatCoordinate x, FloatCoordinate y );
    virtual void lineTo( FloatCoordinate x, FloatCoordinate y );
    virtual void curveTo( FloatCoordinate x, FloatCoordinate y,
                          FloatCoordinate x1, FloatCoordinate y1,
                          FloatCoordinate x2, FloatCoordinate y2 );
    virtual void closePath();
    virtual void stroke( const Color* color, const Brush* brush );

    virtual void fill( const Color* color );
    virtual void character( const Font* font, int32_t ch, FloatCoordinate width,
                            const Color* color,
                            FloatCoordinate x, FloatCoordinate y );
    virtual void stencil( const Bitmap* bitmap, const Color* color,
                          FloatCoordinate x, FloatCoordinate y );
    virtual void image( const Raster* raster,
                        FloatCoordinate x, FloatCoordinate y );

    virtual void pushTransform();
    virtual void transform( const Transformer& transformer );
    virtual void popTransform();

    virtual void pushClipping();
    virtual void clip();
    virtual void popClipping();

    PrinterImplementation* getImplementation() const;

  protected:

    virtual void flush();

  private:

    PrinterImplementation* _printerImplementation;

};


}


#endif
