#ifndef _gkg_graphics_X11_core_LayoutLayer_h_
#define _gkg_graphics_X11_core_LayoutLayer_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


class LayoutLayer : public MonoGlyph
{

  public:

    LayoutLayer( Glyph* between, Glyph* under, Glyph* over );
    virtual ~LayoutLayer();

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void print( Printer* printer,
                        const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );
    virtual void undraw();

  private:

    Glyph* _under;
    Glyph* _over;

};


}


#endif
