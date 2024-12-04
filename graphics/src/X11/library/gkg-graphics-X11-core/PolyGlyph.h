#ifndef _gkg_graphics_X11_core_PolyGlyph_h_
#define _gkg_graphics_X11_core_PolyGlyph_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class PolyGlyphImplementation;


// list of glyphs
class PolyGlyph : public Glyph
{

  public:

    PolyGlyph();
    virtual ~PolyGlyph();

    virtual void undraw();

    virtual void append( Glyph* other );
    virtual void prepend( Glyph* other );
    virtual void insert( Glyph::Index index, Glyph* other );
    virtual void remove( Glyph::Index index );
    virtual void replace( Glyph::Index index, Glyph* other );
    virtual void change( Glyph::Index index );

    virtual Glyph::Index getCount() const;
    virtual Glyph* getComponent( Glyph::Index index ) const;

    virtual void modified( Glyph::Index index );

    PolyGlyphImplementation* getImplementation() const;

  private:

    PolyGlyphImplementation* _polyGlyphImplementation;

};


}


#endif
