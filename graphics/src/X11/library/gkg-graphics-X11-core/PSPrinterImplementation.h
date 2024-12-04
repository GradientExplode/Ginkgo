#ifndef _gkg_graphics_X11_core_PSPrinterImplementation_h_
#define _gkg_graphics_X11_core_PSPrinterImplementation_h_


#include <gkg-graphics-X11-core/PrinterImplementation.h>


namespace gkg
{


class PSPrinterImplementation : public PrinterImplementation
{

  public:

    PSPrinterImplementation( Printer* printer, std::ostream* out );
    virtual ~PSPrinterImplementation();

    PixelCoordinate toPixelCoordinate( FloatCoordinate floatCoordinate ) const;
    FloatCoordinate toFloatCoordinate( PixelCoordinate pixelCoordinate ) const;
    FloatCoordinate toPixelThenFloatCoordinate(
                                       FloatCoordinate floatCoordinate ) const;

    virtual void resize( FloatCoordinate left,
                         FloatCoordinate bottom,
                         FloatCoordinate right,
                         FloatCoordinate top );

    void prolog( const std::string& creator );
    void epilog();

    void setComment( const std::string& comment );
    void setPage( const std::string& label );

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
                    const Color* color,
                    FloatCoordinate x, FloatCoordinate y );
    void stencil( const Bitmap* bitmap, const Color* color,
                  FloatCoordinate x, FloatCoordinate y );
    void image( const Raster* raster, FloatCoordinate x, FloatCoordinate y );

    void pushTransform();
    void transform( const Transformer& transformer );
    void popTransform();

    void pushClipping();
    void clip();
    void popClipping();

    void flush();

    static PSPrinterImplementation* getImplementation( Printer* printer );

  private:

    void doColor( const Color* color );
    void doBrush( const Brush* brush );
    void doFont( const Font* font );

};


}


#endif
