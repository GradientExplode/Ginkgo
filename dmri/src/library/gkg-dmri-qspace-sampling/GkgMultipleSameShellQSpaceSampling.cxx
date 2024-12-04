#include <gkg-dmri-qspace-sampling/GkgMultipleSameShellQSpaceSampling.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


gkg::GkgMultipleSameShellQSpaceSampling::GkgMultipleSameShellQSpaceSampling(
                       const std::vector< float >& shellBValues,
                       int32_t orientationCount,
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                               : gkg::MultipleSameShellQSpaceSampling(
                                                       transform3d,
                                                       gradientCharacteristics )
{

  try
  {

    this->_shellBValues = shellBValues;
    this->_orientationSets = std::vector< gkg::OrientationSet >( 1 );
    this->_orientationSets[ 0 ] = gkg::ElectrostaticOrientationSet(
                                                            orientationCount );

    int32_t shellCount = ( int32_t )this->_shellBValues.size();
    this->_bValues = std::vector< float >( orientationCount * shellCount );
    this->_motionRotation3ds = std::vector< Vector3d< float > >(
                                           orientationCount * shellCount );
    this->_orientations = std::vector< gkg::Vector3d< float > >(
                                           orientationCount * shellCount );
    int32_t s = 0;
    int32_t o = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      for ( o = 0; o < orientationCount; o++ )
      {

        this->_bValues[ s* orientationCount + o ] = this->_shellBValues[ s ];
        this->_orientations[ s * orientationCount + o ] =
          this->_orientationSets[ 0 ].getOrientation( o );

      }

    }

    this->extractDiffusionTimes();

    this->_shellDiffusionTimes.resize( shellCount );
    for ( s = 0; s < shellCount; s++ )
    {

      this->_shellDiffusionTimes[ s ] =
                                  this->_diffusionTimes[ s * orientationCount ];

    }

  }
  GKG_CATCH( "gkg::GkgMultipleSameShellQSpaceSampling::"
             "GkgMultipleSameShellQSpaceSampling( "
             "const std::vector< float >& shellBValues, "
             "int32_t orientationCount, "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}


gkg::GkgMultipleSameShellQSpaceSampling::~GkgMultipleSameShellQSpaceSampling()
{
}


void gkg::GkgMultipleSameShellQSpaceSampling::addInformationToHeader(
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
    diffusion_sensitization_parameters[ "orientation-count" ] =
                                         this->_orientationSets[ 0 ].getCount();
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
          std::string( "spherical_multiple-shell_same-orientation-set_gkg" ) );
    headeredObject.getHeader().addAttribute(
          "diffusion_sensitization_parameters",
          diffusion_sensitization_parameters );

  }
  GKG_CATCH( "void gkg::GkgMultipleSameShellQSpaceSampling::"
             "addInformationToHeader( "
             "gkg::HeaderedObject& headeredObject ) const" );

}

