#include <gkg-graphics-X11-core/Composition.h>
#include <gkg-graphics-X11-core/Box.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Compositor.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Tile.h>
#include <gkg-graphics-X11-core/TileReversed.h>
#include <gkg-graphics-X11-core/Align.h>
#include <gkg-processing-algobase/Math.h>


//
// class Break
//

gkg::Break::Break()
           : _status( 0 )
{
}


void gkg::Break::setPatch( Patch* patch )
{

  _patch = patch;

}


gkg::Patch* gkg::Break::getPatch() const
{

  return _patch;

}


bool gkg::Break::isViewed() const
{

  return ( _status & gkg::Break::BreakViewed ) != 0;

}


void gkg::Break::setViewed( bool value )
{

  if ( value )
  {

    _status |= gkg::Break::BreakViewed;

  }
  else
  {

    _status &= ~gkg::Break::BreakViewed;

  }

}


bool gkg::Break::isValid() const
{

  return ( _status & gkg::Break::BreakValid ) != 0;

}


void gkg::Break::setValid( bool value )
{

  if ( value )
  {

    _status |= gkg::Break::BreakValid;

  }
  else
  {

    _status &= ~gkg::Break::BreakValid;

  }

}


void gkg::Break::setBegin( gkg::FloatCoordinate begin )
{

  _begin = begin;

}


gkg::FloatCoordinate gkg::Break::getBegin() const
{

  return _begin;

}


void gkg::Break::setEnd( gkg::FloatCoordinate end )
{

  _end = end;

}


gkg::FloatCoordinate gkg::Break::getEnd() const
{

  return _end;

}


void gkg::Break::setFirst( gkg::Glyph::Index first )
{

  _first = first;

}


void gkg::Break::incFirst()
{

  ++ _first;

}


void gkg::Break::decFirst()
{

  -- _first;

}


gkg::Glyph::Index gkg::Break::getFirst() const
{

  return _first;

}


void gkg::Break::setLast( gkg::Glyph::Index last )
{

  _last = last;

}


void gkg::Break::incLast()
{

  ++ _last;

}


void gkg::Break::decLast()
{

  -- _last;

}


gkg::Glyph::Index gkg::Break::getLast() const
{

  return _last;

}


//
// local variables and static functions
//

const float epsilon = 0.1;

static gkg::Compositor::Index __length;
static gkg::FloatCoordinate* __naturals;
static gkg::FloatCoordinate* __stretchs;
static gkg::FloatCoordinate* __shrinks;
static int32_t* __penalties; 
static gkg::FloatCoordinate* __spans;
static gkg::Compositor::Index* __breaks;


static void growArrays( gkg::Compositor::Index length )
{

  gkg::FloatCoordinate* naturals = new gkg::FloatCoordinate[ length ];
  gkg::FloatCoordinate* stretchs = new gkg::FloatCoordinate[ length ];
  gkg::FloatCoordinate* shrinks = new gkg::FloatCoordinate[ length ];
  int32_t* penalties = new int32_t[ length ];
  gkg::FloatCoordinate* spans = new gkg::FloatCoordinate[ length ];
  gkg::Compositor::Index* breaks = new gkg::Compositor::Index[ length ];

  for ( gkg::Compositor::Index i = 0; i < __length; i++ )
  {

    naturals[ i ] = __naturals[ i ];
    stretchs[ i ] = __stretchs[ i ];
    shrinks[ i ] = __shrinks[ i ];
    penalties[ i ] = __penalties[ i ];
    spans[ i ] = __spans[ i ];
    breaks[ i ] = __breaks[ i ];

  }

  delete [] __naturals;
  __naturals = naturals;
  delete [] __stretchs;
  __stretchs = stretchs;
  delete [] __shrinks;
  __shrinks = shrinks;
  delete [] __penalties;
  __penalties = penalties;
  delete [] __spans;
  __spans = spans;
  delete [] __breaks;
  __breaks = breaks;

  __length = length;

}


static gkg::Glyph::Index 
getPreviousForcedBreak( gkg::Glyph::Index i,
                        std::deque< gkg::Composition::Component >& components )
{

  while ( i >= 0 )
  {

    const gkg::Composition::Component& component = components[ i ];
    gkg::Glyph* glyph = component.glyph;
    if ( glyph )
    {

      gkg::GeometryRequisition geometryRequisition;
      glyph->request( geometryRequisition );
      if ( geometryRequisition.getPenalty() == gkg::Compositor:: PenaltyGood )
      {

        break;

      }

    }
    -- i;

  }
  return i;

}


static gkg::Glyph::Index 
getNextForcedBreak( gkg::Glyph::Index i,
                    std::deque< gkg::Composition::Component >& components )
{

  gkg::Glyph::Index count = ( gkg::Glyph::Index )components.size();
  while ( i < count )
  {

    const gkg::Composition::Component& component = components[ i ];
    gkg::Glyph* glyph = component.glyph;
    if ( glyph )
    {

      gkg::GeometryRequisition geometryRequisition;
      glyph->request( geometryRequisition );
      if ( geometryRequisition.getPenalty() == gkg::Compositor:: PenaltyGood )
      {

        break;

      }

    }
    ++ i;

  }
  return std::min( i, count - 1 );

}


static gkg::Glyph::Index
fillArrays( gkg::Glyph::Index first,
            gkg::DimensionName dimensionName,
            std::deque< gkg::Composition::Component >& components )
{

  gkg::Glyph::Index count = ( gkg::Glyph::Index )components.size();
  gkg::Glyph::Index last = first;
  while ( last < count )
  {

    gkg::Glyph::Index index = last - first;
    if ( index >= __length )
      growArrays( getNextForcedBreak( last, components ) - first + 1 );
    __naturals[ index ] = 0;
    __stretchs[ index ] = 0;
    __shrinks[ index ] = 0;
    __penalties[ index ] = gkg::Compositor::PenaltyBad;

    const gkg::Composition::Component& component = components[ last ];
    gkg::Glyph* glyph = component.glyph;
    if ( glyph )
    {

      gkg::GeometryRequisition geometryRequisition;
      glyph->request( geometryRequisition );
      gkg::GeometryRequirement&
        geometryRequirement = geometryRequisition.getRequirement( 
                                                   dimensionName );
      if ( geometryRequirement.isDefined() )
      {

        __naturals[ index ] = geometryRequirement.getNatural();
        __stretchs[ index ] = geometryRequirement.getStretch();
        __shrinks[ index ] = geometryRequirement.getShrink();

      }
      __penalties[ index ] = geometryRequisition.getPenalty();

    }
    if ( __penalties[ index ] == gkg::Compositor::PenaltyGood )
    {

      break;

    }
    else
    {

      ++ last;

    }

  }
  return std::min( last, count - 1 );

}


//
// class Composition
//

gkg::Composition::Composition( gkg::Glyph* context,
                               gkg::Compositor* compositor,
                               gkg::Glyph* separator,
                               gkg::DimensionName dimensionName,
                               gkg::FloatCoordinate span,
                               gkg::FloatCoordinate stretch,
                               gkg::FloatCoordinate shrink,
                               gkg::Glyph::Index size )
                 : gkg::MonoGlyph(
                   gkg::LayoutFactory::getInstance().createFlexible( context,
                                                                     stretch,
                                                                     shrink ) ),
                   _compositor( compositor ),
                   _components( size ),
                   _breaks( size / 50 ),
                   _separator( separator ),
                   _viewAll( true ),
                   _damaged( true ),
                   _firstDamage( -1 ),
                   _lastDamage( -1 ),
                   _item( 0 ),
                   _dimensionName( dimensionName ),
                   _span( span )
{

  if ( _separator )
  {

    _separator->addReference();

  }

  _resizable = ( !gkg::equal( stretch, ( FloatCoordinate )0, 1e-4f ) ||
                 !gkg::equal( shrink, ( FloatCoordinate )0, 1e-4f ) );

}


gkg::Composition::~Composition()
{

  if ( _separator )
  {

    _separator->removeReference();

  }
  _compositor = 0;
  gkg::Glyph::Index count = ( gkg::Glyph::Index )_components.size();
  for ( gkg::Glyph::Index i = 0; i < count; i++ )
  {

    gkg::Composition::Component& component = _components[ i ];
    if ( component.glyph )
    {

      ( component.glyph )->removeReference();

    }

  }
  _components.clear();
  _breaks.clear();

}


void gkg::Composition::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  if ( _resizable )
  {

    gkg::FloatCoordinate 
      size = geometryAllocation.getAllotment( _dimensionName ).getSpan();
    if ( !gkg::equal( size, _span, 1e-4f ) )
    {

      _span = size;
      gkg::Glyph::Index breakCount = ( gkg::Glyph::Index )_breaks.size();
      for ( gkg::Glyph::Index b = 0; b < breakCount; b++ )
      {

        _breaks[ b ].setValid( false );

      }
      setDamage( 0, ( gkg::Glyph::Index )_components.size() - 1 );
      repair();

    }

  }
  this->gkg::MonoGlyph::allocate( canvas,
                                  geometryAllocation,
                                  geometryExtension );

}


// translate a pick on the composition contents into what looks like a pick on
// the elements;  that is, if the composition is a list of characters, it will
// be arranged into lines; the pick on the lines will return <line, col>, which
// needs to be compressed into a single index for the composition.
void gkg::Composition::pick( gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation,
                             int32_t depth,
                             gkg::Hit& hit )
{

  int32_t count = hit.getCount();
  this->gkg::MonoGlyph::pick( canvas, geometryAllocation, depth, hit );
  if ( hit.getCount() > count )
  {

    // retrieve the index of the line that was hit into "item".
    // Compositions create two glyphs per formatted line:
    // even-numbered elements refer to the lines and odd-numbered ones
    // refer to the separators between lines.  We will compute
    // the element "index" from "item" and the break information.
    gkg::Glyph::Index item = hit.getIndex( depth );
    gkg::Glyph::Index breakCount = ( gkg::Glyph::Index )_breaks.size();
    gkg::Glyph::Index index;
    if ( item % 2 == 0 )
    {

      gkg::Glyph::Index breakIndex = item / 2;
      if ( breakIndex < breakCount )
      {

        // hit on a line: calculate the character index by adding  the line's 
        // start to the offset into the line.
        const Break& br = _breaks[ breakIndex ];
        index = br.getFirst() + hit.getIndex( depth + 1 ) - 2;
        index = std::max( br.getFirst(), std::min( br.getLast() + 1, index ) );

      }
      else
      {

        // this case only seems to happen when there is a float at the end of
        // the composition.
        index = _breaks[ breakCount - 1 ].getLast() + 1;

      }

    }
    else
    {

      gkg::Glyph::Index breakIndex = item / 2 + 1;
      if ( breakIndex < breakCount )
      {

        // hit on the separator between lines: assume that the hit refers to the
       //  beginning of the following line.
       index = _breaks[ breakIndex ].getFirst();

      }
      else
      {

        // hit on separator at the end of the composition: return the end of 
        // the last line.
        index = _breaks[ breakIndex - 1 ].getLast() + 1;

      }

    }

    // remove the hit target for depth + 1, effectively shifting information
    // below it up one level.
    hit.remove( depth + 1 );

    // finally, add the information for this level.
    hit.retarget( depth, this, index );

  }

}


bool gkg::Composition::repair()
{

  if ( _damaged )
  {

    gkg::Glyph::Index
      componentCount = ( gkg::Glyph::Index )_components.size();
    gkg::Compositor::Index
      breakCount = ( gkg::Compositor::Index )_breaks.size();
    gkg::Compositor::Index forced = getPreviousForcedBreak( _firstDamage,
                                                            _components );
    gkg::Glyph::Index breakIndex = 0;
    for ( ; ; )
    {

      if ( breakIndex < breakCount )
      {

        const gkg::Break& br = _breaks[ breakIndex ];
        if ( br.getFirst() <= forced && br.getLast() < forced )
        {

          breakIndex ++;

        }
        else
        {

          break;

        }

      }
      else
      {

        break;

      }

    }

    while ( forced < componentCount - 1 && forced < _lastDamage )
    {

      gkg::Glyph::Index next = fillArrays( forced + 1, _dimensionName,
                                              _components );
      gkg::Glyph::Index count = next - forced;
      gkg::Glyph::Index b;
      for ( b = 0; b < count && ( b + breakIndex ) < breakCount; b++ )
      {

        const gkg::Break& br = _breaks[ b + breakIndex ];
        __spans[ b ] = _span - br.getBegin() - br.getEnd();

      }
      if ( b < count )
      {

        __spans[ b ] = _span;

      }
      gkg::Compositor::Index
        composeCount = _compositor->compose( __naturals,
                                             __stretchs,
                                             __shrinks,
                                             __penalties,
                                             count,
                                             __spans,
                                             breakCount - breakIndex + 1,
                                             __breaks,
                                             count );
      doRepair( forced + 1, breakIndex, __breaks, composeCount );
      forced = next;
      breakIndex += composeCount;
      breakCount = ( gkg::Compositor::Index )_breaks.size();

    }
    _damaged = false;
    return false;

  }
  else
  {

    return true;

  }

}


gkg::Glyph::Index 
gkg::Composition::getItem( gkg::Glyph::Index index )
{

  gkg::Compositor::Index
    count = ( gkg::Compositor::Index )_breaks.size();
  _item = std::min( std::max( INT32_C( 0 ), _item ), count - 1 );
  while ( ( _item < count - 1 ) && ( _breaks[ _item ].getLast() < index ) )
  {

    ++ _item;

  }
  while ( ( _item > 0 ) && ( _breaks[ _item ].getFirst() > index ) )
  {

    -- _item;

  }
  return std::max( INT32_C( 0 ), _item ) * 2;

}


gkg::Glyph::Index 
gkg::Composition::getBeginningOf( gkg::Compositor::Index item ) const
{

  gkg::Compositor::Index 
    i = std::max( INT32_C( 0 ), std::min( item / 2,
                           ( gkg::Compositor::Index )_breaks.size() - 1 ) );
  return _breaks[ i ].getFirst();

}


gkg::Glyph::Index 
gkg::Composition::getEndOf( gkg::Compositor::Index item ) const
{

  gkg::Compositor::Index 
     i = std::max( INT32_C( 0 ), std::min( item / 2,
                           ( gkg::Compositor::Index )_breaks.size() - 1 ) );
  return _breaks[ i ].getLast();

}


void gkg::Composition::setMargin( gkg::Compositor::Index item,
                                  gkg::FloatCoordinate begin,
                                  gkg::FloatCoordinate end )
{

  gkg::Compositor::Index b = item / 2;
  if ( b < ( gkg::Compositor::Index )_breaks.size() && item % 2 == 0 )
  {

    gkg::Break& br = _breaks[ b ];
    if ( br.getBegin() != begin || br.getEnd() != end )
    {

      br.setBegin( begin );
      br.setEnd( end );
      br.setValid( false );
      setDamage( br.getFirst() - 1, br.getLast() + 1 );

    }

  }

}


void gkg::Composition::view( gkg::Glyph::Index first,
                             gkg::Glyph::Index last )
{

  gkg::Glyph::Index breakCount = ( gkg::Glyph::Index )_breaks.size();
  gkg::Glyph* contents = getBody();
  for ( gkg::Glyph::Index b = 0; b < breakCount; b++ )
  {

    gkg::Break& br = _breaks[ b ];
    if ( br.getLast() >= first && br.getFirst() <= last )
    {

      if ( !br.isViewed() )
      {

        contents->replace( b * 2, makeItem( br, true ) );

      }

    }
    else
    {

      if ( br.isViewed() )
      {

        contents->replace( b * 2, makeItem( br, false ) );

      }

    }

  }
  _viewAll = false;

}


void gkg::Composition::append( gkg::Glyph* other )
{

  insert( ( gkg::Glyph::Index )_components.size(), other );

}


void gkg::Composition::prepend( gkg::Glyph* other )
{

  insert( 0, other );

}

void gkg::Composition::insert( gkg::Glyph::Index index, Glyph* other )
{

  if ( other )
  {

    other->addReference();

  }

  gkg::Composition::Component component;
  component.glyph = other;
  _components.insert( _components.begin() + index, component );
  gkg::Glyph* contents = getBody();
  for ( gkg::Compositor::Index b = getItem( index ) / 2;
        b < ( gkg::Compositor::Index )_breaks.size(); b++ )
  {

    gkg::Break& br = _breaks[ b ];
    if ( br.isViewed() &&
         ( index >= br.getFirst() ) &&
         ( index <= br.getLast() + 1  ) )
    {

      br.setValid( false );
      contents->getComponent( 2 * b )->insert( index - br.getFirst() + 2, 0 );
      contents->change( 2 * b );

    }
    if ( index < br.getFirst() )
    {

      br.incFirst();

    }
    if ( index <= br.getLast() + 1 )
    {

      br.incLast();

    }

  }
  setDamage( index - 1, index + 1 );

}


void gkg::Composition::remove( gkg::Glyph::Index index )
{

  gkg::Composition::Component& component = _components[ index ];
  if ( component.glyph )
  {

    component.glyph->removeReference();

  }

  _components.erase( _components.begin() + index );
  gkg::Glyph* contents = getBody();
  for ( gkg::Compositor::Index b = getItem( index ) / 2;
        b < ( gkg::Compositor::Index )_breaks.size(); b++ )
  {

    gkg::Break& br = _breaks[ b ];
    if ( br.isViewed() &&
         ( index >= br.getFirst() ) &&
         ( index <= br.getLast() + 1 ) )
    {

      br.setValid( false );
      contents->getComponent( 2 * b )->remove( index - br.getFirst() + 2 );
      contents->change( 2 * b );

    }
    if ( index < br.getFirst() )
    {

      br.decFirst();

    }
    if ( index <= br.getLast() + 1 )
    {

      br.decLast();

    }

  }
  setDamage( index - 1, index );

}


void gkg::Composition::replace( gkg::Glyph::Index index,
                                gkg::Glyph* glyph )
{

  gkg::GeometryRequisition oldr, newr;
  if ( glyph )
  {

    glyph->request( newr );
    glyph->addReference();

  }
  gkg::Composition::Component& component = _components[ index ];
  if ( component.glyph )
  {

    component.glyph->request( oldr );
    component.glyph->removeReference();

  }
  component.glyph = glyph;
  gkg::Glyph* contents = getBody();
  for ( gkg::Compositor::Index b = getItem( index ) / 2;
        b < ( gkg::Compositor::Index )_breaks.size(); b++ )
  {

    gkg::Break& br = _breaks[ b ];
    if ( br.isViewed() &&
         ( index >= br.getFirst() - 1 ) &&
         ( index <= br.getLast() + 1 ) )
    {

      if ( newr.isEqualTo( oldr, epsilon ) )
      {

        Glyph* g = ( glyph == 0 ? 0 :
                                  index == br.getFirst() - 1 ?
                                  glyph->compose( gkg::Glyph::PostBreak ) :
                                  index == br.getLast() + 1 ?
                                  glyph->compose( gkg::Glyph::PreBreak ) :
                                  glyph->compose( gkg::Glyph::NoBreak ) );
        contents->getComponent( 2 * b )->replace( index - br.getFirst() + 2, g);

      }
      else
      {

        br.setValid( false );
        setDamage( index - 1, index + 1 );

      }
      contents->change( 2 * b );

    }
    if ( br.getFirst() > index )
    {

      break;

    }

  }

}


void gkg::Composition::change( gkg::Glyph::Index index )
{

  gkg::Glyph* contents = getBody();
  for ( gkg::Compositor::Index b = getItem( index ) / 2;
        b < ( gkg::Compositor::Index )_breaks.size(); b++ )
  {

    gkg::Break& br = _breaks[ b ];
    if ( br.isViewed() &&
         ( index >= br.getFirst() ) &&
         ( index <= br.getLast() + 1 ) )
    {

      contents->getComponent( 2 * b )->change( index - br.getFirst() + 2 );
      contents->change( 2 * b );

    }
    if ( br.getFirst() > index )
    {

      break;

    }

  }

}


gkg::Glyph::Index gkg::Composition::getCount() const
{

  return ( gkg::Glyph::Index )_components.size();

}


gkg::Glyph* 
gkg::Composition::getComponent( gkg::Glyph::Index index ) const
{

  return _components[ index ].glyph;

}


void gkg::Composition::getGeometryAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  // very bad hack to be solved : ( ( gkg::Composition* )this )

  for ( gkg::Glyph::Index
        b = ( ( gkg::Composition* )this )->getItem( index ) / 2;
        b < ( gkg::Compositor::Index )_breaks.size(); b++ )
  {

    const gkg::Break& br = _breaks[ b ];
    if ( index >= br.getFirst() && index <= br.getLast() + 1 )
    {

      if ( br.isViewed() )
      {

        getBody()->getComponent( 2 * b )->getGeometryAllotment(
                                                   index - br.getFirst() + 2,
                                                   dimensionName,
                                                   geometryAllotment );

      }
      else
      {

        getBody()->getGeometryAllotment( 2 * b, dimensionName,
                                         geometryAllotment );

      }

    }

  }

}


void gkg::Composition::doRepair( gkg::Glyph::Index firstComponent,
                                 gkg::Glyph::Index firstBreak,
                                 gkg::Compositor::Index* breaks,
                                 gkg::Compositor::Index breakCount )
{

  gkg::Glyph::Index breakIndex = firstBreak;
  gkg::Glyph* contents = getBody();
  for ( gkg::Glyph::Index i = 0; i < breakCount; i++ )
  {

    gkg::Break b;
    if ( breakIndex < ( gkg::Glyph::Index )_breaks.size() )
    {

      const gkg::Break& br = _breaks[ breakIndex ];
      b.setBegin( br.getBegin() );
      b.setEnd( br.getEnd() );

    }
    else
    {

      b.setBegin( 0 );
      b.setEnd( 0 );

    }
    b.setFirst( firstComponent + ( ( i == 0 ) ? 0 : breaks[ i - 1 ] + 1 ) );
    b.setLast( firstComponent + ( breaks[ i ] - 1 ) );
    bool reBreak = ( breakIndex == ( gkg::Glyph::Index )_breaks.size() );
    if ( !reBreak )
    {

      const gkg::Break& br = _breaks[ breakIndex ];
      reBreak = ( !br.isValid() ||
                  br.getFirst() != b.getFirst() ||
                  br.getLast() != b.getLast() );

    }
    if ( reBreak )
    {

      while ( breakIndex < ( gkg::Glyph::Index )_breaks.size() - 1 &&
              _breaks[ breakIndex + 1 ].getLast() <= b.getLast() )
      {

        contents->remove( breakIndex * 2 + 1 );
        contents->remove( breakIndex * 2 );
        _breaks.erase( _breaks.begin() + breakIndex );

      }
      reBreak = ( breakIndex == ( gkg::Glyph::Index )_breaks.size() );
      if ( !reBreak )
      {

        const Break& br = _breaks[ breakIndex ];
        reBreak = ( ( i < breakCount - 1 &&
                    br.getLast() >= firstComponent + breaks[ i + 1 ] - 1 ) ||
		    ( i == breakCount - 1 && br.getFirst() > b.getLast() + 1 ));

      }
      if ( reBreak )
      {

        contents->insert( breakIndex * 2, makeItem( b, _viewAll ) );
        contents->insert( breakIndex * 2 + 1, getSeparator( b ) );
        _breaks.insert( _breaks.begin() + breakIndex, b );

      }
      else
      {

        gkg::Break& br = _breaks[ breakIndex ];
        if ( br.getPatch() )
        {

          br.getPatch()->redraw();

        }
        contents->replace( breakIndex * 2, makeItem( b, _viewAll ) );
        contents->replace( breakIndex * 2 + 1, getSeparator( b ) );
        br = b;

      }

    }
    ++ breakIndex;

  }

}


void gkg::Composition::setDamage( gkg::Glyph::Index first,
                                  gkg::Glyph::Index last )
{

  if ( _damaged )
  {

    _firstDamage = std::min( _firstDamage, first );
    _lastDamage = std::max( _lastDamage, last );

  }
  else
  {

    _firstDamage = first;
    _lastDamage = last;
    _damaged = true;

  }

}


gkg::Glyph* gkg::Composition::getSeparator( gkg::Break& theBreak )
{

  if ( theBreak.getLast() >= ( gkg::Glyph::Index )_components.size() - 1 )
  {

    return _separator;

  }
  else
  {

    gkg::Glyph* inBreak = _components[ theBreak.getLast() + 1 ].glyph;
    if ( inBreak )
    {

      gkg::Glyph* glyph = inBreak->compose( gkg::Glyph::InBreak );
      if ( glyph )
      {

        return glyph;

      }
      else
      {

        return _separator;

      }

    }
    else
    {

      return _separator;

    }

  }

}


gkg::Glyph* gkg::Composition::makeItem( gkg::Break& theBreak, bool created )
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  theBreak.setValid( true );
  theBreak.setViewed( created );
  if ( created )
  {

    gkg::Glyph* glyph = make( theBreak );
    if ( ( _span > 0 ) && ( _span < fil ) )
    {

      glyph = layoutFactory.createFixedDimension( glyph,
                                                  _dimensionName,
                                                  _span );

    }
    theBreak.setPatch( new gkg::Patch( glyph ) );
    return theBreak.getPatch();

  }
  else
  {

    static gkg::Glyph::Index __reqCount;
    static gkg::GeometryRequisition* __req;
    gkg::Glyph::Index count = theBreak.getLast() - theBreak.getFirst() + 3;
    if ( count >= __reqCount )
    {

      delete [] __req;
      __req = new gkg::GeometryRequisition[ count ];
      __reqCount = count;

    }
    gkg::Glyph::Index index = 0;
    if ( theBreak.getFirst() > 0 )
    {

      gkg::Glyph* glyph = _components[ theBreak.getFirst() - 1 ].glyph;
      if ( glyph )
      {

        glyph = glyph->compose( gkg::Glyph::PostBreak );

      }
      if ( glyph )
      {

        glyph->request( __req[ index ] );
        ++ index;

      }

    }
    for ( gkg::Glyph::Index k = theBreak.getFirst();
          k <= theBreak.getLast(); k++ )
    {

      gkg::Glyph* glyph = _components[ k ].glyph;
      if ( glyph )
        glyph = glyph->compose( gkg::Glyph::NoBreak );
      if ( glyph )
      {

        glyph->request( __req[ index ] );
        ++ index;

      }

    }
    if ( theBreak.getLast() < ( gkg::Glyph::Index )_components.size() - 1 )
    {

      gkg::Glyph* glyph = _components[ theBreak.getLast() + 1 ].glyph;
      if ( glyph )
        glyph = glyph->compose( gkg::Glyph::PreBreak );
      if ( glyph )
      {

        glyph->request( __req[ index ] );
        ++ index;

      }

    }
    gkg::GeometryRequisition geometryRequisition;
    gkg::DimensionName cross;
    if ( _dimensionName == gkg::DimensionX )
    {

      if ( ( _span > 0 ) && ( _span < fil ) )
      {

        gkg::GeometryRequirement geometryRequirement( _span );
        geometryRequisition.setRequirement( _dimensionName,
                                            geometryRequirement );

      }
      else
      {

        gkg::Tile tile( _dimensionName );
        tile.request( index, __req, geometryRequisition );

      }
      cross = gkg::DimensionY;

    } 
    else
    {

      if ( ( _span > 0 ) && ( _span < fil ) )
      {

        gkg::GeometryRequirement geometryRequirement( _span, 0, 0, 1.0 );
        geometryRequisition.setRequirement( _dimensionName,
                                            geometryRequirement );

      }
      else
      {

        gkg::TileReversed tileReversed( _dimensionName );
        tileReversed.request( index, __req, geometryRequisition );

      }
      cross = gkg::DimensionX;

    }
    gkg::Align align( cross );
    align.request( index, __req, geometryRequisition );
    theBreak.setPatch( 0 );
    return layoutFactory.createGlue( geometryRequisition );

  }

}


gkg::Glyph* gkg::Composition::make( gkg::Break& )
{

  return 0;

}
