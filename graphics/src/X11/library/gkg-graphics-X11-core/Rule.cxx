#include <gkg-graphics-X11-core/Rule.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>


gkg::Rule::Rule( gkg::DimensionName dimensionName,
                 const gkg::Color* color,
                 gkg::FloatCoordinate thickness )
          : Glyph(),
            _dimensionName( dimensionName ),
            _color( color ),
            _thickness( thickness )
{

  if ( _color )
  {

    _color->addReference();

  }

}


gkg::Rule::~Rule()
{

  if ( _color )
  {

    _color->removeReference();

  }

}


void 
gkg::Rule::request( gkg::GeometryRequisition& geometryRequisition ) const 
{

  gkg::GeometryRequirement geometryRequirement( _thickness, 0, 0, 0 );
  geometryRequisition.setRequirement( _dimensionName, geometryRequirement );

}


void gkg::Rule::allocate( gkg::Canvas* canvas,
                          const gkg::GeometryAllocation& geometryAllocation,
                          gkg::GeometryExtension& geometryExtension )
{

  geometryExtension.set( canvas, geometryAllocation );

}


void gkg::Rule::draw( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation ) const
{

  canvas->fillRectangle( geometryAllocation.getLeft(),
                         geometryAllocation.getBottom(),
                         geometryAllocation.getRight(),
                         geometryAllocation.getTop(),
                         _color );

}

