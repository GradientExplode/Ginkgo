#include <gkg-graphics-X11-core/Tile.h>
#include <algorithm>


static 
void computeRequest( gkg::DimensionName d,
                     float alignment, gkg::Glyph::Index count,
                     const gkg::GeometryRequisition* geometryRequisitions,
                     gkg::GeometryRequisition& result )
{

  gkg::FloatCoordinate natural = 0, minSize = 0, maxSize = 0;
  for ( gkg::Glyph::Index i = 0; i < count; i++ )
  {

    const gkg::GeometryRequirement&
      geometryRequirement = geometryRequisitions[ i ].getRequirement( d );
    if ( geometryRequirement.isDefined() )
    {

      gkg::FloatCoordinate n = geometryRequirement.getNatural();
      natural += n;
      maxSize += n + geometryRequirement.getStretch();
      minSize += n - geometryRequirement.getShrink();

    }

  }
  gkg::GeometryRequirement& nr = result.getRequirement( d );
  nr.setNatural( natural );
  nr.setStretch( maxSize - natural );
  nr.setShrink( natural - minSize );
  nr.setAlignment( alignment );

}


gkg::Tile::Tile( gkg::DimensionName dimensionName )
          : gkg::Layout(),
            _dimensionName( dimensionName )
{
}


gkg::Tile::~Tile()
{
}


void gkg::Tile::request( gkg::Glyph::Index count,
                         const gkg::GeometryRequisition* geometryRequisitions,
                         gkg::GeometryRequisition& result )
{

  computeRequest( _dimensionName, 0.0, count, geometryRequisitions, result );
  _geometryRequisition = result;

}


void gkg::Tile::allocate( const gkg::GeometryAllocation& given,
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
  bool growing = ( span > natural );
  bool shrinking = ( span < natural );
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
  gkg::FloatCoordinate cumul = 0.0f;
  for ( int32_t index = 0; index < count; index++ )
  {

    const gkg::GeometryRequirement&
      r = geometryRequisitions[ index ].getRequirement( _dimensionName );
    gkg::GeometryAllotment&
      a = result[ index ].getAllotment( _dimensionName );
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
      if ( ( cumul + cspan ) < span )
      {

        a.setSpan( cspan );
        a.setOrigin( p + ( gkg::FloatCoordinate )( r.getAlignment() * cspan ) );
        a.setAlignment( r.getAlignment() );
        cumul += cspan;

      }
      else
      {

        a.setSpan( span - cumul );
        a.setOrigin( p + ( gkg::FloatCoordinate )( r.getAlignment() * cspan ) );
        a.setAlignment( r.getAlignment() );
        cumul = span;

      }
      p += cspan;

    }
    else
    {

      a.setSpan( 0 );
      a.setOrigin( p );
      a.setAlignment( 0 );

    }

  }

}
