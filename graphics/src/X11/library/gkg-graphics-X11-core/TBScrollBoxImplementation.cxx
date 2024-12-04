#include <gkg-graphics-X11-core/TBScrollBoxImplementation.h>
#include <gkg-graphics-X11-core/TBScrollBox.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-processing-algobase/Math.h>


gkg::TBScrollBoxImplementation::TBScrollBoxImplementation(
                                                  gkg::TBScrollBox* tbScrollBox,
                                                  bool shrinkable )
                               : _target( tbScrollBox ),
                                 _shrinkable( shrinkable ),
                                 _start( 0 ),
                                 _end( 0 ),
                                 _changed( true ),
                                 _canvas( 0 )
{

  _partiallyVisible.glyph = 0;

}


gkg::TBScrollBoxImplementation::~TBScrollBoxImplementation()
{
}


gkg::TBScrollBox* gkg::TBScrollBoxImplementation::getTarget() const
{

  return _target;

}


void gkg::TBScrollBoxImplementation::request( 
                                 gkg::GeometryRequisition& geometryRequisition )
{

  gkg::Glyph::Index count = _target->getCount();
  if ( _changed )
  {

    gkg::GeometryRequisition geometryRequisition;
    const gkg::GeometryRequirement&
      rx = geometryRequisition.getRequirementX();
    const gkg::GeometryRequirement&
      ry = geometryRequisition.getRequirementY();
    gkg::FloatCoordinate naturalWidth = 0.0;
    gkg::FloatCoordinate naturalHeight = 0.0;
    for ( gkg::Glyph::Index i = 0; i < count; i++ )
    {

      gkg::Glyph* glyph = _target->getComponent( i );
      if ( glyph )
      {

        glyph->request( geometryRequisition );
        gkg::FloatCoordinate rWidth = rx.getNatural();
        if ( rWidth > naturalWidth )
        {

          naturalWidth = rWidth;

        }
        naturalHeight += ry.getNatural();

      }

    }

    gkg::GeometryRequirement& boxX = _geometryRequisition.getRequirementX();
    boxX.setNatural( naturalWidth );
    boxX.setStretch( fil );
    boxX.setShrink( _shrinkable ? naturalWidth : 0.0 );
    boxX.setAlignment( 0.0 );

    gkg::GeometryRequirement& boxY = _geometryRequisition.getRequirementY();
    boxY.setNatural( naturalHeight );
    boxY.setStretch( fil );
    boxY.setShrink( naturalHeight );
    boxY.setAlignment( 1.0 );
    _changed = false;

  }
  geometryRequisition = _geometryRequisition;

}


void gkg::TBScrollBoxImplementation::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  if ( _changed )
  {

    gkg::GeometryRequisition geometryRequisition;
    _target->request( geometryRequisition );

  }
  geometryExtension.set( canvas, geometryAllocation );
  _canvas = canvas;
  if ( canvas )
  {

    _transformer = canvas->getTransformer();

  }
  _geometryAllocation = geometryAllocation;
  _geometryExtension = geometryExtension;
  reallocate();
  _target->notify( gkg::DimensionX );
  _target->notify( gkg::DimensionY );

}



void gkg::TBScrollBoxImplementation::draw(
                             gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation )
{

  check( canvas, geometryAllocation);
  if ( _canvas->isDamaged( _geometryExtension ) )
  {

    doRefresh();
    canvas->pushClipping();
    canvas->clipRectangle( geometryAllocation.getLeft(),
                           geometryAllocation.getBottom(),
                           geometryAllocation.getRight(),
                           geometryAllocation.getTop() );
    std::deque< gkg::TBScrollBoxImplementation::Info >::iterator
      i = _visible.begin(), ie = _visible.end();
    while ( i != ie )
    {

      const gkg::TBScrollBoxImplementation::Info& info = *i;
      gkg::Glyph* glyph = info.glyph;
      glyph->draw( canvas, info.geometryAllocation );
      ++ i;

    }
    if ( _partiallyVisible.glyph )
    {

      _partiallyVisible.glyph->draw( canvas,
                                     _partiallyVisible.geometryAllocation );

    }
    canvas->popClipping();

  }

}


void gkg::TBScrollBoxImplementation::pick(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              int32_t depth,
                              gkg::Hit& hit )
{

  check( canvas, geometryAllocation );
  if ( ( hit.getLeft() < geometryAllocation.getRight() ) &&
       ( hit.getRight() >= geometryAllocation.getLeft() ) &&
       ( hit.getBottom() < geometryAllocation.getTop() ) &&
       ( hit.getTop() >= geometryAllocation.getBottom() ) )
  {

    doRefresh();

    gkg::Glyph::Index n = _start;

    std::deque< gkg::TBScrollBoxImplementation::Info >::iterator
      i = _visible.begin(), ie = _visible.end();
    while ( i != ie )
    {

      const gkg::TBScrollBoxImplementation::Info& info = *i;
      gkg::Glyph* glyph = info.glyph;
      hit.begin( depth, _target, n );
      glyph->pick( canvas, info.geometryAllocation, depth + 1, hit );
      hit.end();
      ++ i;
      ++ n;

    }
    if ( _partiallyVisible.glyph )
    {

      hit.begin( depth, _target, n );
      _partiallyVisible.glyph->pick( canvas,
                                     _partiallyVisible.geometryAllocation,
                                     depth + 1,
                                     hit );
      hit.end();

    }

  }

}


void gkg::TBScrollBoxImplementation::undraw()
{

  _canvas = 0;

}


void gkg::TBScrollBoxImplementation::modified( gkg::Glyph::Index /*index*/ )
{

  _changed = true;
  doRefresh();

}


bool gkg::TBScrollBoxImplementation::isShown( gkg::Glyph::Index index )
{

  doRefresh();
  return ( index >= _start ) && ( index < _end );

}


gkg::Glyph::Index gkg::TBScrollBoxImplementation::getFirstShown()
{

  doRefresh();
  return _start;

}


gkg::Glyph::Index gkg::TBScrollBoxImplementation::getLastShown()
{

  doRefresh();
  return _end - 1;

}


void gkg::TBScrollBoxImplementation::getGeometryAllotment(
                                     gkg::Glyph::Index index,
                                     gkg::DimensionName dimensionName,
                                     gkg::GeometryAllotment& geometryAllotment )
{

  doRefresh();
  if ( ( index >= _start ) && ( index < _end ) )
  {

    geometryAllotment =
    _visible[ index - _start ].geometryAllocation.getAllotment( dimensionName );

  }

}


gkg::Glyph::Index gkg::TBScrollBoxImplementation::getStart() const
{

  return _start;

}


gkg::Glyph::Index gkg::TBScrollBoxImplementation::getEnd() const
{

  return _end;

}


void gkg::TBScrollBoxImplementation::doScroll( gkg::DimensionName dimensionName,
                                               gkg::Glyph::Index newStart,
                                               gkg::Glyph::Index newEnd )
{

  gkg::Glyph::Index maxEnd = _target->getCount();
  if ( newStart < 0 )
  {

    newStart = 0;

  }

  if ( newEnd > maxEnd )
  {

    newStart -= ( newEnd - maxEnd );
    newEnd = maxEnd;

  }
  if ( newStart != _start || newEnd != _end )
  {

    undrawRange( _start, newStart - 1 );
    gkg::Glyph::Index oldEnd = _end;
    _start = newStart;
    _end = newEnd;
    reallocate();
    undrawRange( _end, oldEnd - 1 );
    redraw();
    _target->notify( dimensionName );

  }

}


void gkg::TBScrollBoxImplementation::doRefresh()
{

  if ( _changed )
  {

    refresh();

  }

}


void gkg::TBScrollBoxImplementation::check(
                             gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation )
{

  if ( !_canvas ||
       ( _canvas != canvas ) ||
       ( _transformer != canvas->getTransformer() ) ||
       !_geometryAllocation.isEqualTo( geometryAllocation, 1e-4 ) )
  {

    gkg::GeometryExtension geometryExtension;
    _target->allocate( canvas, geometryAllocation, geometryExtension );

  }

}


void gkg::TBScrollBoxImplementation::refresh()
{

  gkg::GeometryRequisition geometryRequisition;
  _target->request( geometryRequisition );
  _start = 0;
  reallocate();
  redraw();
  _target->notify( gkg::DimensionX );
  _target->notify( gkg::DimensionY );

}


void gkg::TBScrollBoxImplementation::reallocate()
{

  if ( !_canvas )
  {

    return;

  }

  gkg::Glyph::Index count = _target->getCount();
  _end = count;
  _visible.clear();
  _partiallyVisible.glyph = 0;

  gkg::GeometryRequisition geometryRequisition;
  gkg::TBScrollBoxImplementation::Info info;
  gkg::GeometryExtension geometryExtension;
  const gkg::GeometryRequirement&
    geometryRequirement = geometryRequisition.getRequirementY();
  gkg::FloatCoordinate p = _geometryAllocation.getTop();
  gkg::FloatCoordinate bottom = _geometryAllocation.getBottom();

  bool foundStart = false;
  for ( gkg::Glyph::Index i = _start; i < count; i++ )
  {

    gkg::Glyph* glyph = _target->getComponent( i );
    if ( glyph )
    {

      glyph->request( geometryRequisition );
      gkg::FloatCoordinate span = geometryRequirement.getNatural();
      if ( !gkg::equal( span, ( gkg::FloatCoordinate )0, 1e-2f ) )
      {

        if ( !foundStart )
        {

          _start = i;
          foundStart = true;

        }
        gkg::FloatCoordinate alignment = geometryRequirement.getAlignment();
        p -= span;
        if ( p < bottom )
        {

          _end = i;
          _partiallyVisible.glyph = glyph;
          _partiallyVisible.geometryAllocation.getAllotmentX() =
            _geometryAllocation.getAllotmentX();
          gkg::GeometryAllotment& pay = 
            _partiallyVisible.geometryAllocation.getAllotmentY();
          pay.setSpan( span );
          pay.setOrigin( p + ( gkg::FloatCoordinate )( alignment * span ) );
          pay.setAlignment( alignment );
          glyph->allocate( _canvas,
                           _partiallyVisible.geometryAllocation,
                           geometryExtension );
          break;

        }
        info.glyph = glyph;
        gkg::GeometryAllotment& ax = info.geometryAllocation.getAllotmentX();
        ax = _geometryAllocation.getAllotmentX();
        gkg::GeometryAllotment& ay = info.geometryAllocation.getAllotmentY();
        ay.setSpan( span );
        ay.setOrigin( p + ( gkg::FloatCoordinate )( alignment * span ) );
        ay.setAlignment( alignment );
        _visible.push_back( info );
        glyph->allocate( _canvas, info.geometryAllocation, geometryExtension );

      }

    }

  }

}


void gkg::TBScrollBoxImplementation::redraw()
{

  if ( _canvas )
  {

    _canvas->setDamage( _geometryExtension );

  }

}


void gkg::TBScrollBoxImplementation::undrawRange( gkg::Glyph::Index begin,
                                                  gkg::Glyph::Index end )
{

  for ( gkg::Glyph::Index i = begin; i <= end; i++ )
  {

    gkg::Glyph* glyph = _target->getComponent( i );
    if ( glyph )
    {

      glyph->undraw();

    }

  }

}
