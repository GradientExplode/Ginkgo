#include <gkg-graphics-X11-core/Space.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Canvas.h>


gkg::Space::Space( int32_t count,
                   float each,
                   const gkg::Font* font,
                   const gkg::Color* color )
           : gkg::Glyph(),
             _count( count ),
             _each( each ),
             _font( font ),
             _color( color )
{

  if ( _font )
  {

    _font->addReference();

  }
  if ( _color )
  {

    _color->addReference();

  }
  if ( _font )
  {

    gkg::FontBoundingBox b;
    _font->getFontBoundingBox( b );
    _width = _font->getWidth(' ') * _each * _count;
    _height = b.ascent + b.descent;
    _alignment = ( _height == 0 ) ? 0 : b.descent / _height;

  }
  else
  {

    _width = 0;
    _height = 0;
    _alignment = 0;

  }

}


gkg::Space::~Space()
{

  if ( _font )
  {

    _font->removeReference();

  }
  if ( _color )
  {

    _color->removeReference();

  }

}


void gkg::Space::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  gkg::GeometryRequirement rx( _width, _width * 4, _width / 3, 0 );
  gkg::GeometryRequirement ry( _height, 0, 0, _alignment );
  geometryRequisition.setRequirement( gkg::DimensionX, rx );
  geometryRequisition.setRequirement( gkg::DimensionY, ry );

}


void gkg::Space::allocate( gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& geometryExtension )
{

  geometryExtension.merge( canvas, geometryAllocation );

}


void gkg::Space::draw( gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _count > 0 )
  {

    gkg::FloatCoordinate x = geometryAllocation.getX();
    gkg::FloatCoordinate y = geometryAllocation.getY();
    gkg::FloatCoordinate each = ( geometryAllocation.getRight() - 
                                geometryAllocation.getLeft() ) / _count;
    for ( int32_t i = 0; i < _count; i++ )
    {

      canvas->character( _font, ' ', each, _color, x, y );
      x += each;

    }

  }

}


void gkg::Space::pick( gkg::Canvas*,
                       const gkg::GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       gkg::Hit& hit )
{

  gkg::FloatCoordinate x = hit.getLeft();
  gkg::FloatCoordinate left = geometryAllocation.getLeft();
  gkg::FloatCoordinate right = geometryAllocation.getRight();
  if ( ( x >= left ) && ( x < right ) )
  {

    hit.setTarget( depth, this, ( x > ( left + right ) / 2 ) ? 1 : 0 );

  }

}


