#ifndef _gkg_graphics_X11_core_ArrowDown_h_
#define _gkg_graphics_X11_core_ArrowDown_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Color;


class ArrowDown : public Glyph
{

  public:

    ArrowDown( const Color* color );
    virtual ~ArrowDown();

    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;

  private:

    const Color* _color;

};


}


#endif
