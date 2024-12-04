#include <gkg-graphics-X11-core/LRScrollBoxImplementation.h>
#include <gkg-graphics-X11-core/LRScrollBox.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-processing-algobase/Math.h>


gkg::LRScrollBoxImplementation::LRScrollBoxImplementation(
                                                  gkg::LRScrollBox* lrScrollBox,
                                                  bool shrinkable )
                               : _target( lrScrollBox ),
                                 _shrinkable( shrinkable ),
                                 _start( 0 ),
                                 _end( 0 ),
                                 _changed( true ),
                                 _canvas( 0 )
{

  _partiallyVisible.glyph = 0;

}


gkg::LRScrollBoxImplementation::~LRScrollBoxImplementation()
{
}


gkg::LRScrollBox* gkg::LRScrollBoxImplementation::getTarget() const
{

  return _target;

}


void gkg::LRScrollBoxImplementation::request( 
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
        gkg::FloatCoordinate rHeight = ry.getNatural();
        if ( rHeight > naturalHeight )
        {

          naturalHeight = rHeight;

        }
        naturalWidth += rx.getNatural();

      }

    }

    gkg::GeometryRequirement&
      boxX = _geometryRequisition.getRequirementX();
    boxX.setNatural( naturalWidth );
    boxX.setStretch( fil );
    boxX.setShrink( naturalWidth );
    boxX.setAlignment( 0.0 );

    gkg::GeometryRequirement&
      boxY = _geometryRequisition.getRequirementY();
    boxY.setNatural( naturalHeight );
    boxY.setStretch( fil );
    boxY.setShrink( _shrinkable ? naturalHeight : 0.0 );
    boxY.setAlignment( 0.0 );
    _changed = false;

  }
  geometryRequisition = _geometryRequisition;

}


void gkg::LRScrollBoxImplementation::allocate(
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



void gkg::LRScrollBoxImplementation::draw(
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
    std::deque< gkg::LRScrollBoxImplementation::Info >::iterator
      i = _visible.begin(), ie = _visible.end();
    while ( i != ie )
    {

      const gkg::LRScrollBoxImplementation::Info& info = *i;
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


void gkg::LRScrollBoxImplementation::pick(
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

    std::deque< gkg::LRScrollBoxImplementation::Info >::iterator
      i = _visible.begin(), ie = _visible.end();
    while ( i != ie )
    {

      const gkg::LRScrollBoxImplementation::Info& info = *i;
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


void gkg::LRScrollBoxImplementation::undraw()
{

  _canvas = 0;

}


void 
gkg::LRScrollBoxImplementation::modified( gkg::Glyph::Index /*index*/ )
{

  _changed = true;
  doRefresh();

}


bool 
gkg::LRScrollBoxImplementation::isShown( gkg::Glyph::Index index )
{

  doRefresh();
  return index >= _start && index < _end;

}


gkg::Glyph::Index gkg::LRScrollBoxImplementation::getFirstShown()
{

  doRefresh();
  return _start;

}


gkg::Glyph::Index gkg::LRScrollBoxImplementation::getLastShown()
{

  doRefresh();
  return _end - 1;

}


void gkg::LRScrollBoxImplementation::getGeometryAllotment(
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


gkg::Glyph::Index gkg::LRScrollBoxImplementation::getStart() const
{

  return _start;

}


gkg::Glyph::Index gkg::LRScrollBoxImplementation::getEnd() const
{

  return _end;

}


void gkg::LRScrollBoxImplementation::doScroll(
                                               gkg::DimensionName dimensionName,
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
  if ( ( newStart != _start ) || ( newEnd != _end ) )
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


void gkg::LRScrollBoxImplementation::doRefresh()
{

  if ( _changed )
  {

    refresh();

  }

}


void gkg::LRScrollBoxImplementation::check(
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


void gkg::LRScrollBoxImplementation::refresh()
{

  gkg::GeometryRequisition geometryRequisition;
  _target->request( geometryRequisition );
  _start = 0;
  reallocate();
  redraw();
  _target->notify( gkg::DimensionX );
  _target->notify( gkg::DimensionY );

}


void gkg::LRScrollBoxImplementation::reallocate()
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
  gkg::LRScrollBoxImplementation::Info info;
  gkg::GeometryExtension geometryExtension;
  const gkg::GeometryRequirement&
    geometryRequirement = geometryRequisition.getRequirementX();
  gkg::FloatCoordinate p = _geometryAllocation.getLeft();
  gkg::FloatCoordinate right = _geometryAllocation.getRight();

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
        if ( ( p + span ) > right )
        {

          _end = i;
          _partiallyVisible.glyph = glyph;
          _partiallyVisible.geometryAllocation.getAllotmentY() =
            _geometryAllocation.getAllotmentY();
          gkg::GeometryAllotment& pax = 
            _partiallyVisible.geometryAllocation.getAllotmentX();
          pax.setSpan( span );
          pax.setOrigin( p + ( gkg::FloatCoordinate )( alignment * span ) );
          pax.setAlignment( alignment );
          glyph->allocate( _canvas,
                           _partiallyVisible.geometryAllocation,
                           geometryExtension );
          break;

        }
        info.glyph = glyph;
        gkg::GeometryAllotment& ay = info.geometryAllocation.getAllotmentY();
        ay = _geometryAllocation.getAllotmentY();
        gkg::GeometryAllotment& ax = info.geometryAllocation.getAllotmentX();
        ax.setSpan( span );
        ax.setOrigin( p + ( gkg::FloatCoordinate )( alignment * span ) );
        ax.setAlignment( alignment );
        _visible.push_back( info );
        glyph->allocate( _canvas, info.geometryAllocation, geometryExtension );

        p += span;

      }

    }

  }

}


void gkg::LRScrollBoxImplementation::redraw()
{

  if ( _canvas )
  {

    _canvas->setDamage( _geometryExtension );

  }

}


void gkg::LRScrollBoxImplementation::undrawRange( gkg::Glyph::Index begin,
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
