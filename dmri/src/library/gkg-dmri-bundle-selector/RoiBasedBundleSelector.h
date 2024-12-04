#ifndef _gkg_dmri_bundle_selector_RoiBasedBundleSelector_h_
#define _gkg_dmri_bundle_selector_RoiBasedBundleSelector_h_


#include <gkg-dmri-bundle-selector/BundleSelector.h>
#include <gkg-dmri-fiber-selector/RoiBasedFiberSelector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class L >
class RoiBasedBundleSelector : public BundleSelector
{

  public:

    enum Type
    {

      Intersection,
      NoIntersection

    };
    typedef BundleSelector::Bundle Bundle;

    RoiBasedBundleSelector( RCPointer< Volume< L > > rois,
                            RCPointer< Transform3d< float > >
                                 transform3dFromRoisVoxelToFiberReal,
                            RCPointer< TestFunction< L > > testFunction,
                            Type type,
                            float minimumIntersectionPercentagePerFiber = 0.0,
                            float minimumIntersectingFiberPercentage = 0.0 );
    virtual ~RoiBasedBundleSelector();

    const RoiBasedFiberSelector< L > getRoiBasedFiberSelector() const;

    bool select( const Bundle& bundle ) const;

  protected:

    RoiBasedFiberSelector< L > _roiBasedFiberSelector;
    Type _type;
    float _minimumIntersectingFiberPercentage;

};


}


#endif
