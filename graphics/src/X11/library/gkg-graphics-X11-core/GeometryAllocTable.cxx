#include <gkg-graphics-X11-core/GeometryAllocTable.h>
#include <gkg-graphics-X11-core/GeometryAllocTableImplementation.h>


gkg::GeometryAllocTable::GeometryAllocTable( gkg::Glyph::Index count,
                                            int32_t maximumGeometryAllocations )
{

  _geometryAllocTableImplementation =
    new gkg::GeometryAllocTableImplementation( count,
                                               maximumGeometryAllocations );

}


gkg::GeometryAllocTable::~GeometryAllocTable()
{

  flush();
  delete _geometryAllocTableImplementation;

}


gkg::GeometryAllocationInfo*
gkg::GeometryAllocTable::find(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  return _geometryAllocTableImplementation->find( canvas, geometryAllocation );

}


gkg::GeometryAllocationInfo*
gkg::GeometryAllocTable::findSameSize(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::FloatCoordinate& dx,
                              gkg::FloatCoordinate& dy ) const
{

  return _geometryAllocTableImplementation->findSameSize( canvas,
                                                          geometryAllocation,
                                                          dx, dy );

}


gkg::GeometryAllocationInfo*
gkg::GeometryAllocTable::allocate(
                             gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation )
{

  return _geometryAllocTableImplementation->allocate( canvas,
                                                      geometryAllocation );

}


gkg::GeometryAllocationInfo* 
gkg::GeometryAllocTable::getMostRecent() const
{

  return _geometryAllocTableImplementation->getMostRecent();

}


void gkg::GeometryAllocTable::flush()
{

  _geometryAllocTableImplementation->flush();

}


gkg::GeometryAllocTableImplementation* 
gkg::GeometryAllocTable::getImplementation() const
{

  return _geometryAllocTableImplementation;

}
