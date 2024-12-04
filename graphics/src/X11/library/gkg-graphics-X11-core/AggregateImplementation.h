#ifndef _gkg_graphics_X11_core_AggregateImplementation_h_
#define _gkg_graphics_X11_core_AggregateImplementation_h_


#include <deque>
#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Aggregate;


class AggregateImplementation
{

  public:


    struct AggregateInfo
    {

      AggregateInfo();

      Glyph* glyph;
      GeometryAllocation geometryAllocation;
      GeometryExtension geometryExtension;

    };

    AggregateImplementation( Aggregate* aggregate, Glyph::Index size );
    virtual ~AggregateImplementation();

    Aggregate* getTarget() const;

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

    virtual Glyph::Index getCount() const;
    virtual Glyph* getComponent( Glyph::Index index ) const;
    virtual void setAllotment( Glyph::Index index, DimensionName dimensionName,
                               const GeometryAllotment& geometryAllotment );
    virtual void getGeometryAllotment(
                                   Glyph::Index index,
                                   DimensionName dimensionName,
                                   GeometryAllotment& geometryAllotment ) const;

  private:

    Aggregate* _target;
    std::deque< AggregateInfo > _informations;

};


}


#endif
