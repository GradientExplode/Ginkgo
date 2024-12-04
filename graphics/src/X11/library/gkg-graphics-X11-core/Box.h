#ifndef _gkg_graphics_X11_core_Box_h_
#define _gkg_graphics_X11_core_Box_h_


#include <gkg-graphics-X11-core/PolyGlyph.h>


namespace gkg
{


class Layout;
class BoxImplementation;


// tiling glyph
class Box : public PolyGlyph
{

  public:

    Box( Layout* layout,
         Glyph* g1 = 0, Glyph* g2 = 0, Glyph* g3 = 0, Glyph* g4 = 0,
         Glyph* g5 = 0, Glyph* g6 = 0, Glyph* g7 = 0, Glyph* g8 = 0,
         Glyph* g9 = 0, Glyph* g10 = 0 );
    virtual ~Box();

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
    virtual void undraw();

    virtual void modified( Glyph::Index index );

    virtual void getGeometryAllotment(
                                   Glyph::Index index,
                                   DimensionName dimensionName,
                                   GeometryAllotment& geometryAllotment ) const;

    BoxImplementation* getImplementation() const;

  private:

    BoxImplementation* _boxImplementation;

};


}


#endif
