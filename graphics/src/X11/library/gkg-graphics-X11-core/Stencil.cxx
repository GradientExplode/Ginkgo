#include <gkg-graphics-X11-core/Stencil.h>
#include <gkg-graphics-X11-core/Bitmap.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>


gkg::Stencil::Stencil( const gkg::Bitmap* mask, const gkg::Color* color )
             : gkg::Glyph(),
               _mask( mask ),
               _color( color )
{

  if ( _mask )
  {

    _mask->addReference();

  }
  if ( _color )
  {

    _color->addReference();

  }

}


gkg::Stencil::~Stencil()
{

  if ( _mask )
  {

    _mask->removeReference();

  }
  if ( _color )
  {

    _color->removeReference();

  }

}


void 
gkg::Stencil::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( _mask )
  {

    gkg::FloatCoordinate leftBearing = _mask->getLeftBearing();
    gkg::FloatCoordinate rightBearing = _mask->getRightBearing();
    gkg::FloatCoordinate ascent = _mask->getAscent();
    gkg::FloatCoordinate descent = _mask->getDescent();
    gkg::GeometryRequirement rx( leftBearing, leftBearing, leftBearing,
                                 rightBearing, rightBearing, rightBearing );
    gkg::GeometryRequirement ry( descent, descent, descent,
                                 ascent, ascent, ascent );
    geometryRequisition.setRequirement( gkg::DimensionX, rx );
    geometryRequisition.setRequirement( gkg::DimensionY, ry );

  }

}


void gkg::Stencil::allocate( gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation,
                             gkg::GeometryExtension& geometryExtension )
{

  if ( _mask )
  {

    gkg::FloatCoordinate x = geometryAllocation.getX();
    gkg::FloatCoordinate y = geometryAllocation.getY();
    geometryExtension.setXY( canvas,
                             x - _mask->getLeftBearing(),
                             y - _mask->getDescent(),
                             x + _mask->getRightBearing(),
                             y + _mask->getAscent() );

  }

}

void
gkg::Stencil::draw( gkg::Canvas* canvas,
                    const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _mask )
  {

    canvas->stencil( _mask, _color,
                     geometryAllocation.getX(), geometryAllocation.getY() );

  }

}
