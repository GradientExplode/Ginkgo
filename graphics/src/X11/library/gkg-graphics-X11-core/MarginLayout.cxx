#include <gkg-graphics-X11-core/MarginLayout.h>


gkg::MarginLayout::MarginLayout( gkg::FloatCoordinate margin )
                  : gkg::Layout()
{

  _lNatural = margin; _lStretch = 0; _lShrink = 0;
  _rNatural = margin; _rStretch = 0; _rShrink = 0;
  _bNatural = margin; _bStretch = 0; _bShrink = 0;
  _tNatural = margin; _tStretch = 0; _tShrink = 0;

}


gkg::MarginLayout::MarginLayout( gkg::FloatCoordinate hMargin,
                                 gkg::FloatCoordinate vMargin )
                  : gkg::Layout()
{

  _lNatural = hMargin; _lStretch = 0; _lShrink = 0;
  _rNatural = hMargin; _rStretch = 0; _rShrink = 0;
  _bNatural = vMargin; _bStretch = 0; _bShrink = 0;
  _tNatural = vMargin; _tStretch = 0; _tShrink = 0;

}


gkg::MarginLayout::MarginLayout( gkg::FloatCoordinate lMargin,
                                 gkg::FloatCoordinate rMargin,
                                 gkg::FloatCoordinate bMargin,
                                 gkg::FloatCoordinate tMargin )
                  : gkg::Layout()
{

  _lNatural = lMargin; _lStretch = 0; _lShrink = 0;
  _rNatural = rMargin; _rStretch = 0; _rShrink = 0;
  _bNatural = bMargin; _bStretch = 0; _bShrink = 0;
  _tNatural = tMargin; _tStretch = 0; _tShrink = 0;

}


gkg::MarginLayout::MarginLayout( gkg::FloatCoordinate lMargin,
                                 gkg::FloatCoordinate lStretch,
                                 gkg::FloatCoordinate lShrink,
                                 gkg::FloatCoordinate rMargin,
                                 gkg::FloatCoordinate rStretch,
                                 gkg::FloatCoordinate rShrink,
                                 gkg::FloatCoordinate bMargin,
                                 gkg::FloatCoordinate bStretch,
                                 gkg::FloatCoordinate bShrink,
                                 gkg::FloatCoordinate tMargin,
                                 gkg::FloatCoordinate tStretch,
                                 gkg::FloatCoordinate tShrink )
                  : gkg::Layout()
{

  _lNatural = lMargin; _lStretch = lStretch; _lShrink = lShrink;
  _rNatural = rMargin; _rStretch = rStretch; _rShrink = rShrink;
  _bNatural = bMargin; _bStretch = bStretch; _bShrink = bShrink;
  _tNatural = tMargin; _tStretch = tStretch; _tShrink = tShrink;

}


gkg::MarginLayout::~MarginLayout()
{
}


void gkg::MarginLayout::request( gkg::Glyph::Index,
                                 const gkg::GeometryRequisition*,
                                 gkg::GeometryRequisition& result )
{

  _geometryRequisition = result;
  gkg::GeometryRequirement& x = _geometryRequisition.getRequirementX();
  if ( x.isDefined() )
  {

    x.setNatural( x.getNatural() + _lNatural + _rNatural );
    x.setStretch( x.getStretch() + _lStretch + _rStretch );
    x.setShrink( x.getShrink() + _lShrink + _rShrink );

  }
  gkg::GeometryRequirement& y = _geometryRequisition.getRequirementY();
  if ( y.isDefined() )
  {

    y.setNatural( y.getNatural() + _bNatural + _tNatural );
    y.setStretch( y.getStretch() + _bStretch + _tStretch );
    y.setShrink( y.getShrink() + _bShrink + _tShrink );

  }
  result = _geometryRequisition;

}


void gkg::MarginLayout::allocate( const gkg::GeometryAllocation&,
                                  gkg::Glyph::Index,
                                  const gkg::GeometryRequisition*,
                                  gkg::GeometryAllocation* result )
{

  gkg::GeometryAllotment& x = result[ 0 ].getAllotmentX();
  const gkg::GeometryRequirement&
    rx = _geometryRequisition.getRequirementX();
  gkg::FloatCoordinate lMargin = getSpan( x.getSpan(), rx,
                                          _lNatural, _lStretch, _lShrink );
  gkg::FloatCoordinate rMargin = getSpan( x.getSpan(), rx,
                                          _rNatural, _rStretch, _rShrink );
  x.setSpan( x.getSpan() - ( lMargin + rMargin ) );
  x.addOffset( ( gkg::FloatCoordinate )( ( 1 - rx.getAlignment( )) * lMargin -
                                         rx.getAlignment() * rMargin ) );

  gkg::GeometryAllotment& y = result[ 0 ].getAllotmentY();
  const gkg::GeometryRequirement&
    ry = _geometryRequisition.getRequirementY();
  gkg::FloatCoordinate bMargin = getSpan( y.getSpan(), ry,
                                          _bNatural, _bStretch, _bShrink );
  gkg::FloatCoordinate tMargin = getSpan( y.getSpan(), ry,
                                          _tNatural, _tStretch, _tShrink );
  y.setSpan( y.getSpan() - ( bMargin + tMargin ) );
  y.addOffset( ( gkg::FloatCoordinate )( ( 1 - ry.getAlignment( )) * bMargin -
                                         ry.getAlignment() * tMargin ) );

}


gkg::FloatCoordinate gkg::MarginLayout::getSpan(
                                          gkg::FloatCoordinate span,
                                          const gkg::GeometryRequirement& total,
	                                  gkg::FloatCoordinate natural,
                                          gkg::FloatCoordinate stretch,
                                          gkg::FloatCoordinate shrink )
{

  gkg::FloatCoordinate extra = span - total.getNatural();
  gkg::FloatCoordinate result = natural;
  float ss = 0.0;
  gkg::FloatCoordinate totalStretch = total.getStretch();
  gkg::FloatCoordinate totalShrink = total.getShrink();
  if ( ( extra > 0 ) && ( totalStretch > 0 ) )
  {

    ss = stretch / totalStretch;

  }
  else if ( ( extra < 0 ) && ( totalShrink > 0 ) )
  {

    ss = shrink / totalShrink;

  }
  return result + ( gkg::FloatCoordinate )( ss * extra );

}
