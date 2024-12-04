#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorBoundingBoxIntersectionWithRandomSelection_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorBoundingBoxIntersectionWithRandomSelection_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>
#include <utility>


namespace gkg
{


// 
// scalarParameters[ 0 ] -> percentage of preserved fibers per bundle
//

template < class L, class Compare = std::less< L > >
class BundleMapOperatorBoundingBoxIntersectionWithRandomSelection :
  public BundleMapOperator< L, Compare >,
  public Creator3Arg<
      BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >,
      BundleMapOperator< L, Compare >,
      const std::vector< std::string >&,
      const std::vector< double >&,
      bool >

{

  public:

    virtual ~BundleMapOperatorBoundingBoxIntersectionWithRandomSelection();

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

    friend struct Creator3Arg<
      BundleMapOperatorBoundingBoxIntersectionWithRandomSelection< L, Compare >,
      BundleMapOperator< L, Compare >,
      const std::vector< std::string >&,
      const std::vector< double >&,
      bool >;

    BundleMapOperatorBoundingBoxIntersectionWithRandomSelection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    void addToSelection( std::list< LightCurve3d< float > >& fibers,
                         const LightCurve3d< float >& fiber ) const;

    BoundingBox< float > _boundingBox;
    double _keptFiberPercentage;

};


}


#endif
