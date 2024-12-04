#include <gkg-dmri-qspace-sampling/CartesianQSpaceSampling.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


gkg::CartesianQSpaceSampling::CartesianQSpaceSampling(
                       float maximumBValue,
                       int32_t qSpacePointCount,
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                             : gkg::QSpaceSampling( transform3d,
                                                    gradientCharacteristics ),
                               _maximumBValue( maximumBValue ),
                               _qSpacePointCount( qSpacePointCount )
{

  try
  {

    if ( qSpacePointCount % 2 == 0 )
    {

      throw std::runtime_error( "q-space point count must be odd" );

    }

    int32_t qMaximum = ( _qSpacePointCount - 1 ) / 2;


    this->_orientations.reserve( _qSpacePointCount *
                                 _qSpacePointCount *
                                 _qSpacePointCount );

    int32_t qx, qy, qz;
    for ( qz = -qMaximum; qz <= qMaximum; qz++ )
    {

      for ( qy = -qMaximum; qy <= qMaximum; qy++ )
      {

        for ( qx = -qMaximum; qx <= qMaximum; qx++ )
        {

          if ( qx * qx + qy * qy + qz * qz <= qMaximum * qMaximum )
          {

            this->_orientations.push_back(
              gkg::Vector3d< float >( ( float )qx, ( float )qy, ( float )qz ) );

          }

        }

      }

    }

    this->_motionRotation3ds = std::vector< gkg::Vector3d< float > >(
                                      this->_orientations.size(),
                                      gkg::Vector3d< float >( 0.0, 0.0, 0.0 ) );
    this->_bValues = std::vector< float >( this->_orientations.size(),
                                           maximumBValue );

    this->extractDiffusionTimes();

  }
  GKG_CATCH( "gkg::CartesianQSpaceSampling::CartesianQSpaceSampling( "
             "float maximumBValue, "
             "int32_t qSpacePointCount, "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}


gkg::CartesianQSpaceSampling::~CartesianQSpaceSampling()
{
}


bool gkg::CartesianQSpaceSampling::isCartesian() const
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::CartesianQSpaceSampling::isCartesian() const" );

}


bool gkg::CartesianQSpaceSampling::isSpherical() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::CartesianQSpaceSampling::isSpherical() const" );

}


float gkg::CartesianQSpaceSampling::getMaximumBValue() const
{

  try
  {

    return _maximumBValue;

  }
  GKG_CATCH( "float gkg::CartesianQSpaceSampling::getMaximumBValue() const" );

}


int32_t gkg::CartesianQSpaceSampling::getQSpacePointCount() const
{

  try
  {

    return _qSpacePointCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::CartesianQSpaceSampling::getQSpacePointCount() const" );

}


float gkg::CartesianQSpaceSampling::getDiffusionTime() const
{

  try
  {

    return this->_diffusionTimes[ 0 ];

  }
  GKG_CATCH( "float gkg::CartesianQSpaceSampling::getDiffusionTime() const" );

}




void gkg::CartesianQSpaceSampling::addInformationToHeader(
                                     gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    // first, adding 'diffusion_gradient_orientations' and 'bvalues' for
    // backward compatibility
    this->gkg::QSpaceSampling::addInformationToHeader( headeredObject );

    // adding new diffusion sensitization header
    gkg::Dictionary diffusion_sensitization_parameters;

    diffusion_sensitization_parameters[ "maximum-b-value" ] = _maximumBValue;
    diffusion_sensitization_parameters[ "q-space-point-count" ] =
                                                           _qSpacePointCount;
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
                                  std::string( "cartesian" ) );
    headeredObject.getHeader().addAttribute(
                                  "diffusion_sensitization_parameters",
                                  diffusion_sensitization_parameters );

  }
  GKG_CATCH( "void gkg::CartesianQSpaceSampling::"
             "addInformationToHeader( "
             "gkg::HeaderedObject& headeredObject ) const" );

}

