#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorLengthSelection_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorLengthSelection_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>
#include <utility>


namespace gkg
{


template < class T > class TestFunction;


// 
// stringParameters[ 0 ] -> threshold mode
//
// scalarParameters[ 0 ] -> 1st threshold
// scalarParameters[ 1 ] -> 2nd threshold
//

template < class L, class Compare = std::less< L > >
class BundleMapOperatorLengthSelection :
                 public BundleMapOperator< L, Compare >,
                 public Creator3Arg< BundleMapOperatorLengthSelection< L, Compare >,
                                     BundleMapOperator< L, Compare >,
                                     const std::vector< std::string >&,
                                     const std::vector< double >&,
                                     bool >
{

  public:

    virtual ~BundleMapOperatorLengthSelection();

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

    friend struct Creator3Arg< BundleMapOperatorLengthSelection< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorLengthSelection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    RCPointer< TestFunction< float > > _testFunction;

};


}


#endif
