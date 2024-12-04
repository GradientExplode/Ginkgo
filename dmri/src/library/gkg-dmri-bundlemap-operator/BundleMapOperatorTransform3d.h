#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorTransform3d_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorTransform3d_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>
#include <utility>


namespace gkg
{


template < class T > class Transform3d;
template < class T > class VectorFieldTransform3d;


// 
// stringParameters[ 0 ] -> 3D transformation filename
//

template < class L, class Compare = std::less< L > >
class BundleMapOperatorTransform3d :
                 public BundleMapOperator< L, Compare >,
                 public Creator3Arg< BundleMapOperatorTransform3d< L, Compare >,
                                     BundleMapOperator< L, Compare >,
                                     const std::vector< std::string >&,
                                     const std::vector< double >&,
                                     bool >
{

  public:

    virtual ~BundleMapOperatorTransform3d();

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

    friend struct Creator3Arg< BundleMapOperatorTransform3d< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorTransform3d(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    RCPointer< Transform3d< float > > _transform3d;
    RCPointer< VectorFieldTransform3d< float > > _vectorFieldTransform3d;

};


}


#endif
