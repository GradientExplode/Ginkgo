#include <gkg-graphics-X11-core/Brush.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-processing-algobase/Math.h>
#include <cstdio>
#include "Plot.h"


static const float minSize = 10.0;


Plot::Plot( int32_t& count, float* x, float* y,
            const gkg::Color* color, const gkg::Brush* brush )
{

  _count = &count;
  _x = x;
  _y = y;
  _color = color;
  if ( _color )
  {

    _color->addReference();

  }
  _brush = brush;
  if ( _brush )
  {

    _brush->addReference();

  }

}


Plot::~Plot()
{

  if ( _color )
  {

    _color->removeReference();

  }
  if ( _brush )
  {

    _brush->removeReference();

  }

}


void Plot::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  this->gkg::Glyph::request( geometryRequisition );
  gkg::GeometryRequirement& rx = geometryRequisition.getRequirementX();
  gkg::GeometryRequirement& ry = geometryRequisition.getRequirementY();
  rx.setNatural( minSize );
  rx.setStretch( fil );
  rx.setShrink( minSize );
  rx.setAlignment( 0.0 );
  ry.setNatural( minSize );
  ry.setStretch( fil );
  ry.setShrink( minSize );
  ry.setAlignment( 1.0 );

}


void Plot::allocate( gkg::Canvas* canvas,
                     const gkg::GeometryAllocation& geometryAllocation,
                     gkg::GeometryExtension& geometryExtension )
{

  geometryExtension.merge( canvas, geometryAllocation );

}


void Plot::draw( gkg::Canvas* canvas,
                 const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::FloatCoordinate l = geometryAllocation.getLeft();
  gkg::FloatCoordinate r = geometryAllocation.getRight();
  gkg::FloatCoordinate b = geometryAllocation.getBottom();
  gkg::FloatCoordinate t = geometryAllocation.getTop();
  for ( int32_t i = 0; i < *_count; i++ )
  {

    gkg::FloatCoordinate x = l * ( 1 - _x[ i ] ) + r * _x[ i ];
    gkg::FloatCoordinate y = b * ( 1 - _y[ i ] ) + t * _y[ i ];
    canvas->moveTo( x - 1, y - 1 );
    canvas->lineTo( x - 1, y + 1 );
    canvas->lineTo( x + 1, y + 1 );
    canvas->lineTo( x + 1, y - 1 );
    canvas->lineTo( x - 1, y - 1 );
    canvas->stroke( _color, _brush );

  }

}


void Plot::pick( gkg::Canvas*,
                 const gkg::GeometryAllocation& geometryAllocation,
                 int32_t depth,
                 gkg::Hit& hit )
{

  const gkg::Event* event = hit.getEvent();
  gkg::FloatCoordinate x = event->getPointerX();
  gkg::FloatCoordinate y = event->getPointerY();
  gkg::FloatCoordinate l = geometryAllocation.getLeft();
  gkg::FloatCoordinate r = geometryAllocation.getRight();
  gkg::FloatCoordinate b = geometryAllocation.getBottom();
  gkg::FloatCoordinate t = geometryAllocation.getTop();
  for ( int32_t i = 0; i < *_count; i++ )
  {

    gkg::FloatCoordinate dx = x - ( l * ( 1 - _x[ i ] ) + r * _x[ i ] );
    gkg::FloatCoordinate dy = y - ( b * ( 1 - _y[ i ] ) + t * _y[ i ] );
    if ( gkg::abs( dx ) < 5 && gkg::abs( dy ) < 5 )
    {

      hit.setTarget( depth, this, i );

    }

  }

}
