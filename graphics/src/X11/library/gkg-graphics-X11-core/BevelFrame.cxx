#include <gkg-graphics-X11-core/BevelFrame.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Printer.h>


gkg::BevelFrame::BevelFrame( gkg::Glyph* glyph,
                             gkg::FloatCoordinate thickness,
                             float xAlign, float yAlign,
                             bool hMargin, bool vMargin )
                : gkg::MonoGlyph( glyph ),
                  _thickness( thickness ),
                  _xAlign( xAlign ),
                  _yAlign( yAlign ),
                  _hMargin( hMargin ),
                  _vMargin( vMargin )
{
}


gkg::BevelFrame::~BevelFrame()
{
}


void gkg::BevelFrame::request(
                           gkg::GeometryRequisition& geometryRequisition ) const
{

  gkg::Glyph* glyph = getBody();
  if ( glyph )
  {

    glyph->request( geometryRequisition );
    if ( _hMargin || _vMargin )
    {

      gkg::FloatCoordinate thickness = _thickness + _thickness;
      gkg::GeometryRequirement& rx = geometryRequisition.getRequirementX();
      if ( _hMargin && rx.isDefined() )
      {

        rx.setNatural( rx.getNatural() + thickness );

      }
      gkg::GeometryRequirement& ry = geometryRequisition.getRequirementY();
      if ( _vMargin && ry.isDefined() )
      {

        ry.setNatural( ry.getNatural() + thickness );

      }

    }

  }

}


void gkg::BevelFrame::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  gkg::Glyph* glyph = getBody();
  if ( glyph )
  {

    if ( _hMargin || _vMargin )
    {

      gkg::GeometryAllocation interior( geometryAllocation );
      allocateBody( glyph, getThickness( canvas ), interior );
      glyph->allocate( canvas, interior, geometryExtension );

    }
    else
    {

      glyph->allocate( canvas, geometryAllocation, geometryExtension );

    }

  }
  geometryExtension.merge( canvas, geometryAllocation);

}


void gkg::BevelFrame::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::FloatCoordinate thickness = getThickness( canvas );
  drawFrame( canvas, geometryAllocation, thickness );
  gkg::Glyph* glyph = getBody();
  if ( glyph )
  {

    if ( _hMargin || _vMargin )
    {

      gkg::GeometryAllocation interior( geometryAllocation );
      allocateBody( glyph, thickness, interior );
      glyph->draw( canvas, interior );

    }
    else
    {

      glyph->draw( canvas, geometryAllocation);

    }

  }

}


void gkg::BevelFrame::print(
                       gkg::Printer* printer,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::FloatCoordinate thickness = getThickness( printer );
  drawFrame( printer, geometryAllocation, thickness );
  gkg::Glyph* glyph = getBody();
  if ( glyph )
  {

    if ( _hMargin || _vMargin )
    {

      gkg::GeometryAllocation interior( geometryAllocation );
      allocateBody( glyph, thickness, interior );
      glyph->print( printer, interior );

    }
    else
    {

      glyph->print( printer, geometryAllocation);

    }

  }

}


void gkg::BevelFrame::pick( gkg::Canvas* canvas,
                            const gkg::GeometryAllocation& geometryAllocation,
                            int32_t depth,
                            gkg::Hit& hit )
{

  gkg::Glyph* glyph = getBody();
  if ( glyph )
  {

    if ( _hMargin || _vMargin )
    {

      gkg::GeometryAllocation interior( geometryAllocation );
      allocateBody( glyph, getThickness( canvas ), interior );
      glyph->pick( canvas, interior, depth, hit );

    }
    else
    {

      glyph->pick( canvas, geometryAllocation, depth, hit );

    }

  }

}


void gkg::BevelFrame::drawFrame( gkg::Canvas*,
                                 const gkg::GeometryAllocation&,
                                 gkg::FloatCoordinate ) const
{
}


gkg::FloatCoordinate 
gkg::BevelFrame::getThickness( gkg::Canvas* canvas ) const
{

  return canvas ? canvas->toPixelThenFloatCoordinate( _thickness ) : _thickness;

}


void gkg::BevelFrame::allocateBody(
                             gkg::Glyph* glyph,
                             gkg::FloatCoordinate thickness,
                             gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::GeometryRequisition geometryRequisition;
  glyph->request( geometryRequisition );
  gkg::FloatCoordinate h = _hMargin ? thickness : 0;
  gkg::FloatCoordinate v = _vMargin ? thickness : 0;

  gkg::GeometryAllotment& ax = geometryAllocation.getAllotmentX();
  gkg::FloatCoordinate xSpan = ax.getSpan() - h - h;
  gkg::FloatCoordinate xOffset = h;
  gkg::FloatCoordinate xAlign = ax.getAlignment();
  const gkg::GeometryRequirement& rx = geometryRequisition.getRequirementX();
  if ( rx.isDefined() )
  {

    gkg::FloatCoordinate xUsable = rx.getNatural() + rx.getStretch();
    if ( xSpan > xUsable )
    {

      xOffset += _xAlign * ( xSpan - xUsable );
      xSpan = xUsable;

    }

  }
  ax.setSpan( xSpan );
  ax.addOffset( xOffset * ( 1 - xAlign - xAlign ) );

  gkg::GeometryAllotment& ay = geometryAllocation.getAllotmentY();
  gkg::FloatCoordinate ySpan = ay.getSpan() - v - v;
  gkg::FloatCoordinate yOffset = v;
  gkg::FloatCoordinate yAlign = ay.getAlignment();
  const gkg::GeometryRequirement& ry = geometryRequisition.getRequirementY();
  if ( ry.isDefined() )
  {

    gkg::FloatCoordinate yUsable = ry.getNatural() + ry.getStretch();
    if ( ySpan > yUsable )
    {

      yOffset += _yAlign * ( ySpan - yUsable );
      ySpan = yUsable;

    }

  }
  ay.setSpan( ySpan );
  ay.addOffset( yOffset * ( 1 - yAlign - yAlign ) );

}

