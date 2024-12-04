#ifndef _gkg_dmri_bundle_selector_MeasureBasedBundleSelector_h_
#define _gkg_dmri_bundle_selector_MeasureBasedBundleSelector_h_


#include <gkg-dmri-bundle-selector/BundleSelector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-bundle-measure/BundleMeasure.h>
#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class M >
class MeasureBasedBundleSelector : public BundleSelector
{

  public:

    typedef BundleSelector::Bundle Bundle;

    MeasureBasedBundleSelector( RCPointer< BundleMeasure< M > > bundleMeasure,
                                RCPointer< TestFunction< M > > testFunction );
    virtual ~MeasureBasedBundleSelector();

    bool select( const Bundle& bundle ) const;

  protected:

    RCPointer< BundleMeasure< M > > _bundleMeasure;
    RCPointer< TestFunction< M > > _testFunction;


};


}


#endif
