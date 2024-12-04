#include <gkg-graphics-X11-core/Label.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Hit.h>


gkg::Label::Label( const std::string& text,
                   const gkg::Font* font,
                   const gkg::Color* color )
           : gkg::Glyph(),
             _text( text ),
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
  computeMetrics();

}


gkg::Label::~Label()
{

  if ( _font )
  {

    _font->removeReference();

  }
  if ( _color )
  {

    _color->removeReference();

  }
  delete [] _charWidths;

}


void gkg::Label::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  gkg::FloatCoordinate height = _ascent + _descent;
  float alignment = ( height == 0 ) ? 0 : _descent / height;
  gkg::GeometryRequirement rx( _width, 0, 0, 0 );
  gkg::GeometryRequirement ry( height, 0, 0, alignment );
  geometryRequisition.setRequirement( gkg::DimensionX, rx );
  geometryRequisition.setRequirement( gkg::DimensionY, ry );

}


void gkg::Label::allocate( gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& geometryExtension )
{

  gkg::FloatCoordinate x = geometryAllocation.getX();
  gkg::FloatCoordinate y = geometryAllocation.getY();
  geometryExtension.setXY( canvas,
                           x - _left,
                           y - _descent,
                           x + _right,
                           y + _ascent );

}


void gkg::Label::draw( gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::FloatCoordinate x = geometryAllocation.getX();
  gkg::FloatCoordinate y = geometryAllocation.getY();
  const char* p = _text.c_str();
  const char* q = &p[ _text.length() ];
  gkg::FloatCoordinate* cw = &_charWidths[ 0 ];
  for ( ; p < q; p++, cw++ )
  {

    gkg::FloatCoordinate width = *cw;
    canvas->character( _font, *p, width, _color, x, y );
    x += width;

  }

}


void gkg::Label::pick( gkg::Canvas*,
                       const gkg::GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       gkg::Hit& hit )
{

  gkg::FloatCoordinate x = hit.getLeft();
  if ( ( hit.getRight() >= geometryAllocation.getLeft() ) &&
       ( x < geometryAllocation.getRight() ) &&
       ( hit.getTop() >= geometryAllocation.getBottom() ) &&
       ( hit.getBottom() < geometryAllocation.getTop() ) )
  {

    int32_t index = _font->getIndex( _text, x - geometryAllocation.getX(), 
                                     true );
    hit.setTarget( depth, this, index );

  }

}


void gkg::Label::computeMetrics()
{

  int32_t len = _text.length();

  gkg::FontBoundingBox boundingBox;
  _font->getStringBoundingBox( _text, boundingBox );
  _ascent = boundingBox.fontAscent;
  _descent = boundingBox.fontDescent;
  _left = boundingBox.leftBearing;
  _right = boundingBox.rightBearing;
  _width = boundingBox.width;
  _charWidths = new gkg::FloatCoordinate[ len ];
  for ( int32_t i = 0; i < len; i++ )
  {

    _charWidths[ i ] = _font->getWidth( ( char )_text[i] );

  }

}

