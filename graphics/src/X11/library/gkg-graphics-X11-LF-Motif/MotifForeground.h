#ifndef _gkg_graphics_X11_LF_Motif_MotifForeground_h_
#define _gkg_graphics_X11_LF_Motif_MotifForeground_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <gkg-graphics-X11-core/Color.h>


namespace gkg
{


class MotifForeground : public MonoGlyph
{

  public:

    MotifForeground( Glyph* glyph, const Color* color );
    ~MotifForeground();

    void draw( Canvas* canvas,
               const GeometryAllocation& geometryAllocation ) const;

  private:

    const Color* _foreground;

};


}


#endif
