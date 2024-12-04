#include <gkg-dmri-fiber-measure/FiberCurvatureStatistics.h>
#include <gkg-dmri-fiber-measure/FiberMeasure_i.h>
#include <gkg-dmri-fiber-measure/FiberMeasureChecker.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/StdDevFilter_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::FiberCurvatureStatistics::FiberCurvatureStatistics()
                              : gkg::FiberMeasure<
                                              gkg::StatisticalParameterMap >()

{
}


gkg::FiberCurvatureStatistics::~FiberCurvatureStatistics()
{
}


gkg::StatisticalParameterMap 
gkg::FiberCurvatureStatistics::get(
                       const gkg::FiberCurvatureStatistics::Fiber& fiber ) const
{

  try
  {

    // initializing statistics
    double minimum = 0.0;
    double maximum = 0.0;
    double mean = 0.0;
    double standardDeviation = 0.0;

    // processing curvature profile
    std::vector< float > curvatureProfile = _fiberCurvatureProfile.get( fiber );

    // processing curvature statistics
    _minimumFilter.filter( curvatureProfile, minimum );
    _maximumFilter.filter( curvatureProfile, maximum );
    _averageFilter.filter( curvatureProfile, mean );
    _stdDevFilter.filter( curvatureProfile, standardDeviation );

    gkg::StatisticalParameterMap statisticalParameterMap;
    statisticalParameterMap.setStatisticalParameter( "minimum", minimum );
    statisticalParameterMap.setStatisticalParameter( "maximum", maximum );
    statisticalParameterMap.setStatisticalParameter( "mean", mean );
    statisticalParameterMap.setStatisticalParameter( "standard_deviation",
                                                     standardDeviation );

    return statisticalParameterMap;

  }
  GKG_CATCH( "gkg::StatisticalParameterMap gkg::FiberCurvatureStatistics::get("
             "const gkg::FiberCurvatureStatistics::Fiber& fiber ) const" );

}


template < class L, class Compare >
void gkg::FiberCurvatureStatistics::spreadSheetFunctor(
            const std::vector< std::string >& /*stringParameters*/,
            const std::vector< double >& /*scalarParameters*/,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L,
                      std::map< int32_t, std::map< std::string, gkg::Matrix > >,
                      Compare >& measures )
{

  try
  {

    gkg::FiberCurvatureStatistics fiberCurvatureStatistics;

    typename gkg::BundleMap< L, Compare >::const_iterator
      l = bundleMap->begin(),
      le = bundleMap->end();
    while ( l != le )
    {

      const L& label = l->first;
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;

      int32_t fiberIndex = 0;
      typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        gkg::StatisticalParameterMap
          statisticalParameterMap = fiberCurvatureStatistics.get( *f );
        gkg::Matrix&
          measure = measures[ label ][ fiberIndex ][ "curvature_statistics" ];
        measure.reallocate( 1, 4 );
        measure( 0, 0 ) = statisticalParameterMap.getStatisticalParameter(
                                                         "minimum" );
        measure( 0, 1 ) = statisticalParameterMap.getStatisticalParameter(
                                                         "maximum" );
        measure( 0, 2 ) = statisticalParameterMap.getStatisticalParameter(
                                                         "mean" );
        measure( 0, 3 ) = statisticalParameterMap.getStatisticalParameter(
                                                         "standard_deviation" );

        ++ f;
        ++ fiberIndex;

      }

      ++ l;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::FiberCurvatureStatistics::spreadSheetFunctor( "
             "const std::vector< std::string >& /*stringParameters*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< int32_t, std::map< std::string, gkg::Matrix > >, "
             "Compare >& measures )" );


}


template void gkg::FiberCurvatureStatistics::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void gkg::FiberCurvatureStatistics::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


//
// adding check information to FiberMeasureChecker
//

static bool initializeFiberCurvatureStatisticsChecker()
{

  try
  {

    gkg::FiberMeasureChecker::getInstance().addChecker(
      "curvature_statistics",
      ".  N/A",
      ".  N/A",
      0,
      0,
      0 );

    return true;

  }
  GKG_CATCH( "static bool initializeFiberCurvatureStatisticsChecker()" );

}


static bool initialized __attribute__((unused)) =
                                    initializeFiberCurvatureStatisticsChecker();

