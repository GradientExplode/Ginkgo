#ifndef _gkg_graphics_X11_core_Glue_h_
#define _gkg_graphics_X11_core_Glue_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


// filler glyph
class Glue : public Glyph
{

  public:

    Glue( DimensionName dimensionName,
          FloatCoordinate natural,
          FloatCoordinate stretch,
          FloatCoordinate shrink,
          float alignment );
    Glue( const GeometryRequisition& geometryRequisition );
    virtual ~Glue();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );

  private:

    GeometryRequisition _geometryRequisition;

};


}


#endif
