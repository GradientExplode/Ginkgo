#ifndef _gkg_graphics_X11_core_Deck_h_
#define _gkg_graphics_X11_core_Deck_h_


#include <gkg-graphics-X11-core/PolyGlyph.h>


namespace gkg
{


class Deck : public PolyGlyph
{

  public:

    Deck();
    virtual ~Deck();

    Glyph::Index getCard() const;
    void flipTo( Glyph::Index index );

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

  private:

    Glyph::Index _card;
    bool _changed;
    GeometryRequisition _geometryRequisition;
    GeometryAllocation _geometryAllocation;

};


}


#endif
