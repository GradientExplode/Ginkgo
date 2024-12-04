#ifndef _gkg_dmri_bundle_measure_BundleFiberTortuosityStatistics_h_
#define _gkg_dmri_bundle_measure_BundleFiberTortuosityStatistics_h_


#include <gkg-dmri-bundle-measure/BundleMeasure.h>
#include <gkg-dmri-fiber-measure/FiberTortuosity.h>
#include <gkg-processing-algobase/MinimumFilter.h>
#include <gkg-processing-algobase/MaximumFilter.h>
#include <gkg-processing-algobase/AverageFilter.h>
#include <gkg-processing-algobase/StdDevFilter.h>
#include <gkg-processing-statistics/StatisticalParameterMap.h>
#include <gkg-dmri-container/BundleMeasureSpreadSheet.h>


namespace gkg
{


class BundleFiberTortuosityStatistics :
                                public BundleMeasure< StatisticalParameterMap >
{

  public:

    typedef BundleMeasure< StatisticalParameterMap >::Bundle Bundle;

    BundleFiberTortuosityStatistics();
    virtual ~BundleFiberTortuosityStatistics();

    StatisticalParameterMap get( const Bundle& bundle ) const;

    template < class L, class Compare >
    static void spreadSheetFunctor(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L,
                       std::map< std::string, Matrix >,
                       Compare >& measures );

  protected:

    FiberTortuosity _fiberTortuosity;
    MinimumFilter< std::vector< float >, double > _minimumFilter;
    MaximumFilter< std::vector< float >, double > _maximumFilter;
    AverageFilter< std::vector< float >, double > _averageFilter;
    StdDevFilter< std::vector< float >, double > _stdDevFilter;

};


}


#endif
