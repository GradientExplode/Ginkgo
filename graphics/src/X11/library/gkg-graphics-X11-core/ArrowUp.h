#ifndef _gkg_graphics_X11_core_ArrowUp_h_
#define _gkg_graphics_X11_core_ArrowUp_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Color;


class ArrowUp : public Glyph
{

  public:

    ArrowUp( const Color* color );
    virtual ~ArrowUp();

    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;

  private:

    const Color* _color;

};


}


#endif
