#ifndef _gkg_dmri_fiber_measure_FiberMeasure_h_
#define _gkg_dmri_fiber_measure_FiberMeasure_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Curve3d.h>


namespace gkg
{


template < class M >
class FiberMeasure : public RCObject
{

  public:

    typedef LightCurve3d< float > Fiber;

    virtual ~FiberMeasure();

    virtual M get( const Fiber& fiber ) const = 0;

  protected:

    FiberMeasure();

};


}


#endif
