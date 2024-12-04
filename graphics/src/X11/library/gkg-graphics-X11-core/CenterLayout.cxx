#include <gkg-graphics-X11-core/CenterLayout.h>


gkg::CenterLayout::CenterLayout( const gkg::DimensionName dimensionName,
                                float alignment )
                  : gkg::Layout(),
                    _dimensionName( dimensionName ),
                    _alignment( alignment )
{
}


gkg::CenterLayout::~CenterLayout()
{
}


void gkg::CenterLayout::request( gkg::Glyph::Index,
                                 const gkg::GeometryRequisition*,
                                 gkg::GeometryRequisition& result )
{

  result.getRequirement( _dimensionName ).setAlignment( _alignment );

}


void gkg::CenterLayout::allocate(
                           const gkg::GeometryAllocation&,
                           gkg::Glyph::Index,
                           const gkg::GeometryRequisition* geometryRequisitions,
                           gkg::GeometryAllocation* result )
{

  gkg::GeometryAllotment&
    geometryAllotment = result[ 0 ].getAllotment( _dimensionName );
  float calign = 
    geometryRequisitions[ 0 ].getRequirement( _dimensionName ).getAlignment();
  geometryAllotment.addOffset( ( gkg::FloatCoordinate )(
                              ( float )geometryAllotment.getSpan() *
                              ( calign - geometryAllotment.getAlignment() ) ) );
  geometryAllotment.setAlignment( calign );

}

