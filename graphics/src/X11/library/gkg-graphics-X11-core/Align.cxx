#include <gkg-graphics-X11-core/Align.h>
#include <algorithm>


gkg::Align::Align( gkg::DimensionName dimensionName )
           : gkg::Layout(),
             _dimensionName( dimensionName )
{
}


gkg::Align::~Align()
{
}


void gkg::Align::request( gkg::Glyph::Index count,
                          const gkg::GeometryRequisition* geometryRequisitions,
                          gkg::GeometryRequisition& result )
{

  gkg::FloatCoordinate naturalLead = 0;
  gkg::FloatCoordinate minLead = -fil;
  gkg::FloatCoordinate maxLead = fil;
  gkg::FloatCoordinate naturalTrail = 0;
  gkg::FloatCoordinate minTrail = -fil;
  gkg::FloatCoordinate maxTrail = fil;
  for ( int32_t index = 0; index < count; index++ )
  {

    const gkg::GeometryRequirement&
      geometryRequirement = geometryRequisitions[ index ].getRequirement(
                                                          _dimensionName );
    if ( geometryRequirement.isDefined() )
    {

      gkg::FloatCoordinate rNat = geometryRequirement.getNatural();
      gkg::FloatCoordinate rMax = rNat + geometryRequirement.getStretch();
      gkg::FloatCoordinate rMin = rNat - geometryRequirement.getShrink();
      gkg::FloatCoordinate rAlign = geometryRequirement.getAlignment();
      gkg::FloatCoordinate rInvAlign = 1.0 - rAlign;
      naturalLead = std::max( naturalLead, 
                              ( gkg::FloatCoordinate )( rNat * rAlign ) );
      maxLead = std::min( maxLead,
                          ( gkg::FloatCoordinate )( rMax * rAlign ) );
      minLead = std::max( minLead,
                          ( gkg::FloatCoordinate )( rMin * rAlign ) );
      naturalTrail = std::max( naturalTrail,
                               ( gkg::FloatCoordinate )( rNat * rInvAlign ) );
      maxTrail = std::min( maxTrail,
                           ( gkg::FloatCoordinate )( rMax * rInvAlign ) );
      minTrail = std::max( minTrail,
                           ( gkg::FloatCoordinate )( rMin * rInvAlign ) );

    }

  }

  gkg::GeometryRequirement r( naturalLead, maxLead, minLead,
                              naturalTrail, maxTrail, minTrail );
  result.setRequirement( _dimensionName, r );

}


void gkg::Align::allocate( const gkg::GeometryAllocation& given,
                           gkg::Glyph::Index count,
                           const gkg::GeometryRequisition* geometryRequisitions,
	                   gkg::GeometryAllocation* result )
{

  const gkg::GeometryAllotment&
    geometryAllotment = given.getAllotment( _dimensionName );
  for ( int32_t index = 0; index < count; index++ )
  {

    const gkg::GeometryRequirement&
      geometryRequirement = geometryRequisitions[ index ].getRequirement(
                                                          _dimensionName );
    if ( geometryRequirement.isDefined() )
    {

      gkg::FloatCoordinate span = geometryAllotment.getSpan();
      if ( geometryRequirement.getAlignment() == 0 )
      {

        span = ( gkg::FloatCoordinate )( ( float )span *
                                     ( 1 - geometryAllotment.getAlignment() ) );

      }
      else if ( geometryRequirement.getAlignment() == 1 )
      {

        span = ( gkg::FloatCoordinate )( ( float )span *
                                         geometryAllotment.getAlignment() );

      }
      else
      {

        span = ( gkg::FloatCoordinate )( ( float )span * std::min(
                                       geometryAllotment.getAlignment() /
                                       geometryRequirement.getAlignment(),
                            ( 1 - geometryAllotment.getAlignment() ) /
                            ( 1 - geometryRequirement.getAlignment() ) ) );

      }
      span = std::min( span, geometryRequirement.getNatural() +
                             geometryRequirement.getStretch() );
      span = std::max( span, geometryRequirement.getNatural() -
                             geometryRequirement.getShrink() );
      gkg::GeometryAllotment a( geometryAllotment.getOrigin(), span,
                                geometryRequirement.getAlignment() );
      result[ index ].setAllotment( _dimensionName, a );

    } 
    else
    {

      result[ index ].setAllotment( _dimensionName, geometryAllotment );

    }

  }

}
