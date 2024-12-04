#ifndef _gkg_graphics_X11_core_Background_h_
#define _gkg_graphics_X11_core_Background_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


class Color;


// opaque glyph
class Background : public MonoGlyph
{

  public:

    Background( Glyph* body, const Color* color );
    virtual ~Background();

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas, 
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void print( Printer* printer, 
                        const GeometryAllocation& geometryAllocation ) const;

  private:

    const Color* _color;

};


}


#endif
