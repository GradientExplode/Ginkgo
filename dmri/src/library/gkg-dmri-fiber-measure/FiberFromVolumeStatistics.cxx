#include <gkg-dmri-fiber-measure/FiberFromVolumeStatistics.h>
#include <gkg-dmri-fiber-measure/FiberMeasure_i.h>
#include <gkg-dmri-fiber-measure/FiberMeasureChecker.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/StdDevFilter_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::FiberFromVolumeStatistics< T >::FiberFromVolumeStatistics(
                                     gkg::RCPointer< gkg::Volume< T > > volume,
                                     const T& background,
                                     int32_t rank )
                              : gkg::FiberMeasure<
                                              gkg::StatisticalParameterMap >(),
                                _fiberFromVolumeProfile( volume,
                                                         background,
                                                         rank )
{
}


template < class T >
gkg::FiberFromVolumeStatistics< T >::~FiberFromVolumeStatistics()
{
}


template < class T >
gkg::StatisticalParameterMap 
gkg::FiberFromVolumeStatistics< T >::get(
       const typename gkg::FiberFromVolumeStatistics< T >::Fiber& fiber ) const
{

  try
  {

    // initializing statistics
    double minimum = 0.0;
    double maximum = 0.0;
    double mean = 0.0;
    double standardDeviation = 0.0;

    // processing from volume profile
    std::vector< T > profile = _fiberFromVolumeProfile.get( fiber );

    // processing from volume statistics
    _minimumFilter.filter( profile, minimum );
    _maximumFilter.filter( profile, maximum );
    _averageFilter.filter( profile, mean );
    _stdDevFilter.filter( profile, standardDeviation );

    gkg::StatisticalParameterMap statisticalParameterMap;
    statisticalParameterMap.setStatisticalParameter( "minimum", minimum );
    statisticalParameterMap.setStatisticalParameter( "maximum", maximum );
    statisticalParameterMap.setStatisticalParameter( "mean", mean );
    statisticalParameterMap.setStatisticalParameter( "standard_deviation",
                                                     standardDeviation );

    return statisticalParameterMap;

  }
  GKG_CATCH( "template < class T > "
             "gkg::StatisticalParameterMap "
             "gkg::FiberFromVolumeStatistics< T >::get("
             "const typename gkg::FiberFromVolumeStatistic< T >s::Fiber& "
             "fiber ) const" );

}


template < class T > template < class L, class Compare >
void gkg::FiberFromVolumeStatistics< T >::spreadSheetFunctor(
            const std::vector< std::string >& stringParameters,
            const std::vector< double >& scalarParameters,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L,
                      std::map< int32_t, std::map< std::string, gkg::Matrix > >,
                      Compare >& measures )
{

  try
  {

    // analyzing item type of scalar volume
    gkg::AnalyzedObject analyzedObject;
    std::string format;
    gkg::DiskFormatAnalyzer::getInstance().analyze( stringParameters[ 0 ],
                                                    format,
                                                    analyzedObject,
                                                    "Volume" );
    if ( analyzedObject.getObjectType() != "Volume" )
    {

      throw std::runtime_error( "not the good object type" );

    }
    std::string itemType = "";
    try
    {

      itemType = analyzedObject.getItemType();

    }
    catch( std::exception& )
    {

      throw std::runtime_error( "no item type found" );

    }
    if ( itemType != gkg::TypeOf< T >::getName() )
    {

      throw std::runtime_error( "not the good item type" );

    }

    gkg::RCPointer< gkg::Volume< T > >
      volume( new gkg::Volume< T > );

    gkg::Reader::getInstance().read( stringParameters[ 0 ], *volume );

    gkg::FiberFromVolumeStatistics< T >
      fiberFromVolumeStatistics( volume,
                                ( T )scalarParameters[ 0 ],
                                ( int32_t )( scalarParameters[ 1 ] + 0.5 ) );

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
          fromVolumeStatistics = fiberFromVolumeStatistics.get( *f );
        gkg::Matrix&
          measure = measures[ label ][ fiberIndex ][ "from_volume_statistics" ];
        measure.reallocate( 1, 4 );
        measure( 0, 0 ) = fromVolumeStatistics.getStatisticalParameter(
                                                         "minimum" );
        measure( 0, 1 ) = fromVolumeStatistics.getStatisticalParameter(
                                                         "maximum" );
        measure( 0, 2 ) = fromVolumeStatistics.getStatisticalParameter(
                                                         "mean" );
        measure( 0, 3 ) = fromVolumeStatistics.getStatisticalParameter(
                                                         "standard_deviation" );

        ++ f;
        ++ fiberIndex;

      }

      ++ l;

    }

  }
  GKG_CATCH( "template < class T > "
             "template < class L, class Compare > "
             "void gkg::FiberFromVolumeStatistics< T >::spreadSheetFunctor( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< int32_t, std::map< std::string, gkg::Matrix > >, "
             "Compare >& measures )" );


}


template class gkg::FiberFromVolumeStatistics< int8_t >;
template class gkg::FiberFromVolumeStatistics< uint8_t >;
template class gkg::FiberFromVolumeStatistics< int16_t >;
template class gkg::FiberFromVolumeStatistics< uint16_t >;
template class gkg::FiberFromVolumeStatistics< int32_t >;
template class gkg::FiberFromVolumeStatistics< uint32_t >;
template class gkg::FiberFromVolumeStatistics< int64_t >;
template class gkg::FiberFromVolumeStatistics< uint64_t >;
template class gkg::FiberFromVolumeStatistics< float >;
template class gkg::FiberFromVolumeStatistics< double >;


template void 
gkg::FiberFromVolumeStatistics< int8_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeStatistics< int8_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeStatistics< uint8_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeStatistics< uint8_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeStatistics< int16_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeStatistics< int16_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeStatistics< uint16_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeStatistics< uint16_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeStatistics< int32_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeStatistics< int32_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeStatistics< uint32_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeStatistics< uint32_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeStatistics< int64_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeStatistics< int64_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeStatistics< uint64_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeStatistics< uint64_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeStatistics< float >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeStatistics< float >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeStatistics< double >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeStatistics< double >::spreadSheetFunctor< int16_t >(
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

static bool initializeFiberFromVolumeStatisticsChecker()
{

  try
  {

    gkg::FiberMeasureChecker::getInstance().addChecker(
      "from_volume_statistics",
      ".  <P1>: volume filename",
      ".  <P1>: background intensity level\n"
      ".  <P2>: rank of the volume to be used",
      1,
      2,
      0 );

    return true;

  }
  GKG_CATCH( "static bool initializeFiberFromVolumeStatisticsChecker()" );

}


static bool initialized __attribute__((unused)) =
                                   initializeFiberFromVolumeStatisticsChecker();

