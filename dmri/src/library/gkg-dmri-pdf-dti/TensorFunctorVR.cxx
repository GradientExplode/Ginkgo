#include <gkg-dmri-pdf-dti/TensorFunctorVR.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorVR::TensorFunctorVR()
                     : gkg::TensorFunctor< gkg::Volume< float >, float >()
{
}


gkg::TensorFunctorVR::~TensorFunctorVR()
{
}


std::string gkg::TensorFunctorVR::getName() const
{

  return "volume_ratio";

}


void gkg::TensorFunctorVR::processItem(
                 float& item,
                 const gkg::Tensor& tensor,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    float minEigenValue = tensor.getEigenValue( gkg::Tensor::Minimum );
    float midEigenValue = tensor.getEigenValue( gkg::Tensor::Middle );
    float maxEigenValue = tensor.getEigenValue( gkg::Tensor::Maximum );
    float adc = tensor.getApparentDiffusionCoefficient();

    if ( ( minEigenValue <= 0 ) ||
         ( midEigenValue <= 0 ) ||
         ( maxEigenValue <= 0 ) )
    {

      item = 1.0f;

    }
    else
    {

      item = 1.0f - ( minEigenValue * midEigenValue * maxEigenValue ) /
                    ( adc * adc * adc );

    }

  }
  GKG_CATCH( "void gkg::TensorFunctorVR::processItem( "
             "float& item, "
             "const gkg::Tensor& tensor, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Tensor functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorVR __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                          registerTensorFunctor(
                                          gkg::TensorFunctorVR::getInstance() );
