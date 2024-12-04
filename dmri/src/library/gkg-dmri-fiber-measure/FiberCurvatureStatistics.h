#ifndef _gkg_dmri_fiber_measure_FiberCurvatureStatistics_h_
#define _gkg_dmri_fiber_measure_FiberCurvatureStatistics_h_


#include <gkg-dmri-fiber-measure/FiberMeasure.h>
#include <gkg-dmri-fiber-measure/FiberCurvatureProfile.h>
#include <gkg-processing-algobase/MinimumFilter.h>
#include <gkg-processing-algobase/MaximumFilter.h>
#include <gkg-processing-algobase/AverageFilter.h>
#include <gkg-processing-algobase/StdDevFilter.h>
#include <gkg-processing-statistics/StatisticalParameterMap.h>
#include <gkg-dmri-container/FiberMeasureSpreadSheet.h>


namespace gkg
{


class FiberCurvatureStatistics : public FiberMeasure< StatisticalParameterMap >
{

  public:

    typedef FiberMeasure< StatisticalParameterMap >::Fiber Fiber;

    FiberCurvatureStatistics();
    virtual ~FiberCurvatureStatistics();

    StatisticalParameterMap get( const Fiber& fiber ) const;

    template < class L, class Compare >
    static void spreadSheetFunctor(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L,
                       std::map< int32_t, std::map< std::string, Matrix > >,
                       Compare >& measures );

  protected:

    FiberCurvatureProfile _fiberCurvatureProfile;
    MinimumFilter< std::vector< float >, double > _minimumFilter;
    MaximumFilter< std::vector< float >, double > _maximumFilter;
    AverageFilter< std::vector< float >, double > _averageFilter;
    StdDevFilter< std::vector< float >, double > _stdDevFilter;

};


}


#endif
