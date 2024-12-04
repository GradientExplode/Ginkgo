#ifndef _gkg_dmri_fiber_selector_MeasureBasedFiberSelector_h_
#define _gkg_dmri_fiber_selector_MeasureBasedFiberSelector_h_


#include <gkg-dmri-fiber-selector/FiberSelector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-fiber-measure/FiberMeasure.h>
#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class M >
class MeasureBasedFiberSelector : public FiberSelector
{

  public:

    typedef FiberSelector::Fiber Fiber;

    MeasureBasedFiberSelector( RCPointer< FiberMeasure< M > > fiberMeasure,
                               RCPointer< TestFunction< M > > testFunction );
    virtual ~MeasureBasedFiberSelector();

    bool select( const Fiber& fiber ) const;

  protected:

    RCPointer< FiberMeasure< M > > _fiberMeasure;
    RCPointer< TestFunction< M > > _testFunction;


};


}


#endif
