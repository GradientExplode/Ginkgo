#ifndef _gkg_dmri_fiber_selector_DensityMapBasedFiberSelector_h_
#define _gkg_dmri_fiber_selector_DensityMapBasedFiberSelector_h_


#include <gkg-dmri-fiber-selector/FiberSelector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-dmri-fiber-selector/FiberSelection.h>


namespace gkg
{


class DensityMapBasedFiberSelector : public FiberSelector
{

  public:

    typedef FiberSelector::Fiber Fiber;

    DensityMapBasedFiberSelector(
                               RCPointer< Volume< float > > densityMap,
                               RCPointer< Transform3d< float > >
                                     transform3dFromDensityMapRealToFiberReal,
                               RCPointer< TestFunction< float > > testFunction,
                               float minimumIntersectionLength = 1.0,
                               float inputFiberResamplingStep = 0.1 );
    virtual ~DensityMapBasedFiberSelector();

    RCPointer< Volume< float > > getDensityMap() const;
    RCPointer< Transform3d< float > > 
      getTransform3dFromDensityMapVoxelToFiberReal() const;
    RCPointer< TestFunction< float > > getTestFunction() const; 

    bool select( const Fiber& fiber ) const;

    template < class L, class Compare >
    static void selector(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L, std::set< int32_t >, Compare >& selection );

  protected:

    RCPointer< Volume< float > > _densityMap;
    RCPointer< Transform3d< float > >
      _transform3dFromDensityMapVoxelToFiberReal;
    RCPointer< TestFunction< float > > _testFunction;
    float _minimumIntersectionLength;
    float _inputFiberResamplingStep;
    BoundingBox< int32_t > _boundingBox;

};


}


#endif
