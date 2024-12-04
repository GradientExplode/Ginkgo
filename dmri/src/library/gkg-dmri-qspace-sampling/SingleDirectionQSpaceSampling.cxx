#include <gkg-dmri-qspace-sampling/SingleDirectionQSpaceSampling.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-core-exception/Exception.h>


#define ORIENTATION_PRECISION_IN_DREGREES 5.0f


gkg::SingleDirectionQSpaceSampling::SingleDirectionQSpaceSampling(
                       const std::string& fileNameBValues,
                       const std::string& fileNameOrientations,
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                             : gkg::QSpaceSampling( transform3d,
                                                    gradientCharacteristics )
{

  try
  {

    // computing the mean direction from the prescribed orientation filename
    gkg::OrientationSet orientationSet( fileNameOrientations );

    int32_t orientationCount = orientationSet.getCount();

    _meanOrientation = gkg::Vector3d< float >( 0.0f, 0.0f, 0.0f );
    int32_t i = 0;
    for ( i = 0; i < orientationCount; i++ )
    {

      _meanOrientation += orientationSet.getOrientation( i );

    }
    _meanOrientation /= ( float )orientationCount;
    _meanOrientation.normalize();

    // checking that all orientations are close the mean orientation
    for ( i = 0; i < orientationCount; i++ )
    {

      if ( ( ( double )gkg::getVectorAngles( orientationSet.getOrientation( i ),
                                             _meanOrientation ) *
             180.0f / M_PI ) > ORIENTATION_PRECISION_IN_DREGREES )
      {

        throw std::runtime_error(
                   "orientation discrepancy > 5 degrees wrt mean orientation" );

      }

    }

    this->_bValues = std::vector< float >( orientationCount );

    // reading b-values
    gkg::UncommentCounterInputFileStream is( fileNameBValues.c_str() );
    int32_t bValueCount = 0;
    is >> bValueCount;

    if ( bValueCount != orientationCount )
    {

      is.close();
      throw std::runtime_error(
                               "b-value and orientation counts are different" );

    }

    for ( i = 0; i < bValueCount; i++ )
    {

      is >> this->_bValues[ i ];

    }
    is.close();

    this->_motionRotation3ds = std::vector< Vector3d< float > >(
                                                             orientationCount );
    this->_orientations = std::vector< gkg::Vector3d< float > >(
                                           orientationCount, _meanOrientation );

    this->extractDiffusionTimes();

  }
  GKG_CATCH( "gkg::SingleDirectionQSpaceSampling::"
             "SingleDirectionQSpaceSampling( "
             "const std::string& fileNameBValues, "
             "const std::string& fileNameOrientations, "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}



gkg::SingleDirectionQSpaceSampling::SingleDirectionQSpaceSampling(
                      const std::vector< float >& bValues,
                      const std::vector< gkg::Vector3d< float > >& orientations,
                      const gkg::HomogeneousTransform3d< float >& transform3d,
                      const gkg::Dictionary& gradientCharacteristics )
                             : gkg::QSpaceSampling( transform3d,
                                                    gradientCharacteristics )
{

  try
  {

    if ( bValues.size() != orientations.size() )
    {

      throw std::runtime_error(
                               "b-value and orientation counts are different" );

    }

    int32_t orientationCount = ( int32_t )orientations.size();

    _meanOrientation = gkg::Vector3d< float >( 0.0f, 0.0f, 0.0f );
    int32_t i = 0;
    for ( i = 0; i < orientationCount; i++ )
    {

      _meanOrientation += orientations[ i ];

    }
    _meanOrientation /= ( float )orientationCount;
    _meanOrientation.normalize();

    // checking that all orientations are close the mean orientation
    for ( i = 0; i < orientationCount; i++ )
    {

      if ( ( ( double )gkg::getVectorAngles( orientations[ i ],
                                             _meanOrientation ) *
             180.0f / M_PI ) > ORIENTATION_PRECISION_IN_DREGREES )
      {

        throw std::runtime_error(
                   "orientation discrepancy > 5 degrees wrt mean orientation" );

      }

    }

    this->_bValues = bValues;
    this->_motionRotation3ds = std::vector< Vector3d< float > >(
                                                             orientationCount );
    this->_orientations = std::vector< gkg::Vector3d< float > >(
                                           orientationCount, _meanOrientation );

    this->extractDiffusionTimes();

  }
  GKG_CATCH( "gkg::SingleDirectionQSpaceSampling::"
             "SingleDirectionQSpaceSampling( "
             "const std::vector< float >& bValues, "
             "const std::vector< gkg::Vector3d< float > >& orientations, "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}


gkg::SingleDirectionQSpaceSampling::~SingleDirectionQSpaceSampling()
{
}


bool gkg::SingleDirectionQSpaceSampling::isCartesian() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::SingleDirectionQSpaceSampling::isCartesian() const" );

}


bool gkg::SingleDirectionQSpaceSampling::isSpherical() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::SingleDirectionQSpaceSampling::isSpherical() const" );

}


const gkg::Vector3d< float >&
gkg::SingleDirectionQSpaceSampling::getMeanOrientation() const
{

  try
  {

    return _meanOrientation;

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::SingleDirectionQSpaceSampling::getMeanOrientation() const" );


}


void gkg::SingleDirectionQSpaceSampling::addInformationToHeader(
                                     gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    // first, adding 'diffusion_gradient_orientations' and 'bvalues' for
    // backward compatibility
    this->gkg::QSpaceSampling::addInformationToHeader( headeredObject );


    int32_t qSampleCount = ( int32_t )this->_orientations.size();

    // adding new diffusion sensitization header
    gkg::Dictionary diffusion_sensitization_parameters;

    diffusion_sensitization_parameters[ "b-values" ] = this->_bValues;
    diffusion_sensitization_parameters[ "q-sample-count" ] = qSampleCount;

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

    gkg::GenericObjectList golOrientations( qSampleCount );
    gkg::Vector3d< float > orientation;
    gkg::GenericObjectList golOrientation( 3 );
    int32_t o = 0;
    for ( o = 0; o < qSampleCount; o++ )
    {
    
      orientation = this->_orientations[ o ];
      golOrientation[ 0 ] = orientation.x;
      golOrientation[ 1 ] = orientation.y;
      golOrientation[ 2 ] = orientation.z;
      golOrientations[ o ] = golOrientation;
    
    }
    diffusion_sensitization_parameters[ "orientations" ] = golOrientations;

    this->addGradientCharacteristicsToDiffusionSensitizationParameters(
                                           diffusion_sensitization_parameters );

    headeredObject.getHeader().addAttribute(
                               "diffusion_sensitization_type",
                               std::string( "single-direction" ) );
    headeredObject.getHeader().addAttribute(
                               "diffusion_sensitization_parameters",
                               diffusion_sensitization_parameters );

  }
  GKG_CATCH( "void gkg::SingleDirectionQSpaceSampling::"
             "addInformationToHeader( "
             "gkg::HeaderedObject& headeredObject ) const" );

}


#undef ORIENTATION_PRECISION_IN_DREGREES
