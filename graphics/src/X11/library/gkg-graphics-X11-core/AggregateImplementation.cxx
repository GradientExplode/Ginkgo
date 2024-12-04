#include <gkg-graphics-X11-core/AggregateImplementation.h>
#include <gkg-graphics-X11-core/Aggregate.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Printer.h>
#include <gkg-graphics-X11-core/Hit.h>


//
// class AggregateImplementation::AggregateInfo
//
gkg::AggregateImplementation::AggregateInfo::AggregateInfo()
                                            : glyph( 0 )
{
}


//
// class AggregateImplementation
//
gkg::AggregateImplementation::AggregateImplementation( 
                                                      gkg::Aggregate* aggregate,
                                                      gkg::Glyph::Index size )
                             : _target( aggregate ),
                               _informations( size )
{
}


gkg::AggregateImplementation::~AggregateImplementation()
{

  std::deque< gkg::AggregateImplementation::AggregateInfo >::iterator
    i = _informations.begin(), ie = _informations.end();
  while ( i != ie )
  {

    if ( ( *i ).glyph )
    {

      ( ( *i ).glyph )->removeReference();

    }
    ++ i;

  }
  _informations.clear();

}


gkg::Aggregate* gkg::AggregateImplementation::getTarget() const
{

  return _target;

}


void gkg::AggregateImplementation::allocate(
                                     gkg::Canvas* canvas,
                                     const gkg::GeometryAllocation&,
                                     gkg::GeometryExtension& geometryExtension )
{

  std::deque< gkg::AggregateImplementation::AggregateInfo >::iterator
    i = _informations.begin(), ie = _informations.end();
  while ( i != ie )
  {

    gkg::AggregateImplementation::AggregateInfo& info = *i;
    if ( info.glyph )
    {

      gkg::GeometryAllocation& a = info.geometryAllocation;
      gkg::GeometryExtension& e = info.geometryExtension;
      e.clear();
      info.glyph->allocate( canvas, a, e );
      geometryExtension.merge( e );

    }
    ++ i;

  }

}


void gkg::AggregateImplementation::draw( gkg::Canvas* canvas,
                                         const gkg::GeometryAllocation& ) const
{

  std::deque< gkg::AggregateImplementation::AggregateInfo >::const_iterator
    i = _informations.begin(), ie = _informations.end();
  while ( i != ie )
  {

    const gkg::AggregateImplementation::AggregateInfo& info = *i;
    if ( info.glyph )
    {

      if ( canvas->isDamaged( info.geometryExtension ) )
        info.glyph->draw( canvas, info.geometryAllocation );

    }
    ++ i;

  }

}


void gkg::AggregateImplementation::print( gkg::Printer* printer,
                                          const gkg::GeometryAllocation& ) const
{

  std::deque< gkg::AggregateImplementation::AggregateInfo >::const_iterator
    i = _informations.begin(), ie = _informations.end();
  while ( i != ie )
  {

    const gkg::AggregateImplementation::AggregateInfo& info = *i;
    if ( info.glyph )
    {

      if ( printer->isDamaged( info.geometryExtension ) )
        info.glyph->print( printer, info.geometryAllocation );

    }
    ++ i;

  }

}


void gkg::AggregateImplementation::pick( gkg::Canvas* canvas,
                                         const gkg::GeometryAllocation&,
                                         int32_t depth, Hit& hit )
{

  gkg::FloatCoordinate x = hit.getLeft();
  gkg::FloatCoordinate y = hit.getBottom();
  std::deque< gkg::AggregateImplementation::AggregateInfo >::iterator
    i = _informations.begin(), ie = _informations.end();
  gkg::Glyph::Index index = 0;
  while ( i != ie )
  {

    gkg::AggregateImplementation::AggregateInfo& info = *i;
    if ( info.glyph )
    {

      gkg::GeometryAllocation& a = info.geometryAllocation;
      gkg::GeometryExtension& e = info.geometryExtension;
      if ( ( x >= e.getLeft() ) &&
           ( x < e.getRight() ) &&
           ( y >= e.getBottom() ) &&
           ( y < e.getTop() ) )
      {

        hit.begin( depth, _target, index );
        info.glyph->pick( canvas, a, depth + 1, hit );
        hit.end();

      }

    }
    ++ i;
    ++ index;

  }

}


void gkg::AggregateImplementation::append( gkg::Glyph* other )
{

  gkg::AggregateImplementation::AggregateInfo info;
  info.glyph = other;
  _informations.push_back( info );
  if ( other )
  {

    other->addReference();

  }

}


void gkg::AggregateImplementation::prepend( gkg::Glyph* other )
{

  gkg::AggregateImplementation::AggregateInfo info;
  info.glyph = other;
  _informations.push_front( info );
  if ( other )
  {

    other->addReference();

  }

}


void gkg::AggregateImplementation::insert( gkg::Glyph::Index index,
                                           gkg::Glyph* other )
{

  gkg::AggregateImplementation::AggregateInfo info;
  info.glyph = other;
  _informations.insert( _informations.begin() + index, info );
  if ( other )
  {

    other->addReference();

  }

}



void gkg::AggregateImplementation::remove( gkg::Glyph::Index index )
{

  if ( _informations[ index ].glyph )
  {

    _informations[ index ].glyph->removeReference();

  }
  _informations.erase( _informations.begin() + index );

}


void gkg::AggregateImplementation::replace( gkg::Glyph::Index index,
                                               gkg::Glyph* other )
{

  if ( other )
  {

    other->addReference();

  }
  if ( _informations[ index ].glyph )
  {

    _informations[ index ].glyph->removeReference();

  }
  _informations[ index ].glyph = other;

}


gkg::Glyph::Index gkg::AggregateImplementation::getCount() const
{

  return ( gkg::Glyph::Index )_informations.size();

}


gkg::Glyph* 
gkg::AggregateImplementation::getComponent( gkg::Glyph::Index index ) const
{

  return _informations[ index  ].glyph;

}


void gkg::AggregateImplementation::setAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               const gkg::GeometryAllotment& geometryAllotment )
{

  _informations[ index ].geometryAllocation.setAllotment( dimensionName,
                                                          geometryAllotment );

}


void gkg::AggregateImplementation::getGeometryAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  geometryAllotment = _informations[ index ].geometryAllocation.getAllotment(
                                                               dimensionName );

}
