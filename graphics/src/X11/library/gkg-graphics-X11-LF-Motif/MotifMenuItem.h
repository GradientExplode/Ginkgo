#ifndef _gkg_graphics_X11_LF_Motif_MotifMenuItem_h_
#define _gkg_graphics_X11_LF_Motif_MotifMenuItem_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <gkg-graphics-X11-core/TellTale.h>


namespace gkg
{


class MotifMenuItem : public MonoGlyph
{

  public:

    MotifMenuItem( Glyph* glyph, TellTaleState* tellTaleState );
    ~MotifMenuItem();

    void draw( Canvas* canvas,
               const GeometryAllocation& geometryAllocation ) const;
    void pick( Canvas* canvas,
               const GeometryAllocation& geometryAllocation,
               int32_t depth,
               Hit& hit );

  private:

    TellTaleState* _tellTaleState;

};


}


#endif
