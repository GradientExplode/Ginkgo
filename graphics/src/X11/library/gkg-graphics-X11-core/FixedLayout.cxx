#include <gkg-graphics-X11-core/FixedLayout.h>


gkg::FixedLayout::FixedLayout( const gkg::DimensionName dimensionName,
                               gkg::FloatCoordinate span )
                 : gkg::Layout(),
                   _dimensionName( dimensionName ),
                   _span( span )
{
}


gkg::FixedLayout::~FixedLayout()
{
}


void gkg::FixedLayout::request( gkg::Glyph::Index,
                                const gkg::GeometryRequisition*,
                                gkg::GeometryRequisition& result )
{

  gkg::GeometryRequirement&
    geometryRequirement = result.getRequirement( _dimensionName );
  geometryRequirement.setNatural( _span );
  geometryRequirement.setStretch( 0.0 );
  geometryRequirement.setShrink( 0.0 );

}


void gkg::FixedLayout::allocate( const gkg::GeometryAllocation&,
                                 gkg::Glyph::Index,
                                 const gkg::GeometryRequisition*,
                                 gkg::GeometryAllocation* result )
{

  gkg::GeometryAllotment&
    geometryAllotment = result[ 0 ].getAllotment( _dimensionName );
  geometryAllotment.setSpan( _span );

}

