#ifndef _gkg_graphics_X11_core_TBComposition_h_
#define _gkg_graphics_X11_core_TBComposition_h_


#include <gkg-graphics-X11-core/Composition.h>


namespace gkg
{


class TBComposition : public Composition
{

  public:

    TBComposition( Glyph* context,
                   Compositor* compositor,
                   Glyph* separator,
                   FloatCoordinate height,
                   FloatCoordinate stretch = fil,
                   FloatCoordinate shrink = fil,
                   Glyph::Index size = 10 );
    virtual ~TBComposition();

  protected:

    virtual Glyph* make( Break& theBreak );

};


}


#endif

