#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorMidSagittalPlaneSymmetrizer_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorMidSagittalPlaneSymmetrizer_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>
#include <utility>


namespace gkg
{


// 
// stringParameters[ 0 ] -> mid-sagittal plane coefficient filename
//

template < class L, class Compare = std::less< L > >
class BundleMapOperatorMidSagittalPlaneSymmetrizer :
           public BundleMapOperator< L, Compare >,
           public Creator3Arg< BundleMapOperatorMidSagittalPlaneSymmetrizer<
                                                                      L,
                                                                      Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >

{

  public:

    virtual ~BundleMapOperatorMidSagittalPlaneSymmetrizer();

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

    friend struct Creator3Arg< BundleMapOperatorMidSagittalPlaneSymmetrizer< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorMidSagittalPlaneSymmetrizer(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );
    Vector3d< float > getSymmetric( const Vector3d< float >& site ) const;

    std::vector< float > _midSagittalPlaneCoefficients;

};


}


#endif
