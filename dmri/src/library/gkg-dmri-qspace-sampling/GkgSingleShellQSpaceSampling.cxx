#include <gkg-dmri-qspace-sampling/GkgSingleShellQSpaceSampling.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


gkg::GkgSingleShellQSpaceSampling::GkgSingleShellQSpaceSampling(
                       float bValue,
                       int32_t orientationCount,
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                                  : gkg::SingleShellQSpaceSampling(
                                                       transform3d,
                                                       gradientCharacteristics )
{

  try
  {

    this->_orientationSet = gkg::ElectrostaticOrientationSet(
                                                             orientationCount );

    this->_bValues = std::vector< float >( orientationCount, bValue );
    this->_motionRotation3ds = std::vector< Vector3d< float > >(
                                                             orientationCount );
    this->_orientations = this->_orientationSet.getOrientations();

    this->extractDiffusionTimes();


  }
  GKG_CATCH( "gkg::GkgSingleShellQSpaceSampling::GkgSingleShellQSpaceSampling( "
             "float bValue, "
             "int32_t orientationCount, "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}


gkg::GkgSingleShellQSpaceSampling::~GkgSingleShellQSpaceSampling()
{
}


void gkg::GkgSingleShellQSpaceSampling::addInformationToHeader(
                                     gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    // first, adding 'diffusion_gradient_orientations' and 'bvalues' for
    // backward compatibility
    this->gkg::QSpaceSampling::addInformationToHeader( headeredObject );

    // adding new diffusion sensitization header
    gkg::Dictionary diffusion_sensitization_parameters;

    diffusion_sensitization_parameters[ "b-value" ] = this->getBValue();
    diffusion_sensitization_parameters[ "orientation-count" ] =
                                               this->_orientationSet.getCount();
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
                                  std::string( "spherical_single-shell_gkg" ) );
    headeredObject.getHeader().addAttribute(
                                  "diffusion_sensitization_parameters",
                                  diffusion_sensitization_parameters );

  }
  GKG_CATCH( "void gkg::GkgSingleShellQSpaceSampling::addInformationToHeader( "
             "gkg::HeaderedObject& headeredObject ) const" );

}
