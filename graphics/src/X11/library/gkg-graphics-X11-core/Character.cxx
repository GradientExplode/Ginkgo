#include <gkg-graphics-X11-core/Character.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Hit.h>


gkg::Character::Character( int32_t code,
                           const gkg::Font* font,
                           const gkg::Color* color )
               : gkg::Glyph(),
                 _code( code ),
                 _font( font ),
                 _color( color )
{

  doInitialization();

}


gkg::Character::Character( char c,
                           const gkg::Font* font,
                           const gkg::Color* color )
               : gkg::Glyph(),
                 _code( ( int32_t )*( ( unsigned char* )&c ) ),
                 _font( font ),
                 _color( color )
{

  doInitialization();

}


gkg::Character::~Character()
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


int32_t gkg::Character::getCode() const
{

  return _code;

}


void gkg::Character::request(
                           gkg::GeometryRequisition& geometryRequisition ) const
{

  gkg::GeometryRequirement rx( _width, 0, 0, 0 );
  gkg::GeometryRequirement ry( _height, 0, 0, _alignment );
  geometryRequisition.setRequirement( gkg::DimensionX, rx );
  geometryRequisition.setRequirement( gkg::DimensionY, ry );

}


void gkg::Character::allocate(
                           gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& geometryExtension )
{

  gkg::FloatCoordinate x = geometryAllocation.getX();
  gkg::FloatCoordinate y = geometryAllocation.getY();
  geometryExtension.setXY( canvas,
                           x - _leftBearing, y - _descent,
                           x + _rightBearing, y + _ascent );

}


void 
gkg::Character::draw( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation ) const
{

  canvas->character( _font,
                     _code,
                     geometryAllocation.getRight() -
                     geometryAllocation.getLeft(),
                     _color,
                     geometryAllocation.getX(),
                     geometryAllocation.getY() );

}

void gkg::Character::pick( gkg::Canvas*,
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


void gkg::Character::doInitialization()
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

    gkg::FontBoundingBox boundingBox;
    _font->getCharBoundingBox( _code, boundingBox );
    _leftBearing = boundingBox.leftBearing;
    _rightBearing = boundingBox.rightBearing;
    _width = boundingBox.width;
    _ascent = boundingBox.fontAscent;
    _descent = boundingBox.fontDescent;
    _height = _ascent + _descent;
    _alignment = ( _height == 0 ) ? 0 : _descent / _height;

  }
  else
  {

    _leftBearing = 0;
    _rightBearing = 0;
    _ascent = 0;
    _descent = 0;
    _width = 0;
    _height = 0;
    _alignment = 0;

  }

}
