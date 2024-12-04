#include <gkg-dmri-qspace-sampling/GkgMultipleDifferentShellQSpaceSampling.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


gkg::GkgMultipleDifferentShellQSpaceSampling::
                                      GkgMultipleDifferentShellQSpaceSampling(
                       const std::vector< float >& shellBValues,
                       const std::vector< int32_t >& orientationCounts,
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                               : gkg::MultipleDifferentShellQSpaceSampling(
                                                       transform3d,
                                                       gradientCharacteristics )
{

  try
  {

    this->_shellBValues = shellBValues;

    int32_t shellCount = ( int32_t )this->_shellBValues.size();

    if ( ( int32_t )orientationCounts.size() != shellCount )
    {

      throw std::runtime_error(
                             "incompatible shell and orientation set counts" );

    }

    this->_orientationSets = std::vector< gkg::OrientationSet >( shellCount );

    int32_t s = 0;
    int32_t globalOrientationCount = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      this->_orientationSets[ s ] = gkg::ElectrostaticOrientationSet(
                                                      orientationCounts[ s ] );
      globalOrientationCount += orientationCounts[ s ];

    }

    this->_bValues = std::vector< float >( globalOrientationCount );
    this->_motionRotation3ds = std::vector< Vector3d< float > >(
                                                      globalOrientationCount );
    this->_orientations = std::vector< gkg::Vector3d< float > >(
                                                      globalOrientationCount );
    int32_t o = 0;
    int32_t index = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      for ( o = 0; o < orientationCounts[ s ]; o++ )
      {

        this->_bValues[ index ] = this->_shellBValues[ s ];
        this->_orientations[ index ] =
                               this->_orientationSets[ s ].getOrientation( o );
        ++ index;

      }

    }

    this->extractDiffusionTimes();

    this->_shellDiffusionTimes.resize( shellCount );
    int32_t orientationIndex = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      this->_shellDiffusionTimes[ s ] =
                                      this->_diffusionTimes[ orientationIndex ];
      orientationIndex += orientationCounts[ s ];

    }

  }
  GKG_CATCH( "gkg::GkgMultipleDifferentShellQSpaceSampling::"
             "GkgMultipleDifferentShellQSpaceSampling( "
             "const std::vector< float >& shellBValues, "
             "const std::vector< int32_t >& orientationCounts, "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}


gkg::GkgMultipleDifferentShellQSpaceSampling::~GkgMultipleDifferentShellQSpaceSampling()
{
}


void gkg::GkgMultipleDifferentShellQSpaceSampling::addInformationToHeader(
                                     gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    // first, adding 'diffusion_gradient_orientations' and 'bvalues' for
    // backward compatibility
    this->gkg::QSpaceSampling::addInformationToHeader( headeredObject );

    // adding new diffusion sensitization header
    gkg::Dictionary diffusion_sensitization_parameters;

    diffusion_sensitization_parameters[ "b-values" ] = this->_shellBValues;

    int32_t shellCount = ( int32_t )this->_shellBValues.size();
    int32_t s = 0;
    std::vector< int32_t > orientationCounts( shellCount );
    for ( s = 0; s < shellCount; s++ )
    {

      orientationCounts[ s ] = this->_orientationSets[ s ].getCount();

    }

    diffusion_sensitization_parameters[ "orientation-counts" ] =
                                                             orientationCounts;
    diffusion_sensitization_parameters[ "homogeneous-transform3d" ] =
                                    this->_transform3d->getDirectCoefficients();

    int32_t motionRotation3dCount = ( int32_t )this->_motionRotation3ds.size();
    gkg::GenericObjectList golMotionRotation3ds( motionRotation3dCount );
    gkg::Vector3d< float > motionRotation3d;
    gkg::GenericObjectList golMotionRotation3d( 3 );
    int32_t r = 0;
    for ( r = 0; r < motionRotation3dCount; r++ )
    {
    
      motionRotation3d = this->_motionRotation3ds[ r ];
      golMotionRotation3d[ 0 ] = motionRotation3d.x;
      golMotionRotation3d[ 1 ] = motionRotation3d.y;
      golMotionRotation3d[ 2 ] = motionRotation3d.z;
      golMotionRotation3ds[ r ] = golMotionRotation3d;
    
    }
    diffusion_sensitization_parameters[ "motion-rotation3ds" ] =
                                                      golMotionRotation3ds;

    this->addGradientCharacteristicsToDiffusionSensitizationParameters(
                                           diffusion_sensitization_parameters );

    headeredObject.getHeader().addAttribute(
          "diffusion_sensitization_type",
          std::string(
           "spherical_multiple-shell_different-orientation-sets_gkg" ) );
    headeredObject.getHeader().addAttribute(
          "diffusion_sensitization_parameters",
          diffusion_sensitization_parameters );

  }
  GKG_CATCH( "void gkg::GkgMultipleDifferentShellQSpaceSampling::"
             "addInformationToHeader( "
             "gkg::HeaderedObject& headeredObject ) const" );

}


