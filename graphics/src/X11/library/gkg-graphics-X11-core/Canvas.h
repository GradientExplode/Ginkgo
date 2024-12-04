#ifndef _gkg_graphics_X11_core_Canvas_h_
#define _gkg_graphics_X11_core_Canvas_h_


#include <gkg-graphics-X11-base/Coordinates.h>
#include <string>


namespace gkg
{


class Window;
class Color;
class Brush;
class Font;
class Bitmap;
class Raster;
class Transformer;
class GeometryExtension;
class CanvasImplementation;


class Canvas
{

  public:

    Canvas( Window* window = 0 );
    virtual ~Canvas();

    virtual Window* getWindow() const;

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

    virtual void newPath();
    virtual void moveTo( FloatCoordinate x, FloatCoordinate y );
    virtual void lineTo( FloatCoordinate x, FloatCoordinate y );
    virtual void curveTo( FloatCoordinate x, FloatCoordinate y,
                          FloatCoordinate x1, FloatCoordinate y1,
                          FloatCoordinate x2, FloatCoordinate y2 );
    virtual void closePath();
    virtual void stroke( const Color* color, const Brush* brush );

    virtual void line( FloatCoordinate x1, FloatCoordinate y1,
                       FloatCoordinate x2, FloatCoordinate y2,
                       const Color* color, const Brush* brush );
    virtual void rectangle( FloatCoordinate left, FloatCoordinate bottom,
                            FloatCoordinate right, FloatCoordinate top,
                            const Color* color, const Brush* brush );

    virtual void fill( const Color* color );
    virtual void fillRectangle( FloatCoordinate left, FloatCoordinate bottom,
                                FloatCoordinate right, FloatCoordinate top,
                                const Color* color );
    virtual void character( const Font* font, int32_t ch, FloatCoordinate width,
                            const Color* color,
                            FloatCoordinate x, FloatCoordinate y );
    virtual void string( const Font* font, const std::string& s,
                         const Color* color,
                         FloatCoordinate x, FloatCoordinate y );
    virtual void stencil( const Bitmap* bitmap, const Color* color,
                          FloatCoordinate x, FloatCoordinate y );
    virtual void image( const Raster* raster,
                        FloatCoordinate x, FloatCoordinate y );

    virtual void pushTransform();
    virtual void transform( const Transformer& transformer );
    virtual void popTransform();

    virtual void setTransformer( const Transformer& transformer );
    virtual const Transformer& getTransformer() const;

    virtual void pushClipping();
    virtual void clip();
    virtual void clipRectangle( FloatCoordinate left, FloatCoordinate bottom,
                                FloatCoordinate right, FloatCoordinate top );
    virtual void popClipping();

    virtual void frontBuffer();
    virtual void backBuffer();

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

    virtual void redraw( FloatCoordinate left, FloatCoordinate bottom,
                         FloatCoordinate right, FloatCoordinate top );
    virtual void repair();

    CanvasImplementation* getImplementation() const;

  private:

    CanvasImplementation* _canvasImplementation;

};


}


#endif
