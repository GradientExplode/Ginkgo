#ifndef _gkg_graphics_X11_core_ArrowLeft_h_
#define _gkg_graphics_X11_core_ArrowLeft_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Color;


class ArrowLeft : public Glyph
{

  public:

    ArrowLeft( const Color* color );
    virtual ~ArrowLeft();

    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;

  private:

    const Color* _color;

};


}


#endif
