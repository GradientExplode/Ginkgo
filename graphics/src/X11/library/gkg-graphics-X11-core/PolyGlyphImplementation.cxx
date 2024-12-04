#include <gkg-graphics-X11-core/PolyGlyphImplementation.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>


gkg::PolyGlyphImplementation::PolyGlyphImplementation( 
                                                     gkg::PolyGlyph* polyGlyph )
                             : _target( polyGlyph )
{
}


gkg::PolyGlyphImplementation::~PolyGlyphImplementation()
{

  std::deque< gkg::Glyph* >::iterator g = _components.begin(),
                                      ge = _components.end();

  while ( g != ge )
  {

    if ( *g )
    {

      ( *g )->removeReference();

    }
    ++ g;

  }

}


void gkg::PolyGlyphImplementation::undraw()
{

  std::deque< gkg::Glyph* >::iterator g = _components.begin(),
                                      ge = _components.end();

  while ( g != ge )
  {

    gkg::Glyph* glyph = *g;
    if ( glyph )
    {

      glyph->undraw();

    }
    ++ g;

  }

}


void gkg::PolyGlyphImplementation::append( gkg::Glyph* other )
{

  if ( other )
  {

    other->addReference();

  }
  gkg::Glyph::Index index = ( gkg::Glyph::Index )_components.size();
  _components.push_back( other );
  _target->modified( index );

}


void gkg::PolyGlyphImplementation::prepend( gkg::Glyph* other )
{

  if ( other )
  {

    other->addReference();

  }
  _components.push_front( other );
  _target->modified( 0 );

}


void gkg::PolyGlyphImplementation::insert( gkg::Glyph::Index index,
                                           gkg::Glyph* other )
{

  if ( other )
  {

    other->addReference();

  }
  _components.insert( _components.begin() + index, other );
  _target->modified( index );

}


void gkg::PolyGlyphImplementation::remove( gkg::Glyph::Index index )
{

  Glyph* glyph = _components[ index ];
  if ( glyph )
  {

    glyph->undraw();
    glyph->removeReferenceDeferred();

  }
  _components.erase( _components.begin() + index );
  _target->modified( index );

}


void gkg::PolyGlyphImplementation::replace( gkg::Glyph::Index index,
                                            gkg::Glyph* other )
{

  Glyph* oldGlyph = _components[ index ];
  if ( other != oldGlyph )
  {

    if ( other )
    {

      other->addReference();

    }
    if ( oldGlyph )
    {

      oldGlyph->undraw();
      oldGlyph->removeReferenceDeferred();

    }
    _components[ index ] = other;
    _target->modified( index );

  }

}


gkg::Glyph::Index gkg::PolyGlyphImplementation::getCount() const
{

  return ( gkg::Glyph::Index )_components.size();

}


gkg::Glyph* gkg::PolyGlyphImplementation::getComponent( 
                                                 gkg::Glyph::Index index ) const
{

  return _components[ index ];

}

