#include <gkg-dmri-qspace-sampling/CustomSingleShellQSpaceSampling.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-core-io/TextFileLineCount.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-exception/Exception.h>


gkg::CustomSingleShellQSpaceSampling::CustomSingleShellQSpaceSampling(
                       const std::string& fileNameBValues,
                       const std::string& fileNameOrientations,
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                                     : gkg::SingleShellQSpaceSampling(
                                                       transform3d,
                                                       gradientCharacteristics )
{

  try
  {

    // reading orientations
    this->_orientationSet = gkg::OrientationSet( fileNameOrientations );
    int32_t orientationCount = this->_orientationSet.getCount();

    // allocating b-value(s)
    this->_bValues = std::vector< float >( orientationCount );

    // reading b-values
    int32_t lineCount = gkg::getTextFileLineCount( fileNameBValues );


    ////////////////////////////////////////////////////////////////////////////
    // in case we provide a *.bval file
    ////////////////////////////////////////////////////////////////////////////

    if ( lineCount == 1 )
    {

      int32_t bValueCount = 
                         gkg::getScalarItemCountPerLine( fileNameBValues )[ 0 ];
      if ( bValueCount != orientationCount )
      {

        throw std::runtime_error(
                               "b-value and orientation counts are different" );

      }

      gkg::UncommentCounterInputFileStream is( fileNameBValues.c_str() );
      int32_t i = 0;
      for ( i = 0; i < bValueCount; i++ )
      {

        is >> this->_bValues[ i ];

      }
      is.close();

    }
    ////////////////////////////////////////////////////////////////////////////
    // in case we provide a GKG b-value file
    ////////////////////////////////////////////////////////////////////////////
    else
    {

      gkg::UncommentCounterInputFileStream is( fileNameBValues.c_str() );
      int32_t bValueCount = 0;
      is >> bValueCount;

      if ( bValueCount != orientationCount )
      {

        is.close();
        throw std::runtime_error(
                               "b-value and orientation counts are different" );

      }

      int32_t i = 0;
      for ( i = 0; i < bValueCount; i++ )
      {

        is >> this->_bValues[ i ];

      }

    }

    this->_motionRotation3ds = std::vector< Vector3d< float > >(
                                                             orientationCount );
    this->_orientations = this->_orientationSet.getOrientations();

    this->extractDiffusionTimes();

  }
  GKG_CATCH( "gkg::CustomSingleShellQSpaceSampling::"
             "CustomSingleShellQSpaceSampling( "
             "const std::string& fileNameBValues, "
             "const std::string& fileNameOrientations, "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}


gkg::CustomSingleShellQSpaceSampling::CustomSingleShellQSpaceSampling(
                      const std::vector< float >& bValues,
                      const std::vector< gkg::Vector3d< float > >& orientations,
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                                     : gkg::SingleShellQSpaceSampling(
                                                       transform3d,
                                                       gradientCharacteristics )
{

  try
  {

    if ( bValues.size() != orientations.size() )
    {

      throw std::runtime_error(
                               "b-value and orientation counts are different" );

    }

    this->_orientationSet = gkg::OrientationSet( orientations );

    int32_t orientationCount = this->_orientationSet.getCount();
    this->_bValues = bValues;
    this->_motionRotation3ds = std::vector< Vector3d< float > >(
                                                             orientationCount );
    this->_orientations = this->_orientationSet.getOrientations();

    this->extractDiffusionTimes();

  }
  GKG_CATCH( "gkg::CustomSingleShellQSpaceSampling::"
             "CustomSingleShellQSpaceSampling( "
             "const std::vector< float >& bValues, "
             "const std::vector< gkg::Vector3d< float > >& orientations, "
             "const gkg::HomogeneousTransform3d< float >& transform3d, "
             "const gkg::Dictionary& gradientCharacteristics )" );

}


gkg::CustomSingleShellQSpaceSampling::~CustomSingleShellQSpaceSampling()
{
}


void gkg::CustomSingleShellQSpaceSampling::addInformationToHeader(
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

    int32_t orientationCount = this->_orientationSet.getCount();
    gkg::GenericObjectList golOrientations( orientationCount );
    gkg::Vector3d< float > orientation;
    gkg::GenericObjectList golOrientation( 3 );
    int32_t o = 0;
    for ( o = 0; o < orientationCount; o++ )
    {
    
      orientation = this->_orientationSet.getOrientation( o );
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
                               std::string( "spherical_single-shell_custom" ) );
    headeredObject.getHeader().addAttribute(
                               "diffusion_sensitization_parameters",
                               diffusion_sensitization_parameters );

  }
  GKG_CATCH( "void gkg::CustomSingleShellQSpaceSampling::"
             "addInformationToHeader( "
             "gkg::HeaderedObject& headeredObject ) const" );

}
