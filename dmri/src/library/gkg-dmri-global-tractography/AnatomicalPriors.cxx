#include <gkg-dmri-global-tractography/AnatomicalPriors.h>
#include <gkg-dmri-global-tractography/Log.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-mesh/StaticVertexEvolutionFunction.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-algobase/MinMaxFilter_i.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <cmath>

#include <gkg-core-io/Writer_i.h>


gkg::AnatomicalPriors::AnatomicalPriors(
                            const std::vector< double >& scalarParameters,
                            const std::vector< std::string >& stringParameters,
                            const gkg::Vector3d< int32_t >& maskSize,
                            const gkg::Vector3d< double >& maskResolution )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // extracting information from scalar and string parameters
    ////////////////////////////////////////////////////////////////////////////

    _orientationDispersionScaling = ( float )scalarParameters[ 10 ];
    _spinGlassDensity = ( float )scalarParameters[ 11 ];
    _connectionLikelihood = ( float )scalarParameters[ 12 ];
    _curvatureThreshold = ( float )scalarParameters[ 13 ] * M_PI / 180.0;
    _minimumDistanceToMesh = std::min( maskResolution.x,
                                       std::min( maskResolution.y,
                                                 maskResolution.z ) ) / 10.0;
    _maximumDistanceToMesh = ( float )scalarParameters[ 14 ];
    _cacheStride = ( int32_t )( scalarParameters[ 15 ] + 0.5 );


    //--------------------------------------------------------------------------
    BEGIN_DEBUG
    os << "===================================================================="
       << "===================================================================="
       << std::endl;
    os << "Anatomical Prior(s) preparation " << std::endl;
    os << "_orientationDispersionScaling :  " 
       << _orientationDispersionScaling << std::endl;
    os << "_spinGlassDensity :  " << _spinGlassDensity << std::endl;
    os << "_connectionLikelihood :  " << _connectionLikelihood << std::endl;
    os << "_curvatureThreshold :  " << _curvatureThreshold << std::endl;
    os << "_minimumDistanceToMesh :  " << _minimumDistanceToMesh << std::endl;
    os << "_maximumDistanceToMesh :  " << _maximumDistanceToMesh << std::endl;
    os << "_cacheStride :  " << _cacheStride << std::endl;
    END_DEBUG
    //--------------------------------------------------------------------------


    ////////////////////////////////////////////////////////////////////////////
    // getting access to the float quick resampler
    ////////////////////////////////////////////////////////////////////////////

    _quickResampler =
           gkg::QuickResamplerFactory< float >::getInstance().getQuickResampler(
                                            "Partial Voluming QuickResampler" );


    ////////////////////////////////////////////////////////////////////////////
    // reading pial surface mesh map
    ////////////////////////////////////////////////////////////////////////////

    std::string fileNamePialSurfaceMeshMap = stringParameters[ 0 ];
    if ( fileNamePialSurfaceMeshMap != "none" )
    {

      _pialSurface.reset( new gkg::MeshMap< int32_t, float, 3U > );

      gkg::Reader::getInstance().read( fileNamePialSurfaceMeshMap,
                                       *_pialSurface );

      //////////////////////////////////////////////////////////////////////////
      // reading pial surface mesh map to spin glass frame 3D transform
      //////////////////////////////////////////////////////////////////////////

      this->readTransform3d( stringParameters[ 1 ],
                             _transform3dFromPialSurfaceFrameToSpinGlassFrame );


      //////////////////////////////////////////////////////////////////////////
      // building a scene modeler in order to take benefits from its octree
      //////////////////////////////////////////////////////////////////////////

      // preparing the scene modeler, evolving mesh... for the anatomically
      // constrained orientation computation
      gkg::BoundingBox< float > boundingBox( -0.5f * ( float )maskResolution.x,
                                             ( ( float )maskSize.x - 0.5f ) *
                                             ( float )maskResolution.x,
                                             -0.5f * ( float )maskResolution.y,
                                             ( ( float )maskSize.y - 0.5f ) *
                                             ( float )maskResolution.y,
                                             -0.5f * ( float )maskResolution.z,
                                             ( ( float )maskSize.z - 0.5f ) *
                                             ( float )maskResolution.z );

      _sceneModeler.reset( new gkg::SceneModeler(
                                     boundingBox,
                                     1.0,       // time step
                                     1,         // step count
                                     1,         // temporal sub-sampling count
                                     maskSize / _cacheStride,  // octree size
                                     true ) );  // verbosity

      gkg::RCPointer< gkg::VertexEvolutionFunction >
        staticVertexEvolutionFunction(
                                     new gkg::StaticVertexEvolutionFunction() );
      _evolvingPialSurface.reset( new gkg::EvolvingMesh(
                                         _sceneModeler,
                                         _pialSurface,
                                         staticVertexEvolutionFunction,
                                         _maximumDistanceToMesh,
                                         false ) ); // store vertex trajectories

      _sceneModeler->addEvolvingMesh( "PialSurface", _evolvingPialSurface );

      _enlargedPialMask.reset( new gkg::Volume< uint8_t > );
      _sceneModeler->getEvolvingMeshCache().getBinaryMask( *_enlargedPialMask );
      _pialSurfaceBoundingBox = _sceneModeler->getBoundingBox();

      gkg::Writer::getInstance().write( "enlarged_pial_mask.ima",
                                        *_enlargedPialMask );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "Evolving pial surface created" << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading orientation dispersion volume
    ////////////////////////////////////////////////////////////////////////////
    std::string fileNameOrientationDispersionVolume = stringParameters[ 2 ];
    if ( fileNameOrientationDispersionVolume != "none" )
    {

      _orientationDispersionVolume.reset( new gkg::Volume< float > );

      gkg::Reader::getInstance().read( fileNameOrientationDispersionVolume,
                                       *_orientationDispersionVolume );

      _orientationDispersionVolume->getResolution(
                                       _orientationDispersionVolumeResolution );

      // computing the global min and max of ODI
      std::pair< float, float > minMaxOfODI( 0.0f, 1.0f );
      gkg::MinMaxFilter< gkg::Volume< float >, float > minMaxFilter;
      minMaxFilter.filter( *_orientationDispersionVolume, minMaxOfODI );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "Orientation dispersion volume read" << std::endl;
      os << "Global minimum orientation dispersion index = "
         << minMaxOfODI.first << std::endl;
      os << "Global maximum orientation dispersion index = "
         << minMaxOfODI.second << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------


      //////////////////////////////////////////////////////////////////////////
      // reading orientation dispersion volume to spin glass frame 3D transform
      //////////////////////////////////////////////////////////////////////////
      this->readTransform3d(
                    stringParameters[ 3 ],
                   _transform3dFromOrientationDispersionFrameToSpinGlassFrame );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading spin glass density volume
    ////////////////////////////////////////////////////////////////////////////

    std::string fileNameSpinGlassDensityVolume = stringParameters[ 4 ];
    if ( fileNameSpinGlassDensityVolume != "none" )
    {

      _spinGlassDensityVolume.reset( new gkg::Volume< float > );

      gkg::Reader::getInstance().read( fileNameSpinGlassDensityVolume,
                                       *_spinGlassDensityVolume );
      _spinGlassDensityVolume->getResolution(
                                            _spinGlassDensityVolumeResolution );

      // computing the global min and max of spin glass density
      std::pair< float, float > minMaxOfSpinGlassDensity( 0.0f, 1.0f );
      gkg::MinMaxFilter< gkg::Volume< float >, float > minMaxFilter;
      minMaxFilter.filter( *_spinGlassDensityVolume,
                           minMaxOfSpinGlassDensity );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "Spin glass density volume read" << std::endl;
      os << "Global minimum spin glass density = "
         << minMaxOfSpinGlassDensity.first << std::endl;
      os << "Global maximum spin glass density = "
         << minMaxOfSpinGlassDensity.second << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------


      //////////////////////////////////////////////////////////////////////////
      // reading spin glass density volume to spin glass frame 3D transform
      //////////////////////////////////////////////////////////////////////////

      this->readTransform3d(
                        stringParameters[ 5 ],
                        _transform3dFromSpinGlassDensityFrameToSpinGlassFrame );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading connection likelihood volume
    ////////////////////////////////////////////////////////////////////////////
    std::string fileNameConnectionLikelihoodVolume = stringParameters[ 6 ];
    if ( fileNameConnectionLikelihoodVolume != "none" )
    {

      _connectionLikelihoodVolume.reset( new gkg::Volume< float > );

      gkg::Reader::getInstance().read( fileNameConnectionLikelihoodVolume,
                                       *_connectionLikelihoodVolume );
      _connectionLikelihoodVolume->getResolution(
                                        _connectionLikelihoodVolumeResolution );

      //////////////////////////////////////////////////////////////////////////
      // reading connection likelihood volume to spin glass frame 3D transform
      //////////////////////////////////////////////////////////////////////////
      this->readTransform3d(
                    stringParameters[ 7 ],
                    _transform3dFromConnectionLikelihoodFrameToSpinGlassFrame );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "Connection likelihood volume read" << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading curvature threshold volume
    ////////////////////////////////////////////////////////////////////////////
    std::string fileNameCurvatureThresholdVolume = stringParameters[ 8 ];
    if ( fileNameCurvatureThresholdVolume != "none" )
    {

      _curvatureThresholdVolume.reset( new gkg::Volume< float > );

      gkg::Reader::getInstance().read( fileNameCurvatureThresholdVolume,
                                       *_curvatureThresholdVolume );
      _curvatureThresholdVolume->getResolution(
                                          _curvatureThresholdVolumeResolution );

      //////////////////////////////////////////////////////////////////////////
      // reading curvature threshold volume to spin glass frame 3D transform
      //////////////////////////////////////////////////////////////////////////
      this->readTransform3d(
                      stringParameters[ 9 ],
                      _transform3dFromCurvatureThresholdFrameToSpinGlassFrame );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "Curvature threshold volume read" << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading distance to ventricles volume
    ////////////////////////////////////////////////////////////////////////////
    std::string fileNameDistanceToVentriclesVolume = stringParameters[ 10 ];
    if ( fileNameDistanceToVentriclesVolume != "none" )
    {

      _distanceToVentriclesVolume.reset( new gkg::Volume< float > );

      gkg::Reader::getInstance().read( fileNameDistanceToVentriclesVolume,
                                       *_distanceToVentriclesVolume );

      _distanceToVentriclesVolume->getResolution(
                                        _distanceToVentriclesVolumeResolution );

      //////////////////////////////////////////////////////////////////////////
      // reading distance to ventricles volume to spin glass frame 3D transform
      //////////////////////////////////////////////////////////////////////////
      this->readTransform3d(
                    stringParameters[ 11 ],
                    _transform3dFromDistanceToVentriclesFrameToSpinGlassFrame );

      //------------------------------------------------------------------------
      BEGIN_DEBUG
      os << "Distance to ventricles volume read" << std::endl;
      END_DEBUG
      //------------------------------------------------------------------------

    }

  }
  GKG_CATCH( "gkg::AnatomicalPriors::AnatomicalPriors( "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters )" );

}


gkg::AnatomicalPriors::AnatomicalPriors( const gkg::AnatomicalPriors& other )
                      : _orientationDispersionScaling(
                                          other._orientationDispersionScaling ),
                        _spinGlassDensity( other._spinGlassDensity ),
                        _connectionLikelihood( other._connectionLikelihood ),
                        _curvatureThreshold( other._curvatureThreshold ),
                        _minimumDistanceToMesh( other._minimumDistanceToMesh ),
                        _maximumDistanceToMesh( other._maximumDistanceToMesh ),
                        _cacheStride( other._cacheStride ),
                        _quickResampler( other._quickResampler )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // pial surface
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._pialSurface.isNull() )
    {

      _pialSurface.reset( new gkg::MeshMap< int32_t, float, 3U >(
                                                        *other._pialSurface ) );

    }

    if ( !other._transform3dFromPialSurfaceFrameToSpinGlassFrame.isNull() )
    {

      _transform3dFromPialSurfaceFrameToSpinGlassFrame =
                other._transform3dFromPialSurfaceFrameToSpinGlassFrame->clone();

    }

    ////////////////////////////////////////////////////////////////////////////
    // orientation dispersion volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._orientationDispersionVolume.isNull() )
    {

      _orientationDispersionVolume.reset( new gkg::Volume< float >(
                                        *other._orientationDispersionVolume ) );

    }

    if ( !other._transform3dFromOrientationDispersionFrameToSpinGlassFrame.
                                                                      isNull() )
    {

      _transform3dFromOrientationDispersionFrameToSpinGlassFrame =
      other._transform3dFromOrientationDispersionFrameToSpinGlassFrame->clone();

    }

    _orientationDispersionVolumeResolution =
                                   other._orientationDispersionVolumeResolution;

    ////////////////////////////////////////////////////////////////////////////
    // spin glass density volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._spinGlassDensityVolume.isNull() )
    {

      _spinGlassDensityVolume.reset( new gkg::Volume< float >(
                                             *other._spinGlassDensityVolume ) );

    }

    if ( !other._transform3dFromSpinGlassDensityFrameToSpinGlassFrame.isNull() )
    {

      _transform3dFromSpinGlassDensityFrameToSpinGlassFrame =
           other._transform3dFromSpinGlassDensityFrameToSpinGlassFrame->clone();

    }

    _spinGlassDensityVolumeResolution = other._spinGlassDensityVolumeResolution;

    ////////////////////////////////////////////////////////////////////////////
    // connection likelihood volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._connectionLikelihoodVolume.isNull() )
    {

      _connectionLikelihoodVolume.reset(
               new gkg::Volume< float >( *other._connectionLikelihoodVolume ) );

    }

    if ( !other._transform3dFromConnectionLikelihoodFrameToSpinGlassFrame.
                                                                      isNull() )
    {

      _transform3dFromConnectionLikelihoodFrameToSpinGlassFrame =
       other._transform3dFromConnectionLikelihoodFrameToSpinGlassFrame->clone();

    }

    _connectionLikelihoodVolumeResolution =
                                    other._connectionLikelihoodVolumeResolution;

    ////////////////////////////////////////////////////////////////////////////
    // curvature threshold volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._curvatureThresholdVolume.isNull() )
    {

      _curvatureThresholdVolume.reset( new gkg::Volume< float >(
                                           *other._curvatureThresholdVolume ) );

    }

    if ( !other._transform3dFromCurvatureThresholdFrameToSpinGlassFrame.
                                                                      isNull() )
    {

      _transform3dFromCurvatureThresholdFrameToSpinGlassFrame =
         other._transform3dFromCurvatureThresholdFrameToSpinGlassFrame->clone();

    }

    _curvatureThresholdVolumeResolution =
                                      other._curvatureThresholdVolumeResolution;

    ////////////////////////////////////////////////////////////////////////////
    // distance to ventricles volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._distanceToVentriclesVolume.isNull() )
    {

      _distanceToVentriclesVolume.reset( new gkg::Volume< float >(
                                         *other._distanceToVentriclesVolume ) );

    }

    if ( !other._transform3dFromDistanceToVentriclesFrameToSpinGlassFrame.
                                                                      isNull() )
    {

      _transform3dFromDistanceToVentriclesFrameToSpinGlassFrame =
       other._transform3dFromDistanceToVentriclesFrameToSpinGlassFrame->clone();

    }

    _distanceToVentriclesVolumeResolution =
                                    other._distanceToVentriclesVolumeResolution;



  }
  GKG_CATCH( "gkg::AnatomicalPriors::AnatomicalPriors( "
             "const gkg::AnatomicalPriors& other )" );

}


gkg::AnatomicalPriors::~AnatomicalPriors()
{
}


gkg::AnatomicalPriors& 
gkg::AnatomicalPriors::operator=( const AnatomicalPriors& other )
{

  try
  {

    _orientationDispersionScaling = other._orientationDispersionScaling;
    _spinGlassDensity = other._spinGlassDensity;
    _connectionLikelihood = other._connectionLikelihood;
    _curvatureThreshold = other._curvatureThreshold;
    _maximumDistanceToMesh = other._maximumDistanceToMesh;
    _minimumDistanceToMesh = other._minimumDistanceToMesh;
    _cacheStride = other._cacheStride;
    _quickResampler = other._quickResampler;

    ////////////////////////////////////////////////////////////////////////////
    // pial surface
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._pialSurface.isNull() )
    {

      _pialSurface.reset( new gkg::MeshMap< int32_t, float, 3U >(
                                                        *other._pialSurface ) );

    }

    if ( !other._transform3dFromPialSurfaceFrameToSpinGlassFrame.isNull() )
    {

      _transform3dFromPialSurfaceFrameToSpinGlassFrame =
                other._transform3dFromPialSurfaceFrameToSpinGlassFrame->clone();

    }

    ////////////////////////////////////////////////////////////////////////////
    // orientation dispersion volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._orientationDispersionVolume.isNull() )
    {

      _orientationDispersionVolume.reset( new gkg::Volume< float >(
                                        *other._orientationDispersionVolume ) );

    }

    if ( !other._transform3dFromOrientationDispersionFrameToSpinGlassFrame.
                                                                      isNull() )
    {

      _transform3dFromOrientationDispersionFrameToSpinGlassFrame =
      other._transform3dFromOrientationDispersionFrameToSpinGlassFrame->clone();

    }

    _orientationDispersionVolumeResolution =
                                   other._orientationDispersionVolumeResolution;

    ////////////////////////////////////////////////////////////////////////////
    // spin glass density volume
    ////////////////////////////////////////////////////////////////////////////


    if ( !other._spinGlassDensityVolume.isNull() )
    {

      _spinGlassDensityVolume.reset( new gkg::Volume< float >(
                                             *other._spinGlassDensityVolume ) );

    }

    if ( !other._transform3dFromSpinGlassDensityFrameToSpinGlassFrame.isNull() )
    {

      _transform3dFromSpinGlassDensityFrameToSpinGlassFrame =
           other._transform3dFromSpinGlassDensityFrameToSpinGlassFrame->clone();

    }

    _spinGlassDensityVolumeResolution = other._spinGlassDensityVolumeResolution;

    ////////////////////////////////////////////////////////////////////////////
    // connection likelihood volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._connectionLikelihoodVolume.isNull() )
    {

      _connectionLikelihoodVolume.reset(
               new gkg::Volume< float >( *other._connectionLikelihoodVolume ) );

    }

    if ( !other._transform3dFromConnectionLikelihoodFrameToSpinGlassFrame.
                                                                      isNull() )
    {

      _transform3dFromConnectionLikelihoodFrameToSpinGlassFrame =
       other._transform3dFromConnectionLikelihoodFrameToSpinGlassFrame->clone();

    }

    _connectionLikelihoodVolumeResolution =
                                    other._connectionLikelihoodVolumeResolution;

    ////////////////////////////////////////////////////////////////////////////
    // curvature threshold volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._curvatureThresholdVolume.isNull() )
    {

      _curvatureThresholdVolume.reset( new gkg::Volume< float >(
                                           *other._curvatureThresholdVolume ) );

    }

    if ( !other._transform3dFromCurvatureThresholdFrameToSpinGlassFrame.
                                                                      isNull() )
    {

      _transform3dFromCurvatureThresholdFrameToSpinGlassFrame =
         other._transform3dFromCurvatureThresholdFrameToSpinGlassFrame->clone();

    }

    _curvatureThresholdVolumeResolution =
                                      other._curvatureThresholdVolumeResolution;

    ////////////////////////////////////////////////////////////////////////////
    // distance to ventricles volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !other._distanceToVentriclesVolume.isNull() )
    {

      _distanceToVentriclesVolume.reset( new gkg::Volume< float >(
                                         *other._distanceToVentriclesVolume ) );

    }

    if ( !other._transform3dFromDistanceToVentriclesFrameToSpinGlassFrame.
                                                                      isNull() )
    {

      _transform3dFromDistanceToVentriclesFrameToSpinGlassFrame =
       other._transform3dFromDistanceToVentriclesFrameToSpinGlassFrame->clone();

    }

    _distanceToVentriclesVolumeResolution =
                                    other._distanceToVentriclesVolumeResolution;

    return *this;

  }
  GKG_CATCH( "gkg::AnatomicalPriors& "
             "gkg::AnatomicalPriors::operator=( "
             "const AnatomicalPriors& other )" );

}


float gkg::AnatomicalPriors::getSigmoidCoefficient( float value ) const
{

  try
  {

    if ( value >= _maximumDistanceToMesh )
    {

      return 0.0f;

    }
    else if ( value <= 0 )
    {

      return 1.0f;

    }

    double tmp = std::cos( M_PI * value / _maximumDistanceToMesh );

    return ( float )( 0.5 * ( tmp * tmp * tmp + 1 ) );

  }
  GKG_CATCH( "float gkg::AnatomicalPriors::getSigmoidCoefficient( "
             "float value ) const" );

}


bool gkg::AnatomicalPriors::isFarFromPial(
                             const gkg::Vector3d< float >& position ) const
{

  try
  {

    // processing the voxel coordinates in the ROIs2 voxel frame
    gkg::Vector3d< int32_t > voxelInPialSurfaceVoxelFrame;
    _sceneModeler->getCacheVoxel( position, voxelInPialSurfaceVoxelFrame );

    // checking if the position is valid
    if ( ( *_enlargedPialMask )( voxelInPialSurfaceVoxelFrame ) )
    {

      return false;

    }

    std::cout << "position="
              << position
              << std::endl;
    std::cout << "voxelInPialSurfaceVoxelFrame="
              << voxelInPialSurfaceVoxelFrame
              << std::endl;

    return true;

  }
  GKG_CATCH( "bool gkg::AnatomicalPriors::isFarFromPial( "
             "const gkg::Vector3d< float >& position ) const" );

}


bool gkg::AnatomicalPriors::getDistanceAndNormalToPialSurface(
                             const gkg::Vector3d< float >& position,
                             float& distanceToPialSurface,
                             gkg::Vector3d< float >& normalToPialSurface ) const
{

  try
  {

    if ( this->hasPialSurface() )
    {

      // processing the voxel coordinates in the ROIs2 voxel frame
      gkg::Vector3d< int32_t > voxelInPialSurfaceVoxelFrame;
      _sceneModeler->getCacheVoxel( position, voxelInPialSurfaceVoxelFrame );

      // checking if the position is valid
      if ( _pialSurfaceBoundingBox.contains( position ) &&
           ( *_enlargedPialMask )( voxelInPialSurfaceVoxelFrame ) )
      {

        _evolvingPialSurface->getFastDistanceAndNormal(
                                                   position,
                                                   voxelInPialSurfaceVoxelFrame,
                                                   distanceToPialSurface,
                                                   normalToPialSurface );

        // temporary hack for the null distance to pial: setting it to the lower
        // distance treshold linked to the roi resolution
        if ( distanceToPialSurface == 0.0f )
        {

          distanceToPialSurface = _minimumDistanceToMesh;

        }

        return true;

      }

    }
    else
    {

      distanceToPialSurface = 1e38;

    }

    return false;

  }
  GKG_CATCH( "bool gkg::AnatomicalPriors::getDistanceAndNormalToPialSurface( "
             "const gkg::Vector3d< float >& position, "
             "float& distanceToPialSurface, "
             "gkg::Vector3d< float >& normalToPialSurface ) const" );

}


bool gkg::AnatomicalPriors::getDistanceToPialSurface(
                                         const gkg::Vector3d< float >& position,
                                         float& distanceToPialSurface ) const
{

  try
  {

    bool isOk = false;

    if ( this->hasPialSurface() )
    {

      // processing the voxel coordinates in the ROIs2 voxel frame
      gkg::Vector3d< int32_t > voxelInPialSurfaceVoxelFrame;
      _sceneModeler->getCacheVoxel( position, voxelInPialSurfaceVoxelFrame );

      // checking if the position is valid
      if ( _pialSurfaceBoundingBox.contains( position ) &&
           ( *_enlargedPialMask )( voxelInPialSurfaceVoxelFrame ) )
      {

        isOk = _evolvingPialSurface->getDistance( position,
                                                  voxelInPialSurfaceVoxelFrame,
                                                  distanceToPialSurface );

        // temporary hack for the null distance to pial: setting it to the lower
        // distance treshold linked to the roi resolution
        if ( distanceToPialSurface == 0.0f )
        {

          //--------------------------------------------------------------------
          BEGIN_DEBUG
          os << "distance to pial surface null!" << std::endl;
          END_DEBUG
          //--------------------------------------------------------------------

          distanceToPialSurface = _minimumDistanceToMesh;

        }

      }

    }
    else
    {

      distanceToPialSurface = 1e38;

    }

    return isOk;

  }
  GKG_CATCH( "bool gkg::AnatomicalPriors::getDistanceToPialSurface( "
             "const gkg::Vector3d< float >& position, "
             "float& distanceToPialSurface ) const" );

}


float gkg::AnatomicalPriors::getOrientationDispersion(
                                  const gkg::Vector3d< float >& position ) const
{

  try
  {

    // if not dispersion volume is available, then using the value stored in
    // _orientationDispersionScaling
    if ( _orientationDispersionVolume.isNull() )
    {

      return _orientationDispersionScaling;

    }

    gkg::Vector3d< float > positionInOrientationDispersionFrame;
    _transform3dFromOrientationDispersionFrameToSpinGlassFrame->getInverse(
                                         position,
                                         positionInOrientationDispersionFrame );

    float value = 0.0f;

    // in case we get out of the orientation dispersion volume, then return
    // 1.0, eg fully dispersed orientation
    _quickResampler->resample( *_orientationDispersionVolume,
                               1.0f,
                               positionInOrientationDispersionFrame,
                               value,
                               &_orientationDispersionVolumeResolution,
                               0 );

    // the further scaling allows to manage the situation where the ODI is very
    // low in the cortical ribbon
    return value * _orientationDispersionScaling;

  }
  GKG_CATCH( "float gkg::AnatomicalPriors::getOrientationDispersion( "
             "const Vector3d< float >& position ) const" );

}


float gkg::AnatomicalPriors::getSpinGlassDensity(
                                  const gkg::Vector3d< float >& position ) const
{

  try
  {

    if ( _spinGlassDensityVolume.isNull() )
    {

      return _spinGlassDensity;

    }

    gkg::Vector3d< float > positionInSpinGlassDensityFrame;
    _transform3dFromSpinGlassDensityFrameToSpinGlassFrame->getInverse(
                                              position,
                                              positionInSpinGlassDensityFrame );

    float value = 0.0f;
    _quickResampler->resample( *_spinGlassDensityVolume,
                               0.0f,
                               positionInSpinGlassDensityFrame,
                               value,
                               &_spinGlassDensityVolumeResolution,
                               0 );

    return value;

  }
  GKG_CATCH( "float gkg::AnatomicalPriors::getSpinGlassDensity( "
             "const Vector3d< float >& position ) const" );

}


float gkg::AnatomicalPriors::getConnectionLikelihood(
                                  const gkg::Vector3d< float >& position ) const
{

  try
  {

    if ( _connectionLikelihoodVolume.isNull() )
    {

      return _connectionLikelihood;

    }

    gkg::Vector3d< float > positionInConnectionLikelihoodFrame;
    _transform3dFromConnectionLikelihoodFrameToSpinGlassFrame->getInverse(
                                          position,
                                          positionInConnectionLikelihoodFrame );
    
    float value = 0.0f;
    _quickResampler->resample( *_connectionLikelihoodVolume,
                               0.0f,
                               positionInConnectionLikelihoodFrame,
                               value,
                               &_connectionLikelihoodVolumeResolution,
                               0 );

    return value;

  }
  GKG_CATCH( "float gkg::AnatomicalPriors::getConnectionLikelihood( "
             "const Vector3d< float >& position ) const" );

}


float gkg::AnatomicalPriors::getCurvatureThreshold(
                                  const gkg::Vector3d< float >& position ) const
{

  try
  {

    if ( _curvatureThresholdVolume.isNull() )
    {

      return _curvatureThreshold;

    }

    gkg::Vector3d< float > positionInCurvatureThresholdFrame;
    _transform3dFromCurvatureThresholdFrameToSpinGlassFrame->getInverse(
                                            position,
                                            positionInCurvatureThresholdFrame );
    
    float value = 0.0f;
    _quickResampler->resample( *_curvatureThresholdVolume,
                               0.0f,
                               positionInCurvatureThresholdFrame,
                               value,
                               &_curvatureThresholdVolumeResolution,
                               0 );

    return value;

  }
  GKG_CATCH( "float gkg::AnatomicalPriors::getCurvatureThreshold( "
             "const Vector3d< float >& position ) const" );

}


float gkg::AnatomicalPriors::getMinimumDistanceToMesh() const
{

  try
  {

      return _minimumDistanceToMesh;

  }
  GKG_CATCH( "float gkg::AnatomicalPriors::getMinimumDistanceToMesh( "
             "const Vector3d< float >& /*position*/ ) const" );

}


float gkg::AnatomicalPriors::getMaximumDistanceToMesh() const
{

  try
  {

      return _maximumDistanceToMesh;

  }
  GKG_CATCH( "float gkg::AnatomicalPriors::getMaximumDistanceToMesh( "
             "const Vector3d< float >& /*position*/ ) const" );

}


float gkg::AnatomicalPriors::getDistanceToVentricles(
                                  const gkg::Vector3d< float >& position ) const
{

  try
  {

    // sanity check
    if ( _distanceToVentriclesVolume.isNull() )
    {

      throw std::runtime_error( "No distance to ventricles volume was given" );

    }

    // computing the distance to the ventricles according the position
    gkg::Vector3d< float > positionInDistanceToVentriclesFrame;
    _transform3dFromDistanceToVentriclesFrameToSpinGlassFrame->getInverse(
                                          position,
                                          positionInDistanceToVentriclesFrame );

    float value = 0.0f;
    _quickResampler->resample( *_distanceToVentriclesVolume,
                               0.0f,
                               positionInDistanceToVentriclesFrame,
                               value,
                               &_distanceToVentriclesVolumeResolution,
                               0 );

    return value;

  }
  GKG_CATCH( "float gkg::AnatomicalPriors::getDistanceToVentricles( "
             "const Vector3d< float >& position ) const" );

}


bool gkg::AnatomicalPriors::hasPialSurface() const
{

  try
  {

    return !_pialSurface.isNull();


  }
  GKG_CATCH( "bool gkg::AnatomicalPriors::hasPialSurface() const" );

}


void gkg::AnatomicalPriors::readTransform3d(
                const std::string& fileNameTransform3d,
                gkg::RCPointer< gkg::Transform3d< float > >& transform3d ) const
{

  try
  {

    if ( fileNameTransform3d != "id" )
    {

      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        std::ifstream is( fileNameTransform3d.c_str() );
        if ( is.good() )
        {

          rigidTransform3d->readTrm( is );
          is.close();
          transform3d.reset( rigidTransform3d );

        }
        else
        {

          throw std::runtime_error( std::string( "unable to open '" ) +
                                                  fileNameTransform3d + "'" );

        }

      }
      catch ( std::exception& )
      {

        gkg::AffineTransform3d< float >*
          affineTransform3d =
            new gkg::AffineTransform3d< float >;
        std::ifstream is( fileNameTransform3d.c_str() );
        if ( is.good() )
        {

          affineTransform3d->readTrm( is );
          is.close();
          transform3d.reset( affineTransform3d );

        }
        else
        {

          throw std::runtime_error( std::string( "unable to open '" ) +
                                                  fileNameTransform3d + "'" );

        }

      }

    }
    else
    {

      transform3d.reset( new gkg::IdentityTransform3d< float > );

    }

  }
  GKG_CATCH( "void gkg::AnatomicalPriors::readTransform3d( "
             "const std::string& fileName, "
             "gkg::RCPointer< gkg::Transform3d< float > >& transform3d ) "
             "const" );

}



