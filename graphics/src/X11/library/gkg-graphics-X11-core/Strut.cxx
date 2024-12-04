#include <gkg-graphics-X11-core/Strut.h>
#include <gkg-graphics-X11-core/Font.h>


gkg::Strut::Strut( const gkg::Font* font,
                   gkg::FloatCoordinate natural,
                   gkg::FloatCoordinate stretch,
                   gkg::FloatCoordinate shrink )
           : gkg::Glyph(),
             _font( font ),
             _natural( natural ),
             _stretch( stretch ),
             _shrink( shrink )
{

  if ( _font )
  {

    _font->addReference();

  }
  if ( _font )
  {

    gkg::FontBoundingBox b;
    _font->getFontBoundingBox( b );
    _height = b.ascent + b.descent;
    _alignment = ( _height == 0 ) ? 0 : b.descent / _height;

  }

}


gkg::Strut::~Strut()
{

  if ( _font )
  {

    _font->removeReference();

  }

}


void 
gkg::Strut::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  gkg::GeometryRequirement rx( _natural, _stretch, _shrink, 0 );
  gkg::GeometryRequirement ry( _height, 0, 0, _alignment );
  geometryRequisition.setRequirement( gkg::DimensionX, rx );
  geometryRequisition.setRequirement( gkg::DimensionY, ry );

}


void gkg::Strut::allocate( gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& geometryExtension )
{

  geometryExtension.merge( canvas, geometryAllocation );

}

