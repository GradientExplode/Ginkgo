#ifndef _gkg_dmri_global_tractography_GlobalTractographyAlgorithm_i_h_
#define _gkg_dmri_global_tractography_GlobalTractographyAlgorithm_i_h_


#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-dmri-global-tractography/BoundingBoxCommand.h>
#include <gkg-dmri-global-tractography/ResetCommand.h>
#include <gkg-dmri-global-tractography/InitialSpinGlassConfigurationCommand.h>
#include <gkg-dmri-global-tractography/SpinGlassCreationCommand.h>
#include <gkg-dmri-global-tractography/SpinGlassDeletionCommand.h>
#include <gkg-dmri-global-tractography/SpinGlassMoveCommand.h>
#include <gkg-dmri-global-tractography/SpinGlassConnectionCreationCommand.h>
#include <gkg-dmri-global-tractography/SpinGlassConnectionDeletionCommand.h>
#include <gkg-dmri-global-tractography/SpinGlassInformationCommand.h>
#include <gkg-dmri-global-tractography/Log.h>
#include <gkg-dmri-tractography/TractographyAlgorithm_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-dmri-tractography/SeedingStrategy.h>
#include <gkg-processing-container/ContinuousField_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-algobase/RandomIndexVector.h>
#include <gkg-processing-algobase/MinMaxFilter_i.h>
#include <gkg-processing-algobase/UpdatableMeanAndStandardDeviation.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-cppext/CUniStd.h>
#include <gkg-core-pattern/UniqueIdentityGenerator.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <set>
#include <iomanip>
#include <deque>


#define INFINITE_EXTERNAL_ENERGY 10.0
#define STABILITY_LENGTH 200
#define STABILITY_THRESHOLD_RATIO 0.0001


inline void keyboardHit()
{

  char c;

  std::cout << "click <e> to continue..." << std::flush;
  do
  {

    std::cin >> c;

  }
  while ( c != 'e' );

  std::cout << gkg::Eraser( 25 );

}


//
// class SpinGauge
//


inline gkg::GlobalTractographyInitializationGauge::
                       GlobalTractographyInitializationGauge( int32_t maxCount )
                                                  : _count( 0 ),
                                                    _maxCount( maxCount )
{
}


inline void gkg::GlobalTractographyInitializationGauge::add( int32_t value )
{

  try
  {

    _count += value;

    std::cout << gkg::Eraser( 25 );
    std::cout << std::setw( 10 ) << _count
              << " / " << std::setw( 10 ) << _maxCount
              << " ]" << std::flush;

  }
  GKG_CATCH( "inline void gkg::GlobalTractographyInitializationGauge::add( "
             "int32_t value )" );

}


inline void gkg::GlobalTractographyInitializationGauge::reset()
{

  _count = 0;

}


//
// class SpinContext
//

template < class L, class Compare >
inline 
gkg::GlobalTractographyInitializationContext< L, Compare >::
                                        GlobalTractographyInitializationContext(
  gkg::GlobalTractographyInitializationGauge& gauge,
  const gkg::GlobalTractographyAlgorithm< L, Compare >&
                                                    globalTractographyAlgorithm,
  std::map< gkg::Vector3d< float >,
            std::pair< std::vector< float >,
                       std::pair< float, float > >,
            gkg::Vector3dCompare< float > >& buffer,
  std::vector< gkg::Vector3d< int32_t > >& voxels,
  const gkg::RCPointer< gkg::SeedingStrategy >& seedingStrategy,
  const gkg::Vector3d< double >& roiResolution,
  const gkg::Transform3d< float >& transform3d,
  gkg::OdfContinuousField& odfContinuousField,
  const gkg::NumericalAnalysisImplementationFactory* factory,
  const gkg::RCPointer< gkg::RandomGenerator >& randomGenerator,
  float& externalEnergy,
  float& globalMinimumOdfProbability,
  float& globalMaximumOdfProbability,
  gkg::RCPointer< gkg::UniqueIdentityGenerator >& spinGlassUIDGenerator,
  gkg::CartesianField< std::set< gkg::RCPointer< gkg::SpinGlass > > >& 
                                                        spinGlassCartesianField,
  std::set< gkg::RCPointer< gkg::SpinGlass > >& isolatedSpinGlasses,
  bool& verbose )
                              : gkg::LoopContext< int32_t >( &gauge ),
                                _globalTractographyAlgorithm(
                                                  globalTractographyAlgorithm ),
                                _buffer( buffer ),
                                _voxels( voxels ),
                                _seedingStrategy( seedingStrategy ),
                                _roiResolution( roiResolution ),
                                _transform3d( transform3d ),
                                _odfContinuousField( odfContinuousField ),
                                _factory( factory ),
                                _randomGenerator( randomGenerator ),
                                _externalEnergy( externalEnergy ),
                                _globalMinimumOdfProbability(
                                                  globalMinimumOdfProbability ),
                                _globalMaximumOdfProbability(
                                                  globalMaximumOdfProbability ),
                                _spinGlassUIDGenerator( spinGlassUIDGenerator ),
                                _spinGlassCartesianField(
                                                      spinGlassCartesianField ),
                                _isolatedSpinGlasses( isolatedSpinGlasses ),
                                _verbose( verbose ),
                                _outputOrientationSet(
                                  odfContinuousField.getOutputOrientationSet() )
{
}


template < class L, class Compare >
inline
void gkg::GlobalTractographyInitializationContext< L, Compare >::doIt(
                                             int32_t startIndex, int32_t count )
{

  try
  {

    gkg::Vector3d< float > pointOdfs;
    gkg::Vector3d< int32_t >* v = &_voxels[ startIndex ];
    std::set< gkg::RCPointer< gkg::SpinGlass > > isolatedSpinGlasses;
    float spinGlassLength = 0.0f;
    float externalEnergy = 0.0f;
    int32_t voxelIndex = 0;
    int32_t N = count / 10;

    int32_t seedIndex = 0;
    int32_t seedCount = 0;

    while ( count-- )
    {

      if ( _verbose && _gauge )
      {

        if ( voxelIndex && !( voxelIndex % N ) )
        {

          lock();
          _gauge->add( N );
          unlock();

        }

      }


      // computing the coordinates in the floating voxel frame
      gkg::Vector3d< float > floatVoxelRois( ( float )v->x,
                                             ( float )v->y,
                                             ( float )v->z );
      gkg::Vector3d< float > voxelF( floatVoxelRois.x * _roiResolution.x,
                                      floatVoxelRois.y * _roiResolution.y,
                                      floatVoxelRois.z * _roiResolution.z );


      gkg::SeedingStrategy::RegionId
        regionId = _seedingStrategy->getRegionId( *v );
      seedCount = _seedingStrategy->getSeedCount( regionId );

      // looping over seed(s) in real coordinates
      for ( seedIndex = 0; seedIndex < seedCount; seedIndex++ )
      {

        // computing the spin glass position in the ODF real frame
        _transform3d.getDirect( floatVoxelRois, pointOdfs );


        if ( _odfContinuousField.isValid( pointOdfs ) )
        {

          // computing the spin glass position
          gkg::Vector3d< float > spinGlassPosition;
          _seedingStrategy->getSeed( regionId,
                                     pointOdfs,
                                     seedIndex,
                                     spinGlassPosition );


          // computing the ODF at the current point
          gkg::OrientationDistributionFunction*
            odf = _odfContinuousField.getItem( pointOdfs );

          // collecting the principal orientation of the ODF
          // computing the spin glass orientation and position
          const gkg::Vector3d< float >& principalOdfOrientation =
                                                 odf->getPrincipalOrientation();

          // computing the spin glass orientation
          gkg::Vector3d< float > spinGlassOrientation;
          _globalTractographyAlgorithm.getSpinGlassOrientationAndLength(
                                                        principalOdfOrientation,
                                                        _outputOrientationSet,
                                                        *odf,
                                                        spinGlassPosition,
                                                        spinGlassOrientation,
                                                        spinGlassLength );

          // randomly choosing the sign of the orientation
          if ( _factory->getUniformRandomUInt32( *_randomGenerator, 100U ) % 2
               == 1U )
          {

            spinGlassOrientation *= -1.0f;

          }

          // adding the external energy of the current spin glass

          float spinGlassExternalEnergy = _globalTractographyAlgorithm.
                                                     getSpinGlassExternalEnergy(
                                                 _buffer,
                                                 spinGlassPosition,
                                                 spinGlassOrientation,
                                                 *odf,
                                                 _globalMinimumOdfProbability,
                                                 _globalMaximumOdfProbability );

          lock();
          externalEnergy += spinGlassExternalEnergy;
          unlock();

          // deleting the ODF
          delete odf;

          // creating the spin glass
          lock();
          uint64_t uid = _spinGlassUIDGenerator->getIdentity();
          unlock();
          gkg::RCPointer< gkg::SpinGlass >
            spinGlass( new gkg::SpinGlass( uid,
                                           spinGlassPosition,
                                           spinGlassOrientation,
                                           spinGlassLength ) );

          // adding the spin glass to the spin glass Cartesian field
          _spinGlassCartesianField.getItem( *v )->insert( spinGlass );

          // adding the spin glass to the set of isolated spin glasses
          isolatedSpinGlasses.insert( spinGlass );

        }

      }

      ++ v;
      ++ voxelIndex;

    }

    lock();
    _externalEnergy += externalEnergy;
    _isolatedSpinGlasses.insert( isolatedSpinGlasses.begin(),
                                 isolatedSpinGlasses.end() );
    if ( _verbose && _gauge )
    {

      _gauge->add( voxelIndex % N );

    }
    unlock();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyInitializationContext< L, Compare >::"
             "doIt( "
             "int32_t startIndex, int32_t count )" );

}


//
// class GlobalTractographyAlgorithm
//


template < class L, class Compare >
inline
gkg::GlobalTractographyAlgorithm< L, Compare >::GlobalTractographyAlgorithm(
                            const gkg::Vector3d< int32_t >& size,
                            const gkg::Vector3d< double >& resolution,
                            const std::vector< double >& scalarParameters,
                            const std::vector< std::string >& stringParameters )
                                  : gkg::TractographyAlgorithm< L, Compare >(
                                                                   size,
                                                                   resolution ),
                                    _scalarParameters( scalarParameters ),
                                    _stringParameters( stringParameters )
{

  try
  {

    // initializing protected members
    _minimumFiberLength = ( float )scalarParameters[ 0 ];
    _maximumFiberLength = ( float )scalarParameters[ 1 ];
    _iterationCount = ( int32_t )( scalarParameters[ 2 ] + 0.5 );
    _iterationCountMinusOne = _iterationCount - 1;
    _iterationCountOverThree = _iterationCount / 3;
    _gibbsTemperature = ( float )scalarParameters[ 3 ];
    _externalTemperature = ( float )scalarParameters[ 4 ];
    _initialInternalTemperature = ( float )scalarParameters[ 5 ];
    _finalInternalTemperature = ( float )scalarParameters[ 6 ];
    _externalEnergyWeight = ( float )scalarParameters[ 7 ];
    _internalEnergyWeight = ( float )scalarParameters[ 8 ];
    _cliqueNeighborCount = ( uint32_t )( scalarParameters[ 9 ] + 0.5 );

    // getting access to numerical analysis factory, creating a random generator
    // and opening the writer to the viewer
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    _randomGenerator.reset(
                       new gkg::RandomGenerator( gkg::RandomGenerator::Taus ) );
    _writer.reset( new gkg::RpcCommandWriter( "localhost", 8002 ) );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::GlobalTractographyAlgorithm::GlobalTractographyAlgorithm( "
             "const gkg::Vector3d< int32_t >& size, "
             "const gkg::Vector3d< double >& resolution, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters )" );

}


template < class L, class Compare >
inline
gkg::GlobalTractographyAlgorithm< L, Compare >::~GlobalTractographyAlgorithm()
{
}


template < class L, class Compare >
inline
const std::vector< double >&
gkg::GlobalTractographyAlgorithm< L, Compare >::getScalarParameters() const
{

  try
  {

    return _scalarParameters;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "const std::vector< double >& "
             "gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "getScalarParameters() const" );

}


template < class L, class Compare >
inline
const std::vector< std::string >&
gkg::GlobalTractographyAlgorithm< L, Compare >::getStringParameters() const
{

  try
  {

    return _stringParameters;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "const std::vector< std::string >& "
             "gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "getStringParameters() const" );

}


template < class L, class Compare >
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::prepare(
                  gkg::OdfCartesianField& /* odfCartesianField */,
                  const gkg::Volume< int16_t >& computingMask,
                  const gkg::Transform3d< float >&
                                       /* transform3dFromRoisVoxelToOdfsReal */,
                  const gkg::Transform3d< float >&
                                      /* transform3dFromRoisVoxelToMaskVoxel */,
                  const gkg::Transform3d< float >&
                                       /* transform3dFromOdfsRealToMaskVoxel */,
                  const gkg::Transform3d< float >&
                                       /* transform3dFromOdfsRealToRoisVoxel */,
                  bool verbose )
{

  try
  {

    //--------------------------------------------------------------------------
    INIT_DEBUG
    //--------------------------------------------------------------------------

    ////////////////////////////////////////////////////////////////////////////
    // computing the adequate spin glass length
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "computed spin glass length : " << std::flush;

    }

    // getting the header information about the mask size
    gkg::Vector3d< int32_t > maskSize;
    computingMask.getSize( maskSize );

    // getting the header information about the mask resolution
    gkg::Vector3d< double > maskResolution;
    computingMask.getResolution( maskResolution );

    // computing the minimum mask resolution
    double minimumMaskResolution = std::min( maskResolution.x,
                                             std::min( maskResolution.y,
                                                       maskResolution.z ) );

    if ( ( maskResolution.x > 1.2 * minimumMaskResolution ) ||
         ( maskResolution.y > 1.2 * minimumMaskResolution ) ||
         ( maskResolution.z > 1.2 * minimumMaskResolution ) )
    {

      throw std::runtime_error( "mask voxel resolution is too anisotropic" );

    }

    // setting the initial spin glass length to 80% of the distance between
    // two seeding site(s)

///////////////////////TOBEMODIFIED
    _voxelSamplerCount = 1; //( int32_t )( _scalarParameters[ 0 ] + 0.5 );
    _spinGlassLength = ( float )( 0.8 * minimumMaskResolution / 
                                  std::pow( ( double )_voxelSamplerCount,
                                            1.0 / 3.0 ) );

    if ( verbose )
    {

      std::cout << _spinGlassLength << " mm" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating the anatomical priors
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building the anatomical priors : " << std::flush;

    }

    // resetting the anatomical priors 
    _anatomicalPriors.reset( new gkg::AnatomicalPriors( _scalarParameters,
                                                        _stringParameters,
                                                        maskSize,
                                                        maskResolution ) );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::prepare( "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const gkg::Volume< int16_t >& computingMask, "
             "const gkg::Transform3d< float >& "
             "transform3dFromRoisVoxelToOdfsReal, "
             "const gkg::Transform3d< float >& "
             "transform3dFromRoisVoxelToMaskVoxel, "
             "const gkg::Transform3d< float >& "
             "transform3dFromOdfsRealToMaskVoxel, "
             "const gkg::Transform3d< float >& "
             "transform3dFromOdfsRealToRoisVoxel, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::track(
          const gkg::SiteMap< L, int32_t, Compare >& rois,
          const gkg::SiteMap< L, int32_t, Compare >& /*subRois*/,
          const gkg::RCPointer< gkg::SeedingStrategy >& seedingStrategy,
          const gkg::RCPointer< gkg::StoppingStrategy >& /*stoppingStrategy*/,
          const gkg::RCPointer< gkg::StartingOrientationStrategy >&
                                              /*startingOrientationStrategy*/,
          gkg::OdfContinuousField& odfContinuousField,
          const gkg::Volume< int16_t >& /*computingMask*/,
          const gkg::Transform3d< float >&
                                         transform3dFromRoisVoxelToOdfsReal,
          const gkg::Transform3d< float >&
                                  /* transform3dFromRoisVoxelToMaskVoxel */,
          const gkg::Transform3d< float >&
                                   /* transform3dFromOdfsRealToMaskVoxel */,
          const gkg::Transform3d< float >&
                                   /* transform3dFromOdfsRealToRoisVoxel */,
          gkg::BundleMap< L, Compare >& bundleMap,
          bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // resetting the viewer if connected
    ////////////////////////////////////////////////////////////////////////////

    if ( _writer->isConnected() )
    {

      gkg::ResetCommand resetCommand;
      _writer->send( resetCommand );
      _writer->flush();

    }

    ////////////////////////////////////////////////////////////////////////////
    // evaluating mask size(s) and copying it to a vector of Vector3d< int32_t >
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > roiSize( 1, 1, 1 );
    L label = rois.getRank( 0 );
    int32_t voxelCount = rois.getSiteCount( label );
    std::vector< gkg::Vector3d< int32_t > > voxels( voxelCount );
    int32_t voxelIndex = 0;
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = rois.getSites( label ).begin(),
      se = rois.getSites( label ).end();

    gkg::Vector3d< int32_t > lowerCorner( s->x, s->y, s->z );
    gkg::Vector3d< int32_t > upperCorner( s->x, s->y, s->z );

    while ( s != se )
    {

      // updating lowerCorner
      if ( s->x < lowerCorner.x )
      {

        lowerCorner.x = s->x;

      }
      if ( s->y < lowerCorner.y )
      {

        lowerCorner.y = s->y;

      }
      if ( s->z < lowerCorner.z )
      {

        lowerCorner.z = s->z;

      }

      // updating upperCorner
      if ( s->x > upperCorner.x )
      {

        upperCorner.x = s->x;

      }
      if ( s->y > upperCorner.y )
      {

        upperCorner.y = s->y;

      }
      if ( s->z > upperCorner.z )
      {

        upperCorner.z = s->z;

      }

      // updating roiSize
      if ( s->x + 1 > roiSize.x )
      {

        roiSize.x = s->x + 1;

      }
      if ( s->y + 1 > roiSize.y )
      {

        roiSize.y = s->y + 1;

      }
      if ( s->z + 1 > roiSize.z )
      {

        roiSize.z = s->z + 1;

      }
      voxels[ voxelIndex ] = *s;
      ++ voxelIndex;
      ++ s;

    }

    // collecting the ROI resolution
    gkg::Vector3d< double > roiResolution( 1.0, 1.0, 1.0 );
    if ( rois.getHeader().hasAttribute( "resolutionX" ) )
    {

      rois.getHeader().getAttribute( "resolutionX", roiResolution.x );

    }
    if ( rois.getHeader().hasAttribute( "resolutionY" ) )
    {

      rois.getHeader().getAttribute( "resolutionY", roiResolution.y );

    }
    if ( rois.getHeader().hasAttribute( "resolutionZ" ) )
    {

      rois.getHeader().getAttribute( "resolutionZ", roiResolution.z );

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating a computing mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< int16_t > computingMask( roiSize );
    computingMask.setResolution( roiResolution );
    computingMask.fill( 0 );
    s = rois.getSites( label ).begin();
    while ( s != se )
    {

      computingMask( *s ) = 1U;
      ++ s;

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating a bounding box for the computing mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::BoundingBox< int32_t > computingMaskBoundingBox( 0, roiSize.x - 1,
                                                          0, roiSize.y - 1,
                                                          0, roiSize.z - 1 );

    ////////////////////////////////////////////////////////////////////////////
    // sending the information used to get the ROI bounding box to the viewer if
    // connected
    ////////////////////////////////////////////////////////////////////////////

    if ( _writer->isConnected() )
    {

      gkg::BoundingBoxCommand boundingBoxCommand( lowerCorner,
                                                  upperCorner,
                                                  roiResolution );

      _writer->send( boundingBoxCommand );
      _writer->flush();

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating a set of spin glass(es), and sets to record all the
    // isolated / one extremity attached / fully attached spin glasses
    // and creating a Cartesian LUT of spin glasses to efficiently get access
    // to the spin glasses of given voxel
    ////////////////////////////////////////////////////////////////////////////

    // creating a deque of pointers to the spin glasses
    std::set< gkg::RCPointer< gkg::SpinGlass > > isolatedSpinGlasses;
    std::set< gkg::RCPointer< gkg::SpinGlass > > oneSideAttachedSpinGlasses;
    std::set< gkg::RCPointer< gkg::SpinGlass > > fullyAttachedSpinGlasses;
    std::set< gkg::RCPointer< gkg::SpinGlass > >
                                          oneSideAttachedFarFromPialSpinGlasses;

    // creating a set of connections
    std::set< gkg::RCPointer< gkg::SpinGlassConnection > > spinGlassConnections;

    // creating a generator of unique id for each created spin glass
    gkg::RCPointer< gkg::UniqueIdentityGenerator >
      spinGlassUniqueIdentityGenerator( new gkg::UniqueIdentityGenerator );

    // creating a generator of unique id for each created connection
    gkg::RCPointer< gkg::UniqueIdentityGenerator >
      spinGlassConnectionUniqueIdentityGenerator(
                                             new gkg::UniqueIdentityGenerator );

    // creating also a Cartesian field to provide a lookup table for fast access
    // to the spin glasses within a voxel
    gkg::SiteMap< int32_t, int32_t > tmpRois;
    tmpRois.addSites( 0, rois.getSites( label ) );
    tmpRois.getHeader().addAttribute( "sizeX", ( double )roiSize.x );
    tmpRois.getHeader().addAttribute( "sizeY", ( double )roiSize.y );
    tmpRois.getHeader().addAttribute( "sizeZ", ( double )roiSize.z );
    tmpRois.getHeader().addAttribute( "resolutionX", roiResolution.x );
    tmpRois.getHeader().addAttribute( "resolutionY", roiResolution.y );
    tmpRois.getHeader().addAttribute( "resolutionZ", roiResolution.z );

    gkg::TextureMap< std::set< gkg::RCPointer< gkg::SpinGlass > > >
                                                            spinGlassTextureMap;
    std::vector< std::set< gkg::RCPointer< gkg::SpinGlass > > >
                                                    spinGlassSets( voxelCount );
    spinGlassTextureMap.addTextures( spinGlassSets );

    gkg::CartesianField< std::set< gkg::RCPointer< gkg::SpinGlass > > > 
      spinGlassCartesianField( tmpRois, spinGlassTextureMap );

    ////////////////////////////////////////////////////////////////////////////
    // declaring an internal temperature and computing the slope
    ////////////////////////////////////////////////////////////////////////////

    float internalTemperature = _initialInternalTemperature;
    //double internalTemperatureSlope = std::log( _initialInternalTemperature /
    //                                            _finalInternalTemperature ) /
    //                                  ( double )_iterationCount;

    ////////////////////////////////////////////////////////////////////////////
    // getting access to the output orientation set
    ////////////////////////////////////////////////////////////////////////////

    const gkg::OrientationSet& outputOrientationSet = 
                                   odfContinuousField.getOutputOrientationSet();

    ////////////////////////////////////////////////////////////////////////////
    // computing the minimum and maximum value(s) of ODF probabilities through
    // the entire set of voxels
    ////////////////////////////////////////////////////////////////////////////

    std::map< gkg::Vector3d< float >,
              std::pair< std::vector< float >, std::pair< float, float > >,
              gkg::Vector3dCompare< float > > buffer;

    gkg::Vector3d< float > floatVoxelRois;
    gkg::Vector3d< float > pointOdfs;
    float globalMinimumOdfProbability = 1.0;
    float globalMaximumOdfProbability = 0.0;
    std::vector< float > orientationProbabilities;
    float localMinimumOdfProbability = 0.0;
    float localMaximumOdfProbability = 1.0;

    gkg::Vector3d< float > seed;
    int32_t seedIndex = 0;
    int32_t seedCount = 0;
    std::vector< gkg::Vector3d< int32_t > >::const_iterator
      v = voxels.begin(),
      ve = voxels.end();
    while ( v != ve )
    {

      // computing the coordinates in the floating voxel frame
      floatVoxelRois.x = ( float )v->x;
      floatVoxelRois.y = ( float )v->y;
      floatVoxelRois.z = ( float )v->z;

      // computing the spin glass position in the ODF real frame
      transform3dFromRoisVoxelToOdfsReal.getDirect( floatVoxelRois, pointOdfs );


      if ( odfContinuousField.isValid( pointOdfs ) )
      {

        gkg::SeedingStrategy::RegionId
          regionId = seedingStrategy->getRegionId( *v );
        seedCount = seedingStrategy->getSeedCount( regionId );

        // looping over seed(s) in real coordinates
        for ( seedIndex = 0; seedIndex < seedCount; seedIndex++ )
        {

          // calculating starting point in the ODF real frame
          gkg::Vector3d< float > startingPointOdfs;
          seedingStrategy->getSeed( regionId,
                                     pointOdfs,
                                     seedIndex,
                                     startingPointOdfs );

          // computing the ODF at the current point
          gkg::OrientationDistributionFunction*
            odf = odfContinuousField.getItem( seed );

          // storing the global minimum orientation probability
          odf->getMinMaxAndOrientationProbabilities(
                                                   orientationProbabilities,
                                                   localMinimumOdfProbability,
                                                   localMaximumOdfProbability );
          buffer[ seed ] = std::pair< std::vector< float >,
                                        std::pair< float, float > >(
                                          orientationProbabilities,
                                          std::pair< float, float >(
                                                localMinimumOdfProbability,
                                                localMaximumOdfProbability ) );
          if ( localMinimumOdfProbability < globalMinimumOdfProbability )
          {

            globalMinimumOdfProbability = localMinimumOdfProbability;

          }
          if ( localMaximumOdfProbability > globalMaximumOdfProbability )
          {

            globalMaximumOdfProbability = localMaximumOdfProbability;

          }

          delete odf;

        }

      }
      ++ v;

    }

    // pre-computing a scaling used in the green ration computation of 
    // DEATH proposal
    float deathScaling = std::sqrt( globalMinimumOdfProbability * 
                                    globalMaximumOdfProbability ) /
                         ( ( float )voxelCount * ( float )_voxelSamplerCount );

    //--------------------------------------------------------------------------
    BEGIN_DEBUG
    os << "global minimum ODF probability : "
       << globalMinimumOdfProbability << std::endl;
    os << "global maximum ODF probability : "
       << globalMaximumOdfProbability << std::endl;
    os << "std::sqrt( globalMinimumOdfProbability * \n" 
          "           globalMaximumOdfProbability ) : "
       << std::sqrt( globalMinimumOdfProbability * 
                     globalMaximumOdfProbability ) << std::endl;
    os << "death scaling : "
       << deathScaling << std::endl;
    END_DEBUG
    //--------------------------------------------------------------------------

    ////////////////////////////////////////////////////////////////////////////
    // creating an initial spin glass configuration and computing the
    // corresponding initial external energy
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "creating initial spin configuration [ "
                << std::setw( 10 ) << 0
                << " / " << std::setw( 10 ) << voxelCount
                << " ]" << std::flush;

    }

    voxelIndex = 0;
    gkg::Vector3d< float > normalToPialSurface;

    ////////////////////////////////////////////////////////////////////////////
    // for display purpose
    ////////////////////////////////////////////////////////////////////////////
    std::vector< float > distancesToPial;
    std::vector< float > spinGlassLengths;
    std::vector< float > connectionLikelihoods;

    ////////////////////////////////////////////////////////////////////////////

    float externalEnergy = 0.0f;
    float internalEnergy = 0.0f;
    float globalEnergy = 0.0f;

    gkg::GlobalTractographyInitializationGauge
      gauge( ( int32_t )voxels.size() );
    gkg::GlobalTractographyInitializationContext< L, Compare >
                                   context( gauge,
                                            *this,
                                            buffer,
                                            voxels,
                                            seedingStrategy,
                                            roiResolution,
                                            transform3dFromRoisVoxelToOdfsReal,
                                            odfContinuousField,
                                            _factory,
                                            _randomGenerator,
                                            externalEnergy,
                                            globalMinimumOdfProbability,
                                            globalMaximumOdfProbability,
                                            spinGlassUniqueIdentityGenerator,
                                            spinGlassCartesianField,
                                            isolatedSpinGlasses,
                                            verbose );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context,
                                               0,
                                               ( int32_t )voxels.size() );
    threadedLoop.launch();

    // storing the initial spin glass count
    int32_t initialSpinGlassCount = ( int32_t )isolatedSpinGlasses.size();


    // initializing the global energy
    globalEnergy = _externalEnergyWeight * externalEnergy +
                   _internalEnergyWeight * internalEnergy;


    std::cout << gkg::Eraser( 63 ) << std::flush;

    //--------------------------------------------------------------------------
    BEGIN_DEBUG
    os << "initial spin glass count : " << initialSpinGlassCount << std::endl;
    os << "externalEnergy : " << externalEnergy
       << std::endl;
    os << "internalEnergy : " << internalEnergy
       << std::endl;
    os << "globalEnergy : " << globalEnergy << std::endl;
    END_DEBUG
    //--------------------------------------------------------------------------


    if ( verbose )
    {

      
      std::cout << "log(Pmin) : " << -std::log( globalMinimumOdfProbability )
                << std::endl;
      std::cout << "log(Pmax) : " << -std::log( globalMaximumOdfProbability )
                << std::endl;
      std::cout << "initial spin count : " << initialSpinGlassCount
                << std::endl;
      std::cout << "initial global energy : " << globalEnergy << std::endl;

    }


    // sanity check: checking thet the spin glasses is not empty, because if
    // it is the case, no tractography should be made
    if ( initialSpinGlassCount <= 1 )
    {

      throw std::runtime_error(
                      "less than 2 spin glasses in the initial configuration" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving the initial spin glasses configuration to mesh
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      this->saveSpinGlassesToMesh( isolatedSpinGlasses,
                                   "initial_spinglasses.mesh",
                                   false,
                                   "aimsmesh" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // sending initial configuration to the viewer if connected
    ////////////////////////////////////////////////////////////////////////////
    if ( _writer->isConnected() )
    {

      gkg::InitialSpinGlassConfigurationCommand
        initialSpinGlassConfigurationCommand( isolatedSpinGlasses );

      _writer->send( initialSpinGlassConfigurationCommand );
      _writer->flush();

    }


    ////////////////////////////////////////////////////////////////////////////
    // launching the Metropolis-Hasting optimizer
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << gkg::Eraser( 63 );
      std::cout << "optimizing spin glass field [ "
                << std::setw( 10 ) << 0
                << " / " << std::setw( 10 ) << voxelCount
                << " ]" << std::flush;

    }

    // counters for the spin glass connection proposal
    int32_t acceptedSimpleConnectionCount = 0;
    int32_t rejectedSimpleConnectionCount = 0;
    int32_t reconnectionToOlderCount = 0;
    int32_t acceptedCreationConnectionCount = 0;
    int32_t rejectedCreationConnectionCount = 0;
    int32_t nothingToDoInSpinGlassConnectionCount = 0;
    int32_t proposedSpinGlassConnectionCount = 0;

    // counters for the spin glass optimal motion proposal
    int32_t acceptedSpinGlassOptimalMotionCount = 0;
    int32_t rejectedSpinGlassOptimalMotionCount = 0;
    int32_t nothingToDoInSpinGlassOptimalMotionCount = 0;
    int32_t proposedSpinGlassOptimalMotionCount = 0;

    // counters for the spin glass random motion proposal
    int32_t acceptedSpinGlassRandomMotionCount = 0;
    int32_t rejectedSpinGlassRandomMotionCount = 0;
    int32_t nothingToDoInSpinGlassRandomMotionCount = 0;
    int32_t proposedSpinGlassRandomMotionCount = 0;

    // counters for the spin glass death proposal
    int32_t acceptedSpinGlassDeathCount = 0;
    int32_t rejectedSpinGlassDeathCount = 0;
    int32_t proposedSpinGlassDeathCount = 0;

    // defining the new energies, the associated variation and the Green ratio
    float newExternalEnergy = 0.0f;
    float newInternalEnergy = 0.0f;
    float newGlobalEnergy = 0.0f;
    float deltaExternalEnergy = 0.0f;
    float deltaInternalEnergy = 0.0f;
    float deltaGlobalEnergy = 0.0f;
    float greenRatio = 0.0f;


    // from connection
    float oldConnectionEnergy = 0.0f;
    float newConnectionEnergy = 0.0f;

    // from connection, optimal or random motion
    float oldSpinGlassExternalEnergy = 0.0f;
    float newSpinGlassExternalEnergy = 0.0f;

    // from optimal or random motion
    float oldSpinGlassConnectionEnergy = 0.0f;
    float newSpinGlassConnectionEnergy = 0.0f;

    ////////////////////////////////////////////////////////////////////////////
    // display purposes
    ////////////////////////////////////////////////////////////////////////////
    std::vector< float > globalEnergies( _iterationCount );
    std::vector< float > externalEnergies( _iterationCount );
    std::vector< float > internalEnergies( _iterationCount );
    std::vector< int32_t > iterations( _iterationCount );

    int32_t iteration = 0;
    gkg::RCPointer< gkg::SpinGlass > selectedSpinGlass;
    gkg::Vector3d< int32_t > voxel;


    // from connection
    gkg::Vector3d< int32_t > successorVoxel;
    gkg::RCPointer< gkg::SpinGlass > successorSpinGlass;
    gkg::SpinGlass::ExtremityType successorSpinGlassExtremityType;

    std::deque< std::pair< gkg::RCPointer< gkg::SpinGlass >,
                           gkg::SpinGlass::ExtremityType > >
                                        neighboringSpinGlassesAndExtremityTypes;
    gkg::SpinGlass::ExtremityType selectedSpinGlassExtremityType;
    gkg::RCPointer< gkg::SpinGlassConnection > oldSpinGlassConnection;
    gkg::RCPointer< gkg::SpinGlass > oldConnectedSpinGlass;
    gkg::SpinGlass::ExtremityType oldConnectedSpinGlassExtremityType;

    // from optimal or random motion

    gkg::Vector3d< float > oldSpinGlassPosition;
    gkg::Vector3d< float > oldSpinGlassOrientation;
    gkg::Vector3d< int32_t > newVoxel;
    gkg::Vector3d< float > newSpinGlassPosition;
    gkg::Vector3d< float > newSpinGlassOrientation;

    // from optimal motion
    gkg::RCPointer< gkg::SpinGlass > alternativeSpinGlass;

    // from death
    float spinGlassDensity = 0.0f;
    float spinGlassExternalEnergy = 0.0f;

    for ( iteration = 0; iteration < _iterationCount; iteration++ )
    {

      // display the current iteration
      if ( verbose )
      {

        if ( ( ( iteration + 1 ) % 1000 == 0 ) ||
             ( ( iteration + 1 == _iterationCount ) ) )
        {

          std::cout << gkg::Eraser( 25 );
          std::cout << std::setw( 10 ) << iteration + 1
                    << " / " << std::setw( 10 ) << _iterationCount
                    << " ]" << std::flush;

        }

      }

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "##################################################################"
         << "iteration = " << iteration << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

      //////////////////////////////////////////////////////////////////////////
      // selecting a spin glass
      //////////////////////////////////////////////////////////////////////////

      // computing the spin glass count
      int32_t globalSpinGlassCount = ( int32_t )(
                                            isolatedSpinGlasses.size() +
                                            oneSideAttachedSpinGlasses.size() +
                                            fullyAttachedSpinGlasses.size() );

      // choosing the spin glass
      gkg::GlobalTractographyAlgorithm< L, Compare >::SpinGlassConfiguration
        selectedSpinGlassConfiguration = this->getRandomSpinGlass(
                                                     iteration,
                                                     isolatedSpinGlasses,
                                                     oneSideAttachedSpinGlasses,
                                                     fullyAttachedSpinGlasses,
                                                     globalSpinGlassCount,
                                                     selectedSpinGlass );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "selectedSpinGlass id : "
         << selectedSpinGlass->getIdentity() << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

      // sending the chosen spin glass information to the viewer if connceted
      if ( _writer->isConnected() )
      {

        gkg::SpinGlassInformationCommand
          spinGlassInformationCommand( selectedSpinGlass );
        _writer->send( spinGlassInformationCommand );
        _writer->flush();

      }

      // computing the voxel corresponding to the spin glass position
      this->getVoxelFromPosition( selectedSpinGlass->getPosition(),
                                  roiResolution,
                                  voxel );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "voxel" << voxel << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

      //////////////////////////////////////////////////////////////////////////
      // choosing the modification type
      //////////////////////////////////////////////////////////////////////////

      gkg::GlobalTractographyAlgorithm< L, Compare >::ModificationProposal
        modificationProposal = this->getModificationProposal(
                                                 selectedSpinGlassConfiguration,
                                                 iteration,
                                                 globalSpinGlassCount );


      //////////////////////////////////////////////////////////////////////////
      // case 'connection of a spin-glass'
      //////////////////////////////////////////////////////////////////////////

      if ( modificationProposal == 
                gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                   SpinGlassConnectionProposal )
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "======================== SPIN GLASS CONNECTION ================="
           << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        oldSpinGlassConnection.reset();
        oldConnectedSpinGlass.reset();

        neighboringSpinGlassesAndExtremityTypes.clear();
        successorSpinGlass.reset();

        oldConnectionEnergy = 0.0f;
        newSpinGlassExternalEnergy = 0.0f;
        newConnectionEnergy = 0.0f;

        // making the spin glass connection proposal
        typename gkg::GlobalTractographyAlgorithm< L, Compare >::ConnectionType
          connectionType = this->makeSpinGlassConnectionProposal(
                                        buffer,
                                        selectedSpinGlass,
                                        selectedSpinGlassExtremityType,
                                        voxel,
                                        spinGlassCartesianField,
                                        internalTemperature,
                                        oldSpinGlassConnection,
                                        oldConnectedSpinGlass,
                                        oldConnectedSpinGlassExtremityType,
                                        computingMaskBoundingBox,
                                        computingMask,
                                        roiResolution,
                                        odfContinuousField,
                                        transform3dFromRoisVoxelToOdfsReal,
                                        outputOrientationSet,
                                        globalMinimumOdfProbability,
                                        globalMaximumOdfProbability,
                                        spinGlassUniqueIdentityGenerator,
                                        neighboringSpinGlassesAndExtremityTypes,
                                        successorVoxel,
                                        successorSpinGlass,
                                        successorSpinGlassExtremityType,
                                        isolatedSpinGlasses,
                                        oldConnectionEnergy,
                                        newSpinGlassExternalEnergy,
                                        newConnectionEnergy );

        // in the case of a simple connection
        if ( connectionType ==
             gkg::GlobalTractographyAlgorithm< L, Compare >::SimpleConnection )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "SimpleConnection" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // computing the new internal energy
          newInternalEnergy = internalEnergy +
                              ( newConnectionEnergy - oldConnectionEnergy );

          // computing the internal energy variation
          deltaInternalEnergy =
                 _internalEnergyWeight * ( newInternalEnergy - internalEnergy );

          // computing the new global energy
          newGlobalEnergy = _externalEnergyWeight * externalEnergy +
                            _internalEnergyWeight * newInternalEnergy;

          // computing the global energy variation
          deltaGlobalEnergy = newGlobalEnergy - globalEnergy;

          // computing the Green ratio
          greenRatio = std::exp( -deltaInternalEnergy / internalTemperature );

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "oldConnectionEnergy = " << oldConnectionEnergy << std::endl;
          os << "newConnectionEnergy = " << newConnectionEnergy << std::endl;
          os << "internalEnergy = " << internalEnergy << std::endl;
          os << "newInternalEnergy = " << newInternalEnergy << std::endl;
          os << "deltaInternalEnergy = " << deltaInternalEnergy << std::endl;
          os << "globalEnergy = " << globalEnergy << std::endl;
          os << "newGlobalEnergy = " << newGlobalEnergy << std::endl;
          os << "deltaGlobalEnergy = " << deltaGlobalEnergy << std::endl;
          os << "greenRatio = " << greenRatio << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // checking if the modification is accepted or rejected
          if ( this->getAcceptance( greenRatio ) )
          {

            //------------------------------------------------------------------
            BEGIN_DEBUG
            os << "accepted spin glass simple connection proposal" << std::endl;
            END_DEBUG
            //------------------------------------------------------------------

            // erasing the old spin glass connection
            this->removeSpinGlassConnection( oldSpinGlassConnection,
                                             spinGlassConnections );

            // creating the new spin glass connection
            this->createSpinGlassConnection(
                                     selectedSpinGlass,
                                     selectedSpinGlassExtremityType,
                                     successorSpinGlass,
                                     successorSpinGlassExtremityType,
                                     spinGlassConnectionUniqueIdentityGenerator,
                                     spinGlassConnections );

            // updating the spin glass sets accordingly
            this->updateSpinGlassSets( selectedSpinGlass,
                                       oldConnectedSpinGlass,
                                       successorSpinGlass,
                                       isolatedSpinGlasses,
                                       oneSideAttachedSpinGlasses,
                                       fullyAttachedSpinGlasses,
                                       oneSideAttachedFarFromPialSpinGlasses );

            // updating the energies
            internalEnergy = newInternalEnergy;
            globalEnergy = newGlobalEnergy;

            // updating the simple connection count
            ++ acceptedSimpleConnectionCount;

          }
          else
          {

            //------------------------------------------------------------------
            BEGIN_DEBUG
            os << "rejected spin glass simple connection proposal" << std::endl;
            END_DEBUG
            //------------------------------------------------------------------

            // checking that the old spin glass connection is not null and then
            // reconnecting the spin glasses through the old spin glass
            // connection
            if ( !oldSpinGlassConnection.isNull() )
            {

              oldSpinGlassConnection->connect( 
                         std::make_pair( selectedSpinGlass,
                                         selectedSpinGlassExtremityType ),
                         std::make_pair( oldConnectedSpinGlass,
                                         oldConnectedSpinGlassExtremityType ) );

              /////////////////////////---
              selectedSpinGlass->setSpinGlassConnection(
                                        selectedSpinGlassExtremityType,
                                        oldSpinGlassConnection );
              oldConnectedSpinGlass->setSpinGlassConnection(
                                        oldConnectedSpinGlassExtremityType,
                                        oldSpinGlassConnection );

              // updating the reconnection to older count
              ++ reconnectionToOlderCount;

            }

            // updating the rejected simple connection count
            ++ rejectedSimpleConnectionCount;

          }

        }
        else if ( connectionType ==
                  gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                       ValidCreationConnection )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "ValidCreationConnection" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // computing the new external energy
          newExternalEnergy = externalEnergy + newSpinGlassExternalEnergy;

          // computing the external energy variation
          deltaExternalEnergy =
            _externalEnergyWeight * ( newExternalEnergy - externalEnergy );

          // computing the new internal energy
          newInternalEnergy = internalEnergy + newConnectionEnergy;

          // computing the internal energy variation
          deltaInternalEnergy =
            _internalEnergyWeight * ( newInternalEnergy - internalEnergy );

          // computing the new global energy
          newGlobalEnergy = _externalEnergyWeight * newExternalEnergy +
                            _internalEnergyWeight * newInternalEnergy;

          // computing the global energy variation
          deltaGlobalEnergy = newGlobalEnergy - globalEnergy;

          // computing the Green ratio
          greenRatio =
            std::exp( -( ( deltaExternalEnergy / _externalTemperature ) +
                         ( deltaInternalEnergy / internalTemperature ) ) );

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "newSpinGlassExternalEnergy = "
             << newSpinGlassExternalEnergy << std::endl;
          os << "externalEnergy = " << externalEnergy << std::endl;
          os << "newExternalEnergy = " << newExternalEnergy << std::endl;
          os << "deltaExternalEnergy = " << deltaExternalEnergy << std::endl;
          os << "newConnectionEnergy = " << newConnectionEnergy << std::endl;
          os << "internalEnergy = " << internalEnergy << std::endl;
          os << "newInternalEnergy = " << newInternalEnergy << std::endl;
          os << "deltaInternalEnergy = " << deltaInternalEnergy << std::endl;
          os << "globalEnergy = " << globalEnergy << std::endl;
          os << "newGlobalEnergy = " << newGlobalEnergy << std::endl;
          os << "deltaGlobalEnergy = " << deltaGlobalEnergy << std::endl;
          os << "deltaInternalEnergy / internalTemperature = "
             << deltaInternalEnergy / internalTemperature << std::endl;
          os << "deltaExternalEnergy / _externalTemperature = "
             << deltaExternalEnergy / _externalTemperature << std::endl;
          os << "greenRatio = " << greenRatio << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // storing the successor spin glass position in case it has to be
          // erased
          gkg::Vector3d< float > successorSpinGlassPosition =
                                              successorSpinGlass->getPosition();

          // checking if the modification is accepted or not
          if ( this->getAcceptance( greenRatio ) )
          {

            //------------------------------------------------------------------
            BEGIN_DEBUG
            os << "creation and connection proposal accepted" << std::endl;
            END_DEBUG
            //------------------------------------------------------------------

            // adding the spin glass to the spin glasses Cartesian field
            this->addSpinGlassToCartesianField( successorVoxel,
                                                successorSpinGlass,
                                                spinGlassCartesianField );

            // and adding it to the isolated spin glass set
            isolatedSpinGlasses.insert( successorSpinGlass );

            // creating the new spin glass connection
            this->createSpinGlassConnection(
                                     selectedSpinGlass,
                                     selectedSpinGlassExtremityType,
                                     successorSpinGlass,
                                     successorSpinGlassExtremityType,
                                     spinGlassConnectionUniqueIdentityGenerator,
                                     spinGlassConnections );

            // updating the spin glass sets accordingly
            this->updateSpinGlassSets( selectedSpinGlass,
                                       oldConnectedSpinGlass,
                                       successorSpinGlass,
                                       isolatedSpinGlasses,
                                       oneSideAttachedSpinGlasses,
                                       fullyAttachedSpinGlasses,
                                       oneSideAttachedFarFromPialSpinGlasses );

            // updating the energies
            externalEnergy = newExternalEnergy;
            internalEnergy = newInternalEnergy;
            globalEnergy = newGlobalEnergy;

            // updating the accepted creation connection count
            ++ acceptedCreationConnectionCount;

          }
          else
          {

            //------------------------------------------------------------------
            BEGIN_DEBUG
            os << "creation and connection proposal rejected" << std::endl;
            END_DEBUG
            //------------------------------------------------------------------

            // checking that the old spin glass connection is not null and then
            // reconnecting the spin glasses through the old spin glass
            // connection
            if ( !oldSpinGlassConnection.isNull() )
            {

              oldSpinGlassConnection->connect( 
                         std::make_pair( selectedSpinGlass,
                                         selectedSpinGlassExtremityType ),
                         std::make_pair( oldConnectedSpinGlass,
                                         oldConnectedSpinGlassExtremityType ) );

              /////////////////////////---
              selectedSpinGlass->setSpinGlassConnection(
                                        selectedSpinGlassExtremityType,
                                        oldSpinGlassConnection );
              oldConnectedSpinGlass->setSpinGlassConnection(
                                        oldConnectedSpinGlassExtremityType,
                                        oldSpinGlassConnection );

              // erasing the buffer at the successor spin glass position
              buffer.erase( successorSpinGlassPosition );

              // updating the reconnection to older count
              ++ reconnectionToOlderCount;

            }
            else
            {

              // updating the nothing to do in connection count
              ++ rejectedCreationConnectionCount;

            }

          }

        }
        else if ( connectionType ==
                  gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                             InvalidConnection )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "InvalidConnection "
             << "(chosen spin glass extremity too close to the cortex)"
             << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // updating the nothing to do in spin glass connection count
          ++ nothingToDoInSpinGlassConnectionCount;

        }
        else if ( connectionType ==
                  gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                     InvalidCreationConnection )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "InvalidCreationConnection "
             << "(successor spin glass gets out of mask)" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // updating the nothing to do in spin glass connection count
          ++ nothingToDoInSpinGlassConnectionCount;

        }
        else if ( connectionType ==
                  gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                           ReconnectionToOlder )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "ReconnectionToOlder" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // reconnecting the spin glasses through the old spin glass connection
          oldSpinGlassConnection->connect( 
                         std::make_pair( selectedSpinGlass,
                                         selectedSpinGlassExtremityType ),
                         std::make_pair( oldConnectedSpinGlass,
                                         oldConnectedSpinGlassExtremityType ) );

          //////////////////////// to review ... ///////////////////////////////
          selectedSpinGlass->setSpinGlassConnection(
                                        selectedSpinGlassExtremityType,
                                        oldSpinGlassConnection );
          oldConnectedSpinGlass->setSpinGlassConnection(
                                        oldConnectedSpinGlassExtremityType,
                                        oldSpinGlassConnection );

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "spin glass id : "
             << selectedSpinGlass->getIdentity() << std::endl;
          os << "connected spin glass id : "
             << selectedSpinGlass->getConnectedSpinGlass(
                selectedSpinGlassExtremityType )->getIdentity() << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // updating the reconnection to older count
          ++ reconnectionToOlderCount;

        }
        else
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "problem with the connection type" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

        }

        // updating the proposed spin glass connection count
        ++ proposedSpinGlassConnectionCount;

      }

      //////////////////////////////////////////////////////////////////////////
      // case 'optimal motion of a spin-glass'
      //////////////////////////////////////////////////////////////////////////
      else if ( modificationProposal == 
                gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                SpinGlassOptimalMotionProposal )
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "======================= SPIN GLASS OPTIMAL MOTION =============="
           << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        oldSpinGlassExternalEnergy = 0.0f;
        newSpinGlassExternalEnergy = 0.0f;
        oldSpinGlassConnectionEnergy = 0.0f;
        newSpinGlassConnectionEnergy = 0.0f;

        // making an optimal move proposal
        alternativeSpinGlass.reset();
        bool isProposalValid = this->makeSpinGlassOptimalMotionProposal(
                                             buffer,
                                             selectedSpinGlass,
                                             alternativeSpinGlass,
                                             roiResolution,
                                             odfContinuousField,
                                             transform3dFromRoisVoxelToOdfsReal,
                                             globalMinimumOdfProbability,
                                             globalMaximumOdfProbability,
                                             computingMaskBoundingBox,
                                             computingMask,
                                             oldSpinGlassPosition,
                                             oldSpinGlassOrientation,
                                             newVoxel,
                                             newSpinGlassPosition,
                                             newSpinGlassOrientation,
                                             oldSpinGlassExternalEnergy,
                                             newSpinGlassExternalEnergy,
                                             oldSpinGlassConnectionEnergy,
                                             newSpinGlassConnectionEnergy );

        // if the optimal spin glass is not the same as the original one, then
        // we need to update the voxel
        if ( alternativeSpinGlass != selectedSpinGlass )
        {

          this->getVoxelFromPosition( alternativeSpinGlass->getPosition(),
                                      roiResolution,
                                      voxel );

        }

        // checking the validity of the spin glass optimal motion proposal
        if ( isProposalValid )
        {

          // computing the new external energy
          newExternalEnergy = externalEnergy + ( newSpinGlassExternalEnergy -
                                                 oldSpinGlassExternalEnergy );

          // computing the new internal energy
          newInternalEnergy = internalEnergy + ( newSpinGlassConnectionEnergy -
                                                 oldSpinGlassConnectionEnergy );

          // computing the new global energy
          newGlobalEnergy = _externalEnergyWeight * newExternalEnergy +
                            _internalEnergyWeight * newInternalEnergy;

          // computing the external energy variation
          deltaExternalEnergy =
            _externalEnergyWeight * ( newExternalEnergy - externalEnergy );

          // computing the internal energy variation
          deltaInternalEnergy =
            _internalEnergyWeight * ( newInternalEnergy - internalEnergy );

          // computing the global energy variation
          deltaGlobalEnergy = newGlobalEnergy - globalEnergy;

          // Green ratio computation
          greenRatio = std::exp( -( ( deltaExternalEnergy / 
                                     _externalTemperature ) +
                                    ( deltaInternalEnergy /
                                      internalTemperature ) ) );

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "newExternalEnergy = " << newExternalEnergy << std::endl;
          os << "externalEnergy = " << externalEnergy << std::endl;
          os << "newSpinGlassExternalEnergy = "
             << newSpinGlassExternalEnergy << std::endl;
          os << "oldSpinGlassExternalEnergy = "
             << oldSpinGlassExternalEnergy << std::endl;
          os << "newInternalEnergy = " << newInternalEnergy << std::endl;
          os << "internalEnergy = " << internalEnergy << std::endl;
          os << "newSpinGlassConnectionEnergy = "
             << newSpinGlassConnectionEnergy << std::endl;
          os << "oldSpinGlassConnectionEnergy = "
             << oldSpinGlassConnectionEnergy << std::endl;
          os << "globalEnergy = " << globalEnergy << std::endl;
          os << "newGlobalEnergy = " << newGlobalEnergy << std::endl;
          os << "deltaExternalEnergy = " << deltaExternalEnergy << std::endl;
          os << "deltaExternalEnergy / _externalTemperature = "
             << deltaExternalEnergy / _externalTemperature
             << std::endl;
          os << "deltaInternalEnergy = " << deltaInternalEnergy << std::endl;
          os << "deltaInternalEnergy / internalTemperature = "
             << deltaInternalEnergy / internalTemperature
             << std::endl;
          os << "greenRatio = " << greenRatio << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // checking if the modification is accepted or not
          if ( this->getAcceptance( greenRatio ) )
          {

            //------------------------------------------------------------------
            BEGIN_DEBUG
            os << "spin glass optimal motion proposal accepted" << std::endl;
            END_DEBUG
            //------------------------------------------------------------------

            // accepting the optimal move proposal
            this->acceptSpinGlassOptimalMotionProposal(
                                                      voxel,
                                                      spinGlassCartesianField,
                                                      alternativeSpinGlass,
                                                      newVoxel,
                                                      newSpinGlassPosition,
                                                      newSpinGlassOrientation );

            // updating the energies of the new configuration
            externalEnergy = newExternalEnergy;
            internalEnergy = newInternalEnergy;
            globalEnergy = newGlobalEnergy;

            // erasing the buffer at the old spin glass position
            buffer.erase( oldSpinGlassPosition );

            // updating the accepted spin glass optimal motion count
            ++ acceptedSpinGlassOptimalMotionCount;

          }
          else
          {

            //------------------------------------------------------------------
            BEGIN_DEBUG
            os << "spin glass optimal motion proposal rejected" << std::endl;
            END_DEBUG
            //------------------------------------------------------------------

            // erasing the buffer at the new spin glass position
            buffer.erase( newSpinGlassPosition );

            // updating the rejected spin glass optimal motion count
            ++ rejectedSpinGlassOptimalMotionCount;

          }

        }
        else
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "spin glass optimal motion proposal invalid" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // updating the nothing to do in spin glass optimal motion count
          ++ nothingToDoInSpinGlassOptimalMotionCount;

        }

        // updating the proposed spin glass optimal motion count
        ++ proposedSpinGlassOptimalMotionCount;

      }
      //////////////////////////////////////////////////////////////////////////
      // case 'spin-glass random motion'
      //////////////////////////////////////////////////////////////////////////
      else if ( modificationProposal == 
                gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                 SpinGlassRandomMotionProposal )
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "======================= SPIN GLASS RANDOM MOTION ==============="
           << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        oldSpinGlassExternalEnergy = 0.0f;
        newSpinGlassExternalEnergy = 0.0f;
        oldSpinGlassConnectionEnergy = 0.0f;
        newSpinGlassConnectionEnergy = 0.0f;

        bool isProposalValid = this->makeSpinGlassRandomMotionProposal(
                                             buffer, 
                                             selectedSpinGlass,
                                             roiResolution,
                                             odfContinuousField,
                                             transform3dFromRoisVoxelToOdfsReal,
                                             globalMinimumOdfProbability,
                                             globalMaximumOdfProbability,
                                             computingMaskBoundingBox,
                                             computingMask,
                                             oldSpinGlassPosition,
                                             oldSpinGlassOrientation,
                                             newVoxel,
                                             newSpinGlassPosition,
                                             newSpinGlassOrientation,
                                             oldSpinGlassExternalEnergy,
                                             newSpinGlassExternalEnergy,
                                             oldSpinGlassConnectionEnergy,
                                             newSpinGlassConnectionEnergy );

        // checking the validity of the spin glass optimal random proposal
        if ( isProposalValid )
        {

          // computing the new external energy
          newExternalEnergy = externalEnergy  + ( newSpinGlassExternalEnergy -
                                                  oldSpinGlassExternalEnergy );

          // computing the external energy variation
          deltaExternalEnergy =
            _externalEnergyWeight * ( newExternalEnergy - externalEnergy );

          // computing the new internal energy
          newInternalEnergy = internalEnergy + ( newSpinGlassConnectionEnergy -
                                                 oldSpinGlassConnectionEnergy );

          // computing the internal energy variation
          deltaInternalEnergy =
            _internalEnergyWeight * ( newInternalEnergy - internalEnergy );

         // computing the new global energy
          newGlobalEnergy = _externalEnergyWeight * newExternalEnergy +
                            _internalEnergyWeight * newInternalEnergy;

          // computing the global energy variation
          deltaGlobalEnergy = newGlobalEnergy - globalEnergy;

          // Green ratio computation
          greenRatio = std::exp( -( ( deltaExternalEnergy / 
                                     _externalTemperature ) +
                                    ( deltaInternalEnergy /
                                      internalTemperature ) ) );

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "externalEnergy = " << externalEnergy << std::endl;
          os << "oldSpinGlassExternalEnergy = "
             << oldSpinGlassExternalEnergy << std::endl;
          os << "newSpinGlassExternalEnergy = "
             << newSpinGlassExternalEnergy << std::endl;
          os << "newExternalEnergy = " << newExternalEnergy << std::endl;
          os << "deltaExternalEnergy = " << deltaExternalEnergy << std::endl;
          os << "internalEnergy = " << internalEnergy << std::endl;
          os << "oldSpinGlassConnectionEnergy = "
             << oldSpinGlassConnectionEnergy << std::endl;
          os << "newSpinGlassConnectionEnergy = "
             << newSpinGlassConnectionEnergy << std::endl;
          os << "newInternalEnergy = " << newInternalEnergy << std::endl;
          os << "deltaInternalEnergy = " << deltaInternalEnergy << std::endl;
          os << "globalEnergy = " << globalEnergy << std::endl;
          os << "newGlobalEnergy = " << newGlobalEnergy << std::endl;
          os << "deltaGlobalEnergy = " << deltaGlobalEnergy << std::endl;
          os << "greenRatio = " << greenRatio << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          if ( this->getAcceptance( greenRatio ) )
          {

            //------------------------------------------------------------------
            BEGIN_DEBUG
            os << "spin glass random motion proposal accepted" << std::endl;
            END_DEBUG
            //------------------------------------------------------------------

            // accepting the optimal move proposal
            this->acceptSpinGlassRandomMotionProposal(
                                                      voxel,
                                                      spinGlassCartesianField,
                                                      selectedSpinGlass,
                                                      newVoxel,
                                                      newSpinGlassPosition,
                                                      newSpinGlassOrientation );

            // updating the energies of the new configuration
            externalEnergy = newExternalEnergy;
            internalEnergy = newInternalEnergy;
            globalEnergy = newGlobalEnergy;

            // erasing the buffer at the old spin glass position
            buffer.erase( oldSpinGlassPosition );

            // updating the accepted spin glass random motion count
            ++ acceptedSpinGlassRandomMotionCount;

          }
          else
          {

            //------------------------------------------------------------------
            BEGIN_DEBUG
            os << "spin glass random motion proposal rejected" << std::endl;
            END_DEBUG
            //------------------------------------------------------------------

            // erasing the buffer at the new spin glass position
            buffer.erase( newSpinGlassPosition );

            // updating the rejected spin glass optimal motion count
            ++ rejectedSpinGlassRandomMotionCount;

          }

        }
        else
        {

          // updating the nothing to do in spin glass random motion count
          ++ nothingToDoInSpinGlassRandomMotionCount;

        }

        // updating the proposed spin glass random motion count
        ++ proposedSpinGlassRandomMotionCount;

      }
      //////////////////////////////////////////////////////////////////////////
      // case 'spin-glass death'
      //////////////////////////////////////////////////////////////////////////
      else if ( modificationProposal == 
                gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                        SpinGlassDeathProposal )
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "====================== SPIN GLASS DEATH ========================"
           << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        spinGlassDensity = 0.0f;
        spinGlassExternalEnergy = 0.0f;
        this->makeSpinGlassDeathProposal( buffer,
                                          selectedSpinGlass,
                                          roiResolution,
                                          odfContinuousField,
                                          transform3dFromRoisVoxelToOdfsReal,
                                          globalMinimumOdfProbability,
                                          globalMaximumOdfProbability,
                                          spinGlassDensity,
                                          spinGlassExternalEnergy );

        // storing the position of the potential dead spin glass to knwo this
        // information when erasing it from the buffer
        gkg::Vector3d< float > deadSpinGlassPosition = 
                                               selectedSpinGlass->getPosition();

        // computing the new external energy
        newExternalEnergy = externalEnergy - spinGlassExternalEnergy;

        // computing the global energy variation
        deltaExternalEnergy = newExternalEnergy - externalEnergy;

        // computing the new global energy
        newGlobalEnergy = _externalEnergyWeight * newExternalEnergy +
                          _internalEnergyWeight * internalEnergy;

        // computing the global energy variation
        deltaGlobalEnergy = newGlobalEnergy - globalEnergy;

        // computing the Green ratio
        greenRatio = std::exp( - deltaExternalEnergy / _externalTemperature ) *
                     deathScaling *
                     ( ( float )( globalSpinGlassCount - 1 ) /
                       spinGlassDensity );
                                      
        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "externalEnergy = " << externalEnergy << std::endl;
        os << "spinGlassExternalEnergy = "
           << spinGlassExternalEnergy << std::endl;
        os << "deltaExternalEnergy = " << deltaExternalEnergy << std::endl;
        os << "newExternalEnergy = " << newExternalEnergy << std::endl;
        os << "globalEnergy = " << globalEnergy << std::endl;
        os << "newGlobalEnergy = " << newGlobalEnergy << std::endl;
        os << "deltaGlobalEnergy = " << deltaGlobalEnergy << std::endl;
        os << "greenRatio = " << greenRatio << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        // if the modification is accepted...
        if ( this->getAcceptance( greenRatio ) )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "spin glass death proposal accepted" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // removing the spin glass to the spin glasses Cartesian field
          this->removeSpinGlassFromCartesianField( voxel,
                                                   selectedSpinGlass,
                                                   spinGlassCartesianField );

          // and from the isolated spin glasses set
          if ( isolatedSpinGlasses.find( selectedSpinGlass ) !=
               isolatedSpinGlasses.end() )
          {

            isolatedSpinGlasses.erase( selectedSpinGlass );

          }
          else
          {

            throw std::runtime_error(
             "Selected spin glass not found in the isolated spin glasses set" );

          }

          // updating the energies of the new configuration
          externalEnergy = newExternalEnergy;
          globalEnergy = newGlobalEnergy;

          // erasing the buffer at the new spin glass position
          buffer.erase( deadSpinGlassPosition );

          // updating the accepted spin glass optimal motion count
          ++ acceptedSpinGlassDeathCount;

        }
        else
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "spin glass death proposal rejected..." << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          // updating the rejected spin glass death count
          ++ rejectedSpinGlassDeathCount;

        }

        // updating the proposed spin glass death count
        ++ proposedSpinGlassDeathCount;

      }

      // for display
      iterations[ iteration ] = iteration;
      globalEnergies[ iteration ] = globalEnergy;
      externalEnergies[ iteration ] = externalEnergy;
      internalEnergies[ iteration ] = internalEnergy;

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "isolatedSpinGlasses count = "
         << isolatedSpinGlasses.size() << std::endl;
      os << "oneSideAttachedSpinGlasses count = "
         << oneSideAttachedSpinGlasses.size() << std::endl;
      os << "oneSideAttachedFarFromPialSpinGlasses count = "
         << oneSideAttachedFarFromPialSpinGlasses.size() << std::endl;
      os << "fullyAttachedSpinGlasses = "
         << fullyAttachedSpinGlasses.size() << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

      // updating the internal temperature
      internalTemperature = _initialInternalTemperature;/* * 
                            ( float )std::exp( -internalTemperatureSlope *
                                               ( double )iteration );*/

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }

    // display purpose
    this->saveDistribution( "GlobalEnergies.py", iterations, globalEnergies );
    this->saveDistribution( "ExternalEnergies.py",
                            iterations, externalEnergies );
    this->saveDistribution( "InternalEnergies.py",
                            iterations, internalEnergies );
    this->saveDistribution( "SpinGlassLengths.py", spinGlassLengths );
    this->saveDistribution( "SpinGlassLengthsAndDistances.py",
                            distancesToPial, spinGlassLengths );
    this->saveDistribution( "ConnectionLikelihoodsAndDistances.py",
                            distancesToPial, connectionLikelihoods );

    //--------------------------------------------------------------------------
    BEGIN_DEBUG
    os << "final global energy = " << globalEnergy << std::endl;

    // printing counters for the spin glass connection proposal
    os << "--------------------------------------------------------------------"
       << std::endl;
    os << "Spin glass connection counts :" << std::endl;
    os << "  - accepted simple connection count                 : "
       << acceptedSimpleConnectionCount << std::endl;
    os << "  - rejected simple connection count                 : "
       << rejectedSimpleConnectionCount << std::endl;
    os << "  - accepted creation connection count               : "
       << acceptedCreationConnectionCount << std::endl;
    os << "  - rejected creation connection count               : "
       << rejectedCreationConnectionCount << std::endl;
    os << "  - reconnection to older count                      : "
       << reconnectionToOlderCount << std::endl;
    os << "  - nothing to do in connection count                : "
       << nothingToDoInSpinGlassConnectionCount << std::endl;
    os << "  - proposed spin glass connection count             : "
       << proposedSpinGlassConnectionCount << std::endl;

    // printing counters for the spin glass optimal motion proposal
    os << "--------------------------------------------------------------------"
       << std::endl;
    os << "Spin glass optimal motion counts :" << std::endl;
    os << "  - accepted spin glass optimal motion count         : "
       << acceptedSpinGlassOptimalMotionCount << std::endl;
    os << "  - rejected spin glass optimal motion count         : "
       << rejectedSpinGlassOptimalMotionCount << std::endl;
    os << "  - nothing to do in spin glass optimal motion count : "
       << nothingToDoInSpinGlassOptimalMotionCount << std::endl;
    os << "  - proposed spin glass optimal motion count         : "
       << proposedSpinGlassOptimalMotionCount << std::endl;

    // printing counters for the spin glass random motion proposal
    os << "--------------------------------------------------------------------"
       << std::endl;
    os << "Spin glass random motion counts :" << std::endl;
    os << "  - accepted spin glass random motion count         : "
       << acceptedSpinGlassRandomMotionCount << std::endl;
    os << "  - rejected spin glass random motion count         : "
       << rejectedSpinGlassRandomMotionCount << std::endl;
    os << "  - nothing to do in spin glass random motion count : "
       << nothingToDoInSpinGlassRandomMotionCount << std::endl;
    os << "  - proposed spin glass random motion count         : "
       << proposedSpinGlassRandomMotionCount << std::endl;

    // printing counters for the spin glass death proposal
    os << "--------------------------------------------------------------------"
       << std::endl;
    os << "Spin glass death counts :" << std::endl;
    os << "  - accepted spin glass death count         : "
       << acceptedSpinGlassDeathCount << std::endl;
    os << "  - rejected spin glass death count         : "
       << rejectedSpinGlassDeathCount << std::endl;
    os << "  - proposed spin glass death count         : "
       << proposedSpinGlassDeathCount << std::endl;

    END_DEBUG
    //--------------------------------------------------------------------------


    if ( verbose )
    {

      // printing the final global energy and the size of each spin glasses sets
      std::cout << "-------------------------------------"
                   "-------------------------------"
                << std::endl;
      std::cout << "final global energy = " << globalEnergy << std::endl;
      std::cout << "isolated spin glasses count = "
                << isolatedSpinGlasses.size() << std::endl;
      std::cout << "one side attached spin glasses count = "
                << oneSideAttachedSpinGlasses.size() << std::endl;
      std::cout << "fully spin glasses count = "
                << fullyAttachedSpinGlasses.size() << std::endl;

      // printing counters for the spin glass connection proposal
      std::cout << "-------------------------------------"
                   "-------------------------------"
                << std::endl;
      std::cout << "Spin glass connection counts :" << std::endl;
      std::cout << "  - accepted simple connection count                 : "
                << acceptedSimpleConnectionCount << std::endl;
      std::cout << "  - rejected simple connection count                 : "
                << rejectedSimpleConnectionCount << std::endl;
      std::cout << "  - accepted creation connection count               : "
                << acceptedCreationConnectionCount << std::endl;
      std::cout << "  - rejected creation connection count               : "
                << rejectedCreationConnectionCount << std::endl;
      std::cout << "  - reconnection to older count                      : "
                << reconnectionToOlderCount << std::endl;
      std::cout << "  - nothing to do in connection count                : "
                << nothingToDoInSpinGlassConnectionCount << std::endl;
      std::cout << "  - proposed spin glass connection count             : "
                << proposedSpinGlassConnectionCount << std::endl;

      // printing counters for the spin glass optimal motion proposal
      std::cout << "-------------------------------------"
                   "-------------------------------"
                << std::endl;
      std::cout << "Spin glass optimal motion counts :" << std::endl;
      std::cout << "  - accepted spin glass optimal motion count         : "
                << acceptedSpinGlassOptimalMotionCount << std::endl;
      std::cout << "  - rejected spin glass optimal motion count         : "
                << rejectedSpinGlassOptimalMotionCount << std::endl;
      std::cout << "  - nothing to do in spin glass optimal motion count : "
                << nothingToDoInSpinGlassOptimalMotionCount << std::endl;
      std::cout << "  - proposed spin glass optimal motion count         : "
                << proposedSpinGlassOptimalMotionCount << std::endl;

      // printing counters for the spin glass random motion proposal
      std::cout << "-------------------------------------"
                   "-------------------------------"
                << std::endl;
      std::cout << "Spin glass random motion counts :" << std::endl;
      std::cout << "  - accepted spin glass random motion count          : "
                << acceptedSpinGlassRandomMotionCount << std::endl;
      std::cout << "  - rejected spin glass random motion count          : "
                << rejectedSpinGlassRandomMotionCount << std::endl;
      std::cout << "  - nothing to do in spin glass random motion count  : "
                << nothingToDoInSpinGlassRandomMotionCount << std::endl;
      std::cout << "  - proposed spin glass random motion count          : "
                << proposedSpinGlassRandomMotionCount << std::endl;

      // printing counters for the spin glass death proposal
      std::cout << "-------------------------------------"
                   "-------------------------------"
                << std::endl;
      std::cout << "Spin glass death counts :" << std::endl;
      std::cout << "  - accepted spin glass death count                  : "
                << acceptedSpinGlassDeathCount << std::endl;
      std::cout << "  - rejected spin glass death count                  : "
                << rejectedSpinGlassDeathCount << std::endl;
      std::cout << "  - proposed spin glass death count                  : "
                << proposedSpinGlassDeathCount << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // converting the spin glass connections into a bundle map
    ////////////////////////////////////////////////////////////////////////////

    this->saveSpinGlassesToBundleMap( oneSideAttachedSpinGlasses,
                                      rois.begin()->first,
                                      bundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::track( "
             "const gkg::SiteMap< L, int32_t, Compare >& rois, "
             "const gkg::SiteMap< L, int32_t, Compare >& /*subRois*/, "
             "const gkg::RCPointer< gkg::SeedingStrategy >& seedingStrategy, "
             "gkg::OdfContinuousField& odfContinuousField, "
             "const gkg::SiteMap< int32_t, int32_t, "
             "                    std::less< int32_t > >& /*mask*/, "
             "const gkg::Transform3d< float >& "
             "                   transform3dFromRoisVoxelToOdfsReal, "
             "const gkg::Transform3d< float >& "
             "                   transform3dFromRoisVoxelToMaskVoxel, "
             "const gkg::Transform3d< float >& "
             "                   transform3dFromOdfsRealToMaskVoxel, "
             "const gkg::Transform3d< float >& "
             "                   transform3dFromOdfsRealToRoisVoxel, "
             "gkg::BundleMap< L, Compare >& bundleMap, "
             "bool verbose ) const" );

}


template < class L, class Compare >
inline
std::string  gkg::GlobalTractographyAlgorithm< L, Compare >::getStaticName()
{

  try
  {

    return "global-spin-glass";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string  "
             "gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "getStaticName()" );

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// I/O of intermediate result(s)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::saveSpinGlassesToMesh(
              const std::set< gkg::RCPointer< gkg::SpinGlass > >& spinGlasses,
              const std::string& fileName,
              bool ascii,
              const std::string& format ) const
{

  try
  {

    size_t spinCount = spinGlasses.size() * 2;
    gkg::MeshMap< int32_t, float, 2U > meshMap;

    std::vector< gkg::Vector3d< float > > vertices( spinCount );
    std::vector< gkg::Polygon< 2U > > polygons( spinCount );

    uint32_t vertexCount = 0;
    std::set< gkg::RCPointer< gkg::SpinGlass > >::const_iterator
      s = spinGlasses.begin(),
      se = spinGlasses.end();
    gkg::Vector3d< float >* vertex = &vertices[ 0 ];
    gkg::Polygon< 2U >* polygon = &polygons[ 0 ];

    while ( s != se )
    {

      *vertex++ = (*s)->getExtremity( gkg::SpinGlass::Backward );
      *vertex++ = (*s)->getExtremity( gkg::SpinGlass::Forward );
      polygon->indices[ 0 ] = vertexCount++;
      polygon->indices[ 1 ] = vertexCount++;
      polygon++;
      ++ s;

    }

    meshMap.vertices.addSites( 0, vertices );
    meshMap.polygons.addPolygons( 0, polygons );

    gkg::Writer::getInstance().write( fileName, meshMap, ascii, format );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "saveSpinGlassesToMesh( "
             "const std::set< gkg::RCPointer< gkg::SpinGlass > >& "
             "spinGlasses, "
             "const std::string& fileName, "
             "bool ascii, "
             "const std::string& format ) const" );

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::saveSpinGlassesToBundleMap(
                const std::set< gkg::RCPointer< gkg::SpinGlass > >& spinGlasses,
                const L& label,
                gkg::BundleMap< L, Compare >& bundleMap ) const
{

  try
  {

    std::list< gkg::LightCurve3d< float > > fibers;
    gkg::LightCurve3d< float > fiber;
    float fiberLength = 0.0f;

    std::set< gkg::RCPointer< gkg::SpinGlass > >::const_iterator
      s = spinGlasses.begin(),
      se = spinGlasses.end();
    while ( s != se )
    {

      ( *s )->trackFromMiddle( fiber );
      fiberLength = fiber.getLength();

      // checking the fiber length before adding it to the ones to reconstruct 
      if ( ( fiberLength >= _minimumFiberLength ) &&
           ( fiberLength <= _maximumFiberLength ) )
      {

        fibers.push_back( fiber );

      }

      ++ s;

    }

    bundleMap.addCurve3ds( label, fibers );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "saveSpinGlassesToBundleMap( "
             "const std::set< gkg::RCPointer< gkg::SpinGlass > >& "
             "spinGlasses, "
             " const L& label, "
             "gkg::BundleMap< L, Compare >& bundleMap ) const" );

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// general access method(s)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template < class L, class Compare > 
inline
typename gkg::GlobalTractographyAlgorithm< L, Compare >::SpinGlassConfiguration
gkg::GlobalTractographyAlgorithm< L, Compare >::getRandomSpinGlass(
 int32_t iteration, 
 const std::set< gkg::RCPointer< gkg::SpinGlass > >& isolatedSpinGlasses,
 const std::set< gkg::RCPointer< gkg::SpinGlass > >& oneSideAttachedSpinGlasses,
 const std::set< gkg::RCPointer< gkg::SpinGlass > >& fullyAttachedSpinGlasses,
 int32_t globalSpinGlassCount,
 gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass ) const
{

  try
  {

    // allocating static containers to follow spin glass counts per type during
    // iterative process
    static std::vector< int32_t >
      iterations( _iterationCount, 0 );
    static std::vector< int32_t >
      isolatedSpinGlassCounts( _iterationCount, 0 );
    static std::vector< int32_t >
      oneSideAttachedSpinGlassCounts( _iterationCount, 0 );
    static std::vector< int32_t >
      fullyAttachedSpinGlassCounts( _iterationCount, 0 );
    static std::vector< int32_t >
      oneSideAttachedSpinGlassStabilityIndices( _iterationCount, 0 );
    static std::vector< float >
      oneSideAttachedSpinGlassStdDev( _iterationCount, 0.0f );

    // computing the size of all the spin glasses sets
    int32_t isolatedSpinGlassCount = ( int32_t )isolatedSpinGlasses.size();
    int32_t oneSideAttachedSpinGlassCount =
                                   ( int32_t )oneSideAttachedSpinGlasses.size();
    int32_t fullyAttachedSpinGlassCount =
                                     ( int32_t )fullyAttachedSpinGlasses.size();
    int32_t isolatedPlusOneSideAttachesSpinGlassCount =
          ( int32_t )( isolatedSpinGlassCount + oneSideAttachedSpinGlassCount );

    // filling the static containers
    iterations[ iteration ] = iteration;
    isolatedSpinGlassCounts[ iteration ] = isolatedSpinGlassCount;
    oneSideAttachedSpinGlassCounts[ iteration ] = oneSideAttachedSpinGlassCount;
    fullyAttachedSpinGlassCounts[ iteration ] = fullyAttachedSpinGlassCount;

    // computing the stability of each container
    bool oneSideAttachedSpinGlassStability = false;
    if ( iteration > STABILITY_LENGTH )
    {

      gkg::UpdatableMeanAndStandardDeviation
        oneSideAttachedSpinGlassMeanAndStandardDeviation;
      int32_t i = 0;
      for ( i = 0; i < STABILITY_LENGTH; i++ )
      {

        oneSideAttachedSpinGlassMeanAndStandardDeviation.add(
                           ( double )isolatedSpinGlassCounts[ iteration ] -
                           ( double )isolatedSpinGlassCounts[ iteration - i ] );

      }

      float thresholdRatio = ( float )isolatedSpinGlassCounts[ 0 ] * 
                              STABILITY_THRESHOLD_RATIO;

      oneSideAttachedSpinGlassStdDev[ iteration ] = 
                      ( float )oneSideAttachedSpinGlassMeanAndStandardDeviation.
                                                         getStandardDeviation();

      if ( oneSideAttachedSpinGlassStdDev[ iteration ] < thresholdRatio )
      {

        oneSideAttachedSpinGlassStability = true;
        oneSideAttachedSpinGlassStabilityIndices[ iteration ] = 1;

      }

    }


    if ( iteration == _iterationCountMinusOne )
    {

      this->saveDistribution( "isolatedSpinGlassCounts.py",
                              iterations,
                              isolatedSpinGlassCounts );
      this->saveDistribution( "oneSideAttachedSpinGlassCounts.py",
                              iterations,
                              oneSideAttachedSpinGlassCounts );
      this->saveDistribution( "fullyAttachedSpinGlassCounts.py",
                              iterations,
                              fullyAttachedSpinGlassCounts );
      this->saveDistribution( "oneSideAttachedSpinGlassStabilityIndices.py",
                              iterations,
                              oneSideAttachedSpinGlassStabilityIndices );
      this->saveDistribution( "oneSideAttachedSpinGlassStdDev.py",
                              iterations,
                              oneSideAttachedSpinGlassStdDev );

    }

    typename 
      gkg::GlobalTractographyAlgorithm< L, Compare >::SpinGlassConfiguration
        spinGlassConfiguration =
                       gkg::GlobalTractographyAlgorithm< L, Compare >::Isolated;
    uint32_t choice = 0;
    if ( !oneSideAttachedSpinGlassStability )
    {

      // choosing a random spin glass among the isolated and one side attached
      // available sets
      choice = _factory->getUniformRandomUInt32(
               *_randomGenerator,
               ( u_int32_t )( 2 * isolatedPlusOneSideAttachesSpinGlassCount ) );
      choice %= ( u_int32_t )isolatedPlusOneSideAttachesSpinGlassCount;

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "choice=" << choice << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------
      spinGlassConfiguration = 
                       gkg::GlobalTractographyAlgorithm< L, Compare >::Isolated;
      std::set< gkg::RCPointer< gkg::SpinGlass > >::iterator s;
      if ( choice < ( u_int32_t )isolatedSpinGlassCount )
      {

        s = isolatedSpinGlasses.begin();

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "isolated" << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

      }
      else
      {

        s = oneSideAttachedSpinGlasses.begin();
        choice -= ( u_int32_t )isolatedSpinGlassCount;

        spinGlassConfiguration = 
                gkg::GlobalTractographyAlgorithm< L, Compare >::OneSideAttached;

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "one side attached" << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

      }
      selectedSpinGlass = *std::next( s, choice );

    }
    else
    {

      // choosing a random spin glass among the three available sets
      choice = _factory->getUniformRandomUInt32(
                                    *_randomGenerator,
                                    ( u_int32_t )( 2 * globalSpinGlassCount ) );
      choice %= ( u_int32_t )globalSpinGlassCount;

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "choice=" << choice << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

      spinGlassConfiguration = 
                       gkg::GlobalTractographyAlgorithm< L, Compare >::Isolated;
      std::set< gkg::RCPointer< gkg::SpinGlass > >::iterator s;
      if ( choice < ( u_int32_t )isolatedSpinGlassCount )
      {

        s = isolatedSpinGlasses.begin();

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "isolated" << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

      }
      else if ( choice <
                ( u_int32_t )isolatedPlusOneSideAttachesSpinGlassCount )
      {

        s = oneSideAttachedSpinGlasses.begin();
        choice -= ( u_int32_t )isolatedSpinGlassCount;
        spinGlassConfiguration = 
                gkg::GlobalTractographyAlgorithm< L, Compare >::OneSideAttached;

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "one side attached" << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

      }
      else
      {


        if ( iteration < _iterationCount / 3 )
        {

          s = fullyAttachedSpinGlasses.begin();
          choice -= ( u_int32_t )isolatedPlusOneSideAttachesSpinGlassCount;
          spinGlassConfiguration = 
                  gkg::GlobalTractographyAlgorithm< L, Compare >::FullyAttached;

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "fully attached" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------


        }
        else
        {

          uint32_t spinGlassChoice = _factory->getUniformRandomUInt32(
                                                       *_randomGenerator, 10U );
          if ( spinGlassChoice <= 1 )
          {

            s = fullyAttachedSpinGlasses.begin();
            choice -= ( u_int32_t )isolatedPlusOneSideAttachesSpinGlassCount;
            spinGlassConfiguration = 
                gkg::GlobalTractographyAlgorithm< L, Compare >::FullyAttached;

            //------------------------------------------------------------------
            BEGIN_DEBUG
            os << "fully attached" << std::endl;
            END_DEBUG
            //------------------------------------------------------------------

          }
          else
          {

            s = oneSideAttachedSpinGlasses.begin();
            bool isFarFromPialSurface = false;
            bool isSpinGlassCloseToCortex = true;
            float tenTimesMinimumDistanceToMesh =
                          _anatomicalPriors->getMinimumDistanceToMesh() * 10.0f;
            float distanceToPialSurface = 0.0f;
            do
            {

              choice = _factory->getUniformRandomUInt32(
                            *_randomGenerator,
                            ( uint32_t )( oneSideAttachedSpinGlassCount - 1 ) );

              selectedSpinGlass = *std::next( s, choice );

              isSpinGlassCloseToCortex = 
                                    _anatomicalPriors->getDistanceToPialSurface(
                                               selectedSpinGlass->getPosition(),
                                               distanceToPialSurface );

              if ( !isSpinGlassCloseToCortex ||
                   ( distanceToPialSurface > tenTimesMinimumDistanceToMesh ) )
              {

                isFarFromPialSurface = true;

              }

            }
            while ( !isFarFromPialSurface );
            spinGlassConfiguration = 
                gkg::GlobalTractographyAlgorithm< L, Compare >::OneSideAttached;

            //------------------------------------------------------------------
            BEGIN_DEBUG
            os << "one side attached" << std::endl;
            END_DEBUG
            //------------------------------------------------------------------

          }


        }

      }

      selectedSpinGlass = *std::next( s, choice );

    }


    //--------------------------------------------------------------------------
    BEGIN_DEBUG
    os << "isolatedSpinGlassCount = " << isolatedSpinGlassCount << std::endl;
    os << "oneSideAttachedSpinGlassCount = " << oneSideAttachedSpinGlassCount
       << std::endl;
    os << "fullyAttachedSpinGlassCount = " << fullyAttachedSpinGlassCount
       << std::endl;
    END_DEBUG
    //--------------------------------------------------------------------------

    // sanity check
    if ( selectedSpinGlass.isNull() )
    {

      throw std::runtime_error( "Selected spin glass is null" );

    }

    return spinGlassConfiguration;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "typename gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "SpinGlassConfiguration "
             "gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "getRandomSpinGlass( "
             "int32_t iteration, "
             "const std::set< gkg::RCPointer< gkg::SpinGlass > >& "
             "isolatedSpinGlasses, "
             "const std::set< gkg::RCPointer< gkg::SpinGlass > >& "
             "oneSideAttachedSpinGlasses, "
             "const std::set< gkg::RCPointer< gkg::SpinGlass > >& "
             "fullyAttachedSpinGlasses, "
             "int32_t globalSpinGlassCount, "
             "gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass ) const" );

}


template < class L, class Compare > 
inline
void 
gkg::GlobalTractographyAlgorithm< L, Compare >::getRandomSpinGlassExtremityType(
                   gkg::SpinGlass::ExtremityType& spinGlassExtremityType ) const
{

  try
  {

    if ( _factory->getUniformRandomUInt32( *_randomGenerator, 100U ) % 2
         == 1U )
    {

      spinGlassExtremityType = gkg::SpinGlass::Backward;

    }
    else
    {

      spinGlassExtremityType = gkg::SpinGlass::Forward;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "getRandomSpinGlassExtremityType( "
             "gkg::SpinGlass::ExtremityType& spinGlassExtremityType ) const" );

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::getVoxelFromPosition(
                                   const gkg::Vector3d< float >& position,
                                   const gkg::Vector3d< double >& roiResolution,
                                   gkg::Vector3d< int32_t >& voxel ) const
{

  try
  {

    voxel.x = gkg::round( position.x / roiResolution.x );
    voxel.y = gkg::round( position.y / roiResolution.y );
    voxel.z = gkg::round( position.z / roiResolution.z );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "getVoxelFromPosition( "
             "const gkg::Vector3d< float >& position, "
             "const gkg::Vector3d< double >& roiResolution, "
             "gkg::Vector3d< int32_t >& voxel ) const" );

}


template < class L, class Compare >
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::getOdf(
            gkg::OdfContinuousField& odfContinuousField,
            const gkg::Vector3d< float >& spinGlassPosition,
            const gkg::Vector3d< double >& roiResolution,
            const gkg::Transform3d< float >& transform3dFromRoisVoxelToOdfsReal,
            gkg::RCPointer< gkg::OrientationDistributionFunction >& odf ) const

{

  try
  {

    gkg::Vector3d< float > floatVoxelRois;
    gkg::Vector3d< float > pointOdfs;

    // computing the coordinates in the floating voxel frame
    floatVoxelRois.x = spinGlassPosition.x / roiResolution.x;
    floatVoxelRois.y = spinGlassPosition.y / roiResolution.y;
    floatVoxelRois.z = spinGlassPosition.z / roiResolution.z;

    // computing the spin glass position in the ODF real frame
    transform3dFromRoisVoxelToOdfsReal.getDirect( floatVoxelRois, pointOdfs );

    // computing the spin glass external energy
    if ( odfContinuousField.isValid( pointOdfs ) )
    {

      // computing the ODF at the current point
      odf.reset( odfContinuousField.getItem( pointOdfs ) );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline"
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::getOdf( "
             "gkg::OdfContinuousField& odfContinuousField, "
             "const gkg::Vector3d< float >& spinGlassPosition, "
             "const gkg::Vector3d< double >& roiResolution, "
             "const gkg::Transform3d< float >& "
             "transform3dFromRoisVoxelToOdfsReal, "
             "gkg::RCPointer< gkg::OrientationDistributionFunction >& odf ) "
             "const" );

}


template < class L, class Compare > 
inline
void
gkg::GlobalTractographyAlgorithm< L, Compare >::
                                               getSpinGlassOrientationAndLength(
                          const gkg::Vector3d< float >& orientation,
                          const gkg::OrientationSet& /* outputOrientationSet */,
                          const gkg::OrientationDistributionFunction& odf,
                          const gkg::Vector3d< float >& spinGlassPosition,
                          gkg::Vector3d< float >& spinGlassOrientation,
                          float& spinGlassLength ) const
{

  try
  {

    // computing the curvature threshold at the new spin glass position
    float curvatureThreshold = _anatomicalPriors->getCurvatureThreshold(
                                                            spinGlassPosition );

    // computing the principal odf orientation centered on the given orientation
    // and with an aperture angle equal to the local curvature threshold
    const gkg::Vector3d< float >&
      principalOdfOrientation = odf.getPrincipalOrientation(
                                                           orientation,
                                                           curvatureThreshold );

    spinGlassLength = _spinGlassLength;

    // if there is no given pial surface, the spin glass orientation is the odf
    // principal one, centered on the orientation given and within a cone of
    // curvature threshold angle
    if ( _anatomicalPriors->hasPialSurface() )
    {

      // computing the distance and the normal direction to the pial surface
      float distanceToPialSurface = 10.0;
      gkg::Vector3d< float > normalToPialSurface;
      bool isSpinGlassCloseToCortex =
        _anatomicalPriors->getDistanceAndNormalToPialSurface(
                                                          spinGlassPosition,
                                                          distanceToPialSurface,
                                                          normalToPialSurface );

      // if the spin glass is not close enough to the cortex, the spin glass 
      // orientation is the odf principal one, centered on the orientation 
      // given and within a cone of curvature threshold angle
      if ( isSpinGlassCloseToCortex &&
           ( distanceToPialSurface <
             _anatomicalPriors->getMaximumDistanceToMesh() ) )
      {

        // tuning the spin glass length according to the pial distance
        spinGlassLength *= ( 1.0f - std::exp( -distanceToPialSurface /
                                              _spinGlassLength ) );

        // computing the weight of the sigmoid corresponding to the distance to
        // the pial surface
        float sigmoidWeight = _anatomicalPriors->getSigmoidCoefficient(
                                                        distanceToPialSurface );

        // remove the influence of normal to pial in case orientation dispersion
        // is low, meaning the direction provided by ODF is actually the one to
        // follow
        sigmoidWeight *= _anatomicalPriors->getOrientationDispersion(
                                                            spinGlassPosition );
        sigmoidWeight = std::min( sigmoidWeight, 1.0f );

        // computing the spin glass orientation with the anatomical constraint
        float sigmoidWeight1 = 1.0f - sigmoidWeight;
        spinGlassOrientation.x = principalOdfOrientation.x * sigmoidWeight1 +
                                 normalToPialSurface.x * sigmoidWeight;
        spinGlassOrientation.y = principalOdfOrientation.y * sigmoidWeight1 +
                                 normalToPialSurface.y * sigmoidWeight;
        spinGlassOrientation.z = principalOdfOrientation.z * sigmoidWeight1 +
                                 normalToPialSurface.z * sigmoidWeight;

        // normalizing the spin glass orientation
        spinGlassOrientation = spinGlassOrientation.normalize();

        // computing the forward extremity
        float halfLength = spinGlassLength / 2.0f;
        gkg::Vector3d< float > forwardExtremity(
                    spinGlassPosition.x + spinGlassOrientation.x * halfLength,
                    spinGlassPosition.y + spinGlassOrientation.y * halfLength,
                    spinGlassPosition.z + spinGlassOrientation.z * halfLength );

        // computing the backward extremity
        gkg::Vector3d< float > backwardExtremity(
                    spinGlassPosition.x - spinGlassOrientation.x * halfLength,
                    spinGlassPosition.y - spinGlassOrientation.y * halfLength,
                    spinGlassPosition.z - spinGlassOrientation.z * halfLength );

        // computing the distance to ventricles for both extremities
        float distanceToVentriclesFromForwardExtremity =
                _anatomicalPriors->getDistanceToVentricles( forwardExtremity );
        float distanceToVentriclesFromBackwardExtremity =
                _anatomicalPriors->getDistanceToVentricles( backwardExtremity );

        // need to change the sign of the principal odf orientation in order to
        // make growing fibers from the ventricles to the boundaries
        if ( distanceToVentriclesFromForwardExtremity <
             distanceToVentriclesFromBackwardExtremity )
        {

          spinGlassOrientation.x = normalToPialSurface.x * sigmoidWeight -
                                   principalOdfOrientation.x * sigmoidWeight1;
          spinGlassOrientation.y = normalToPialSurface.y * sigmoidWeight -
                                   principalOdfOrientation.y * sigmoidWeight1;
          spinGlassOrientation.z = normalToPialSurface.z * sigmoidWeight -
                                   principalOdfOrientation.z * sigmoidWeight1;

          // normalizing the spin glass orientation
          spinGlassOrientation = spinGlassOrientation.normalize();

        }

      }
      else
      {

        // if the spin glass is too far from the cortex, we only rely on the odf
        // information
        spinGlassOrientation.x = principalOdfOrientation.x;
        spinGlassOrientation.y = principalOdfOrientation.y;
        spinGlassOrientation.z = principalOdfOrientation.z;

      }

    }
    else
    {

      // if no anatomical priors are given, we only rely on the odf information
      spinGlassOrientation.x = principalOdfOrientation.x;
      spinGlassOrientation.y = principalOdfOrientation.y;
      spinGlassOrientation.z = principalOdfOrientation.z;

    }

  }
  GKG_CATCH(
            "template < class L, class Compare > "
            "inline"
            "void "
            "gkg::GlobalTractographyAlgorithm< L, Compare >::"
            "getSpinGlassOrientationAndLength( "
            "const gkg::Vector3d< float >& orientation, "
            "const gkg::OrientationSet& outputOrientationSet, "
            "const gkg::OrientationDistributionFunction& odf, "
            "const gkg::Vector3d< float >& spinGlassPosition, "
            "gkg::Vector3d< float >& spinGlassOrientation, "
            "float& spinGlassLength ) const" );

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::
                                          getNeighboringSpinGlassesWithinSphere(
    const gkg::RCPointer< gkg::SpinGlass >& spinGlass,
    const gkg::SpinGlass::ExtremityType& spinGlassExtremityType,
    const gkg::CartesianField< std::set< gkg::RCPointer< gkg::SpinGlass > > >&
                                                        spinGlassCartesianField,
    const gkg::Vector3d< int32_t >& voxel,
    std::deque< std::pair< gkg::RCPointer< gkg::SpinGlass >,
                           gkg::SpinGlass::ExtremityType > >&
                                 neighboringSpinGlassesAndExtremityTypes ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // first resetting the deque
    ////////////////////////////////////////////////////////////////////////////

    neighboringSpinGlassesAndExtremityTypes.clear();


    ////////////////////////////////////////////////////////////////////////////
    // collecting spin glass information: extremity, orientation, length
    // and retrieving the opposite spin glass extremity type to avoid loop
    ////////////////////////////////////////////////////////////////////////////

    gkg::SpinGlass::ExtremityType oppositeSpinGlassExtremityType;
    const gkg::Vector3d< float >&
      spinGlassExtremityInRoisFrame =
        spinGlass->getExtremityAndOppositeExtremityType(
                                               spinGlassExtremityType,
                                               oppositeSpinGlassExtremityType );
    float quarterOfLength = spinGlass->getLength() / 4.0;


    ////////////////////////////////////////////////////////////////////////////
    // computing the bounding box within which the sphere (centered at the
    // chosen extremity of the spin glass and with diameter equal to the spin
    // glass length) is included
    ////////////////////////////////////////////////////////////////////////////

    int32_t lowerX = voxel.x - 1;
    int32_t lowerY = voxel.y - 1;
    int32_t lowerZ = voxel.z - 1;
    int32_t upperX = voxel.x + 1;
    int32_t upperY = voxel.y + 1;
    int32_t upperZ = voxel.z + 1;

    gkg::Vector3d< float > deltaExtremities;
    float neighborQuarterOfLength = 0.0;
    float radius = 0.0;
    float squaredRadius = 0.0;
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    for ( k = lowerZ; k <= upperZ; k++ )
    {

      for ( j = lowerY; j <= upperY; j++ )
      {

        for ( i = lowerX; i <= upperX; i++ )
        {

          const std::set< gkg::RCPointer< gkg::SpinGlass > >*
            spinGlassSet = spinGlassCartesianField.getItem( i, j, k );

          // the spin glass set is not empty
          if ( spinGlassSet && !spinGlassSet->empty() )
          {

            std::set< gkg::RCPointer< gkg::SpinGlass > >::const_iterator
              s = spinGlassSet->begin(),
              se = spinGlassSet->end();
            while ( s != se )
            {

              const gkg::RCPointer< gkg::SpinGlass >& neighboringSpinGlass = *s;

              if ( ( neighboringSpinGlass != spinGlass ) &&
                   ( neighboringSpinGlass !=
                       spinGlass->getConnectedSpinGlass(
                                            oppositeSpinGlassExtremityType ) ) )
              {

                neighborQuarterOfLength = 
                                        neighboringSpinGlass->getLength() / 4.0;

                // we define the radius with the average length in order to
                // preserve the bijectivity of the connection
                radius = quarterOfLength + neighborQuarterOfLength;
                squaredRadius = radius * radius;

                // checking that the neighbor is not connected at the forward
                // extremity
                if ( !neighboringSpinGlass->isConnected(
                                                     gkg::SpinGlass::Forward ) )
                {

                  const gkg::Vector3d< float >&
                    forwardExtremity = neighboringSpinGlass->getExtremity(
                                                      gkg::SpinGlass::Forward );

                  // computing the distance between the two positions
                  deltaExtremities.x = forwardExtremity.x -
                                       spinGlassExtremityInRoisFrame.x;
                  deltaExtremities.y = forwardExtremity.y -
                                       spinGlassExtremityInRoisFrame.y;
                  deltaExtremities.z = forwardExtremity.z -
                                       spinGlassExtremityInRoisFrame.z;

                  if ( deltaExtremities.getNorm2() <= squaredRadius )
                  {

                    neighboringSpinGlassesAndExtremityTypes.push_back(
                                    std::make_pair( neighboringSpinGlass,
                                                    gkg::SpinGlass::Forward ) );

                  }

                }

                // checking that the neighbor is not connected at the backward
                // extremity
                if ( !neighboringSpinGlass->isConnected(
                                                    gkg::SpinGlass::Backward ) )
                {

                  const gkg::Vector3d< float >&
                    backwardExtremity = neighboringSpinGlass->getExtremity(
                                                     gkg::SpinGlass::Backward );

                  // computing the distance between the two positions
                  deltaExtremities.x = backwardExtremity.x -
                                       spinGlassExtremityInRoisFrame.x;
                  deltaExtremities.y = backwardExtremity.y -
                                       spinGlassExtremityInRoisFrame.y;
                  deltaExtremities.z = backwardExtremity.z -
                                       spinGlassExtremityInRoisFrame.z;

                  if ( deltaExtremities.getNorm2() <= squaredRadius )
                  {

                    neighboringSpinGlassesAndExtremityTypes.push_back(
                                   std::make_pair( neighboringSpinGlass,
                                                   gkg::SpinGlass::Backward ) );

                  }

                }

              }
              ++ s;

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline"
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "getNeighboringSpinGlassesWithinSphere( "
             "const gkg::RCPointer< gkg::SpinGlass >& spinGlass, "
             "const gkg::SpinGlass::ExtremityType& spinGlassExtremityType, "
             "const gkg::CartesianField< std::deque< "
             "gkg::RCPointer< gkg::SpinGlass > > >& "
             "spinGlassCartesianField, "
             "const gkg::Vector3d< int32_t >& voxel, "
             "std::deque< std::pair< gkg::RCPointer< gkg::SpinGlass >, "
             "gkg::SpinGlass::Extremity > >& "
             "neighboringSpinGlassesAndExtremityTypes ) const" );

}


template < class L, class Compare > 
inline
void 
gkg::GlobalTractographyAlgorithm< L, Compare >::addSpinGlassToCartesianField(
       const gkg::Vector3d< int32_t >& voxel,
       const gkg::RCPointer< gkg::SpinGlass >& spinGlass,
       gkg::CartesianField< std::set< gkg::RCPointer< gkg::SpinGlass > > >&
                                                 spinGlassCartesianField ) const
{

  try
  {

    // adding the spin glass to the spin glasses Cartesian field
    spinGlassCartesianField.getItem( voxel )->insert( spinGlass );

    // creating the spin glass creation command and sending it to the writer
    // if connected
    if ( _writer->isConnected() )
    {

      gkg::SpinGlassCreationCommand spinGlassCreationCommand( spinGlass );
      _writer->send( spinGlassCreationCommand );
      _writer->flush();

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "addSpinGlassToCartesianField( "
             "const gkg::Vector3d< int32_t >& voxel, "
             "const gkg::RCPointer< gkg::SpinGlass >& spinGlass, "
             "gkg::CartesianField< std::set< "
             "gkg::RCPointer< gkg::SpinGlass > > >& "
             "spinGlassCartesianField ) const" );

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::updateSpinGlassSets(
  const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass,
  const gkg::RCPointer< gkg::SpinGlass >& oldConnectedSpinGlass,
  const gkg::RCPointer< gkg::SpinGlass >& successorSpinGlass,
  std::set< gkg::RCPointer< gkg::SpinGlass > >& isolatedSpinGlasses,
  std::set< gkg::RCPointer< gkg::SpinGlass > >& oneSideAttachedSpinGlasses,
  std::set< gkg::RCPointer< gkg::SpinGlass > >& fullyAttachedSpinGlasses,
  std::set< gkg::RCPointer< gkg::SpinGlass > >&
                                   oneSideAttachedFarFromPialSpinGlasses ) const
{

  try
  {

    bool isSpinGlassCloseToCortex = false;
    float distanceToPialSurface = 0.0;
    bool isFarFromCortex = false;

    ////////////////////////////////////////////////////////////////////////////
    // moving the old connected spin glass to the appropriate set
    ////////////////////////////////////////////////////////////////////////////
    if ( !oldConnectedSpinGlass.isNull() )
    {

      // computing the distance to cortex only if anatomical priors are given
      isSpinGlassCloseToCortex =
        _anatomicalPriors->getDistanceToPialSurface(
                                           oldConnectedSpinGlass->getPosition(),
                                           distanceToPialSurface );

      // computing the boolean to know if the old conencted spin glass is far
      // from the cortex
      isFarFromCortex = !isSpinGlassCloseToCortex ||
                        ( distanceToPialSurface >
                          _anatomicalPriors->getMaximumDistanceToMesh() );

      // updating the sets with the old connected spin glass
      if ( oldConnectedSpinGlass->isIsolated() )
      {

        if ( oneSideAttachedSpinGlasses.find( oldConnectedSpinGlass ) !=
             oneSideAttachedSpinGlasses.end() )
        {

          oneSideAttachedSpinGlasses.erase( oldConnectedSpinGlass );
          if ( isFarFromCortex )
          {

            oneSideAttachedFarFromPialSpinGlasses.erase(
                                                        oldConnectedSpinGlass );

          }

        }
        else
        {

          // should never enter here
          throw std::runtime_error(
            "old connected spin glass not found in the one side attached set" );

        }

        isolatedSpinGlasses.insert( oldConnectedSpinGlass );

      }
      else if ( oldConnectedSpinGlass->isAnExtremity() )
      {

        if ( fullyAttachedSpinGlasses.find( oldConnectedSpinGlass ) !=
             fullyAttachedSpinGlasses.end() )
        {

          fullyAttachedSpinGlasses.erase( oldConnectedSpinGlass );

        }
        else
        {

          // should never enter here
          throw std::runtime_error(
               "old connected spin glass not found in the fully attached set" );

        }

        oneSideAttachedSpinGlasses.insert( oldConnectedSpinGlass );

        if ( isFarFromCortex )
        {

          oneSideAttachedFarFromPialSpinGlasses.insert( oldConnectedSpinGlass );

        }

      }
      else
      {

        throw std::runtime_error( "old spin glass cannot be fully attached" );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // moving the selected spin glass to the appropriate set
    ////////////////////////////////////////////////////////////////////////////

    // computing the distance to cortex only if anatomical priors are given
    isSpinGlassCloseToCortex =
        _anatomicalPriors->getDistanceToPialSurface(
                                               selectedSpinGlass->getPosition(),
                                               distanceToPialSurface );

    // computing the boolean to know if the selected spin glass is far from the
    // cortex
    isFarFromCortex = !isSpinGlassCloseToCortex ||
                      ( distanceToPialSurface >
                        _anatomicalPriors->getMaximumDistanceToMesh() );
    if ( selectedSpinGlass->isIsolated() )
    {

      // should never enter here
      throw std::runtime_error(
                  "selected spin glass cannot be isolated after a connection" );

    }
    else if ( selectedSpinGlass->isAnExtremity() &&
              oldConnectedSpinGlass.isNull() )
    {

      if ( isolatedSpinGlasses.find( selectedSpinGlass ) !=
           isolatedSpinGlasses.end() )
      {

        isolatedSpinGlasses.erase( selectedSpinGlass );

      }
      else
      {

        // should never enter here
        throw std::runtime_error(
                 "selected spin glass not found in the one side attached set" );

      }

      oneSideAttachedSpinGlasses.insert( selectedSpinGlass );
      if ( isFarFromCortex )
      {

        oneSideAttachedFarFromPialSpinGlasses.insert( selectedSpinGlass );

      }

    }
    else
    {

      if ( oldConnectedSpinGlass.isNull() )
      {

        if ( oneSideAttachedSpinGlasses.find( selectedSpinGlass ) !=
             oneSideAttachedSpinGlasses.end() )
        {

          oneSideAttachedSpinGlasses.erase( selectedSpinGlass );
          if ( isFarFromCortex )
          {

            oneSideAttachedFarFromPialSpinGlasses.erase( selectedSpinGlass );

          }

        }
        else
        {

          // should never enter here
          throw std::runtime_error(
                 "selected spin glass not found in the one side attached set" );

        }

        fullyAttachedSpinGlasses.insert( selectedSpinGlass );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // moving the successor spin glass to the appropriate set
    ////////////////////////////////////////////////////////////////////////////

    // computing the distance to cortex only if anatomical priors are given
    isSpinGlassCloseToCortex =
        _anatomicalPriors->getDistanceToPialSurface(
                                              successorSpinGlass->getPosition(),
                                              distanceToPialSurface );

    // computing the boolean to know if the successor spin glass is far from the
    // cortex
    isFarFromCortex = !isSpinGlassCloseToCortex ||
                      ( distanceToPialSurface >
                        _anatomicalPriors->getMaximumDistanceToMesh() );

    if ( successorSpinGlass->isIsolated() )
    {

      throw std::runtime_error(
                 "successor spin glass cannot be isolated after a connection" );

    }
    else if ( successorSpinGlass->isAnExtremity() )
    {

      if ( isolatedSpinGlasses.find( successorSpinGlass ) !=
           isolatedSpinGlasses.end() )
      {

        isolatedSpinGlasses.erase( successorSpinGlass );

      }
      else
      {

        // should never enter here
        throw std::runtime_error(
                         "successor spin glass not found in the isolated set" );

      }

      oneSideAttachedSpinGlasses.insert( successorSpinGlass );
      if ( isFarFromCortex )
      {

        oneSideAttachedFarFromPialSpinGlasses.insert( successorSpinGlass );

      }

    }
    else
    {

      if ( oneSideAttachedSpinGlasses.find( successorSpinGlass ) !=
           oneSideAttachedSpinGlasses.end() )
      {

        oneSideAttachedSpinGlasses.erase( successorSpinGlass );
        if ( isFarFromCortex )
        {

          oneSideAttachedFarFromPialSpinGlasses.erase( successorSpinGlass );

        }

      }
      else
      {

         // should never enter here
        throw std::runtime_error(
                "successor spin glass not found in the one side attached set" );

      }

      fullyAttachedSpinGlasses.insert( successorSpinGlass );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "updateSpinGlassSets( "
             "const gkg::RCPointer< gkg::SpinGlass >& selectedSpinGlass, "
             "const gkg::RCPointer< gkg::SpinGlass >& oldConnectedSpinGlass, "
             "const gkg::RCPointer< gkg::SpinGlass >& successorSpinGlass, "
             "std::set< gkg::RCPointer< gkg::SpinGlass > >& "
             "isolatedSpinGlasses, "
             "std::set< gkg::RCPointer< gkg::SpinGlass > >& "
             "oneSideAttachedSpinGlasses, "
             "std::set< gkg::RCPointer< gkg::SpinGlass > >& "
             "fullyAttachedSpinGlasses, "
             "std::set< gkg::RCPointer< gkg::SpinGlass > >& "
             "oneSideAttachedFarFromPialSpinGlasses ) const" );

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Metropolis-Hasting method(s)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template < class L, class Compare > 
inline
typename gkg::GlobalTractographyAlgorithm< L, Compare >::ModificationProposal
gkg::GlobalTractographyAlgorithm< L, Compare >::getModificationProposal(
typename gkg::GlobalTractographyAlgorithm< L, Compare >::SpinGlassConfiguration
                                                         spinGlassConfiguration,
int32_t iteration,
int32_t /* globalSpinGlassCount */ ) const
{

  try
  {

    // defining the scenarii
    gkg::GlobalTractographyAlgorithm< L, Compare >::ModificationProposal
      modificationProposal =
            gkg::GlobalTractographyAlgorithm< L, Compare >::NothingToDoProposal;


    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////// ISOLATED /////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    if ( spinGlassConfiguration ==
         gkg::GlobalTractographyAlgorithm< L, Compare >::Isolated )
    {

      modificationProposal =
        gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                    SpinGlassConnectionProposal;

      if ( iteration > _iterationCountOverThree )
      {

        uint32_t choice = _factory->getUniformRandomUInt32( *_randomGenerator,
                                                            100U ) % 3;

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "choice = " << choice << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        if ( choice == 1U )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "isolated -> connection" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          modificationProposal =
            gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                    SpinGlassConnectionProposal;

        }
        else if ( choice == 2U )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "isolated -> random motion" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          modificationProposal =
            gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                  SpinGlassRandomMotionProposal;

        }
        else
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "isolated -> death" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          modificationProposal =
            gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                         SpinGlassDeathProposal;

        }

      }

    }
    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////// ONE SIDE ATTACHED ////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    else if ( spinGlassConfiguration ==
              gkg::GlobalTractographyAlgorithm< L, Compare >::OneSideAttached )
    {

      modificationProposal =
        gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                    SpinGlassConnectionProposal;

      if ( iteration > _iterationCountOverThree )
      {

        uint32_t choice = _factory->getUniformRandomUInt32( *_randomGenerator,
                                                            100U ) % 10;

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "choice = " << choice << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        if ( choice == 0U )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "one side attached -> random motion" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          modificationProposal =
            gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                  SpinGlassRandomMotionProposal;

        }

      }
      

    }
    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////// FULLY ATTACHED ///////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    else
    {

      uint32_t choice = _factory->getUniformRandomUInt32( *_randomGenerator,
                                                          100U ) % 10;

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "choice = " << choice << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------
      if ( choice > 2U )
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "fully attached -> connection" << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        modificationProposal =
          gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                    SpinGlassConnectionProposal;

      }
      else if ( choice == 9 )
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "fully attached -> random motion" << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        modificationProposal = 
          gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                  SpinGlassRandomMotionProposal;

      }
      else
      {

        //----------------------------------------------------------------------
        BEGIN_DEBUG
        os << "fully attached -> optimal motion" << std::endl;
        END_DEBUG
        //----------------------------------------------------------------------

        modificationProposal = 
          gkg::GlobalTractographyAlgorithm< L, Compare >::
                                                 SpinGlassOptimalMotionProposal;

      }

    }

    return modificationProposal;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "typename gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "ModificationProposal "
             "gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "getModificationProposal( "
             "typename gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "SpinGlassConfiguration spinGlassConfiguration, "
             "int32_t iteration, "
             "int32_t globalSpinGlassCount ) const" );

}


template < class L, class Compare > 
inline
int32_t gkg::GlobalTractographyAlgorithm< L, Compare >::
                                             getRandomSelectionFromGibbsSampler(
                                      const std::vector< float >& probabilities,
                                      float gibbsTemperature ) const
{

  try
  {

    // computing probability minimum and maximum
    std::pair< float, float > minMaxProbabilities;
    gkg::MinMaxFilter< std::vector< float >, float > minMaxFilter;
    minMaxFilter.filter( probabilities, minMaxProbabilities );

    // processing Gibb's likelyhoods
    float probabilityRange = minMaxProbabilities.second -
                             minMaxProbabilities.first;
    float temperatureTimesProbabilityRange = gibbsTemperature *
                                             probabilityRange;

    // converting probabilities to length(s)
    std::list< float > likelyhoods;
    float likelyhood = 0.0;
    std::vector< float >::const_iterator
      p = probabilities.begin(),
      pe = probabilities.end();
    while ( p != pe )
    {

      if ( probabilityRange != 0.0 )
      {

        likelyhood += ( float )std::exp( ( *p - minMaxProbabilities.first ) /
		                         temperatureTimesProbabilityRange );

      }
      else
      {

        likelyhood += ( float )( *p - minMaxProbabilities.first );

      }

      likelyhoods.push_back( likelyhood );
      ++p;

    }

    // choosing randomly the orientation
    float maximumLikelyhood = likelyhoods.back();
    float randomLikelyhood = ( float )_factory->getUniformRandomNumber(
                                                  *_randomGenerator,
                                                  0.0,
                                                  ( double )maximumLikelyhood );

    int32_t probabilityCount = ( int32_t )probabilities.size();
    int32_t randomProbabilityIndex = 0;
    int32_t index = 0;

    std::list< float >::const_iterator l = likelyhoods.begin();
    while ( index < probabilityCount )
    {

      if ( *l >= randomLikelyhood )
      {

        randomProbabilityIndex = index;
        break;

      }

      ++ index;
      ++ l;

    }

    return randomProbabilityIndex;

  }
  GKG_CATCH ( "template < class L, class Compare > "
              "inline "
              "int32_t gkg::GlobalTractographyAlgorithm< L, Compare >::"
              "getRandomSelectionFromGibbsSampler( "
              "const std::vector< float >& probabilities, "
              "float gibbsTemperature ) const" );

}


template < class L, class Compare > 
inline
float
gkg::GlobalTractographyAlgorithm< L, Compare >::getSpinGlassExternalEnergy(
         std::map< gkg::Vector3d< float >,
                   std::pair< std::vector< float >, std::pair< float, float > >,
                   gkg::Vector3dCompare< float > >& buffer,
         const gkg::Vector3d< float >& spinGlassPosition,
         const gkg::Vector3d< float >& spinGlassOrientation,
         const gkg::OrientationDistributionFunction& odf,
         float globalMinimumOdfProbability,
         float globalMaximumOdfProbability ) const
{

  try
  {

    float orientationProbability = 1.0;


    std::map< gkg::Vector3d< float >,
              std::pair< std::vector< float >, std::pair< float, float > >,
              gkg::Vector3dCompare< float > >::const_iterator
      p = buffer.find( spinGlassPosition );
    if ( p != buffer.end() )
    {

      // computing the orientation probability
      orientationProbability =
        odf.getOrientationProbabilityWithSpecificNormalization(
                                                  p->second.first,
                                                  p->second.second.first,
                                                  p->second.second.second,
                                                  spinGlassOrientation,
                                                  globalMinimumOdfProbability,
                                                  globalMaximumOdfProbability );

    }
    else
    {

      // computing the orientation probability
      std::vector< float > orientationProbabilities;
      float localMinimumProbability = 0.0f;
      float localMaximumProbability = 0.0f;
      orientationProbability =
        odf.getOrientationProbabilityWithSpecificNormalization(
                                                  spinGlassOrientation,
                                                  globalMinimumOdfProbability,
                                                  globalMaximumOdfProbability,
                                                  orientationProbabilities,
                                                  localMinimumProbability,
                                                  localMaximumProbability );

      buffer[ spinGlassPosition ] = std::pair< std::vector< float >,
                                       std::pair< float, float > >(
                                         orientationProbabilities,
                                         std::pair< float, float >(
                                               localMinimumProbability,
                                               localMaximumProbability ) );

    }

    // returning the spin glass external energy (data attachment)
    float spinGlassExternalEnergy = 0.0;
    if ( orientationProbability > 0.0 )
    {

      spinGlassExternalEnergy = -std::log( orientationProbability );

    }
    else if ( orientationProbability < 0.0 )
    {

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "getSpinGlassExternalEnergy(): negative orientation probability!"
         << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

      spinGlassExternalEnergy = INFINITE_EXTERNAL_ENERGY;

    }
    else
    {

      spinGlassExternalEnergy = INFINITE_EXTERNAL_ENERGY;

    }
    return spinGlassExternalEnergy;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "float gkg::GlobalTractographyAlgorithm< L, Compare >:: "
             "getSpinGlassExternalEnergy( "
             "std::map< gkg::Vector3d< float >, "
             "std::pair< std::vector< float >, std::pair< float, float > >, "
             "gkg::Vector3dCompare< float > >& buffer, "
             "const gkg::Vector3d< float >& spinGlassPosition, "
             "const gkg::Vector3d< float >& spinGlassOrientation, "
             "const gkg::OrientationDistributionFunction& odf, "
             "float globalMinimumOdfProbability, "
             "float globalMaximumOdfProbability ) const" );

}


template < class L, class Compare > 
inline
bool gkg::GlobalTractographyAlgorithm< L, Compare >::getAcceptance(
                                                        float greenRatio ) const
{

  try
  {

    bool acceptModification = false;

    if ( greenRatio >= 1 )
    {

      acceptModification = true;

    }
    else
    {

      float randomThreshold = _factory->getUniformRandomNumber(
                                                              *_randomGenerator,
                                                              0.0, 1.0 );

      if ( greenRatio >= randomThreshold )
      {

        acceptModification = true;

      }
      else
      {

        acceptModification = false;

      }

    }

    return acceptModification;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool gkg::GlobalTractographyAlgorithm< L, Compare >:: "
             "getAcceptance( float greenRatio ) const" );

}



template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::saveDistribution(
                                            const std::string& fileName,
                                            std::vector< float >& values ) const
{

  try
  {

    if ( !values.empty() )
    {

      std::ofstream os( fileName.c_str() );

      if ( os.good() )
      {

        os << "import matplotlib.pyplot as plt" << std::endl;
        std::vector< float >::const_iterator
          v = values.begin(),
          ve = values.end();

        ve  --;

        os << "x = [ " << std::flush;

        while ( v != ve  )
        {

          os << *v << ", " << std::flush;
          ++ v;

        }

        os << *v << " ]" << std::endl;
        os << "num_bins = 200" << std::endl;
        os << "n, bins, patches = plt.hist(x, num_bins, normed=1, "
              "facecolor='green', alpha=0.5)" << std::endl;
        os << "plt.show()" << std::endl;
        os.close();

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline"
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "saveDistribution( const std::string& fileName, "
             "std::vector< float >& values ) const" )

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::saveDistribution(
                                            const std::string& fileName,
                                            std::vector< int32_t >& iterations,
                                            std::vector< float >& values ) const
{

  try
  {

    if ( !iterations.empty() && !values.empty() )
    {

      std::ofstream os( fileName.c_str() );

      if ( os.good() )
      {

        os << "import matplotlib.pyplot as plt" << std::endl;

        os << "plt.plot( [" << std::flush;

        std::vector< int32_t >::const_iterator
          i = iterations.begin(),
          ie = iterations.end();

        ie --;

        while ( i != ie  )
        {

          os << *i << ", " << std::flush;
          ++ i;

        }

        os << *i << "], [" << std::endl;


        std::vector< float >::const_iterator
          v = values.begin(),
          ve = values.end();

        ve --;

        while ( v != ve  )
        {

          os << *v << ", " << std::flush;
          ++ v;

        }

        os << *v << "] )" << std::endl;

        os << "plt.show()" << std::flush;
        os.close();

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline"
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "saveDistribution( const std::string& fileName, "
             "std::vector< int32_t >& iterations, "
             "std::vector< float >& values ) const" )

}

template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::saveDistribution(
                                          const std::string& fileName,
                                          std::vector< int32_t >& iterations,
                                          std::vector< int32_t >& values ) const
{

  try
  {

    if ( !iterations.empty() && !values.empty() )
    {

      std::ofstream os( fileName.c_str() );

      if ( os.good() )
      {

        os << "import matplotlib.pyplot as plt" << std::endl;

        os << "plt.plot( [" << std::flush;

        std::vector< int32_t >::const_iterator
          i = iterations.begin(),
          ie = iterations.end();

        ie --;

        while ( i != ie  )
        {

          os << *i << ", " << std::flush;
          ++ i;

        }

        os << *i << "], [" << std::endl;


        std::vector< int32_t >::const_iterator
          v = values.begin(),
          ve = values.end();

        ve --;

        while ( v != ve  )
        {

          os << *v << ", " << std::flush;
          ++ v;

        }

        os << *v << "] )" << std::endl;

        os << "plt.show()" << std::flush;
        os.close();

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline"
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "saveDistribution( const std::string& fileName, "
             "std::vector< int32_t >& iterations, "
             "std::vector< int32_t >& values ) const" )

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::saveDistribution(
                                            const std::string& fileName,
                                            std::vector< float >& abscisses,
                                            std::vector< float >& values ) const
{

  try
  {

    if ( !abscisses.empty() && !values.empty() )
    {

      std::ofstream os( fileName.c_str() );

      if ( os.good() )
      {

        os << "import matplotlib.pyplot as plt" << std::endl;

        os << "x = [" << std::flush;

        std::vector< float >::const_iterator
          i = abscisses.begin(),
          ie = abscisses.end();

        ie --;

        while ( i != ie  )
        {

          os << *i << ", " << std::flush;
          ++ i;

        }

        os << *i << "]" << std::endl;

        os << "y = [" << std::flush;

        std::vector< float >::const_iterator
          v = values.begin(),
          ve = values.end();

        ve --;

        while ( v != ve  )
        {

          os << *v << ", " << std::flush;
          ++ v;

        }

        os << *v << "]" << std::endl;

        os << "plt.scatter(x,y,s=100)" << std::endl;
        os << "plt.show()" << std::flush;
        os.close();

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline"
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "saveDistribution( const std::string& fileName, "
             "std::vector< float >& abscisses, "
             "std::vector< float >& values ) const" )

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::saveDistribution(
                                         const std::string& fileName,
                                         std::vector< uint32_t >& iterations,
                                         std::vector< uint32_t >& values ) const
{

  try
  {

    if ( !iterations.empty() && !values.empty() )
    {

      std::ofstream os( fileName.c_str() );

      if ( os.good() )
      {

        os << "import matplotlib.pyplot as plt" << std::endl;

        os << "plt.plot( [" << std::flush;

        std::vector< uint32_t >::const_iterator
          i = iterations.begin(),
          ie = iterations.end();

        ie --;

        while ( i != ie  )
        {

          os << *i << ", " << std::flush;
          ++ i;

        }

        os << *i << "], [" << std::endl;


        std::vector< uint32_t >::const_iterator
          v = values.begin(),
          ve = values.end();

        ve --;

        while ( v != ve  )
        {

          os << *v << ", " << std::flush;
          ++ v;

        }

        os << *v << "] )" << std::endl;

        os << "plt.show()" << std::flush;
        os.close();

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline"
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "saveDistribution( const std::string& fileName, "
             "std::vector< uint32_t >& iterations, "
             "std::vector< uint32_t >& values ) const" )

}


template < class L, class Compare > 
inline
void gkg::GlobalTractographyAlgorithm< L, Compare >::saveDistribution(
                                         const std::string& fileName,
                                         std::vector< uint32_t >& iterations,
                                         std::vector< double >& values ) const
{

  try
  {

    if ( !iterations.empty() && !values.empty() )
    {

      std::ofstream os( fileName.c_str() );

      if ( os.good() )
      {

        os << "import matplotlib.pyplot as plt" << std::endl;

        os << "plt.plot( [" << std::flush;

        std::vector< uint32_t >::const_iterator
          i = iterations.begin(),
          ie = iterations.end();

        ie --;

        while ( i != ie  )
        {

          os << *i << ", " << std::flush;
          ++ i;

        }

        os << *i << "], [" << std::endl;


        std::vector< double >::const_iterator
          v = values.begin(),
          ve = values.end();

        ve --;

        while ( v != ve  )
        {

          os << *v << ", " << std::flush;
          ++ v;

        }

        os << *v << "] )" << std::endl;

        os << "plt.show()" << std::flush;
        os.close();

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline"
             "void gkg::GlobalTractographyAlgorithm< L, Compare >::"
             "saveDistribution( const std::string& fileName, "
             "std::vector< uint32_t >& iterations, "
             "std::vector< double >& values ) const" )

}





#include <gkg-dmri-global-tractography/SpinGlassConnectionProposal_i.h>
#include <gkg-dmri-global-tractography/SpinGlassOptimalMotionProposal_i.h>
#include <gkg-dmri-global-tractography/SpinGlassRandomMotionProposal_i.h>
#include <gkg-dmri-global-tractography/SpinGlassDeathProposal_i.h>



#undef INFINITE_EXTERNAL_ENERGY 
#undef STABILITY_LENGTH 
#undef STABILITY_THRESHOLD_RATIO 


#endif
