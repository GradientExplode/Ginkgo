#ifndef _gkg_dmri_fiber_selector_FiberSelector_h_
#define _gkg_dmri_fiber_selector_FiberSelector_h_


#include <gkg-processing-container/Curve3d.h>


namespace gkg
{


class FiberSelector
{

  public:

    typedef LightCurve3d< float > Fiber;

    virtual ~FiberSelector();

    virtual bool select( const Fiber& fiber ) const= 0;

  protected:

    FiberSelector();

};


}


#endif
