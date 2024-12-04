#ifndef _gkg_graphics_X11_core_Border_h_
#define _gkg_graphics_X11_core_Border_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


class Color;


// a glyph for framing
class Border : public MonoGlyph
{

  public:

    Border( Glyph* body, const Color* color );
    Border( Glyph* body, const Color* color, FloatCoordinate thickness );
    virtual ~Border();

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas, 
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void print( Printer* printer, 
                        const GeometryAllocation& geometryAllocation ) const;

  private:

    const Color* _color;
    FloatCoordinate _thickness;

};


}


#endif
