#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorIntraSubjectClusterLabelSimplifier_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorIntraSubjectClusterLabelSimplifier_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>
#include <utility>
#include <set>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class BundleMapOperatorIntraSubjectClusterLabelSimplifier :
     public BundleMapOperator< L, Compare >,
     public Creator3Arg< BundleMapOperatorIntraSubjectClusterLabelSimplifier< L,
                                                                      Compare >,
                         BundleMapOperator< L, Compare >,
                         const std::vector< std::string >&,
                         const std::vector< double >&,
                         bool >

{

  public:

    virtual ~BundleMapOperatorIntraSubjectClusterLabelSimplifier();

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

    friend struct Creator3Arg< BundleMapOperatorIntraSubjectClusterLabelSimplifier< L,
                                                                      Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorIntraSubjectClusterLabelSimplifier(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    L getNewLabel( const L& label );

    std::string _fileNameLabelLut;

    std::map< std::string, int32_t > _roiToIndexLut;
    std::map< std::string, std::string > _clusterLabelToNewLabelLut;
    int32_t _currentRoiIndex;
    std::map< int32_t, int32_t > _currentClusterIndices;

};


}


#endif
