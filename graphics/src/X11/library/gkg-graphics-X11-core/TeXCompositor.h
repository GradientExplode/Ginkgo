#ifndef _gkg_graphics_X11_core_TeXCompositor_h_
#define _gkg_graphics_X11_core_TeXCompositor_h_


#include <gkg-graphics-X11-core/Compositor.h>


namespace gkg
{


class TeXCompositor : public Compositor
{

  public:

    TeXCompositor( int32_t penalty );
    virtual ~TeXCompositor();

    virtual Compositor::Index
      compose( FloatCoordinate* naturals,
               FloatCoordinate* stretchs,
               FloatCoordinate* shrinks,
               int32_t* penalties, Compositor::Index componentCount,
               FloatCoordinate* spans, Compositor::Index spanCount,
               Compositor::Index* breaks, Compositor::Index breakCount );

  private:

    int32_t _penalty;

};


}


#endif
