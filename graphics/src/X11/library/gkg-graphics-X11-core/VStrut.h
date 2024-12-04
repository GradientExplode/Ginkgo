#ifndef _gkg_graphics_X11_core_VStrut_h_
#define _gkg_graphics_X11_core_VStrut_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class VStrut : public Glyph
{

  public:

    VStrut( FloatCoordinate ascent,
            FloatCoordinate descent = 0,
            FloatCoordinate natural = 0,
            FloatCoordinate stretch = 0,
            FloatCoordinate shrink = 0 );
    virtual ~VStrut();

    virtual void request( GeometryRequisition& geometryRequisition ) const;

  private:

    FloatCoordinate _ascent;
    FloatCoordinate _descent;
    FloatCoordinate _natural;
    FloatCoordinate _stretch;
    FloatCoordinate _shrink;

};


}


#endif
