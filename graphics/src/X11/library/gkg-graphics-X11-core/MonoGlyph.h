#ifndef _gkg_graphics_X11_core_MonoGlyph_h_
#define _gkg_graphics_X11_core_MonoGlyph_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class MonoGlyph : public Glyph
{

  public:

    virtual ~MonoGlyph();

    virtual void setBody( Glyph* glyph );
    virtual Glyph* getBody() const;

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

    virtual void append( Glyph* other );
    virtual void prepend( Glyph* other );
    virtual void insert( Glyph::Index index, Glyph* other );
    virtual void remove( Glyph::Index index );
    virtual void replace( Glyph::Index index, Glyph* other );
    virtual void change( Glyph::Index index );

    virtual Glyph::Index getCount() const;
    virtual Glyph* getComponent( Glyph::Index index ) const;
    virtual void getGeometryAllotment(
                                   Glyph::Index index,
                                   DimensionName dimensionName,
                                   GeometryAllotment& geometryAllotment ) const;

  protected:

    MonoGlyph( Glyph* glyph = 0 );

  private:

    Glyph* _body;

};


}


#endif
