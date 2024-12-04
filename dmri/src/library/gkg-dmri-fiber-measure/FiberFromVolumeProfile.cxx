#include <gkg-dmri-fiber-measure/FiberFromVolumeProfile.h>
#include <gkg-dmri-fiber-measure/FiberMeasure_i.h>
#include <gkg-dmri-fiber-measure/FiberMeasureChecker.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-resampling/QuickResamplerFactory.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::FiberFromVolumeProfile< T >::FiberFromVolumeProfile(
                                    gkg::RCPointer< gkg::Volume< T > > volume,
                                    const T& background,
                                    int32_t rank )
                                 : gkg::FiberMeasure< std::vector< T > >(),
                                   _volume( volume ),
                                   _background( background ),
                                   _rank( rank )
{

  try
  {

    _volume->getResolution( _resolution );

    _resampler = gkg::QuickResamplerFactory< T >::getInstance().
                   getQuickResampler( "Partial Voluming QuickResampler" );

  }
  GKG_CATCH( "template < class T > "
             "gkg::FiberFromVolumeProfile< T >::FiberFromVolumeProfile( "
             "gkg::RCPointer< gkg::Volume< T > > volume, "
             "const T& background, "
             "int32_t rank )" );

}


template < class T >
gkg::FiberFromVolumeProfile< T >::~FiberFromVolumeProfile()
{
}


template < class T >
std::vector< T > 
gkg::FiberFromVolumeProfile< T >::get(
          const typename gkg::FiberFromVolumeProfile< T >::Fiber& fiber ) const
{

  try
  {

    int32_t pointCount = fiber.getPointCount();
    std::vector< T > profile( pointCount );

    int32_t p = 0;
    for ( p = 0; p < pointCount; p++ )
    {

      _resampler->resample( *_volume,
                            _background,
                            fiber.getPoint( p ),
                            profile[ p ],
                            &_resolution,
                            _rank );

    }

    return profile;

  }
  GKG_CATCH( "template < class T >"
             "std::vector< T > gkg::FiberFromVolumeProfile< T >::get( "
             "const typename gkg::FiberFromVolumeProfile< T >::Fiber& "
             "fiber ) const" );

}



template < class T > template < class L, class Compare >
void gkg::FiberFromVolumeProfile< T >::spreadSheetFunctor(
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

    gkg::FiberFromVolumeProfile< T >
      fiberFromVolumeProfile( volume,
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

        std::vector< T >
          fromVolumeProfile = fiberFromVolumeProfile.get( *f );
        int32_t size = ( int32_t )fromVolumeProfile.size();
        gkg::Matrix&
          measure = measures[ label ][ fiberIndex ][ "from_volume_profile" ];
        measure.reallocate( 1, size );
        int32_t p = 0;
        for ( p = 0; p < size; p++ )
        {

          measure( 0, p ) = ( double )fromVolumeProfile[ p ];

        }
        ++ f;
        ++ fiberIndex;

      }

      ++ l;

    }

  }
  GKG_CATCH( "template < class T > "
             "template < class L, class Compare > "
             "void gkg::FiberFromVolumeProfile< T >::spreadSheetFunctor( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< int32_t, std::map< std::string, gkg::Matrix > >, "
             "Compare >& measures )" );


}


template class gkg::FiberFromVolumeProfile< int8_t >;
template class gkg::FiberFromVolumeProfile< uint8_t >;
template class gkg::FiberFromVolumeProfile< int16_t >;
template class gkg::FiberFromVolumeProfile< uint16_t >;
template class gkg::FiberFromVolumeProfile< int32_t >;
template class gkg::FiberFromVolumeProfile< uint32_t >;
template class gkg::FiberFromVolumeProfile< int64_t >;
template class gkg::FiberFromVolumeProfile< uint64_t >;
template class gkg::FiberFromVolumeProfile< float >;
template class gkg::FiberFromVolumeProfile< double >;



template void 
gkg::FiberFromVolumeProfile< int8_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeProfile< int8_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeProfile< uint8_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeProfile< uint8_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeProfile< int16_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeProfile< int16_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeProfile< uint16_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeProfile< uint16_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeProfile< int32_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeProfile< int32_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeProfile< uint32_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeProfile< uint32_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeProfile< int64_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeProfile< int64_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeProfile< uint64_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeProfile< uint64_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeProfile< float >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeProfile< float >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );


template void 
gkg::FiberFromVolumeProfile< double >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< int32_t, std::map< std::string,
                                                        gkg::Matrix > > 
                         >& measures );

template void 
gkg::FiberFromVolumeProfile< double >::spreadSheetFunctor< int16_t >(
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

static bool initializeFiberFromVolumeProfileChecker()
{

  try
  {

    gkg::FiberMeasureChecker::getInstance().addChecker(
      "from_volume_profile",
      ".  <P1>: volume filename",
      ".  <P1>: background intensity level\n"
      ".  <P2>: rank of the volume to be used",
      1,
      2,
      0 );

    return true;

  }
  GKG_CATCH( "static bool initializeFiberFromVolumeProfileChecker()" );

}


static bool initialized __attribute__((unused)) =
                                      initializeFiberFromVolumeProfileChecker();

