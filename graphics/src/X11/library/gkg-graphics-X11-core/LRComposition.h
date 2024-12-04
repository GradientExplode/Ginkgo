#ifndef _gkg_graphics_X11_core_LRComposition_h_
#define _gkg_graphics_X11_core_LRComposition_h_


#include <gkg-graphics-X11-core/Composition.h>


namespace gkg
{


class LRComposition : public Composition
{

  public:

    LRComposition( Glyph* context,
                   Compositor* compositor,
                   Glyph* separator,
                   FloatCoordinate width,
                   FloatCoordinate stretch = fil,
                   FloatCoordinate shrink = fil,
                   Glyph::Index size = 10 );
    virtual ~LRComposition();

  protected:

    virtual Glyph* make( Break& theBreak );

};


}


#endif

