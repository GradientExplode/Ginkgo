#include <gkg-graphics-X11-core/ArrowDown.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


gkg::ArrowDown::ArrowDown( const gkg::Color* color )
               : _color( color )
{

  if ( _color )
  {

    _color->addReference();

  }

} 


gkg::ArrowDown::~ArrowDown()
{

  if ( _color )
  {

    _color->removeReference();

  }

}


void gkg::ArrowDown::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::FloatCoordinate x1 = geometryAllocation.getLeft();
  gkg::FloatCoordinate y1 = geometryAllocation.getBottom();
  gkg::FloatCoordinate x2 = geometryAllocation.getRight();
  gkg::FloatCoordinate y2 = geometryAllocation.getTop();
  canvas->newPath();
  canvas->moveTo( x1, y2 );
  canvas->lineTo( x2, y2 );
  canvas->lineTo( ( x1 + x2 ) * 0.5, y1 );
  canvas->closePath();
  canvas->fill( _color );

}
