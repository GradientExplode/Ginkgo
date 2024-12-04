#include <gkg-graphics-X11-core/Aggregate.h>
#include <gkg-graphics-X11-core/AggregateImplementation.h>


gkg::Aggregate::Aggregate( gkg::Glyph::Index size )
               : gkg::Glyph()
{

  _aggregateImplementation = new gkg::AggregateImplementation( this, size );

}


gkg::Aggregate::~Aggregate()
{

  delete _aggregateImplementation;

}


void gkg::Aggregate::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  _aggregateImplementation->allocate( canvas,
                                      geometryAllocation,
                                      geometryExtension );

}


void gkg::Aggregate::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  _aggregateImplementation->draw( canvas, geometryAllocation );

}


void gkg::Aggregate::print(
                       gkg::Printer* printer,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  _aggregateImplementation->print( printer, geometryAllocation );

}


void gkg::Aggregate::pick( gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           int32_t depth,
                           Hit& hit )
{

  _aggregateImplementation->pick( canvas, geometryAllocation, depth, hit );

}


void gkg::Aggregate::append( gkg::Glyph* other )
{

  _aggregateImplementation->append( other );

}


void gkg::Aggregate::prepend( gkg::Glyph* other )
{

  _aggregateImplementation->prepend( other );

}


void gkg::Aggregate::insert( gkg::Glyph::Index index, gkg::Glyph* other )
{

  _aggregateImplementation->insert( index, other );

}



void gkg::Aggregate::remove( gkg::Glyph::Index index )
{

  _aggregateImplementation->remove( index );

}


void gkg::Aggregate::replace( gkg::Glyph::Index index, gkg::Glyph* other )
{

  _aggregateImplementation->replace( index, other );

}


void gkg::Aggregate::change( gkg::Glyph::Index )
{
}


gkg::Glyph::Index gkg::Aggregate::getCount() const
{

  return _aggregateImplementation->getCount();

}


gkg::Glyph* gkg::Aggregate::getComponent( gkg::Glyph::Index index ) const
{

  return _aggregateImplementation->getComponent( index );

}


void gkg::Aggregate::setAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               const gkg::GeometryAllotment& geometryAllotment )
{

  return _aggregateImplementation->setAllotment( index,
                                                 dimensionName,
                                                 geometryAllotment );

}


void gkg::Aggregate::getGeometryAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  return _aggregateImplementation->getGeometryAllotment( index,
                                                         dimensionName,
                                                         geometryAllotment );

}


gkg::AggregateImplementation* gkg::Aggregate::getImplementation() const
{

    return _aggregateImplementation;

}
