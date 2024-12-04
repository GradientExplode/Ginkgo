#include <gkg-graphics-X11-core/Box.h>
#include <gkg-graphics-X11-core/BoxImplementation.h>
#include <gkg-graphics-X11-core/Layout.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Printer.h>
#include <gkg-graphics-X11-core/GeometryAllocationInfo.h>
#include <gkg-graphics-X11-core/Hit.h>


gkg::Box::Box( gkg::Layout* layout,
               gkg::Glyph* g1, gkg::Glyph* g2,
               gkg::Glyph* g3, gkg::Glyph* g4,
               gkg::Glyph* g5, gkg::Glyph* g6,
               gkg::Glyph* g7, gkg::Glyph* g8,
               gkg::Glyph* g9, gkg::Glyph* g10 )
         : gkg::PolyGlyph()
{

  _boxImplementation = new gkg::BoxImplementation( this, layout );
  if ( g1 )
  {

    append( g1 );

  }
  if ( g2 )
  {

    append( g2 );

  }
  if ( g3 )
  {

    append( g3 );

  }
  if ( g4 )
  {

    append( g4 );

  }
  if ( g5 )
  {

    append( g5 );

  }
  if ( g6 )
  {

    append( g6 );

  }
  if ( g7 )
  {

    append( g7 );

  }
  if ( g8 )
  {

    append( g8 );

  }
  if ( g9 )
  {

    append( g9 );

  }
  if ( g10 )
  {

    append( g10 );

  }

}


gkg::Box::~Box()
{

  delete _boxImplementation;

}


void 
gkg::Box::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( !_boxImplementation->getRequested() )
  {

    _boxImplementation->request();

  }
  geometryRequisition = _boxImplementation->getGeometryRequisition();

}


void 
gkg::Box::allocate( gkg::Canvas* canvas,
                    const gkg::GeometryAllocation& geometryAllocation,
                    gkg::GeometryExtension& geometryExtension )
{

  _boxImplementation->getInfo( canvas, geometryAllocation, geometryExtension );

}


void 
gkg::Box::draw( gkg::Canvas* canvas,
                const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::GeometryExtension geometryExtension;
  geometryExtension.clear();
  gkg::GeometryAllocationInfo& 
    info = _boxImplementation->getInfo( canvas,
                                        geometryAllocation,
                                        geometryExtension);
  if ( canvas->isDamaged( geometryExtension ) )
  {

    gkg::GeometryAllocation*
      geometryAllocations = info.getComponentGeometryAllocations();
    gkg::Glyph::Index count = getCount();
    for ( gkg::Glyph::Index i = 0; i < count; i++ )
    {

      gkg::Glyph* glyph = getComponent( i );
      if ( glyph )
      {

        glyph->draw( canvas, geometryAllocations[ i ] );

      }

    }

  }

}


void gkg::Box::print( gkg::Printer* printer,
                      const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::GeometryExtension geometryExtension;
  geometryExtension.clear();
  gkg::GeometryAllocationInfo& 
    info = _boxImplementation->getInfo( printer,
                                        geometryAllocation,
                                        geometryExtension);
  if ( printer->isDamaged( geometryExtension ) )
  {

    gkg::GeometryAllocation*
      geometryAllocations = info.getComponentGeometryAllocations();
    gkg::Glyph::Index count = getCount();
    for ( gkg::Glyph::Index i = 0; i < count; i++ )
    {

      gkg::Glyph* glyph = getComponent( i );
      if ( glyph )
      {

        glyph->print( printer, geometryAllocations[ i ] );

      }

    }

  }

}


void gkg::Box::pick( gkg::Canvas* canvas,
                     const gkg::GeometryAllocation& geometryAllocation,
                     int32_t depth, gkg::Hit& hit )
{


  if ( hit.getRight() >= geometryAllocation.getLeft() &&
       hit.getLeft() < geometryAllocation.getRight() &&
       hit.getTop() >= geometryAllocation.getBottom() &&
       hit.getBottom() < geometryAllocation.getTop() )

  {

    gkg::GeometryExtension geometryExtension;
    geometryExtension.clear();
    gkg::GeometryAllocationInfo& 
      info = _boxImplementation->getInfo( canvas,
                                          geometryAllocation,
                                          geometryExtension);
    gkg::GeometryAllocation*
      geometryAllocations = info.getComponentGeometryAllocations();
    gkg::Glyph::Index count = getCount();
    for ( gkg::Glyph::Index i = 0; i < count; i++ )
    {

      gkg::Glyph* glyph = getComponent( i );
      if ( glyph )
      {

        hit.begin( depth, this, i );
        glyph->pick( canvas, geometryAllocations[ i ], depth + 1, hit );
        hit.end();

      }

    }

  }

}


void gkg::Box::undraw()
{

  _boxImplementation->undraw();
  this->gkg::PolyGlyph::undraw();

}


void gkg::Box::modified( gkg::Glyph::Index )
{

  _boxImplementation->invalidate();

}


void gkg::Box::getGeometryAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  _boxImplementation->getGeometryAllotment( index, dimensionName,
                                            geometryAllotment );

}


gkg::BoxImplementation* gkg::Box::getImplementation() const
{

  return _boxImplementation;

}
