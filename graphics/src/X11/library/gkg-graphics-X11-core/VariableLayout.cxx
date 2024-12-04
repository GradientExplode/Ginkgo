#include <gkg-graphics-X11-core/VariableLayout.h>
#include <algorithm>


gkg::VariableLayout::VariableLayout( const gkg::DimensionName dimensionName,
                                     gkg::FloatCoordinate stretch,
                                     gkg::FloatCoordinate shrink )
                    : gkg::Layout(),
                      _dimensionName( dimensionName ),
                      _stretch( stretch ),
                      _shrink( shrink )
{
}


gkg::VariableLayout::~VariableLayout()
{
}


void gkg::VariableLayout::request( gkg::Glyph::Index,
                                   const gkg::GeometryRequisition*,
                                   gkg::GeometryRequisition& result )
{

  gkg::GeometryRequirement&
    geometryRequirement = result.getRequirement( _dimensionName );
  geometryRequirement.setStretch( _stretch );
  geometryRequirement.setShrink( std::min( _shrink,
                                           geometryRequirement.getNatural() ) );

}


void gkg::VariableLayout::allocate( const gkg::GeometryAllocation&,
                                    gkg::Glyph::Index,
                                    const gkg::GeometryRequisition*,
                                    gkg::GeometryAllocation* )
{

  // leave it as is

}

