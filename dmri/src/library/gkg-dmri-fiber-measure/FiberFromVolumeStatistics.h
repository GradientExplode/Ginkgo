#ifndef _gkg_dmri_fiber_measure_FiberFromVolumeStatistics_h_
#define _gkg_dmri_fiber_measure_FiberFromVolumeStatistics_h_


#include <gkg-dmri-fiber-measure/FiberMeasure.h>
#include <gkg-dmri-fiber-measure/FiberFromVolumeProfile.h>
#include <gkg-processing-algobase/MinimumFilter.h>
#include <gkg-processing-algobase/MaximumFilter.h>
#include <gkg-processing-algobase/AverageFilter.h>
#include <gkg-processing-algobase/StdDevFilter.h>
#include <gkg-processing-statistics/StatisticalParameterMap.h>
#include <gkg-dmri-container/FiberMeasureSpreadSheet.h>


namespace gkg
{


template < class T >
class FiberFromVolumeStatistics : public FiberMeasure< StatisticalParameterMap >
{

  public:

    typedef FiberMeasure< StatisticalParameterMap >::Fiber Fiber;

    FiberFromVolumeStatistics( RCPointer< Volume< T > > volume,
                               const T& background = 0,
                               int32_t rank = 0);
    virtual ~FiberFromVolumeStatistics();

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

    FiberFromVolumeProfile< T > _fiberFromVolumeProfile;
    MinimumFilter< std::vector< T >, double > _minimumFilter;
    MaximumFilter< std::vector< T >, double > _maximumFilter;
    AverageFilter< std::vector< T >, double > _averageFilter;
    StdDevFilter< std::vector< T >, double > _stdDevFilter;

};


}


#endif
