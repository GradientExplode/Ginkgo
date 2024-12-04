#ifndef _gkg_dmri_fiber_selector_RoiBasedFiberSelector_h_
#define _gkg_dmri_fiber_selector_RoiBasedFiberSelector_h_


#include <gkg-dmri-fiber-selector/FiberSelector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-dmri-fiber-selector/FiberSelection.h>
#include <gkg-communication-thread/LoopContext.h>


namespace gkg
{


//
// class RoiBasedFiberSelector< T >
//

template < class T >
class RoiBasedFiberSelector : public FiberSelector
{

  public:

    enum Type
    {

      Intersection,
      NoIntersection

    };

    typedef FiberSelector::Fiber Fiber;

    RoiBasedFiberSelector( RCPointer< Volume< T > > rois,
                           RCPointer< Transform3d< float > >
                                 transform3dFromRoisRealToFiberReal,
                           RCPointer< TestFunction< T > > testFunction,
                           Type type,
                           float minimumIntersectionLength = 1.0,
                           float inputFiberResamplingStep = 0.1 );
    virtual ~RoiBasedFiberSelector();

    RCPointer< Volume< T > > getRois() const;
    RCPointer< Transform3d< float > > 
      getTransform3dFromRoisVoxelToFiberReal() const;
    RCPointer< TestFunction< T > > getTestFunction() const; 

    bool select( const Fiber& fiber ) const;

    template < class L, class Compare >
    static void selector(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L, std::set< int32_t >, Compare >& selection );

  protected:

    RCPointer< Volume< T > > _rois;
    RCPointer< TestFunction< T > > _testFunction;
    Type _type;
    float _minimumIntersectionLength;
    float _inputFiberResamplingStep;
    BoundingBox< int32_t > _boundingBox;
    RCPointer< Transform3d< float > > _transform3dFromRoisVoxelToFiberReal;

};


//
// class RoiBasedFiberSelectorContext
//

template < class T, class L, class Compare = std::less< L > >
class RoiBasedFiberSelectorContext : public LoopContext< int32_t >
{

  public:

    RoiBasedFiberSelectorContext(
                      const typename BundleMap< L, Compare >::Bundle& bundle,
                      const RoiBasedFiberSelector< T >& roiBasedFiberSelector,
                      const float& inputFiberResamplingStep,
                      std::set< int32_t >& selection );
    virtual ~RoiBasedFiberSelectorContext();

    void doIt( int32_t startIndex, int32_t count );

  private:

    const typename BundleMap< L, Compare >::Bundle& _bundle;
    const RoiBasedFiberSelector< T >& _roiBasedFiberSelector;
    const float& _inputFiberResamplingStep;
    std::set< int32_t >& _selection;
    
};


}


#endif
