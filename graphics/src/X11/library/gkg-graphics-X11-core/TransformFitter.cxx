#include <gkg-graphics-X11-core/TransformFitter.h>
#include <gkg-processing-algobase/Math.h>


gkg::TransformFitter::TransformFitter( Glyph* glyph )
                     : gkg::TransformSetter( glyph )
{
}


gkg::TransformFitter::~TransformFitter()
{
}


void gkg::TransformFitter::transform(
                              gkg::Transformer& transformer,
                              const gkg::GeometryAllocation& geometryAllocation,
                              const gkg::GeometryAllocation& natural ) const
{

  const gkg::GeometryAllotment& naturalX = natural.getAllotmentX();
  const gkg::GeometryAllotment& naturalY = natural.getAllotmentY();
  if ( !gkg::equal( naturalX.getSpan(), ( gkg::FloatCoordinate )0, 1e-2f ) &&
       !gkg::equal( naturalY.getSpan(), ( gkg::FloatCoordinate )0, 1e-2f ) )
  {

    transformer.scale( geometryAllocation.getAllotmentX().getSpan() /
                       naturalX.getSpan(),
                       geometryAllocation.getAllotmentY().getSpan() /
                       naturalY.getSpan() );

  }
  transformer.translate( geometryAllocation.getX(), geometryAllocation.getY() );

}
