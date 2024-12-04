#include <gkg-dmri-pdf-dti/TensorFunctorLambdaTransverse.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorLambdaTransverse::TensorFunctorLambdaTransverse()
                     : gkg::TensorFunctor< gkg::Volume< float >, float >()
{
}


gkg::TensorFunctorLambdaTransverse::~TensorFunctorLambdaTransverse()
{
}


std::string gkg::TensorFunctorLambdaTransverse::getName() const
{

  return "lambda_transverse";

}


void gkg::TensorFunctorLambdaTransverse::processItem(
                 float& item,
                 const gkg::Tensor& tensor,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = tensor.getLambdaTransverse();

  }
  GKG_CATCH( "void gkg::TensorFunctorLambdaTransverse::processItem( "
             "float& item, "
             "const gkg::Tensor& tensor, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Tensor functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorLambdaTransverse __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                        registerTensorFunctor(
                            gkg::TensorFunctorLambdaTransverse::getInstance() );
