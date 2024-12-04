#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorComputeCentroids_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorComputeCentroids_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-algobase/MinimumIndexFilter.h>
#include <vector>
#include <utility>


namespace gkg
{




template < class L, class Compare = std::less< L > >
class BundleMapOperatorComputeCentroids :
           public BundleMapOperator< L, Compare >,
           public Creator3Arg< BundleMapOperatorComputeCentroids< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >

{

  public:

    enum CentroidType
    {

      ClosestFiber,
      AverageFiber

    };

    enum SelectionMode
    {

      Random,
      All

    };

    virtual ~BundleMapOperatorComputeCentroids();

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

    friend struct Creator3Arg< BundleMapOperatorComputeCentroids< L, Compare >,
                               BundleMapOperator< L, Compare >,
                               const std::vector< std::string >&,
                               const std::vector< double >&,
                               bool >;

    BundleMapOperatorComputeCentroids(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose );

    int32_t getFastCentroidIndex(
                 const typename BundleMap< L, Compare >::Bundle& bundle ) const;
    int32_t getSlowCentroidIndex(
                 const typename BundleMap< L, Compare >::Bundle& bundle ) const;


    CentroidType _centroidType;
    SelectionMode _selectionMode;
    LightCurve3d< float >::DistanceType _distanceType;
    int32_t _maximumFiberCount;
    int32_t _centroidPointCount;
    bool _verbose;
    MinimumIndexFilter< Vector, int32_t > _minimumIndexFilter;

};


}


#endif
