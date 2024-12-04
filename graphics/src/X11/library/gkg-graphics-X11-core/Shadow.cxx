#include <gkg-graphics-X11-core/Shadow.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Printer.h>


gkg::Shadow::Shadow( gkg::Glyph* glyph,
                     gkg::FloatCoordinate x,
                     gkg::FloatCoordinate y,
                     const gkg::Color* color,
                     bool single )
            : gkg::MonoGlyph( glyph ),
              _offsetX( x ),
              _offsetY( y ),
              _color( color ),
             _single( single )
{

  if ( _color )
  {

    _color->addReference();

  }

}


gkg::Shadow::~Shadow()
{

  if ( _color )
  {

    _color->removeReference();

  }

}


void gkg::Shadow::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  this->gkg::MonoGlyph::request( geometryRequisition );
  computeGeometryRequirement( geometryRequisition.getRequirementX(), _offsetX );
  computeGeometryRequirement( geometryRequisition.getRequirementY(), _offsetY );

}


void gkg::Shadow::allocate( gkg::Canvas* canvas,
                            const gkg::GeometryAllocation& geometryAllocation,
                            gkg::GeometryExtension& geometryExtension )
{

  gkg::GeometryAllocation g( geometryAllocation );
  computeGeometryAllocation( g );
  this->gkg::MonoGlyph::allocate( canvas, g, geometryExtension );
  geometryExtension.merge( canvas, geometryAllocation );

}


void 
gkg::Shadow::draw( gkg::Canvas* canvas,
                   const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::GeometryAllocation g( geometryAllocation );
  computeGeometryAllocation( g );
  if ( !_single )
  {

    drawShadow( canvas, g );
    drawBody( canvas, g );
    return;

  }

  gkg::FloatCoordinate gLeft = g.getLeft();
  gkg::FloatCoordinate sLeft = gLeft + _offsetX;
  gkg::FloatCoordinate gBottom = g.getBottom();
  gkg::FloatCoordinate sBottom = gBottom + _offsetY;
  gkg::FloatCoordinate gRight = g.getRight();
  gkg::FloatCoordinate sRight = gRight + _offsetX;
  gkg::FloatCoordinate gTop = g.getTop();
  gkg::FloatCoordinate sTop = gTop + _offsetY;
  gkg::FloatCoordinate x1, y1, x2, y2, x3, y3, x4, y4;
  if ( _offsetX > 0 )
  {

    x1 = sLeft;
    x2 = sRight;
    x3 = gRight;
    x4 = sRight;

  }
  else
  {

    x1 = sLeft;
    x2 = sRight;
    x3 = sLeft;
    x4 = gLeft;

  }
  if ( _offsetY > 0 )
  {

    y1 = gTop;
    y2 = sTop;
    y3 = sBottom;
    y4 = gTop;

  }
  else
  {

    y1 = sBottom;
    y2 = gBottom;
    y3 = gBottom;
    y4 = sTop;

  }
  gkg::GeometryExtension e1, e2;
  e1.setXY( canvas, x1, y1, x2, y2 );
  e2.setXY( canvas, x3, y3, x4, y4 );
  if ( !canvas->isDamaged( e1 ) && !canvas->isDamaged( e2 ) )
  {

    drawBody( canvas, g );
    return;

  }
  canvas->frontBuffer();
  drawShadow( canvas, g );
  canvas->backBuffer();
  gkg::GeometryExtension geometryExtension;
  geometryExtension.set( canvas, g );
  canvas->restrictDamage( geometryExtension );
  drawBody( canvas, g );

}


void 
gkg::Shadow::print( gkg::Printer* printer,
                    const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::GeometryAllocation g( geometryAllocation );
  computeGeometryAllocation( g );
  printer->fillRectangle( g.getLeft() + _offsetX,
                          g.getBottom() + _offsetY,
                          g.getRight() + _offsetX,
                          g.getTop() + _offsetY,
                          _color );
  this->gkg::MonoGlyph::print( printer, g );

}


void gkg::Shadow::pick( gkg::Canvas* canvas,
                        const gkg::GeometryAllocation& geometryAllocation,
                        int32_t depth,
                        gkg::Hit& hit )
{

  gkg::GeometryAllocation g( geometryAllocation );
  computeGeometryAllocation( g );
  this->gkg::MonoGlyph::pick( canvas, g, depth, hit );

}


void gkg::Shadow::computeGeometryRequirement(
                                  gkg::GeometryRequirement& geometryRequirement,
                                  gkg::FloatCoordinate offset ) const
{

  if ( geometryRequirement.isDefined() )
  {

    gkg::FloatCoordinate natural = geometryRequirement.getNatural();
    if ( offset > 0 )
    {

      geometryRequirement.setNatural( natural + offset );
      geometryRequirement.setAlignment( geometryRequirement.getAlignment() * 
                                        natural /
                                        geometryRequirement.getNatural() );

    }
    else
    {

      geometryRequirement.setNatural( natural - offset );
      geometryRequirement.setAlignment( ( geometryRequirement.getAlignment() *
                                          natural - offset ) /
                                        geometryRequirement.getNatural() );

    }

  }

}


void gkg::Shadow::computeGeometryAllocation(
                             gkg::GeometryAllocation& geometryAllocation ) const
{

  computeGeometryAllotment( geometryAllocation.getAllotmentX(), _offsetX );
  computeGeometryAllotment( geometryAllocation.getAllotmentY(), _offsetY );

}


void gkg::Shadow::computeGeometryAllotment(
                                      gkg::GeometryAllotment& geometryAllotment,
                                      gkg::FloatCoordinate offset ) const
{

  gkg::FloatCoordinate span = geometryAllotment.getSpan();
  if ( offset > 0 )
  {

    geometryAllotment.setSpan( span - offset );
    geometryAllotment.setAlignment( geometryAllotment.getAlignment() * span /
                                    geometryAllotment.getSpan() );

  }
  else
  {

    geometryAllotment.setSpan( span + offset );
    geometryAllotment.setAlignment( ( geometryAllotment.getAlignment() *
                                      span + offset ) /
                                    geometryAllotment.getSpan() );

  }

}


void gkg::Shadow::drawShadow(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  canvas->fillRectangle( geometryAllocation.getLeft() + _offsetX,
                         geometryAllocation.getBottom() + _offsetY,
                         geometryAllocation.getRight() + _offsetX,
                         geometryAllocation.getTop() + _offsetY,
                         _color );

}


void gkg::Shadow::drawBody(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  this->gkg::MonoGlyph::draw( canvas, geometryAllocation );

}

 
