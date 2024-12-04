#ifndef _gkg_dmri_fiber_selector_MeasureBasedFiberSelector_i_h_
#define _gkg_dmri_fiber_selector_MeasureBasedFiberSelector_i_h_


#include <gkg-dmri-fiber-selector/MeasureBasedFiberSelector.h>
#include <gkg-dmri-fiber-measure/FiberMeasure_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-core-exception/Exception.h>


template < class M >
inline
gkg::MeasureBasedFiberSelector< M >::MeasureBasedFiberSelector(
                        gkg::RCPointer< gkg::FiberMeasure< M > > fiberMeasure,
                        gkg::RCPointer< gkg::TestFunction< M > > testFunction )
                                    : gkg::FiberSelector(),
                                      _fiberMeasure( fiberMeasure ),
                                      _testFunction( testFunction )
{
}


template < class M >
inline
gkg::MeasureBasedFiberSelector< M >::~MeasureBasedFiberSelector()
{
}


template < class M >
inline
bool gkg::MeasureBasedFiberSelector< M >::select(
        const typename gkg::MeasureBasedFiberSelector< M >::Fiber& fiber ) const
{

  try
  {

    return _testFunction->getState( _fiberMeasure->get( fiber ) );

  }
  GKG_CATCH( "template < class M > "
             "inline "
             "bool gkg::MeasureBasedFiberSelector< M >::select( "
             "const typename gkg::MeasureBasedFiberSelector< M >::Fiber& "
             "fiber ) const" );

}



#endif
