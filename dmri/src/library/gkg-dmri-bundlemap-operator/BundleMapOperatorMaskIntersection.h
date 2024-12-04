#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorMaskIntersection_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorMaskIntersection_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <vector>
#include <utility>


namespace gkg
{


//
// stringParameters[ 0 ] -> ROI mask filename
// stringParameters[ 1 ] -> ROI to DWI transform filename
// stringParameters[ 2 ] -> test mode (lt, le, eq, di, ge, gt, be, bt, oe, ou
// 
// scalarParameters[ 0 ] -> first threshold
// scalarParameters[ 1 ] -> second threshold (set 0.0 when useless)
// scalarParameters[ 2 ] -> minimum length in mm of fiber portions to be kept
// scalarParameters[ 3 ] -> input fiber resampling step in mm (default=0.1mm)
//

template < class L, class Compare = std::less< L > >
class BundleMapOperatorMaskIntersection :
           public BundleMapOperator< L, Compare >,
           public Creator3Arg< BundleMapOperatorMaskIntersection< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >

{

  public:

    virtual ~BundleMapOperatorMaskIntersection();

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

    friend struct Creator3Arg< BundleMapOperatorMaskIntersection< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorMaskIntersection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    std::string _fileNameRois;
    std::string _fileNameTransform3d;
    RCPointer< TestFunction< int32_t > > _testFunction;
    float _minimumPortionLength;
    float _inputFiberResamplingStep;

    RCPointer< Volume< int32_t > > _rois;
    RCPointer< Transform3d< float > > _transform3dFromRoisRealToFiberReal;

};


}


#endif
