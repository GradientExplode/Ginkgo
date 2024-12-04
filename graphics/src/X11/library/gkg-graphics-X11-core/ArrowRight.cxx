#include <gkg-graphics-X11-core/ArrowRight.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


gkg::ArrowRight::ArrowRight( const gkg::Color* color )
                : _color( color )
{

  if ( _color )
  {

    _color->addReference();

  }

} 


gkg::ArrowRight::~ArrowRight()
{

  if ( _color )
  {

    _color->removeReference();

  }

}


void gkg::ArrowRight::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::FloatCoordinate x1 = geometryAllocation.getLeft();
  gkg::FloatCoordinate y1 = geometryAllocation.getBottom();
  gkg::FloatCoordinate x2 = geometryAllocation.getRight();
  gkg::FloatCoordinate y2 = geometryAllocation.getTop();
  canvas->newPath();
  canvas->moveTo( x1, y1 );
  canvas->lineTo( x1, y2 );
  canvas->lineTo( x2, ( y1 + y2 ) * 0.5 );
  canvas->closePath();
  canvas->fill( _color );

}
