#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorFiberCountFilter_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorFiberCountFilter_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <vector>
#include <utility>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class BundleMapOperatorFiberCountFilter :
           public BundleMapOperator< L, Compare >,
           public Creator3Arg< BundleMapOperatorFiberCountFilter< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >

{

  public:

    virtual ~BundleMapOperatorFiberCountFilter();

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

    friend struct Creator3Arg< BundleMapOperatorFiberCountFilter< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorFiberCountFilter(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    RCPointer< TestFunction< int32_t > > _testFunction; 

};


}


#endif
