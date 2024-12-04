#include <gkg-dmri-bundle-measure/BundleFromVolumeStatistics.h>
#include <gkg-dmri-bundle-measure/BundleMeasure_i.h>
#include <gkg-dmri-bundle-measure/BundleMeasureChecker.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-resampling/QuickResamplerFactory.h>
#include <gkg-processing-algobase/UpdatableMeanStandardDeviationMinMax.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>

template < class T >
gkg::BundleFromVolumeStatistics< T >::
                                   BundleFromVolumeStatistics(
           gkg::RCPointer< gkg::Volume< T > > scalarVolume,
           gkg::RCPointer< gkg::Transform3d< float > >
                                           transform3dFromScalarVolumeToBundles,
           int32_t rank,
           gkg::RCPointer< gkg::Volume< uint8_t > > maskVolume,
           gkg::RCPointer< gkg::Transform3d< float > >
                                             transform3dFromMaskVolumeToBundles,
           float resamplingStep,
           const T& background )
                        : gkg::BundleMeasure< gkg::StatisticalParameterMap >(),
                            _scalarVolume( scalarVolume ),
                            _scalarVolumeResolution( 1.0, 1.0, 1.0 ),
                            _rank( rank ),
                            _transform3dFromScalarVolumeToBundles(
                                         transform3dFromScalarVolumeToBundles ),
                            _maskVolume( maskVolume ),
                            _maskVolumeResolution( 1.0, 1.0, 1.0 ),
                            _transform3dFromMaskVolumeToBundles(
                                           transform3dFromMaskVolumeToBundles ),
                            _resamplingStep( resamplingStep ),
                            _background( background )
{

  try
  {

    // collection scalar volume resolution
    _scalarVolume->getResolution( _scalarVolumeResolution );

    // collecting mask volume resolution
    if ( !_maskVolume.isNull() )
    {

      _maskVolume->getResolution( _maskVolumeResolution );

    }

    // initializing resampler
    _resampler = gkg::QuickResamplerFactory< T >::getInstance().
                         getQuickResampler( "Partial Voluming QuickResampler" );

  }
  GKG_CATCH( "template < class T > "
             "gkg::BundleFromVolumeStatistics< T >::"
             "BundleFromVolumeStatistics( "
             "gkg::RCPointer< gkg::Volume< T > > scalarVolume, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "transform3dFromScalarVolumeToBundles, "
             "int32_t rank, "
             "gkg::RCPointer< gkg::Volume< uint8_t > > maskVolume, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "transform3dFromMaskVolumeToBundles, "
             "float resamplingStep, "
             "const T& background )" );

}


template < class T >
gkg::BundleFromVolumeStatistics< T >::
                                 ~BundleFromVolumeStatistics()
{
}


template < class T >
gkg::StatisticalParameterMap
gkg::BundleFromVolumeStatistics< T >::get(
  const typename gkg::BundleFromVolumeStatistics< T >::Bundle& bundle ) const
{

  try
  {

    // allocating  profile and count vector of pairs
    gkg::UpdatableMeanStandardDeviationMinMax 
      updatableMeanStandardDeviationMinMax;

    // looping over fiber(s)
    gkg::Vector3d< int32_t > voxelInMaskVolume;
    gkg::Vector3d< float > siteInScalarVolume;
    gkg::Vector3d< float > siteInMaskVolume;
    T value = 0;

    if ( _maskVolume.isNull() )
    {

      typename
      gkg::BundleFromVolumeStatistics< T >::Bundle::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        float fiberLength = f->getLength();
        int32_t resamplingPointCount = std::max( 2,
                                                 ( int32_t )( fiberLength /
                                                      _resamplingStep + 0.5 ) );
        gkg::LightCurve3d< float >
          resampledFiber = f->getEquidistantCurve( resamplingPointCount );
        gkg::LightCurve3d< float >::const_iterator s = resampledFiber.begin(),
                                                   se = resampledFiber.end();
        while ( s != se )
        {

          _transform3dFromScalarVolumeToBundles->getInverse(
                                                           *s,
                                                           siteInScalarVolume );
          _resampler->resample( *_scalarVolume,
                                _background,
                                siteInScalarVolume,
                                value,
                                &_scalarVolumeResolution,
                                _rank );

          updatableMeanStandardDeviationMinMax.add( ( double )value );

          ++ s;

        }

        ++ f;

      }

    }
    else
    {

      // computing boundig box of mask volume
      gkg::BoundingBox< int32_t > boundingBoxMask( *_maskVolume );

      typename
      gkg::BundleFromVolumeStatistics< T >::Bundle::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        float fiberLength = f->getLength();
        int32_t resamplingPointCount = std::max( 2,
                                                 ( int32_t )( fiberLength /
                                                      _resamplingStep + 0.5 ) );
        gkg::LightCurve3d< float >
          resampledFiber = f->getEquidistantCurve( resamplingPointCount );
        gkg::LightCurve3d< float >::const_iterator s = resampledFiber.begin(),
                                                   se = resampledFiber.end();
        while ( s != se )
        {

          _transform3dFromMaskVolumeToBundles->getInverse( *s,
                                                           siteInMaskVolume );

          voxelInMaskVolume.x = ( int32_t )( siteInMaskVolume.x /
                                             _maskVolumeResolution.x );
          voxelInMaskVolume.y = ( int32_t )( siteInMaskVolume.y /
                                             _maskVolumeResolution.y );
          voxelInMaskVolume.z = ( int32_t )( siteInMaskVolume.z /
                                             _maskVolumeResolution.z );

          if ( boundingBoxMask.contains( voxelInMaskVolume ) )
          {

            if ( ( *_maskVolume )( voxelInMaskVolume ) )
            {

              _transform3dFromScalarVolumeToBundles->getInverse(
                                                           *s,
                                                           siteInScalarVolume );
              _resampler->resample( *_scalarVolume,
                                    _background,
                                    siteInScalarVolume,
                                    value,
                                    &_scalarVolumeResolution,
                                    _rank );

              updatableMeanStandardDeviationMinMax.add( ( double )value );

            }

          }
          ++ s;

        }

        ++ f;

      }

    }

    // allocating statistical parameter map
    gkg::StatisticalParameterMap statisticalParameterMap;

    statisticalParameterMap.setStatisticalParameter(
              "minimum",
              updatableMeanStandardDeviationMinMax.getMinimum() );
    statisticalParameterMap.setStatisticalParameter(
              "maximum",
              updatableMeanStandardDeviationMinMax.getMaximum() );
    statisticalParameterMap.setStatisticalParameter(
              "mean",
              updatableMeanStandardDeviationMinMax.getMean() );
    statisticalParameterMap.setStatisticalParameter(
              "standard_deviation",
              updatableMeanStandardDeviationMinMax.getStandardDeviation() );

    return statisticalParameterMap;

  }
  GKG_CATCH( "template < class T >"
             "gkg::StatisticalParameterMap "
             "gkg::BundleFromVolumeStatistics< T >::get( "
             "const typename "
             "gkg::BundleFromVolumeStatistics< T >::Bundle& "
             "bundle ) const" );

}


template < class T > template < class L, class Compare >
void gkg::BundleFromVolumeStatistics< T >::spreadSheetFunctor(
            const std::vector< std::string >& stringParameters,
            const std::vector< double >& scalarParameters,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L,
                      std::map< std::string, gkg::Matrix >,
                      Compare >& measures )
{

  try
  {

    // stringParameters[ 0 ] : input scalar volume filename
    // stringParameters[ 1 ] : input scalar to bundle map 3D transform filename
    // stringParameters[ 2 ] : mask filename
    // stringParameters[ 3 ] : mask to bundle map 3D transform file name

    // scalarParameters[ 0 ] : rank to be used in the input scalar volume
    // scalarParameters[ 1 ] : resampling step in mm
    // scalarParameters[ 2 ] : resampling background level
    // scalarParameters[ 3 ] : verbose


    ////////////////////////////////////////////////////////////////////////////
    // collecting verbosity
    ////////////////////////////////////////////////////////////////////////////
    bool verbose = ( ( ( int32_t )( scalarParameters[ 3 ] + 0.5 ) ) ?
                     true : false );


    ////////////////////////////////////////////////////////////////////////////
    // reading the input scalar volume
    ////////////////////////////////////////////////////////////////////////////

    // analyzing item type of ROI volume
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

    gkg::RCPointer< gkg::Volume< T > > scalarVolume( new gkg::Volume< T > );
    gkg::Reader::getInstance().read( stringParameters[ 0 ], *scalarVolume );


    ////////////////////////////////////////////////////////////////////////////
    // reading the 3D transform
    ////////////////////////////////////////////////////////////////////////////
    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromScalarVolumeToBundles;
    if ( stringParameters[ 1 ] == "id" )
    {

      if ( verbose )
      {

        std::cout << "setting scalar volume to bundle 3D transform to identity"
                  << std::endl;

      }
      transform3dFromScalarVolumeToBundles.reset(
                                        new gkg::IdentityTransform3d< float > );

    }
    else
    {

      if ( verbose )
      {

        std::cout << "reading scalar volume to bundle 3D transform : "
                  << std::flush;

      }

      std::ifstream is( stringParameters[ 1 ].c_str() );
      if ( is.fail() )
      {

        throw std::runtime_error( "error while opening " +
                                  stringParameters[ 1 ] );

      }
      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        rigidTransform3d->readTrm( is );
        transform3dFromScalarVolumeToBundles.reset( rigidTransform3d );

      }
      catch ( std::exception& )
      {

        try
        {

          gkg::AffineTransform3d< float >*
            affineTransform3d =
              new gkg::AffineTransform3d< float >;
          affineTransform3d->readTrm( is );
          transform3dFromScalarVolumeToBundles.reset( affineTransform3d );

        }
        catch ( std::exception& )
        {

          throw std::runtime_error(
            "unable to read linear scalar volume to bundles transformation" );

        }

      }

      is.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading the input mask volume
    ////////////////////////////////////////////////////////////////////////////
    gkg::RCPointer< gkg::Volume< uint8_t > > maskVolume;
    gkg::RCPointer< gkg::Transform3d< float > > 
      transform3dFromMaskVolumeToBundles;

    if ( stringParameters[ 2 ] != "none" )
    {

      if ( verbose )
      {

        std::cout << "reading '" << stringParameters[ 2 ] << "' mask : "
                  << std::flush;

      }

      maskVolume.reset( new gkg::Volume< uint8_t > );
      try
      {

        gkg::Reader::getInstance().read( stringParameters[ 2 ], *maskVolume );

      }
      catch ( std::exception& )
      {

        try
        {

          gkg::Volume< int16_t > tmpMaskVolume;
          gkg::Reader::getInstance().read( stringParameters[ 3 ],
                                           tmpMaskVolume );

          gkg::Converter< gkg::Volume< int16_t >, gkg::Volume< uint8_t > >
            converter;
          converter.convert( tmpMaskVolume, *maskVolume );

        }
        catch( std::exception& )
        {

          maskVolume.reset( 0 );
          throw std::runtime_error(
                            "invalid mask type, should be uint8_t or int16_t" );

        }

      }

      // reading the 3D transform
      if ( stringParameters[ 3 ] == "id" )
      {

        if ( verbose )
        {

          std::cout << "setting mask volume to bundle 3D transform "
                    << "to identity : "
                    << std::flush;

        }
        transform3dFromMaskVolumeToBundles.reset(
                                        new gkg::IdentityTransform3d< float > );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      else
      {

        if ( verbose )
        {

          std::cout << "reading mask volume to bundle 3D transform : "
                    << std::endl;

        }
        std::ifstream is( stringParameters[ 3 ].c_str() );
        if ( is.fail() )
        {

          throw std::runtime_error( "error while opening " +
                                    stringParameters[ 3 ] );

        }
        try
        {

          gkg::RigidTransform3d< float >*
            rigidTransform3d = new gkg::RigidTransform3d< float >;
          rigidTransform3d->readTrm( is );
          transform3dFromMaskVolumeToBundles.reset( rigidTransform3d );

        }
        catch ( std::exception& )
        {

          try
          {

            gkg::AffineTransform3d< float >*
              affineTransform3d =
                new gkg::AffineTransform3d< float >;
            affineTransform3d->readTrm( is );
            transform3dFromMaskVolumeToBundles.reset( affineTransform3d );

          }
          catch ( std::exception& )
          {

          throw std::runtime_error(
            "unable to read linear mask volume to bundles transformation" );

          }

        }

        is.close();
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

        std::cout << "mask volume set to none" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // converting scalar information
    ////////////////////////////////////////////////////////////////////////////

    int32_t rank = ( int32_t )( scalarParameters[ 0 ] + 0.5 );
    float resamplingStep = ( float )scalarParameters[ 1 ];
    T background = ( T )scalarParameters[ 2 ];


    ////////////////////////////////////////////////////////////////////////////
    // looping over bundle(s)
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "looping over bundle(s) : "
                << std::flush;

    }
    gkg::BundleFromVolumeStatistics< T >
      bundleFromVolumeStatistics( scalarVolume,
                                  transform3dFromScalarVolumeToBundles,
                                  rank,
                                  maskVolume,
                                  transform3dFromMaskVolumeToBundles,
                                  resamplingStep,
                                  background );

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
        fromVolumeStatistics = bundleFromVolumeStatistics.get( bundle );

      gkg::Matrix measure( 1, 4 );
      measure( 0, 0 ) = fromVolumeStatistics.
                          getStatisticalParameter( "minimum" );
      measure( 0, 1 ) = fromVolumeStatistics.
                          getStatisticalParameter( "maximum" );
      measure( 0, 2 ) = fromVolumeStatistics.
                          getStatisticalParameter( "mean" );
      measure( 0, 3 ) = fromVolumeStatistics.
                          getStatisticalParameter( "standard_deviation" );

      measures[ label ][ "from_volume_statistics" ] = measure;

      ++ l;

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "template < class L, class Compare > "
             "void gkg::BundleFromVolumeStatistics< T >::"
             "spreadSheetFunctor( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< std::string, gkg::Matrix >, "
             "Compare >& measures )" );


}


template class gkg::BundleFromVolumeStatistics< int8_t >;
template class gkg::BundleFromVolumeStatistics< uint8_t >;
template class gkg::BundleFromVolumeStatistics< int16_t >;
template class gkg::BundleFromVolumeStatistics< uint16_t >;
template class gkg::BundleFromVolumeStatistics< int32_t >;
template class gkg::BundleFromVolumeStatistics< uint32_t >;
template class gkg::BundleFromVolumeStatistics< int64_t >;
template class gkg::BundleFromVolumeStatistics< uint64_t >;
template class gkg::BundleFromVolumeStatistics< float >;
template class gkg::BundleFromVolumeStatistics< double >;


template void 
gkg::BundleFromVolumeStatistics< int8_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatistics< int8_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatistics< uint8_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatistics< uint8_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatistics< int16_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatistics< int16_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatistics< uint16_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatistics< uint16_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatistics< int32_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatistics< int32_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatistics< uint32_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatistics< uint32_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatistics< int64_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatistics< int64_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatistics< uint64_t >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatistics< uint64_t >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatistics< float >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatistics< float >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatistics< double >::spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatistics< double >::spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


//
// adding check information to FiberMeasureChecker
//

static bool initializeBundleFromVolumeStatisticsChecker()
{

  try
  {

    gkg::BundleMeasureChecker::getInstance().addChecker(
      "from_volume_statistics",

      ".  <P1>: input scalar volume filename\n"
      ".  <P2>: input scalar->bundles 3D transform filename "
                              "(if 'id', take the identity)\n"
      ".  <P3>: input mask volume filename (if 'none', no mask)\n"
      ".  <P4>: input mask->bundles 3D transform filename "
                              "(if 'id', take the identity)",

      ".  <P1>: rank to be used in the input scalar volume\n"
      ".  <P2>: resampling step in mm\n"
      ".  <P3>: resampling background level\n"
      ".  <P4>: verbosity (1->true, 0-> false)",
      4,
      4,
      0 );

    return true;

  }
  GKG_CATCH( "static bool "
             "initializeBundleFromVolumeStatisticsChecker()" );

}


static bool initialized __attribute__((unused)) =
                 initializeBundleFromVolumeStatisticsChecker();

