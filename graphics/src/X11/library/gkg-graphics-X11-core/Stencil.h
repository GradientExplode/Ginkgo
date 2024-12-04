#ifndef _gkg_graphics_X11_core_Stencil_h_
#define _gkg_graphics_X11_core_Stencil_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Bitmap;
class Color;


// mask Glyph
class Stencil : public Glyph
{

  public:

    Stencil( const Bitmap* mask, const Color* color );
    virtual ~Stencil();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;

  private:

    const Bitmap* _mask;
    const Color* _color;

};


}


#endif
