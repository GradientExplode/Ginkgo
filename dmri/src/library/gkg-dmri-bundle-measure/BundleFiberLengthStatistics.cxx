#include <gkg-dmri-bundle-measure/BundleFiberLengthStatistics.h>
#include <gkg-dmri-bundle-measure/BundleMeasure_i.h>
#include <gkg-dmri-fiber-measure/FiberMeasure_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/StdDevFilter_i.h>
#include <gkg-dmri-bundle-measure/BundleMeasureChecker.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>


gkg::BundleFiberLengthStatistics::BundleFiberLengthStatistics()
                              : gkg::BundleMeasure<
                                              gkg::StatisticalParameterMap >()

{
}


gkg::BundleFiberLengthStatistics::~BundleFiberLengthStatistics()
{
}


gkg::StatisticalParameterMap 
gkg::BundleFiberLengthStatistics::get(
                 const gkg::BundleFiberLengthStatistics::Bundle& bundle ) const
{

  try
  {

    // initializing statistics
    double minimum = 0.0;
    double maximum = 0.0;
    double mean = 0.0;
    double standardDeviation = 0.0;

    // processing fiber lengths
    int32_t fiberCount = bundle.getCurve3dCount();
    std::vector< float > fiberLengths( fiberCount );

    int32_t f = 0;
    for ( f = 0; f < fiberCount; f++ )
    {

      fiberLengths[ f ] = _fiberLength.get( bundle.getCurve3d( f ) );

    }

    // processing fiber length statistics
    _minimumFilter.filter( fiberLengths, minimum );
    _maximumFilter.filter( fiberLengths, maximum );
    _averageFilter.filter( fiberLengths, mean );
    _stdDevFilter.filter( fiberLengths, standardDeviation );

    gkg::StatisticalParameterMap statisticalParameterMap;
    statisticalParameterMap.setStatisticalParameter( "minimum", minimum );
    statisticalParameterMap.setStatisticalParameter( "maximum", maximum );
    statisticalParameterMap.setStatisticalParameter( "mean", mean );
    statisticalParameterMap.setStatisticalParameter( "standard_deviation",
                                                     standardDeviation );

    return statisticalParameterMap;

  }
  GKG_CATCH( "gkg::StatisticalParameterMap "
             "gkg::BundleFiberLengthStatistics::get("
             "const gkg::BundleFiberLengthStatistics::Bundle& bundle ) const" );

}


template < class L, class Compare >
void gkg::BundleFiberLengthStatistics::spreadSheetFunctor(
            const std::vector< std::string >& /*stringParameters*/,
            const std::vector< double >& scalarParameters,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L,
                      std::map< std::string, gkg::Matrix >,
                      Compare >& measures )
{

  try
  {

    // collecting verbosity
    bool verbose = ( ( ( int32_t )( scalarParameters[ 0 ] + 0.5 ) ) ?
                     true : false );
    if ( verbose )
    {

      std::cout << "looping over bundle(s) : "
                << std::flush;

    }
    gkg::BundleFiberLengthStatistics bundleFiberLengthStatistics;

    typename gkg::BundleMap< L, Compare >::const_iterator
      l = bundleMap->begin(),
      le = bundleMap->end();
    while ( l != le )
    {

      const L& label = l->first;
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;

      if ( verbose )
      {

        std::cout << gkg::StringConverter::toString( label ) << " "
                  << std::flush;

      }

      gkg::StatisticalParameterMap
        statisticalParameterMap = bundleFiberLengthStatistics.get( bundle );

      gkg::Matrix&
          measure = measures[ label ][ "bundle_fiber_length_statistics" ];
      measure.reallocate( 1, 4 );
      measure( 0, 0 ) = statisticalParameterMap.getStatisticalParameter(
                                                         "minimum" );
      measure( 0, 1 ) = statisticalParameterMap.getStatisticalParameter(
                                                         "maximum" );
      measure( 0, 2 ) = statisticalParameterMap.getStatisticalParameter(
                                                         "mean" );
      measure( 0, 3 ) = statisticalParameterMap.getStatisticalParameter(
                                                         "standard_deviation" );

      ++ l;

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::BundleFiberLengthStatistics::spreadSheetFunctor( "
             "const std::vector< std::string >& /*stringParameters*/, "
             "const std::vector< double >& /*scalarParameters*/, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< std::string, gkg::Matrix >, "
             "Compare >& measures )" );

}


template 
void gkg::BundleFiberLengthStatistics::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template 
void gkg::BundleFiberLengthStatistics::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

//
// adding check information to BundleFiberLengthStatistics
//

static bool initializeBundleFiberLengthStatisticsChecker()
{

  try
  {

    gkg::BundleMeasureChecker::getInstance().addChecker(
      "bundle_fiber_length_statistics",
      ".  N/A",
      ".  <P1>: verbosity (1->true, 0-> false)",
      0,
      1,
      0 );

    return true;

  }
  GKG_CATCH( "static bool initializeBundleFiberLengthStatisticsChecker()" );

}


static bool initialized __attribute__((unused)) =
                                 initializeBundleFiberLengthStatisticsChecker();
