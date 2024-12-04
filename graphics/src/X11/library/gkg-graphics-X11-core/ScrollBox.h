#ifndef _gkg_graphics_X11_core_ScrollBox_h_
#define _gkg_graphics_X11_core_ScrollBox_h_


#include <gkg-graphics-X11-core/Adjustable.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>


namespace gkg
{


// scrollable list of glyphs
class ScrollBox : public PolyGlyph, public Adjustable
{

  protected:

    ScrollBox();
    virtual ~ScrollBox();

    virtual bool isShown( Glyph::Index index ) const;
    virtual Glyph::Index getFirstShown() const;
    virtual Glyph::Index getLastShown() const;

};


}


#endif
