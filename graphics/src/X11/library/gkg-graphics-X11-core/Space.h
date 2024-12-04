#ifndef _gkg_graphics_X11_core_Space_h_
#define _gkg_graphics_X11_core_Space_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Font;
class Color;


class Space : public Glyph
{

  public:

    Space( int32_t count,
           FloatCoordinate each,
           const Font* font,
           const Color* color );
    virtual ~Space();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );

  private:

    int32_t _count;
    FloatCoordinate _each;
    const Font* _font;
    const Color* _color;
    FloatCoordinate _width;
    FloatCoordinate _height;
    float _alignment;

};


}


#endif
