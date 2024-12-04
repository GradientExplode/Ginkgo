#include <gkg-graphics-X11-core/Image.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Raster.h>


gkg::Image::Image( const gkg::Raster* raster )
           : gkg::Glyph(),
             _raster( raster )
{

  if ( _raster )
  {

    _raster->addReference();

  }

}


gkg::Image::~Image()
{

  if ( _raster )
  {

    _raster->removeReference();

  }

}


void gkg::Image::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( _raster )
  {

    gkg::FloatCoordinate leftBearing = _raster->getLeftBearing();
    gkg::FloatCoordinate rightBearing = _raster->getRightBearing();
    gkg::FloatCoordinate ascent = _raster->getAscent();
    gkg::FloatCoordinate descent = _raster->getDescent();
    gkg::GeometryRequirement rx( leftBearing, leftBearing, leftBearing,
                                 rightBearing, rightBearing, rightBearing );
    gkg::GeometryRequirement ry( descent, descent, descent,
                                 ascent, ascent, ascent );
    geometryRequisition.setRequirement( gkg::DimensionX, rx );
    geometryRequisition.setRequirement( gkg::DimensionY, ry );

  }

}


void gkg::Image::allocate( gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& geometryExtension )
{

  if ( _raster )
  {

    gkg::FloatCoordinate x = geometryAllocation.getX();
    gkg::FloatCoordinate y = geometryAllocation.getY();
    geometryExtension.setXY( canvas,
                             x - _raster->getLeftBearing(),
                             y - _raster->getDescent(),
                             x + _raster->getRightBearing(),
                             y + _raster->getAscent() );

  }

}


void gkg::Image::draw( gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _raster )
  {

    canvas->image( _raster,
                   geometryAllocation.getX(),
                   geometryAllocation.getY() );

  }

}
