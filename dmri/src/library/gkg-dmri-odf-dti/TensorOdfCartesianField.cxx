#include <gkg-dmri-odf-dti/TensorOdfCartesianField.h>
#include <gkg-dmri-odf-dti/TensorOdf.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::TensorOdfCartesianField::TensorOdfCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::OrientationSet* outputOrientationSet,
                               const std::vector< double >&
                                                  specificScalarParameters,
                               const std::vector< std::string >&
                                                  specificStringParameters,
                               bool verbose )
                             : gkg::OdfCartesianField( t2, dw, mask,
                                                       outputOrientationSet ),
                               _tensorCartesianField(
                                  t2, dw, mask,
                                  specificScalarParameters,
                                  specificStringParameters,
                                  verbose )
{

  try
  {

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::TensorOdf >::getName() );

    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::TensorOdfCartesianField::TensorOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& /*specificScalarParameters*/, "
             "const std::vector< std::string >& specificStringParameters,"
             "bool verbose )" );

}


gkg::TensorOdfCartesianField::TensorOdfCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::BoundingBox< float >& boundingBox,
                               const gkg::Vector3d< float >& superResolution,
                               const gkg::OrientationSet* outputOrientationSet,
                               const std::vector< double >&
                                                  specificScalarParameters,
                               const std::vector< std::string >&
                                                  specificStringParameters,
                               bool verbose )
                             : gkg::OdfCartesianField( t2, dw, mask,
                                                       boundingBox,
                                                       superResolution,
                                                       outputOrientationSet ),
                                _tensorCartesianField(
                                  t2, dw, mask,
                                  boundingBox,
                                  superResolution,
                                  specificScalarParameters,
                                  specificStringParameters,
                                  verbose )
{

  try
  {

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::TensorOdf >::getName() );

    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::TensorOdfCartesianField::TensorOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >&  /*specificScalarParameters*/, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::TensorOdfCartesianField::TensorOdfCartesianField(
                               gkg::RCPointer< gkg::Volume< float > > t2,
                               gkg::RCPointer< gkg::Volume< float > > dw,
                               gkg::RCPointer< gkg::Volume< int16_t > > mask,
                               const gkg::BoundingBox< int32_t >& boundingBox,
                               const gkg::OrientationSet* outputOrientationSet,
                               const std::vector< double >&
                                                  specificScalarParameters,
                               const std::vector< std::string >&
                                                  specificStringParameters,
                               bool verbose )
                             : gkg::OdfCartesianField( t2, dw, mask,
                                                       boundingBox,
                                                       outputOrientationSet ),
                               _tensorCartesianField(
                                  t2, dw, mask,
                                  boundingBox,
                                  specificScalarParameters,
                                  specificStringParameters,
                                  verbose )
{

  try
  {

    _items.getHeader().addAttribute( "odf_type",
                                     gkg::TypeOf< gkg::TensorOdf >::getName() );

    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::TensorOdfCartesianField::TensorOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& /*specificScalarParameters*/, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::TensorOdfCartesianField::~TensorOdfCartesianField()
{
}


gkg::TensorCartesianField& 
gkg::TensorOdfCartesianField::getTensorCartesianField()
{

  return _tensorCartesianField;

}


void gkg::TensorOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( !specificScalarParameters.empty() )
    {

      throw std::runtime_error( "no specific scalar parameter" );

    }
    if ( specificStringParameters.empty() )
    {

      specificStringParameters.resize( 1 );
      specificStringParameters[ 0 ] = "linear_square";

    }
    else
    {

      if ( specificStringParameters.size() != 1U )
      {

        throw std::runtime_error( "invalid string parameter count" );

      }

      if ( ( specificStringParameters[ 0 ] != "linear_square" ) && 
           ( specificStringParameters[ 0 ] != "robust_positive_definite" ) )
      {

        throw std::runtime_error( "invalid string parameter" );

      }

    }

  }
  GKG_CATCH( "void gkg::TensorOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


gkg::Tensor::FitMethod 
gkg::TensorOdfCartesianField::getFitMethod(
                                    const std::string& specificStringParameter )
{

  try
  {

    return gkg::TensorCartesianField::getFitMethod( specificStringParameter );

  }
  GKG_CATCH( "static gkg::Tensor::FitMethod "
             "gkg::TensorOdfCartesianField::getFitMethod( "
             "const std::string& specificStringParameter )" );

}


gkg::OrientationDistributionFunction 
gkg::TensorOdfCartesianField::newItem( const Vector3d< int32_t >& originSite,
                                       const Vector3d< int32_t >& /*lutSite*/ )
{

  try
  {

    return gkg::TensorOdf( &_outputOrientationSet,
                           *_tensorCartesianField.getItem( originSite ) );

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::TensorOdfCartesianField::newItem( "
             "const Vector3d< int32_t >& originSite,"
             "const Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::TensorOdfCartesianField::newItem( const Vector3d< float >& /*originSite*/,
                                       const Vector3d< int32_t >& lutSite )
{

  try
  {

    return gkg::TensorOdf( &_outputOrientationSet,
                           *_tensorCartesianField.getItem( lutSite ) );

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::TensorOdfCartesianField::newItem( "
             "const Vector3d< float >& originSite,"
             "const Vector3d< int32_t >& lutSite )" );

}


