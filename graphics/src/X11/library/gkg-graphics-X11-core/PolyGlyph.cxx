#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/PolyGlyphImplementation.h>



gkg::PolyGlyph::PolyGlyph()
{

  _polyGlyphImplementation = new gkg::PolyGlyphImplementation( this );

}


gkg::PolyGlyph::~PolyGlyph()
{

  delete _polyGlyphImplementation;

}


void gkg::PolyGlyph::undraw()
{

  _polyGlyphImplementation->undraw();

}


void gkg::PolyGlyph::append( gkg::Glyph* other )
{

  _polyGlyphImplementation->append( other );

}


void gkg::PolyGlyph::prepend( gkg::Glyph* other )
{

  _polyGlyphImplementation->prepend( other );

}


void gkg::PolyGlyph::insert( gkg::Glyph::Index index,
                             gkg::Glyph* other )
{

  _polyGlyphImplementation->insert( index, other );

}


void gkg::PolyGlyph::remove( gkg::Glyph::Index index )
{

  _polyGlyphImplementation->remove( index );

}


void gkg::PolyGlyph::replace( gkg::Glyph::Index index, gkg::Glyph* other )
{

  _polyGlyphImplementation->replace( index, other );

}


void gkg::PolyGlyph::change( gkg::Glyph::Index index )
{

  modified( index );

}


gkg::Glyph::Index gkg::PolyGlyph::getCount() const
{

  return _polyGlyphImplementation->getCount();

}


gkg::Glyph* gkg::PolyGlyph::getComponent( gkg::Glyph::Index index ) const
{

  return _polyGlyphImplementation->getComponent( index );

}


void gkg::PolyGlyph::modified( gkg::Glyph::Index )
{
}


gkg::PolyGlyphImplementation* gkg::PolyGlyph::getImplementation() const
{

  return _polyGlyphImplementation;

}
