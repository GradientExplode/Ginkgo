#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorFlipper3d_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorFlipper3d_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>
#include <utility>


namespace gkg
{


// 
// stringParameters[ 0..N ] -> flipping type(s)
//

template < class L, class Compare = std::less< L > >
class BundleMapOperatorFlipper3d :
                 public BundleMapOperator< L, Compare >,
                 public Creator3Arg< BundleMapOperatorFlipper3d< L, Compare >,
                                     BundleMapOperator< L, Compare >,
                                     const std::vector< std::string >&,
                                     const std::vector< double >&,
                                     bool >
{

  public:

    virtual ~BundleMapOperatorFlipper3d();

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

    friend struct Creator3Arg< BundleMapOperatorFlipper3d< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorFlipper3d(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    std::vector< std::string > _flippingTypes;

};


}


#endif
