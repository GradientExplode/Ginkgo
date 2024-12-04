#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorRemoveDuplicates_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorRemoveDuplicates_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-bundle-distance/BundleDistanceFunctor.h>
#include <vector>
#include <utility>


namespace gkg
{



// 
// stringParameters[ 0 ] -> bundle distance functor name (one among
//                                   , ...)
//
// scalarParameters[     0 ] -> bundle distance threshold (default=0.0f)
// scalarParameters[     1 ] -> bundle distance functor parameter 1
//     ...
// scalarParameters[ N + 1 ] -> bundle distance functor parameter N

//

template < class L, class Compare = std::less< L > >
class BundleMapOperatorRemoveDuplicates :
            public BundleMapOperator< L, Compare >,
            public Creator3Arg< BundleMapOperatorRemoveDuplicates< L, Compare >,
                                BundleMapOperator< L, Compare >,
                                const std::vector< std::string >&,
                                const std::vector< double >&,
                                bool >
{

  public:

    virtual ~BundleMapOperatorRemoveDuplicates();

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

    friend struct Creator3Arg< BundleMapOperatorRemoveDuplicates< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorRemoveDuplicates(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    float _bundleDistanceThreshold;
    RCPointer< BundleDistanceFunctor > _bundleDistanceFunctor;

};


}


#endif
