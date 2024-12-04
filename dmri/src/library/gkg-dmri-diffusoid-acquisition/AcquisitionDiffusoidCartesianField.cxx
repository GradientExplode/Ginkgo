#include <gkg-dmri-diffusoid-acquisition/AcquisitionDiffusoidCartesianField.h>
#include <gkg-dmri-diffusoid-acquisition/AcquisitionDiffusoid.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::AcquisitionDiffusoidCartesianField::AcquisitionDiffusoidCartesianField(
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    gkg::RCPointer< gkg::Volume< int16_t > > mask,
                    const gkg::OrientationSet* outputOrientationSet,
                    const gkg::SO3Interpolator* so3Interpolator,
                    const std::vector< double >& /*specificScalarParameters*/,
                    const std::vector< std::string >& /*specificStringParameters*/,
                    bool verbose )
                                        : gkg::DiffusoidCartesianField(
                                                          t2, dw, mask,
                                                          outputOrientationSet,
                                                          so3Interpolator )
{

  try
  {

    getReconstructionMatrix( verbose );
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::AcquisitionDiffusoidCartesianField::"
             "AcquisitionDiffusoidCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator, "
             "const std::vector< double >& /*specificScalarParameters*/, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::AcquisitionDiffusoidCartesianField::AcquisitionDiffusoidCartesianField(
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    gkg::RCPointer< gkg::Volume< int16_t > > mask,
                    const gkg::BoundingBox< float >& boundingBox,
                    const gkg::Vector3d< float >& superResolution,
                    const gkg::OrientationSet* outputOrientationSet,
                    const gkg::SO3Interpolator* so3Interpolator,
                    const std::vector< double >& /*specificScalarParameters*/,
                    const std::vector< std::string >& /*specificStringParameters*/,
                    bool verbose )
                                        : gkg::DiffusoidCartesianField(
                                                  t2, dw, mask,
                                                  boundingBox, superResolution,
                                                  outputOrientationSet,
                                                  so3Interpolator )
{

  try
  {

    getReconstructionMatrix( verbose );
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::AcquisitionDiffusoidCartesianField::"
             "AcquisitionDiffusoidCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator, "
             "const std::vector< double >& /*specificScalarParameters*/, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::AcquisitionDiffusoidCartesianField::AcquisitionDiffusoidCartesianField(
                    gkg::RCPointer< gkg::Volume< float > > t2,
                    gkg::RCPointer< gkg::Volume< float > > dw,
                    gkg::RCPointer< gkg::Volume< int16_t > > mask,
                    const gkg::BoundingBox< int32_t >& boundingBox,
                    const gkg::OrientationSet* outputOrientationSet,
                    const gkg::SO3Interpolator* so3Interpolator,
                    const std::vector< double >& /*specificScalarParameters*/,
                    const std::vector< std::string >& /*specificStringParameters*/,
                    bool verbose )
                                        : gkg::DiffusoidCartesianField(
                                                     t2, dw, mask, boundingBox,
                                                     outputOrientationSet,
                                                     so3Interpolator )
{

  try
  {

    getReconstructionMatrix( verbose );
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::AcquisitionDiffusoidCartesianField::"
             "AcquisitionDiffusoidCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator, "
             "const std::vector< double >& /*specificScalarParameters*/, "
             "const std::vector< std::string >& /*specificStringParameters*/, "
             "bool verbose )" );

}


gkg::AcquisitionDiffusoidCartesianField::~AcquisitionDiffusoidCartesianField()
{
}


void 
gkg::AcquisitionDiffusoidCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( !specificScalarParameters.empty() )
    {

      throw std::runtime_error( "no specific scalar parameter" );

    }
    if ( !specificStringParameters.empty() )
    {

      throw std::runtime_error( "no specific string parameter" );

    }

  }
  GKG_CATCH( "void gkg::AcquisitionDiffusoidCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


gkg::Diffusoid
gkg::AcquisitionDiffusoidCartesianField::newItem(
                                   const gkg::Vector3d< int32_t >& originSite,
                                   const gkg::Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {

    return gkg::AcquisitionDiffusoid( &_outputOrientationSet,
                                      _so3Interpolator,
                                      *_t2, *_dw,
                                      originSite,
                                      _reconstructionMatrix );

  }
  GKG_CATCH( "gkg::Diffusoid "
             "gkg::AcquisitionDiffusoidCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::Diffusoid 
gkg::AcquisitionDiffusoidCartesianField::newItem(
                                   const gkg::Vector3d< float >& originSite,
                                   const gkg::Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {

    return gkg::AcquisitionDiffusoid( &_outputOrientationSet,
                                      _so3Interpolator,
                                      *_t2, *_dw,
                                      originSite,
                                      _reconstructionMatrix );

  }
  GKG_CATCH( "gkg::Diffusoid "
             "gkg::AcquisitionDiffusoidCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}
