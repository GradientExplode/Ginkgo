#ifndef _gkg_graphics_X11_core_Compositor_h_
#define _gkg_graphics_X11_core_Compositor_h_


#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


// find breaks
class Compositor
{

  public:

    typedef int32_t Index;

    enum Penalty
    {

      PenaltyBad = 10000,
      PenaltyGood = -10000

    };

    Compositor();
    virtual ~Compositor();

    virtual Index compose( FloatCoordinate* naturals,
                           FloatCoordinate* stretchs,
                           FloatCoordinate* shrinks,
                           int32_t* penalties, Index componentCount,
                           FloatCoordinate* spans, Index spanCount,
                           Index* breaks, Index breakCount );

};


}


#endif
