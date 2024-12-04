#include <gkg-graphics-X11-core/Glue.h>


gkg::Glue::Glue( gkg::DimensionName dimensionName,
                    gkg::FloatCoordinate natural,
                    gkg::FloatCoordinate stretch,
                    gkg::FloatCoordinate shrink,
                    float alignment )
          : gkg::Glyph()
{

  gkg::GeometryRequirement
    geometryRequirement( natural, stretch, shrink, alignment );
  _geometryRequisition.setRequirement( dimensionName, geometryRequirement );

}


gkg::Glue::Glue( const gkg::GeometryRequisition& geometryRequisition )
          : Glyph(),
            _geometryRequisition( geometryRequisition )
{
}


gkg::Glue::~Glue()
{
}


void 
gkg::Glue::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  geometryRequisition = _geometryRequisition;

}


void gkg::Glue::allocate( gkg::Canvas* canvas,
                          const gkg::GeometryAllocation& geometryAllocation,
                          gkg::GeometryExtension& geometryExtension )
{

  geometryExtension.merge( canvas, geometryAllocation );

}

