#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Printer.h>


gkg::Glyph::Glyph()
           : gkg::RCGUIObject()
{
}


gkg::Glyph::~Glyph()
{
}


void gkg::Glyph::request( gkg::GeometryRequisition& ) const
{
}


void gkg::Glyph::allocate( gkg::Canvas*,
                           const gkg::GeometryAllocation&,
                           gkg::GeometryExtension& )
{
}


void gkg::Glyph::draw( gkg::Canvas*,
                       const gkg::GeometryAllocation& ) const
{
}


void 
gkg::Glyph::print( gkg::Printer* printer,
                   const gkg::GeometryAllocation& geometryAllocation ) const
{

  draw( printer, geometryAllocation );

}


void gkg::Glyph::pick( gkg::Canvas*,
                       const gkg::GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       gkg::Hit& hit )
{

  gkg::FloatCoordinate x = hit.getLeft();
  gkg::FloatCoordinate y = hit.getBottom();
  if ( ( x >= geometryAllocation.getLeft() ) &&
       ( x < geometryAllocation.getRight() ) &&
       ( y >= geometryAllocation.getBottom() ) &&
       ( y < geometryAllocation.getTop() ) )
  {

    hit.setTarget( depth, this, 0 );

  }

}


void gkg::Glyph::undraw()
{

  gkg::Glyph::Index count = getCount();
  for ( gkg::Glyph::Index i = 0; i < count; i++ )
  {

    gkg::Glyph* glyph = getComponent( i );
    if ( glyph )
    {

      glyph->undraw();

    }

  }

}


gkg::Glyph* gkg::Glyph::clone() const
{

  return 0;

}


gkg::Glyph* gkg::Glyph::compose( gkg::Glyph::BreakType breakType )
{

  return ( ( breakType == gkg::Glyph::NoBreak ) ||
           ( breakType == gkg::Glyph::PreBreak ) ) ? this : 0;

}


void gkg::Glyph::append( gkg::Glyph* )
{
}


void gkg::Glyph::prepend( gkg::Glyph* )
{
}


void gkg::Glyph::insert( gkg::Glyph::Index, gkg::Glyph* )
{
}


void gkg::Glyph::remove( gkg::Glyph::Index )
{
}


void gkg::Glyph::replace( gkg::Glyph::Index, gkg::Glyph* )
{
}


void gkg::Glyph::change( gkg::Glyph::Index )
{
}


gkg::Glyph::Index gkg::Glyph::getCount() const
{

  return 0;

}


gkg::Glyph* gkg::Glyph::getComponent( gkg::Glyph::Index ) const
{

  return 0;

}


void gkg::Glyph::getGeometryAllotment(
                               gkg::Glyph::Index,
                               gkg::DimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  gkg::GeometryAllotment defaultGeometryAllotment;
  geometryAllotment = defaultGeometryAllotment;

}
