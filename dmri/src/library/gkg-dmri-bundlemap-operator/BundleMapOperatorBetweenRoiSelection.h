#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorBetweenRoiSelection_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorBetweenRoiSelection_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-transform/Transform3d.h>
#include <vector>
#include <utility>


namespace gkg
{


//
// stringParameters[ 0 ] -> ROI mask filename
// stringParameters[ 1 ] -> ROI to DWI transform filename
// 
// scalarParameters[ 0 ] -> minimum intersection length with the ROI in mm
// scalarParameters[ 1 ] -> do not process inter-ROI bundles
// scalarParameters[ 2 ] -> in case L=int16_t offset for label composition
//                          when two labels are present
//

template < class L, class Compare = std::less< L > >
class BundleMapOperatorBetweenRoiSelection :
         public BundleMapOperator< L, Compare >,
         public Creator3Arg< BundleMapOperatorBetweenRoiSelection< L, Compare >,
                             BundleMapOperator< L, Compare >,
                             const std::vector< std::string >&,
                             const std::vector< double >&,
                             bool >

{

  public:

    virtual ~BundleMapOperatorBetweenRoiSelection();

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

    friend struct Creator3Arg< BundleMapOperatorBetweenRoiSelection< L,
                                                                     Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorBetweenRoiSelection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    int16_t getRange( const std::vector< int16_t >& labels,
                      int32_t index,
                      std::pair< int32_t, int32_t >& range ) const;
    L getLabel( int16_t label1, int16_t label2 ) const;

    std::string _fileNameRois;
    std::string _fileNameTransform3d;
    float _minimumIntersectionLength;
    bool _noInterRoiFibers;
    int16_t _offset;
    RCPointer< Volume< int16_t > > _rois;
    RCPointer< Transform3d< float > > _transform3dFromRoisVoxelToFiberReal;

};


}


#endif
