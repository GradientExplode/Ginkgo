#ifndef _gkg_graphics_X11_core_PolyGlyphImplementation_h_
#define _gkg_graphics_X11_core_PolyGlyphImplementation_h_


#include <deque>
#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class PolyGlyph;


class PolyGlyphImplementation
{

  public:

    PolyGlyphImplementation( PolyGlyph* polyGlyph );
    virtual ~PolyGlyphImplementation();

    virtual void undraw();

    virtual void append( Glyph* other );
    virtual void prepend( Glyph* other );
    virtual void insert( Glyph::Index index, Glyph* other );
    virtual void remove( Glyph::Index index );
    virtual void replace( Glyph::Index index, Glyph* other );

    virtual Glyph::Index getCount() const;
    virtual Glyph* getComponent( Glyph::Index index ) const;

  private:

    PolyGlyph* _target;
    std::deque< Glyph* > _components;

};


}


#endif
