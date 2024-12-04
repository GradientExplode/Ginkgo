#include <gkg-graphics-X11-core/BoxImplementation.h>
#include <gkg-graphics-X11-core/Box.h>
#include <gkg-graphics-X11-core/Layout.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/GeometryAllocationInfo.h>
#include <gkg-graphics-X11-core/GeometryAllocTable.h>


gkg::BoxImplementation::BoxImplementation( gkg::Box* box, gkg::Layout* layout )
                       : _target( box ),
                         _layout( layout ),
                         _requested( false ),
                         _geometryAllocations( 0 )
{
}


gkg::BoxImplementation::~BoxImplementation()
{

  delete _layout;
  delete _geometryAllocations;

}


gkg::Box* gkg::BoxImplementation::getTarget() const
{

  return _target;

}


bool gkg::BoxImplementation::getRequested() const
{

  return _requested;

}


const gkg::GeometryRequisition& 
gkg::BoxImplementation::getGeometryRequisition() const
{

  return _geometryRequisition;

}


void gkg::BoxImplementation::request()
{

  gkg::Glyph::Index count = _target->getCount();
  gkg::GeometryRequisition*
    geometryRequisitions = new gkg::GeometryRequisition[ count ];

  for ( gkg::Glyph::Index i = 0; i < count; i++ )
  {

    gkg::Glyph* glyph = _target->getComponent( i );
    if ( glyph )
    {

      glyph->request( geometryRequisitions[ i ] );

    }

  }
  _layout->request( count, geometryRequisitions, _geometryRequisition );
  _requested = true;
  delete [] geometryRequisitions;

}


gkg::GeometryAllocationInfo&  
gkg::BoxImplementation::getInfo(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  if ( !_geometryAllocations )
  {

    _geometryAllocations = new gkg::GeometryAllocTable( _target->getCount());

  }

  gkg::GeometryAllocationInfo*
    info = _geometryAllocations->find( canvas, geometryAllocation );
  if ( !info )
  {

    gkg::FloatCoordinate dx, dy;
    info = _geometryAllocations->findSameSize( canvas, geometryAllocation,
                                               dx, dy );
    if ( info )
    {

      info->setGeometryExtension( geometryExtension );
      offsetAllocate( *info, dx, dy );

    }
    else
    {

      info = _geometryAllocations->allocate( canvas, geometryAllocation );
      info->setGeometryExtension( geometryExtension );
      fullAllocate( *info );

    }

  }
  geometryExtension = info->getGeometryExtension();
  return *info;

}


void gkg::BoxImplementation::offsetAllocate( gkg::GeometryAllocationInfo& info,
                                             gkg::FloatCoordinate dx,
                                             gkg::FloatCoordinate dy )
{

  gkg::Canvas* canvas = info.getCanvas();
  gkg::GeometryAllocation*
    geometryAllocations = info.getComponentGeometryAllocations();
  gkg::GeometryExtension& box = info.getGeometryExtension();
  gkg::GeometryExtension child;
  gkg::Glyph::Index count = _target->getCount();
  for ( gkg::Glyph::Index i = 0; i < count; i++ )
  {

    gkg::Glyph* glyph = _target->getComponent( i );
    if ( glyph )
    {

      gkg::GeometryAllocation& ai = geometryAllocations[ i ];
      gkg::GeometryAllotment& ax = ai.getAllotmentX();
      gkg::GeometryAllotment& ay = ai.getAllotmentY();
      ax.addOffset( dx );
      ay.addOffset( dy );
      child.clear();
      glyph->allocate( canvas, ai, child );
      box.merge( child );

    }

  }

}


void gkg::BoxImplementation::fullAllocate( gkg::GeometryAllocationInfo& info )
{

  gkg::Canvas* canvas = info.getCanvas();
  gkg::Glyph::Index count = _target->getCount();
  gkg::GeometryAllocation*
    geometryAllocations = info.getComponentGeometryAllocations();
  gkg::GeometryRequisition*
    geometryRequisitions = new gkg::GeometryRequisition[ count ];
  gkg::Glyph::Index i;
  for ( i = 0; i < count; i++ )
  {

    gkg::Glyph* glyph = _target->getComponent( i );
    if ( glyph )
    {

      glyph->request( geometryRequisitions[ i ] );

    }

  }
  _layout->allocate( info.getGeometryAllocation(), count,
                     geometryRequisitions, geometryAllocations );
  delete [] geometryRequisitions;

  gkg::GeometryExtension& box = info.getGeometryExtension();
  gkg::GeometryExtension child;
  for ( i = 0; i < count; i++ )
  {

    gkg::Glyph* glyph = _target->getComponent( i );
    if ( glyph )
    {

      child.clear();
      glyph->allocate( canvas, geometryAllocations[ i ], child );
      box.merge( child );

    }

  }

}


void gkg::BoxImplementation::invalidate()
{

  _requested = false;
  delete _geometryAllocations;
  _geometryAllocations = 0;

}


void gkg::BoxImplementation::undraw()
{

  if ( _geometryAllocations  )
  {

    _geometryAllocations->flush();

  }

}


void gkg::BoxImplementation::getGeometryAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  if ( _geometryAllocations )
  {

    gkg::GeometryAllocationInfo*
      info = _geometryAllocations->getMostRecent();
    if ( info )
    {

      gkg::GeometryAllocation*
        geometryAllocations = info->getComponentGeometryAllocations();
      geometryAllotment = geometryAllocations[ index ].getAllotment( 
                                                      dimensionName );

    }

  }

}

