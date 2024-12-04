#include <gkg-graphics-X11-core/TileReversedFirstAligned.h>
#include <algorithm>


gkg::TileReversedFirstAligned::TileReversedFirstAligned(
                                              gkg::DimensionName dimensionName )
                              : gkg::Layout(),
                                _dimensionName( dimensionName )
{
}


gkg::TileReversedFirstAligned::~TileReversedFirstAligned()
{
}


void gkg::TileReversedFirstAligned::request(
                           gkg::Glyph::Index count,
                           const gkg::GeometryRequisition* geometryRequisitions,
                           gkg::GeometryRequisition& result )
{

  gkg::FloatCoordinate naturalLead = 0;
  gkg::FloatCoordinate minLead = 0;
  gkg::FloatCoordinate maxLead = 0;
  gkg::FloatCoordinate naturalTrail = 0;
  gkg::FloatCoordinate minTrail = 0;
  gkg::FloatCoordinate maxTrail = 0;
  for ( int32_t index = 0; index < count; index++ )
  {

    const gkg::GeometryRequirement& 
      geometryRequirement = geometryRequisitions[ index ].getRequirement(
                                                          _dimensionName );
    if ( geometryRequirement.isDefined() )
    {

      if ( index == 0 )
      {

        naturalLead =
               ( gkg::FloatCoordinate )( geometryRequirement.getNatural() *
                                         geometryRequirement.getAlignment() );
        maxLead =
               ( gkg::FloatCoordinate )( ( geometryRequirement.getNatural() +
                                           geometryRequirement.getStretch() ) *
                                           geometryRequirement.getAlignment() );
        minLead =
               ( gkg::FloatCoordinate )( ( geometryRequirement.getNatural() -
                                           geometryRequirement.getShrink() ) *
                                           geometryRequirement.getAlignment() );
        naturalTrail =
               ( gkg::FloatCoordinate )( geometryRequirement.getNatural()*
                                   ( 1 - geometryRequirement.getAlignment() ) );
        maxTrail =
               ( gkg::FloatCoordinate )( ( geometryRequirement.getNatural() +
                                           geometryRequirement.getStretch() )*
                                   ( 1 - geometryRequirement.getAlignment() ) );
        minTrail =
               ( gkg::FloatCoordinate )( ( geometryRequirement.getNatural() -
                                           geometryRequirement.getShrink() ) *
                                   ( 1 - geometryRequirement.getAlignment() ) );

      }
      else
      {

        naturalLead += geometryRequirement.getNatural();
        maxLead += geometryRequirement.getNatural() +
                   geometryRequirement.getStretch();
        minLead += geometryRequirement.getNatural() -
                   geometryRequirement.getShrink();

      }

    }

  }
  gkg::GeometryRequirement nr( naturalLead, maxLead, minLead,
                               naturalTrail, maxTrail, minTrail );
  result.setRequirement( _dimensionName, nr );
  _geometryRequisition = result;

}


void gkg::TileReversedFirstAligned::allocate(
                           const gkg::GeometryAllocation& given,
                           gkg::Glyph::Index count,
                           const gkg::GeometryRequisition* geometryRequisitions,
	                   gkg::GeometryAllocation* result )
{

  const gkg::GeometryAllotment&
    geometryAllotment = given.getAllotment( _dimensionName );
  gkg::GeometryRequirement&
    geometryRequirement = _geometryRequisition.getRequirement( _dimensionName );
  gkg::FloatCoordinate span = geometryAllotment.getSpan();
  if ( geometryRequirement.getAlignment() == 0 )
  {

    span = ( gkg::FloatCoordinate )( (float )span * ( 1 - 
                                           geometryAllotment.getAlignment() ) );

  }
  else if ( geometryRequirement.getAlignment() == 1 )
  {

    span = ( gkg::FloatCoordinate )( (float )span *
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
  gkg::FloatCoordinate natural = geometryRequirement.getNatural();
  bool growing = span > natural;
  bool shrinking = span < natural;
  float f;
  if ( growing && geometryRequirement.getStretch() > 0 )
  {

    f = ( float )( span - natural ) / ( float )geometryRequirement.getStretch();

  }
  else if ( shrinking && geometryRequirement.getShrink() > 0 )
  {

    f = ( float )( natural - span ) / ( float )geometryRequirement.getShrink();

  }
  else
  {

    f = 0;

  }
  gkg::FloatCoordinate p = geometryAllotment.getOrigin();
  for ( int32_t index = 0; index < count; index++ )
  {

    const gkg::GeometryRequirement&
      r = geometryRequisitions[ index ].getRequirement( _dimensionName );
    gkg::GeometryAllotment a;
    if ( r.isDefined() )
    {

      gkg::FloatCoordinate cspan = r.getNatural();
      if ( growing )
      {

        cspan += ( gkg::FloatCoordinate )( ( float )r.getStretch() * f );

      }
      else if ( shrinking )
      {

        cspan -= ( gkg::FloatCoordinate )( ( float )r.getShrink() * f );

      }
      if ( index == 0 )
      {

        p -= ( gkg::FloatCoordinate )( r.getAlignment() * cspan );

      }
      a.setSpan( cspan );
      a.setOrigin( p + ( gkg::FloatCoordinate )( r.getAlignment() * cspan ) );
      a.setAlignment( r.getAlignment() );
      p += cspan;

    }
    else
    {

      a.setSpan( 0 );
      a.setOrigin( p );
      a.setAlignment( 0 );

    }
    result[ index ].setAllotment( _dimensionName, a );

  }

}
