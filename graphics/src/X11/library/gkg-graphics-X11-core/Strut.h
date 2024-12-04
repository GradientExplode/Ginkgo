#ifndef _gkg_graphics_X11_core_Strut_h_
#define _gkg_graphics_X11_core_Strut_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Font;


class Strut : public Glyph
{

  public:

    Strut( const Font* font,
           FloatCoordinate natural = 0,
           FloatCoordinate stretch = 0,
           FloatCoordinate shrink = 0 );
    virtual ~Strut();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );

  private:

    const Font* _font;
    FloatCoordinate _natural;
    FloatCoordinate _stretch;
    FloatCoordinate _shrink;
    FloatCoordinate _height;
    float _alignment;

};


}


#endif
