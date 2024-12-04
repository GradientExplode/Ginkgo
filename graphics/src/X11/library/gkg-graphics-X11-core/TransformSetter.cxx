#include <gkg-graphics-X11-core/TransformSetter.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Printer.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-processing-algobase/Math.h>


//
// static functions
//

static void 
computeGeometryRequisition( gkg::GeometryRequirement& geometryRequirement,
                            gkg::FloatCoordinate first,
                            gkg::FloatCoordinate last )
{

  gkg::FloatCoordinate natural = last - first;
  geometryRequirement.setNatural( natural );
  geometryRequirement.setStretch( 0.0 );
  geometryRequirement.setShrink( 0.0 );
  if ( gkg::equal( natural, 0.0f, 1e-3f ) )
  {

    geometryRequirement.setAlignment( 0.0 );

  }
  else
  {

    geometryRequirement.setAlignment( -first / natural );

  }

}


//
// class TransformSetter
//

gkg::TransformSetter::TransformSetter( gkg::Glyph* glyph )
                     : gkg::MonoGlyph( glyph )
{
}


gkg::TransformSetter::TransformSetter( gkg::Glyph* glyph,
                                       const gkg::Transformer& transformer )
                     : gkg::MonoGlyph( glyph ),
                       _transformer( transformer )
{
}


gkg::TransformSetter::~TransformSetter()
{
}


const gkg::Transformer& gkg::TransformSetter::getTransformer() const
{

  return _transformer;

}


gkg::Transformer& gkg::TransformSetter::getTransformer()
{

  return _transformer;

}


void gkg::TransformSetter::setTransformer( const gkg::Transformer& transformer )
{

  _transformer = transformer;

}


void gkg::TransformSetter::request(
                           gkg::GeometryRequisition& geometryRequisition ) const
{

  this->gkg::MonoGlyph::request( geometryRequisition );

  gkg::GeometryRequirement& rx = geometryRequisition.getRequirementX();
  // hack to be suppressed
  gkg::GeometryAllotment& 
    ax = ( ( gkg::TransformSetter* )this )->_natural.getAllotmentX();
  ax.setOrigin( 0.0 );
  ax.setSpan( rx.getNatural() );
  ax.setAlignment( rx.getAlignment() );

  gkg::GeometryRequirement& ry = geometryRequisition.getRequirementY();
  // hack to be suppressed
  gkg::GeometryAllotment&
    ay = ( ( gkg::TransformSetter* )this )->_natural.getAllotmentY();
  ay.setOrigin( 0.0 );
  ay.setSpan( ry.getNatural() );
  ay.setAlignment( ry.getAlignment() );

  gkg::FloatCoordinate left = ax.getBegin();
  gkg::FloatCoordinate bottom = ay.getBegin();
  gkg::FloatCoordinate right = ax.getEnd();
  gkg::FloatCoordinate top = ay.getEnd();
  gkg::FloatCoordinate x1, y1, x2, y2, x3, y3, x4, y4;
  _transformer.transform( left, bottom, x1, y1 );
  _transformer.transform( left, top, x2, y2 );
  _transformer.transform( right, top, x3, y3 );
  _transformer.transform( right, bottom, x4, y4 );
  left = gkg::min( x1, x2, x3, x4 );
  bottom = gkg::min( y1, y2, y3, y4 );
  right = gkg::max( x1, x2, x3, x4 );
  top = gkg::max( y1, y2, y3, y4 );

  computeGeometryRequisition( rx, left, right );
  computeGeometryRequisition( ry, bottom, top );

}


void gkg::TransformSetter::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{


  // shouldn't need to test for nil canvas
  if ( canvas )
  {

    pushTransform( canvas, geometryAllocation, _natural );
    this->gkg::MonoGlyph::allocate( canvas, _natural, geometryExtension );
    canvas->popTransform();

  }

}


void gkg::TransformSetter::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  pushTransform( canvas, geometryAllocation, _natural );
  this->gkg::MonoGlyph::draw( canvas, _natural );
  canvas->popTransform();

}


void gkg::TransformSetter::print(
                       gkg::Printer* printer,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  pushTransform( printer, geometryAllocation, _natural );
  this->gkg::MonoGlyph::draw( printer, _natural );
  printer->popTransform();

}


void gkg::TransformSetter::pick(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              int32_t depth,
                              gkg::Hit& hit )
{

  gkg::Transformer transformer( _transformer );
  transform( transformer, geometryAllocation, _natural );
  canvas->pushTransform();
  canvas->transform( transformer );
  hit.pushTransform();
  hit.setTransform( transformer );
  this->gkg::MonoGlyph::pick( canvas, _natural, depth, hit );
  canvas->popTransform();
  hit.popTransform();

}


void gkg::TransformSetter::pushTransform(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              const gkg::GeometryAllocation& natural ) const
{

  gkg::Transformer transformer( _transformer );
  transform( transformer, geometryAllocation, natural );
  canvas->pushTransform();
  canvas->transform( transformer );

}


void gkg::TransformSetter::transform(
                              gkg::Transformer& transformer,
                              const gkg::GeometryAllocation& geometryAllocation,
                              const gkg::GeometryAllocation& ) const
{

  transformer.translate( geometryAllocation.getX(), geometryAllocation.getY() );

}
