#include <gkg-graphics-X11-core/GeometryAllocTableImplementation.h>
#include <gkg-graphics-X11-core/GeometryAllocTable.h>
#include <gkg-graphics-X11-core/GeometryAllocationInfo.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/GeometryAllocation.h>
#include <gkg-processing-algobase/Math.h>
#include <cassert>


const float gkg::GeometryAllocTableImplementation::_epsilon = 1e-4;


gkg::GeometryAllocTableImplementation::GeometryAllocTableImplementation(
                                            gkg::Glyph::Index count,
                                            int32_t maximumGeometryAllocations )
                     : _count( count ),
                       _maximumGeometryAllocations( maximumGeometryAllocations )
{
}


gkg::GeometryAllocTableImplementation::~GeometryAllocTableImplementation()
{
}


// find an element on the list that matches the given canvas and allocation
gkg::GeometryAllocationInfo*
gkg::GeometryAllocTableImplementation::find(
                             gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation )
{

  std::deque< gkg::GeometryAllocationInfo* >::iterator
    i = _geometryAllocations.begin(), ie = _geometryAllocations.end();

  while ( i != ie )
  {

    gkg::GeometryAllocationInfo* info = *i;
    if ( ( info->getCanvas() == canvas ) &&
         ( !canvas || ( info->getTransformer() ==canvas->getTransformer() ) ) &&
         gkg::GeometryAllocTableImplementation::equal(
                           info->getGeometryAllocation(), geometryAllocation ) )
    {

      if ( _geometryAllocations.size() > 1 )
      {

        _geometryAllocations.erase( i );
        _geometryAllocations.push_back( info );

      }
      return info;

    }
    ++ i;

  }
  return 0;

}


// find an element on the list with the same canvas and same size
// allocation as given.  If found, set the difference in the position
// of the new allocation's origin to dx and dy.
gkg::GeometryAllocationInfo*
gkg::GeometryAllocTableImplementation::findSameSize(
                            gkg::Canvas* canvas,
                            const gkg::GeometryAllocation& geometryAllocation,
                            gkg::FloatCoordinate& dx, gkg::FloatCoordinate& dy )
{

  const gkg::GeometryAllotment& x = geometryAllocation.getAllotmentX();
  const gkg::GeometryAllotment& y = geometryAllocation.getAllotmentY();

  std::deque< gkg::GeometryAllocationInfo* >::iterator
    i = _geometryAllocations.begin(), ie = _geometryAllocations.end();

  while ( i != ie )
  {

    gkg::GeometryAllocationInfo* info = *i;
    if ( info->getCanvas() == canvas &&
         ( !canvas || ( info->getTransformer() == canvas->getTransformer() ) ) )
    {

      gkg::GeometryAllotment& 
        oldX = info->getGeometryAllocation().getAllotmentX();
      gkg::GeometryAllotment& 
        oldY = info->getGeometryAllocation().getAllotmentY();

      if ( gkg::GeometryAllocTableImplementation::sameSize( x, oldX ) &&
           gkg::GeometryAllocTableImplementation::sameSize( y, oldY ) )
      {

        dx = x.getOrigin() - oldX.getOrigin();
        dy = y.getOrigin() - oldY.getOrigin();
        oldX.setOrigin( x.getOrigin() );
        oldY.setOrigin( y.getOrigin() );
        if ( _geometryAllocations.size() > 1 )
        {

          _geometryAllocations.erase( i );
          _geometryAllocations.push_back( info );

        }
        return info;

      }

    }
    ++ i;

  }
  return 0;

}


// allocate a new table entry for the given canvas and allocation.
// if the table is full, then use the first (least recently used) entry.
gkg::GeometryAllocationInfo*
gkg::GeometryAllocTableImplementation::allocate(
                             gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation )
{

  gkg::GeometryAllocationInfo* info = 0;

  if ( ( int32_t )_geometryAllocations.size() < _maximumGeometryAllocations )
  {

    info = new gkg::GeometryAllocationInfo;
    if ( _count == 0 )
    {

      info->setComponentGeometryAllocations( 0 );

    }
    else
    {

      info->setComponentGeometryAllocations(
                 new gkg::GeometryAllocation[ _count ] );

    }

  }
  else
  {

    info = _geometryAllocations[ 0 ];
    _geometryAllocations.erase( _geometryAllocations.begin() );

  }
  info->setCanvas( canvas );
  assert( canvas ); // canvas should never be null.. see comments in IV
  info->setTransformer( canvas->getTransformer() );
  info->setGeometryAllocation( geometryAllocation );

  _geometryAllocations.push_back( info );

  return info;

}


// return the most recently used allocation information, if any
gkg::GeometryAllocationInfo* 
gkg::GeometryAllocTableImplementation::getMostRecent() const
{

  if ( !_geometryAllocations.empty() )
  {

    return _geometryAllocations.back();

  }
  return 0;

}


// flush all the entries from the table
void gkg::GeometryAllocTableImplementation::flush()
{

  std::deque< gkg::GeometryAllocationInfo* >::iterator
    i = _geometryAllocations.begin(), ie = _geometryAllocations.end();

  while ( i != ie )
  {

    gkg::GeometryAllocationInfo* info = *i;
    delete info;
    ++ i;

  }
  _geometryAllocations.clear();

}


bool gkg::GeometryAllocTableImplementation::equal(
                                       const gkg::GeometryAllocation& a1,
                                       const gkg::GeometryAllocation& a2 )
{

  return a1.isEqualTo( a2, _epsilon );

}


bool gkg::GeometryAllocTableImplementation::sameSize(
                                       const gkg::GeometryAllotment& a1,
                                       const gkg::GeometryAllotment& a2 )
{

  return gkg::equal( a1.getSpan(), a2.getSpan(), _epsilon ) &&
         gkg::equal( a1.getAlignment(), a2.getAlignment(), _epsilon );

}
