#include <gkg-graphics-X11-core/NaturalLayout.h>


gkg::NaturalLayout::NaturalLayout( const gkg::DimensionName dimensionName,
                                   gkg::FloatCoordinate natural )
                   : gkg::Layout(),
                     _dimensionName( dimensionName ),
                     _natural( natural )
{
}


gkg::NaturalLayout::~NaturalLayout()
{
}


void gkg::NaturalLayout::request( gkg::Glyph::Index,
                                  const gkg::GeometryRequisition*,
                                  gkg::GeometryRequisition& result )
{

  gkg::GeometryRequirement&
    geometryRequirement = result.getRequirement( _dimensionName );
  geometryRequirement.setNatural( _natural );

}


void gkg::NaturalLayout::allocate( const gkg::GeometryAllocation&,
                                   gkg::Glyph::Index,
                                   const gkg::GeometryRequisition*,
                                   gkg::GeometryAllocation* )
{

  // leave it as is

}

