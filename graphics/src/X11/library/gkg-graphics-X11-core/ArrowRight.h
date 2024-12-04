#ifndef _gkg_graphics_X11_core_ArrowRight_h_
#define _gkg_graphics_X11_core_ArrowRight_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Color;


class ArrowRight : public Glyph
{

  public:

    ArrowRight( const Color* color );
    virtual ~ArrowRight();

    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;

  private:

    const Color* _color;

};


}


#endif
