#include <gkg-graphics-X11-core/Border.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Printer.h>


gkg::Border::Border( gkg::Glyph* body, const gkg::Color* color )
            : gkg::MonoGlyph( body ),
              _color( color ),
              _thickness( 1 )
{

  if ( _color )
  {

    _color->addReference();

  }

}


gkg::Border::Border( gkg::Glyph* body, const gkg::Color* color,
                     gkg::FloatCoordinate thickness )
            : gkg::MonoGlyph( body ),
              _color( color ),
              _thickness( thickness )
{

  if ( _color )
  {

    _color->addReference();

  }

}


gkg::Border::~Border()
{

  if ( _color )
  {

    _color->removeReference();

  }

}


void gkg::Border::allocate( gkg::Canvas* canvas,
                            const gkg::GeometryAllocation& geometryAllocation,
                            gkg::GeometryExtension& geometryExtension )
{

  this->gkg::MonoGlyph::allocate( canvas,
                                  geometryAllocation, geometryExtension );
  geometryExtension.merge( canvas, geometryAllocation );

}


void gkg::Border::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::FloatCoordinate left = geometryAllocation.getLeft();
  gkg::FloatCoordinate bottom = geometryAllocation.getBottom();
  gkg::FloatCoordinate right = geometryAllocation.getRight();
  gkg::FloatCoordinate top = geometryAllocation.getTop();
  gkg::FloatCoordinate t = _thickness;

  canvas->fillRectangle( left, bottom + t, left + t, top, _color );
  canvas->fillRectangle( left + t, top - t, right, top, _color );
  canvas->fillRectangle( right - t, top - t, right, bottom, _color );
  canvas->fillRectangle( right - t, bottom + t, left, bottom, _color );

  this->gkg::MonoGlyph::draw( canvas, geometryAllocation );

}


void gkg::Border::print(
                       gkg::Printer* printer,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::FloatCoordinate left = geometryAllocation.getLeft();
  gkg::FloatCoordinate bottom = geometryAllocation.getBottom();
  gkg::FloatCoordinate right = geometryAllocation.getRight();
  gkg::FloatCoordinate top = geometryAllocation.getTop();
  gkg::FloatCoordinate t = _thickness;

  printer->fillRectangle( left, bottom + t, left + t, top, _color );
  printer->fillRectangle( left + t, top - t, right, top, _color );
  printer->fillRectangle( right - t, top - t, right, bottom, _color );
  printer->fillRectangle( right - t, bottom + t, left, bottom, _color );

  this->gkg::MonoGlyph::print( printer, geometryAllocation );

}
