#include <gkg-dmri-pdf-dti/TensorFunctorRGB.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorRGB::TensorFunctorRGB()
                      : gkg::TensorFunctor< gkg::Volume< gkg::RGBComponent >,
                                            gkg::RGBComponent >()
{
}


gkg::TensorFunctorRGB::~TensorFunctorRGB()
{
}


std::string gkg::TensorFunctorRGB::getName() const
{

  return "rgb";

}


void gkg::TensorFunctorRGB::processItem(
                 RGBComponent& item,
                 const gkg::Tensor& tensor,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    float scaling = tensor.getFractionalAnisotropy() *
                    scalarParameters[ 0 ];

    gkg::Vector3d< float >
      principalOrientation = tensor.getEigenVector( gkg::Tensor::Maximum );
    float r = std::abs( principalOrientation.x ) * 255.0 * scaling;
    float g = std::abs( principalOrientation.y ) * 255.0 * scaling;
    float b = std::abs( principalOrientation.z ) * 255.0 * scaling;
    r = ( r > 255.0 ? 255.0 : r );
    g = ( g > 255.0 ? 255.0 : g );
    b = ( b > 255.0 ? 255.0 : b );

    item = gkg::RGBComponent( ( uint8_t )r, ( uint8_t )g, ( uint8_t )b );

  }
  GKG_CATCH( "void gkg::TensorFunctorRGB::processItem( "
             "float& item, "
             "const gkg::Tensor& tensor, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Tensor functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorRGB __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::Volume< gkg::RGBComponent >,
                           gkg::RGBComponent >::getInstance().
                                                        registerTensorFunctor(
                                         gkg::TensorFunctorRGB::getInstance() );
