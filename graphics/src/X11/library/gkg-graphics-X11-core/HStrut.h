#ifndef _gkg_graphics_X11_core_HStrut_h_
#define _gkg_graphics_X11_core_HStrut_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class HStrut : public Glyph
{

  public:

    HStrut( FloatCoordinate rightBearing,
            FloatCoordinate leftBearing = 0,
            FloatCoordinate natural = 0,
            FloatCoordinate stretch = 0,
            FloatCoordinate shrink = 0 );
    virtual ~HStrut();

    virtual void request( GeometryRequisition& geometryRequisition ) const;

  private:

    FloatCoordinate _rightBearing;
    FloatCoordinate _leftBearing;
    FloatCoordinate _natural;
    FloatCoordinate _stretch;
    FloatCoordinate _shrink;

};


}


#endif
