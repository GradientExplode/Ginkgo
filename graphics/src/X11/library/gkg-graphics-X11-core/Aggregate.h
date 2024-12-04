#ifndef _gkg_graphics_X11_core_Aggregate_h_
#define _gkg_graphics_X11_core_Aggregate_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class AggregateImplementation;


class Aggregate : public Glyph
{

  public:

    Aggregate( Glyph::Index size = 4 );
    virtual ~Aggregate();

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void print( Printer* printer,
                        const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth, Hit& hit );

    virtual void append( Glyph* other );
    virtual void prepend( Glyph* other );
    virtual void insert( Glyph::Index index, Glyph* other );
    virtual void remove( Glyph::Index index );
    virtual void replace( Glyph::Index index, Glyph* other );
    virtual void change( Glyph::Index index );

    virtual Glyph::Index getCount() const;
    virtual Glyph* getComponent( Glyph::Index index ) const;
    virtual void setAllotment( Glyph::Index index, DimensionName d,
                               const GeometryAllotment& geometryAllotment );
    virtual void getGeometryAllotment( Glyph::Index index,
                                       DimensionName dimensionName,
                                   GeometryAllotment& geometryAllotment ) const;

    AggregateImplementation* getImplementation() const;

  private:

    AggregateImplementation* _aggregateImplementation;

};


}


#endif
