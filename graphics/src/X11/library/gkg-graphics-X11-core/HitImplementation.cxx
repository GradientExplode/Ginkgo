#include <gkg-graphics-X11-core/HitImplementation.h>
#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-graphics-X11-core/EventHandler.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-processing-algobase/Math.h>
#include <cstring>
#include <iostream>


gkg::HitImplementation::HitImplementation( const gkg::Event* event )
                       : _event( event ),
                         _defaultEventHandler( 0 )
{

  gkg::FloatCoordinate x = event->getPointerX();
  gkg::FloatCoordinate y = event->getPointerY();
  initialize( x, y, x, y );

}


gkg::HitImplementation::HitImplementation( gkg::FloatCoordinate x1,
                                           gkg::FloatCoordinate y1,
                                           gkg::FloatCoordinate x2,
                                           gkg::FloatCoordinate y2 )
                       : _event( 0 ),
                         _defaultEventHandler( 0 )
{

  initialize( x1, y1, x2, y2 );

}


gkg::HitImplementation::~HitImplementation()
{

  for ( int32_t i = 0; i < _items.used; i++ )
  {

    gkg::HitTargetList& targetList = _items.lists[ i ];
    if ( targetList.targets != targetList.fixedTargets )
    {

      delete [] targetList.targets;

    }

  }
  if ( _items.lists != _items.fixedLists )
  {

    delete [] _items.lists;

  }
  if ( _picks.possibleTargets != _picks.fixedPossibleTargets )
  {

    delete [] _picks.possibleTargets;

  }
  if ( _areas.areas != _areas.fixedAreas )
  {

    delete [] _areas.areas;

  }

}


const gkg::HitTargetArea& gkg::HitImplementation::getArea() const
{

  return _areas.areas[ _areas.used - 1 ];

}


void gkg::HitImplementation::setArea( gkg::FloatCoordinate left,
                                      gkg::FloatCoordinate bottom,
                                      gkg::FloatCoordinate right,
                                      gkg::FloatCoordinate top )
{

  gkg::HitTargetArea& area = _areas.areas[ _areas.used - 1 ];
  area.left = left;
  area.bottom = bottom;
  area.right = right;
  area.top = top;

}


const gkg::Event* gkg::HitImplementation::getEvent() const
{

  return _event;

}


gkg::FloatCoordinate gkg::HitImplementation::getLeft() const
{

  return getArea().left;

}


gkg::FloatCoordinate gkg::HitImplementation::getBottom() const
{

  return getArea().bottom;

}


gkg::FloatCoordinate gkg::HitImplementation::getRight() const
{

  return getArea().right;

}


gkg::FloatCoordinate gkg::HitImplementation::getTop() const
{

  return getArea().top;

}


void gkg::HitImplementation::pushTransform()
{

  if ( _areas.used >= _areas.available )
  {

    int32_t newAvailable = _areas.available << 1;
    gkg::HitTargetArea* newAreas = new gkg::HitTargetArea[ newAvailable ];
    memcpy( newAreas, _areas.areas, 
            _areas.used * sizeof( gkg::HitTargetArea ) );
    if ( _areas.areas != _areas.fixedAreas )
    {

      delete [] _areas.areas;

    }
    _areas.areas = newAreas;
    _areas.available = newAvailable;

  }
  _areas.areas[ _areas.used ] = _areas.areas[ _areas.used - 1 ];
  _areas.used ++;

}


void gkg::HitImplementation::setTransform( const gkg::Transformer& transformer )
{

  const gkg::HitTargetArea& area = getArea();
  gkg::FloatCoordinate x1, y1, x2, y2, x3, y3, x4, y4;

  transformer.inverseTransform( area.left, area.bottom, x1, y1 );
  transformer.inverseTransform( area.left, area.top, x2, y2 );
  transformer.inverseTransform( area.right, area.top, x3, y3 );
  transformer.inverseTransform( area.right, area.bottom, x4, y4 );

  setArea( gkg::min( x1, x2, x3, x4 ), 
           gkg::min( y1, y2, y3, y4 ),
           gkg::max( x1, x2, x3, x4 ),
           gkg::max( y1, y2, y3, y4 ) );

}


void gkg::HitImplementation::popTransform()
{

  if ( _areas.used )
  {

    -- _areas.used;

  }

}


void gkg::HitImplementation::begin( int32_t depth, gkg::Glyph* glyph,
                                    gkg::Glyph::Index index,
                                    gkg::EventHandler* eventHandler )
{

  if ( _picks.used >= _picks.available )
  {

    int32_t newAvailable = _picks.available << 1;
    gkg::PossibleHitTarget* 
      newPossible = new gkg::PossibleHitTarget[ newAvailable ];
    memcpy( newPossible, _picks.possibleTargets,
            _picks.used * sizeof( gkg::PossibleHitTarget ) );
    if ( _picks.possibleTargets != _picks.fixedPossibleTargets )
    {

      delete [] _picks.possibleTargets;

    }
    _picks.possibleTargets = newPossible;
    _picks.available = newAvailable;

  }
  gkg::PossibleHitTarget& p = _picks.possibleTargets[ _picks.used ];
  p.picked = false;
  p.count = _items.used;
  p.depth = depth;
  p.item.glyph = glyph;
  p.item.index = index;
  p.item.eventHandler = eventHandler;
  ++ _picks.used;

}


void gkg::HitImplementation::setTarget( int32_t depth, gkg::Glyph* glyph,
                                        gkg::Glyph::Index index,
                                        gkg::EventHandler* eventHandler )
{

  int32_t top = _picks.used - 1;
  if ( top >= 0 )
  {

    _picks.possibleTargets[ top ].picked = true;

  }

  if ( _items.used >= _items.available )
  {

    int32_t newAvailable = _items.available << 1;
    gkg::HitTargetList* newLists = new gkg::HitTargetList[ newAvailable ];
    for ( int32_t e = 0; e < _items.used; e++ )
    {

      gkg::HitTargetList& tOld = _items.lists[ e ];
      gkg::HitTargetList& tNew = newLists[ e ];
      tNew.available = tOld.available;
      tNew.used = tOld.used;
      if ( tOld.targets == tOld.fixedTargets )
      {

        tNew.targets = tNew.fixedTargets;
        memcpy( tNew.fixedTargets, tOld.fixedTargets, 
                sizeof( tNew.fixedTargets ) );

      }
      else
      {

        tNew.targets = tOld.targets;

      }

    }
    if ( _items.lists != _items.fixedLists )
    {

      delete [] _items.lists;

    }
    _items.lists = newLists;
    _items.available = newAvailable;

  }
  gkg::HitTargetList& t = _items.lists[ _items.used ];
  t.available = HIT_TARGET_LIST_FIXED;
  t.used = -1;
  t.targets = t.fixedTargets;
  for ( gkg::HitTarget* ht = t.targets; ht < &t.targets[ t.available ]; ht++)
  {

    ht->glyph = 0;

  }
  ++ _items.used;
  addItem( false, depth, glyph, index, eventHandler, 0 );

}


void gkg::HitImplementation::end()
{

  int32_t top = _picks.used - 1;
  if ( top >= 0 )
  {

    const gkg::PossibleHitTarget& p = _picks.possibleTargets[ top ];
    if ( p.picked )
    {

      gkg::Glyph::Index newTargets = _items.used - p.count;
      for ( int32_t i = 0; i < newTargets; i++ )
      {

        addItem( false, p.depth, p.item.glyph, p.item.index,
                 p.item.eventHandler, i	);

      }
      if ( top > 0 )
      {

        _picks.possibleTargets[ top - 1 ].picked = true;

      }

    }
    _picks.used = top;

  }

}


void gkg::HitImplementation::remove( int32_t depth, gkg::Glyph::Index target )
{

  if ( ( target < 0 ) || ( target >= _items.used ) )
  {

    std::cerr << "internal error: list index " << target << " out of range"
              << std::endl;
    ::abort();

  }
  gkg::HitTargetList& targetList = _items.lists[ target ];
  for ( int32_t i = depth + 1; i <= targetList.used; i++ )
  {

    targetList.targets[ i - 1 ] = targetList.targets[ i ];

  }
  -- targetList.used;
  if ( targetList.targets[ depth ].eventHandler == _defaultEventHandler )
  {

    _defaultEventHandlerDepth = depth;

  }

}


bool gkg::HitImplementation::hasAny() const
{

  return _items.used != 0;

}


int32_t gkg::HitImplementation::getCount() const
{

  return _items.used;

}


int32_t gkg::HitImplementation::getDepth( gkg::Glyph::Index target ) const
{

  if ( ( target < 0 ) || ( target >= _items.used ) )
  {

    std::cerr << "internal error: list index " << target << " out of range"
              << std::endl;
    ::abort();

  }
  return _items.lists[ target ].used;

}


gkg::Glyph*
gkg::HitImplementation::getTarget( int32_t depth, gkg::Glyph::Index target )
{

  return getItem( depth, target ).glyph;

}


gkg::Glyph::Index 
gkg::HitImplementation::getIndex( int32_t depth, gkg::Glyph::Index target  )
{

  return getItem( depth, target ).index;

}


gkg::EventHandler* gkg::HitImplementation::getEventHandler() const
{

  return _defaultEventHandler;

}


// add an item to the current hit list.  Use the depth to place it appropriately
void gkg::HitImplementation::addItem( bool override, int32_t depth,
                                      gkg::Glyph* glyph,
                                      gkg::Glyph::Index index,
                                      gkg::EventHandler* eventHandler,
                                      gkg::Glyph::Index target )
{

  gkg::HitTarget& t = getItem( depth, target );
  if ( override || !t.glyph )
  {

    t.glyph = glyph;
    t.index = index;
    t.eventHandler = eventHandler;

  }
  if ( eventHandler &&
       ( !_defaultEventHandler || ( depth >= _defaultEventHandlerDepth ) ) )
  {

    _defaultEventHandler = eventHandler;
    _defaultEventHandlerDepth = depth;

  }

}


// the way we specify targets is the opposite of how we store them:
// "item(depth, 0)" is the most recently targetted item at that depth,
// so, we use "used_ - 1 - t" as the appropriate target index
gkg::HitTarget& 
gkg::HitImplementation::getItem( int32_t depth, gkg::Glyph::Index index )
{

  if ( ( index < 0 ) || ( index >= _items.used ) )
  {

    std::cerr << "internal error: list index " << index << " out of range"
              << std::endl;
    ::abort();

  }
  gkg::Glyph::Index target = _items.used - 1 - index;
  gkg::HitTargetList& targetList = _items.lists[ target ];
  if ( depth >= targetList.available )
  {

    int32_t newAvailable = depth + HIT_TARGET_LIST_FIXED;
    gkg::HitTarget* newTargets = new gkg::HitTarget[ newAvailable ];
    memcpy( newTargets, targetList.targets,
           ( targetList.used + 1 ) * sizeof( gkg::HitTarget ) );
    if ( targetList.targets != targetList.fixedTargets )
    {

      delete [] targetList.targets;

    }
    gkg::HitTarget* i;
    for ( i = &newTargets[ targetList.available ];
          i < &newTargets[ newAvailable ]; i++ )
    {

      i->glyph = 0;

    }
    targetList.available = newAvailable;
    targetList.targets = newTargets;

  }
  if ( depth > targetList.used )
  {

    targetList.used = depth;

  }
  return targetList.targets[ depth ];

}


void gkg::HitImplementation::initialize( gkg::FloatCoordinate x1,
                                         gkg::FloatCoordinate y1,
                                         gkg::FloatCoordinate x2,
                                         gkg::FloatCoordinate y2 )
{

  _items.available = HIT_LIST_FIXED;
  _items.used = 0;
  _items.lists = _items.fixedLists;
  _picks.available = HIT_STACK_FIXED;
  _picks.used = 0;
  _picks.possibleTargets = _picks.fixedPossibleTargets;
  _areas.available = HIT_TARGET_AREA_LIST_FIXED;
  _areas.used = 1;
  _areas.areas = _areas.fixedAreas;
  gkg::HitTargetArea& area = _areas.areas[ 0 ];
  area.left = x1;
  area.bottom = y1;
  area.right = x2;
  area.top = y2;

}


