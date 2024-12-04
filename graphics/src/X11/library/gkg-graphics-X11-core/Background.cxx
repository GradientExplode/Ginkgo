#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Printer.h>


gkg::Background::Background( gkg::Glyph* body,
                             const gkg::Color* color )
                : gkg::MonoGlyph( body ),
                  _color( color )
{

  if ( _color )
  {

    _color->addReference();

  }

}


gkg::Background::~Background()
{

  if ( _color )
  {

    _color->removeReference();

  }

}


void gkg::Background::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  this->gkg::MonoGlyph::allocate( canvas,
                                  geometryAllocation,
                                  geometryExtension );
  geometryExtension.merge( canvas, geometryAllocation );

}


void gkg::Background::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::GeometryExtension geometryExtension;
  geometryExtension.set( canvas, geometryAllocation );
  if ( canvas->isDamaged( geometryExtension ) )
  {

    canvas->fillRectangle( geometryAllocation.getLeft(),
                           geometryAllocation.getBottom(),
                           geometryAllocation.getRight(),
                           geometryAllocation.getTop(),
                           _color );

  }
  this->gkg::MonoGlyph::draw( canvas, geometryAllocation );

}


void gkg::Background::print(
                       gkg::Printer* printer,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  printer->fillRectangle( geometryAllocation.getLeft(),
                          geometryAllocation.getBottom(),
                          geometryAllocation.getRight(),
                          geometryAllocation.getTop(),
                          _color );
  this->gkg::MonoGlyph::print( printer, geometryAllocation );

}
