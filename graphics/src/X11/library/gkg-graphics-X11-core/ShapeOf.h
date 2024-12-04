#ifndef _gkg_graphics_X11_core_ShapeOf_h_
#define _gkg_graphics_X11_core_ShapeOf_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class ShapeOf : public Glyph
{

  public:

    ShapeOf( Glyph* x, Glyph* y);
    virtual ~ShapeOf();

    virtual void request( GeometryRequisition& geometryRequisition ) const;

  private:

    Glyph* _x;
    Glyph* _y;

};


}


#endif
