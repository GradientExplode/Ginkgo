#include <gkg-dmri-bundle-measure/BundleFromVolumeStatisticsAlongCentroid.h>
#include <gkg-dmri-bundle-measure/BundleMeasure_i.h>
#include <gkg-dmri-bundle-measure/BundleMeasureChecker.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-resampling/QuickResamplerFactory.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/StdDevFilter_i.h>
#include <gkg-processing-algobase/UpdatableMeanStandardDeviationMinMax.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
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


template < class T >
gkg::BundleFromVolumeStatisticsAlongCentroid< T >::
                                   BundleFromVolumeStatisticsAlongCentroid(
           gkg::RCPointer< std::vector< LightCurve3d< float > > > centroids,
           gkg::RCPointer< std::map< std::string, int32_t > > labelToIndexLut,
           gkg::RCPointer< gkg::Volume< int16_t > > sectionVolume,
           gkg::RCPointer< std::vector< int32_t > > sectionCounts,
           gkg::RCPointer< gkg::Volume< T > > scalarVolume,
           gkg::RCPointer< gkg::Transform3d< float > >
                                           transform3dFromScalarVolumeToBundles,
           int32_t rank,
           gkg::RCPointer< gkg::Volume< uint8_t > > maskVolume,
           gkg::RCPointer< gkg::Transform3d< float > >
                                             transform3dFromMaskVolumeToBundles,
           const std::string& fileNameIndividualBundle,
           const T& background )
                        : gkg::BundleMeasure< 
                                std::vector< gkg::StatisticalParameterMap > >(),
                            _centroids( centroids ),
                            _labelToIndexLut( labelToIndexLut ),
                            _sectionVolume( sectionVolume ),
                            _sectionCounts( sectionCounts ),
                            _sectionVolumeResolution( 1.0, 1.0, 1.0 ),
                            _scalarVolume( scalarVolume ),
                            _scalarVolumeResolution( 1.0, 1.0, 1.0 ),
                            _rank( rank ),
                            _transform3dFromScalarVolumeToBundles(
                                         transform3dFromScalarVolumeToBundles ),
                            _maskVolume( maskVolume ),
                            _maskVolumeResolution( 1.0, 1.0, 1.0 ),
                            _transform3dFromMaskVolumeToBundles(
                                           transform3dFromMaskVolumeToBundles ),
                            _fileNameIndividualBundle(
                                                     fileNameIndividualBundle ),
                            _background( background ),
                            _centroidLabel( "" )
{

  try
  {

    // collecting section volume resolution
    _sectionVolume->getResolution( _sectionVolumeResolution );

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
             "gkg::BundleFromVolumeStatisticsAlongCentroid< T >::"
             "BundleFromVolumeStatisticsAlongCentroid( "
             "gkg::RCPointer< std::vector< LightCurve3d< float > > >"
             " centroids, "
             "gkg::RCPointer< std::map< std::string, int32_t > > "
             "labelToIndexLut, "
             "gkg::RCPointer< gkg::Volume< int16_t > > sectionVolume, "
             "gkg::RCPointer< std::vector< int32_t > > sectionCounts, "
             "gkg::RCPointer< gkg::Volume< T > > scalarVolume, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "transform3dFromScalarVolumeToBundles, "
             "int32_t rank, "
             "gkg::RCPointer< gkg::Volume< uint8_t > > maskVolume, "
             "gkg::RCPointer< gkg::Transform3d< float > > "
             "transform3dFromMaskVolumeToBundles, "
             "const std::string& fileNameIndividualBundle, "
             "const T& background )" );

}


template < class T >
gkg::BundleFromVolumeStatisticsAlongCentroid< T >::
                                 ~BundleFromVolumeStatisticsAlongCentroid()
{
}


template < class T >
void gkg::BundleFromVolumeStatisticsAlongCentroid< T >::setCentroidLabel(
                                                      const std::string& label )
{

  try
  {

    _centroidLabel = label;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::BundleFromVolumeStatisticsAlongCentroid< T >::"
             "setCentroidLabel( const std::string& label )" );

}


template < class T >
std::vector< gkg::StatisticalParameterMap > 
gkg::BundleFromVolumeStatisticsAlongCentroid< T >::get(
  const typename gkg::BundleFromVolumeStatisticsAlongCentroid< T >::Bundle&
                                                                 bundle ) const
{

  try
  {

    // finding index corresponding to current centroid label
    std::map< std::string, int32_t >::const_iterator
      labelIterator = _labelToIndexLut->find( _centroidLabel );
    int32_t bundleIndex = labelIterator->second;

    // saving current bundle
    if ( _fileNameIndividualBundle != "none" )
    {

      gkg::BundleMap< std::string > individualBundleMap;
      individualBundleMap.addBundle( _centroidLabel, bundle );
      std::string fileNameCurrentBundle = _fileNameIndividualBundle + '-' +
                                          _centroidLabel;
      gkg::Writer::getInstance().write( fileNameCurrentBundle,
                                        individualBundleMap );

    }

    // collecting the section count
    int32_t sectionCount = ( *_sectionCounts )[ bundleIndex ];

    // allocating  profile and count vector of pairs
    std::vector< gkg::UpdatableMeanStandardDeviationMinMax > 
      profileUpdatableMeanStandardDeviationMinMax( sectionCount );

    // looping over fiber(s)
    gkg::Vector3d< int32_t > voxelInSectionVolume;
    gkg::Vector3d< int32_t > voxelInMaskVolume;
    gkg::Vector3d< float > siteInScalarVolume;
    gkg::Vector3d< float > siteInMaskVolume;
    T value = 0;

    if ( _maskVolume.isNull() )
    {

      typename
      gkg::BundleFromVolumeStatisticsAlongCentroid< T >::Bundle::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        gkg::LightCurve3d< float >::const_iterator s = f->begin(),
                                                   se = f->end();
        while ( s != se )
        {

          voxelInSectionVolume.x = ( int32_t )( s->x /
                                                _sectionVolumeResolution.x );
          voxelInSectionVolume.y = ( int32_t )( s->y /
                                                _sectionVolumeResolution.y );
          voxelInSectionVolume.z = ( int32_t )( s->z /
                                                _sectionVolumeResolution.z );


          int16_t label = ( *_sectionVolume )( voxelInSectionVolume,
                                               bundleIndex );

          if ( label > 0 )
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

            profileUpdatableMeanStandardDeviationMinMax[ label - 1 ].add(
                                                              ( double )value );

          }
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
      gkg::BundleFromVolumeStatisticsAlongCentroid< T >::Bundle::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        gkg::LightCurve3d< float >::const_iterator s = f->begin(),
                                                   se = f->end();
        while ( s != se )
        {

          voxelInSectionVolume.x = ( int32_t )( s->x /
                                                _sectionVolumeResolution.x );
          voxelInSectionVolume.y = ( int32_t )( s->y /
                                                _sectionVolumeResolution.y );
          voxelInSectionVolume.z = ( int32_t )( s->z /
                                                _sectionVolumeResolution.z );

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

              int16_t label = ( *_sectionVolume )( voxelInSectionVolume,
                                                   bundleIndex );

              if ( label > 0 )
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

                profileUpdatableMeanStandardDeviationMinMax[ label - 1 ].add(
                                                              ( double )value );

              }

            }

          }
          ++ s;

        }

        ++ f;

      }

    }

    // allocating profile
    std::vector< gkg::StatisticalParameterMap > profile( sectionCount );

    int32_t p = 0;
    for ( p = 0; p < sectionCount; p++ )
    {

      profile[ p ].setStatisticalParameter(
              "minimum",
              profileUpdatableMeanStandardDeviationMinMax[ p ].getMinimum() );
      profile[ p ].setStatisticalParameter(
              "maximum",
              profileUpdatableMeanStandardDeviationMinMax[ p ].getMaximum() );
      profile[ p ].setStatisticalParameter(
              "mean",
              profileUpdatableMeanStandardDeviationMinMax[ p ].getMean() );
      profile[ p ].setStatisticalParameter(
              "standard_deviation",
              profileUpdatableMeanStandardDeviationMinMax[ p ].
                                                       getStandardDeviation() );

    }

    return profile;

  }
  GKG_CATCH( "template < class T >"
             "std::vector< gkg::StatisticalParameterMap > "
             "gkg::BundleFromVolumeStatisticsAlongCentroid< T >::get( "
             "const typename "
             "gkg::BundleFromVolumeStatisticsAlongCentroid< T >::Bundle& "
             "bundle ) const" );

}


template < class T > template < class L, class Compare >
void gkg::BundleFromVolumeStatisticsAlongCentroid< T >::spreadSheetFunctor(
            const std::vector< std::string >& stringParameters,
            const std::vector< double >& scalarParameters,
            gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap,
            std::map< L,
                      std::map< std::string, gkg::Matrix >,
                      Compare >& measures )
{

  try
  {

    // stringParameters[ 0 ] : input centroid bundle map filename
    // stringParameters[ 1 ] : input scalar volume filename
    // stringParameters[ 2 ] : input scalar to bundle map 3D transform filename
    // stringParameters[ 3 ] : mask filename
    // stringParameters[ 4 ] : mask to bundle map 3D transform file name
    // stringParameters[ 5 ] : output density filename
    // stringParameters[ 6 ] : output section filename
    // stringParameters[ 7 ] : output individual bundle root filename

    // scalarParameters[ 0 ] : rank to be used in the input scalar volume
    // scalarParameters[ 1 ] : section width in mm
    // scalarParameters[ 2 ] : resampling step in mm
    // scalarParameters[ 3 ] : X resolution of the section volume(s)
    // scalarParameters[ 4 ] : Y resolution of the section volume(s)
    // scalarParameters[ 5 ] : Z resolution of the section volume(s)
    // scalarParameters[ 6 ] : resampling background level
    // scalarParameters[ 7 ] : verbose

    ////////////////////////////////////////////////////////////////////////////
    // collecting verbosity
    ////////////////////////////////////////////////////////////////////////////
    bool verbose = ( ( ( int32_t )( scalarParameters[ 7 ] + 0.5 ) ) ?
                     true : false );

    ////////////////////////////////////////////////////////////////////////////
    // reading the input scalar volume
    ////////////////////////////////////////////////////////////////////////////

    // analyzing item type of ROI volume
    gkg::AnalyzedObject analyzedObject;
    std::string format;
    gkg::DiskFormatAnalyzer::getInstance().analyze( stringParameters[ 1 ],
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
    gkg::Reader::getInstance().read( stringParameters[ 1 ], *scalarVolume );

    ////////////////////////////////////////////////////////////////////////////
    // reading the input centroid bundle map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading '" << stringParameters[ 0 ] << "' : "
                << std::flush;

    }
    gkg::BundleMap< L, Compare > centroidBundleMap;
    gkg::Reader::getInstance().read( stringParameters[ 0 ], centroidBundleMap );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // preparing centroid(s)
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "preparing centroids : "
                << std::flush;

    }
    int32_t centroidCount = centroidBundleMap.getBundleCount();
    gkg::RCPointer< std::vector< gkg::LightCurve3d< float > > > centroids(
                                new std::vector< gkg::LightCurve3d< float > > );
    centroids->reserve( centroidCount );

    typename gkg::BundleMap< L, Compare >::const_iterator
      l = centroidBundleMap.begin(),
      le = centroidBundleMap.end();
    gkg::RCPointer< std::map< std::string, int32_t > > 
      labelToIndexLut( new std::map< std::string, int32_t > );
    int32_t count = 0;
    while ( l != le )
    {

      const L& label = l->first;
      const typename gkg::BundleMap< L, Compare >::Bundle& bundle = l->second;

      if ( bundle.getCurve3dCount() != 1 )
      {

        throw std::runtime_error(
                  "bundles corresponding to centroids must have only 1 fiber" );

      }

      ( *labelToIndexLut )[ gkg::StringConverter::toString( label ) ] = count;
      centroids->push_back( bundle.getCurve3d( 0 ) );

      ++ count;
      ++ l;

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading the 3D transform
    ////////////////////////////////////////////////////////////////////////////
    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromScalarVolumeToBundles;
    if ( stringParameters[ 2 ] == "id" )
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

      std::ifstream is( stringParameters[ 2 ].c_str() );
      if ( is.fail() )
      {

        throw std::runtime_error( "error while opening " +
                                  stringParameters[ 2 ] );

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

          gkg::AffineWoShearingTransform3d< float >*
            affineWoShearingTransform3d =
              new gkg::AffineWoShearingTransform3d< float >;
          affineWoShearingTransform3d->readTrm( is );
          transform3dFromScalarVolumeToBundles.reset(
                                                  affineWoShearingTransform3d );

        }
        catch ( std::exception& )
        {

          gkg::AffineTransform3d< float >*
            affineTransform3d =
              new gkg::AffineTransform3d< float >;
          affineTransform3d->readTrm( is );
          transform3dFromScalarVolumeToBundles.reset( affineTransform3d );

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

    if ( stringParameters[ 3 ] != "none" )
    {

      if ( verbose )
      {

        std::cout << "reading '" << stringParameters[ 3 ] << "' mask : "
                  << std::flush;

      }

      maskVolume.reset( new gkg::Volume< uint8_t > );
      try
      {

        gkg::Reader::getInstance().read( stringParameters[ 3 ], *maskVolume );

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
      if ( stringParameters[ 4 ] == "id" )
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
        std::ifstream is( stringParameters[ 4 ].c_str() );
        if ( is.fail() )
        {

          throw std::runtime_error( "error while opening " +
                                    stringParameters[ 4 ] );

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

            gkg::AffineWoShearingTransform3d< float >*
              affineWoShearingTransform3d =
                new gkg::AffineWoShearingTransform3d< float >;
            affineWoShearingTransform3d->readTrm( is );
            transform3dFromMaskVolumeToBundles.reset( 
                                                  affineWoShearingTransform3d );

          }
          catch ( std::exception& )
          {

            gkg::AffineTransform3d< float >*
              affineTransform3d =
                new gkg::AffineTransform3d< float >;
            affineTransform3d->readTrm( is );
            transform3dFromMaskVolumeToBundles.reset( affineTransform3d );

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
    // collecting other string information
    ////////////////////////////////////////////////////////////////////////////
    std::string fileNameDensityVolume = stringParameters[ 5 ];
    std::string fileNameSectionVolume = stringParameters[ 6 ];
    std::string fileNameIndividualBundle = stringParameters[ 7 ];

    ////////////////////////////////////////////////////////////////////////////
    // converting scalar information
    ////////////////////////////////////////////////////////////////////////////
    int32_t rank = ( int32_t )( scalarParameters[ 0 ] + 0.5 );
    float sectionWidth = ( float )scalarParameters[ 1 ];
    float resamplingStep = ( float )scalarParameters[ 2 ];
    gkg::Vector3d< double > sectionVolumeResolution;
    sectionVolumeResolution.x = scalarParameters[ 3 ];
    sectionVolumeResolution.y = scalarParameters[ 4 ];
    sectionVolumeResolution.z = scalarParameters[ 5 ];
    T background = ( T )scalarParameters[ 6 ];

    ////////////////////////////////////////////////////////////////////////////
    // computing density and section mask(s)
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {
    
      std::cout << "computing the density and section mask(s) : "
                << std::endl;
    
    }
    gkg::RCPointer< gkg::Volume< float > > 
      densityVolume( new gkg::Volume< float > );
    gkg::RCPointer< gkg::Volume< int16_t > > 
      sectionVolume( new gkg::Volume< int16_t > );
    gkg::RCPointer< std::vector< int32_t > >
      sectionCounts( new std::vector< int32_t > );
    bundleMap->getDensityAndSectionMasks( *densityVolume,
                                          *sectionVolume,
                                          *sectionCounts,
                                          *centroids,
                                          *labelToIndexLut,
                                          sectionWidth,
                                          resamplingStep,
                                          sectionVolumeResolution,
                                          true,
                                          verbose );


    ////////////////////////////////////////////////////////////////////////////
    // writing output section mask
    ////////////////////////////////////////////////////////////////////////////

    if ( stringParameters[ 6 ] != "none" )
    {

      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameSectionVolume
                  << "' : " << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameSectionVolume,
                                        *sectionVolume );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }  


    ////////////////////////////////////////////////////////////////////////////
    // writing output density mask
    ////////////////////////////////////////////////////////////////////////////

    if ( stringParameters[ 5 ] != "none" )
    {

      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameDensityVolume
                  << "' : " << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameDensityVolume,
                                        *densityVolume );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // looping over bundle(s)
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "looping over bundle(s) : "
                << std::flush;

    }
    gkg::BundleFromVolumeStatisticsAlongCentroid< T >
    bundleFromVolumeStatisticsAlongCentroid(
                                          centroids,
                                          labelToIndexLut,
                                          sectionVolume,
                                          sectionCounts,
                                          scalarVolume,
                                          transform3dFromScalarVolumeToBundles,
                                          rank,
                                          maskVolume,
                                          transform3dFromMaskVolumeToBundles,
                                          fileNameIndividualBundle,
                                          background );

    l = bundleMap->begin();
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

      bundleFromVolumeStatisticsAlongCentroid.setCentroidLabel( 
                                     gkg::StringConverter::toString( label ) );
      std::vector< gkg::StatisticalParameterMap >
        fromVolumeStatisticsAlongCentroid =
                          bundleFromVolumeStatisticsAlongCentroid.get( bundle );

      int32_t size = ( int32_t )fromVolumeStatisticsAlongCentroid.size();
      gkg::Matrix measure( 4, size );
      int32_t p = 0;
      for ( p = 0; p < size; p++ )
      {

        measure( 0, p ) = fromVolumeStatisticsAlongCentroid[ p ].
                              getStatisticalParameter( "minimum" );
        measure( 1, p ) = fromVolumeStatisticsAlongCentroid[ p ].
                              getStatisticalParameter( "maximum" );
        measure( 2, p ) = fromVolumeStatisticsAlongCentroid[ p ].
                              getStatisticalParameter( "mean" );
        measure( 3, p ) = fromVolumeStatisticsAlongCentroid[ p ].
                              getStatisticalParameter( "standard_deviation" );

      }

      measures[ label ][ "from_volume_statistics_along_centroid" ] = measure;

      ++ l;

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "template < class L, class Compare > "
             "void gkg::BundleFromVolumeStatisticsAlongCentroid< T >::"
             "spreadSheetFunctor( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap, "
             "std::map< L, "
             "std::map< std::string, gkg::Matrix >, "
             "Compare >& measures )" );


}


template class gkg::BundleFromVolumeStatisticsAlongCentroid< int8_t >;
template class gkg::BundleFromVolumeStatisticsAlongCentroid< uint8_t >;
template class gkg::BundleFromVolumeStatisticsAlongCentroid< int16_t >;
template class gkg::BundleFromVolumeStatisticsAlongCentroid< uint16_t >;
template class gkg::BundleFromVolumeStatisticsAlongCentroid< int32_t >;
template class gkg::BundleFromVolumeStatisticsAlongCentroid< uint32_t >;
template class gkg::BundleFromVolumeStatisticsAlongCentroid< int64_t >;
template class gkg::BundleFromVolumeStatisticsAlongCentroid< uint64_t >;
template class gkg::BundleFromVolumeStatisticsAlongCentroid< float >;
template class gkg::BundleFromVolumeStatisticsAlongCentroid< double >;


template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< int8_t >::
                                              spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< int8_t >::
                                                  spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< uint8_t >::
                                              spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< uint8_t >::
                                                  spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< int16_t >::
                                              spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< int16_t >::
                                                  spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< uint16_t >::
                                              spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< uint16_t >::
                                                  spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< int32_t >::
                                              spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< int32_t >::
                                                  spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< uint32_t >::
                                              spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< uint32_t >::
                                                  spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< int64_t >::
                                              spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< int64_t >::
                                                  spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< uint64_t >::
                                              spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< uint64_t >::
                                                  spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< float >::
                                              spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< float >::
                                                  spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< double >::
                                              spreadSheetFunctor< std::string >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< std::string > > bundleMap,
                 std::map< std::string,
                           std::map< std::string, gkg::Matrix >
                         >& measures );

template void 
gkg::BundleFromVolumeStatisticsAlongCentroid< double >::
                                                  spreadSheetFunctor< int16_t >(
                 const std::vector< std::string >& stringParameters,
                 const std::vector< double >& scalarParameters,
                 gkg::RCPointer< gkg::BundleMap< int16_t > > bundleMap,
                 std::map< int16_t,
                           std::map< std::string, gkg::Matrix >
                         >& measures );


//
// adding check information to FiberMeasureChecker
//

static bool initializeBundleFromVolumeStatisticsAlongCentroidChecker()
{

  try
  {

    gkg::BundleMeasureChecker::getInstance().addChecker(
      "from_volume_statistics_along_centroid",

      ".  <P1>: input centroid bundle map filename\n"
      ".  <P2>: input scalar volume filename\n"
      ".  <P3>: input scalar->bundles 3D transform filename "
                              "(if 'id', take the identity)\n"
      ".  <P4>: input mask volume filename (if 'none', no mask)\n"
      ".  <P5>: input mask->bundles 3D transform filename "
                              "(if 'id', take the identity)\n"
      ".  <P6>: output density filename (if 'none', not saved)\n"
      ".  <P7>: output section filename (if 'none', not saved)\n"
      ".  <P8>: output individual bundle root filename (if 'none', not saved)",

      ".  <P1>: rank to be used in the input scalar volume\n"
      ".  <P2>: section width in mm\n"
      ".  <P3>: resampling step in mm\n"
      ".  <P4>: X resolution of the section volume(s)\n"
      ".  <P5>: Y resolution of the section volume(s)\n"
      ".  <P6>: Z resolution of the section volume(s)\n"
      ".  <P7>: resampling background level\n"
      ".  <P8>: verbosity (1->true, 0-> false)",
      8,
      8,
      0 );

    return true;

  }
  GKG_CATCH( "static bool "
             "initializeBundleFromVolumeStatisticsAlongCentroidChecker()" );

}


static bool initialized __attribute__((unused)) =
                 initializeBundleFromVolumeStatisticsAlongCentroidChecker();

