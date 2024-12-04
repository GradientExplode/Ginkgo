#ifndef _gkg_dmri_bundle_selector_MeasureBasedBundleSelector_i_h_
#define _gkg_dmri_bundle_selector_MeasureBasedBundleSelector_i_h_


#include <gkg-dmri-bundle-selector/MeasureBasedBundleSelector.h>
#include <gkg-dmri-bundle-measure/BundleMeasure_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-core-exception/Exception.h>


template < class M >
inline
gkg::MeasureBasedBundleSelector< M >::MeasureBasedBundleSelector(
                       gkg::RCPointer< gkg::BundleMeasure< M > > bundleMeasure,
                       gkg::RCPointer< gkg::TestFunction< M > > testFunction )
                                     : gkg::BundleSelector(),
                                       _bundleMeasure( bundleMeasure ),
                                       _testFunction( testFunction )
{
}


template < class M >
inline
gkg::MeasureBasedBundleSelector< M >::~MeasureBasedBundleSelector()
{
}


template < class M >
inline
bool gkg::MeasureBasedBundleSelector< M >select(
    const typename gkg::MeasureBasedBundleSelector< M >::Bundle& bundle ) const
{

  try
  {

    return _testFunction->getState( _bundleMeasure->get( bundle ) );

  }
  GKG_CATCH( "template < class M > "
             "inline "
             "bool gkg::MeasureBasedBundleSelector< M >::select( "
             "const typename gkg::MeasureBasedBundleSelector< M >::Bundle& "
             "bundle ) const" );

}



#endif
