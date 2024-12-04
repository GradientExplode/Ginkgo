#ifndef _gkg_graphics_X11_core_ArrayCompositor_h_
#define _gkg_graphics_X11_core_ArrayCompositor_h_


#include <gkg-graphics-X11-core/Compositor.h>


namespace gkg
{

class ArrayCompositor : public Compositor
{

  public:

    ArrayCompositor( Compositor::Index count );
    virtual ~ArrayCompositor();

    virtual Compositor::Index
      compose( FloatCoordinate* naturals,
               FloatCoordinate* stretchs,
               FloatCoordinate* shrinks,
               int32_t* penalties, Compositor::Index componentCount,
               FloatCoordinate* spans, Compositor::Index spanCount,
               Compositor::Index* breaks, Compositor::Index breakCount );

  private:

    Compositor::Index _count;

};


}


#endif
