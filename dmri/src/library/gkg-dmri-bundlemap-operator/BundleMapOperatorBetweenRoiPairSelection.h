#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorBetweenRoiPairSelection_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorBetweenRoiPairSelection_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <vector>
#include <utility>
#include <map>


namespace gkg
{


//
// stringParameters[ 0 ] -> ROI mask filename
// stringParameters[ 1 ] -> ROI Label to Name Lut filename
// stringParameters[ 2 ] -> Output hierarchy 
// stringParameters[ 3 ] -> Label changes 
// stringParameters[ 4 ] -> ROI to DWI linear transform filename
// stringParameters[ 5 ] -> ROI to DWI diffeomorphic direct transform filename
// stringParameters[ 6 ] -> ROI to DWI diffeomorphic inverse transform filename
// 
// scalarParameters[ 0 ] -> threshold on distance from the extremities
// scalarParameters[ 1 ] -> input fiber resampling step in mm (default=0.1mm)
//

template < class L, class Compare = std::less< L > >
class BundleMapOperatorBetweenRoiPairSelection :
           public BundleMapOperator< L, Compare >,
           public Creator3Arg< BundleMapOperatorBetweenRoiPairSelection<
                                                                      L,
                                                                      Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >

{

  public:

    virtual ~BundleMapOperatorBetweenRoiPairSelection();

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

    friend struct Creator3Arg< BundleMapOperatorBetweenRoiPairSelection<
                                                                      L,
                                                                      Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorBetweenRoiPairSelection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    L getLabel( int32_t roiLabel1,
                int32_t roiLabel2,
                int32_t clusterIndex ) const;

    void createHierarchy( const BundleMap< L, Compare >& bundleMap ) const;
    void saveLabelChanges() const;

    std::string _fileNameRois;
    std::string _fileNameROILabelToNameLut;
    std::string _fileNameOutputHierarchy;
    std::string _fileNameLabelChanges;

    std::vector< std::string > _fileNameTransform3ds;
    float _distanceThresholdFromExtremity;
    float _inputFiberResamplingStep;

    RCPointer< Volume< int32_t > > _rois;
    int32_t _roiCount;
    std::vector< int32_t > _roiLabels;
    std::map< int32_t, std::string > _roiLabelToNameLut;
    std::map< std::pair< int32_t, int32_t >, int32_t >
                                 _roiForwardLabelToRoiBackwardLabelClusterCount;
    RCPointer< Transform3d< float > > _transform3dFromRoisVoxelToFiberReal;
    std::map< L, L > _labelChanges;

};


}


#endif
