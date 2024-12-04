#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorRoiBasedSelection_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorRoiBasedSelection_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-communication-thread/LoopContext.h>
#include <vector>
#include <utility>


namespace gkg
{




//
// class RoiBasedSelectionContext
//

template < class L, class Compare = std::less< L > >
class RoiBasedSelectionContext : public LoopContext< int32_t >
{

  public:

    RoiBasedSelectionContext(
      const typename BundleMap< L, Compare >::Bundle& bundle,
      const float& inputFiberResamplingStep,
      const RCPointer< Volume< int32_t > >& rois,
      const BoundingBox< int32_t >& boundingBox,
      const RCPointer< TestFunction< int32_t > >& testFunction,
      const RCPointer< Transform3d< float > >& 
                                            transform3dFromRoisVoxelToFiberReal,
      int32_t& globalBundlePointCount,
      int32_t& bundleIntersectingPointCount );
    virtual ~RoiBasedSelectionContext();

    void doIt( int32_t startIndex, int32_t count );

  private:

    const typename BundleMap< L, Compare >::Bundle& _bundle;
    const float& _inputFiberResamplingStep;
    const RCPointer< Volume< int32_t > >& _rois;
    const BoundingBox< int32_t >& _boundingBox;
    const RCPointer< TestFunction< int32_t > >& _testFunction;
    const RCPointer< Transform3d< float > >& 
                                           _transform3dFromRoisVoxelToFiberReal;
    int32_t& _globalBundlePointCount;
    int32_t& _bundleIntersectingPointCount;
    
};


//
// stringParameters[ 0 ] -> ROI mask filename
// stringParameters[ 1 ] -> ROI to DWI transform filename
// stringParameters[ 2 ] -> test mode (lt, le, eq, di, ge, gt, be, bt, oe, ou
// stringParameters[ 3 ] -> intersection type (intersection or no_intersection)
// 
// scalarParameters[ 0 ] -> first threshold
// scalarParameters[ 1 ] -> second threshold (set 0.0 when useless)
// scalarParameters[ 2 ] -> ratio of points of the bundle intersecting the 
//                          selected ROI (it is a minimum ratio when choosing
//                          the 'intersection' mode and a maximum ratio when
//                          choosing the 'no_intersection' mode
// scalarParameters[ 3 ] -> input fiber resampling step in mm (default=0.1mm)
//

template < class L, class Compare = std::less< L > >
class BundleMapOperatorRoiBasedSelection :
           public BundleMapOperator< L, Compare >,
           public Creator3Arg< BundleMapOperatorRoiBasedSelection< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >

{

  public:

    enum Type
    {

      Intersection,
      NoIntersection

    };


    virtual ~BundleMapOperatorRoiBasedSelection();

    void startContribution(
       RCPointer< BundleMap< L, Compare > > inputBundleMap,
       RCPointer< BundleMap< L, Compare > > outputBundleMap );
    void addContribution(
       RCPointer< BundleMap< L, Compare > > inputBundleMap,
       RCPointer< BundleMap< L, Compare > > outputBundleMap );
    void endContribution(
       RCPointer< BundleMap< L, Compare > > inputBundleMap,
       RCPointer< BundleMap< L, Compare > > outputBundleMap );

    std::string getName() const;

    static std::string getStaticName();
    static std::string getStringParameterHelp();
    static std::string getScalarParameterHelp();


  protected:

    friend struct Creator3Arg< BundleMapOperatorRoiBasedSelection< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorRoiBasedSelection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    std::string _fileNameRois;
    std::string _fileNameTransform3d;
    RCPointer< TestFunction< int32_t > > _testFunction;
    Type _type;
    float _intersectionRatio;
    float _inputFiberResamplingStep;

    RCPointer< Volume< int32_t > > _rois;
    RCPointer< Transform3d< float > > _transform3dFromRoisVoxelToFiberReal;

};


}


#endif
