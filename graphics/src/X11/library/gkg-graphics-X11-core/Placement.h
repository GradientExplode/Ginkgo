#ifndef _gkg_graphics_X11_core_Placement_h_
#define _gkg_graphics_X11_core_Placement_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


class Layout;


// monoglyph that positions its body using a layout
class Placement : public MonoGlyph
{

  public:

    Placement( Glyph* body, Layout* layout );
    virtual ~Placement();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
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

  private:

    Layout* _layout;

};




}


#endif
