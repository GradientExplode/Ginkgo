#include <gkg-dmri-pdf-dti/TensorFunctorLambdaParallel.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorLambdaParallel::TensorFunctorLambdaParallel()
                     : gkg::TensorFunctor< gkg::Volume< float >, float >()
{
}


gkg::TensorFunctorLambdaParallel::~TensorFunctorLambdaParallel()
{
}


std::string gkg::TensorFunctorLambdaParallel::getName() const
{

  return "lambda_parallel";

}


void gkg::TensorFunctorLambdaParallel::processItem(
                 float& item,
                 const gkg::Tensor& tensor,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = tensor.getLambdaParallel();

  }
  GKG_CATCH( "void gkg::TensorFunctorLambdaParallel::processItem( "
             "float& item, "
             "const gkg::Tensor& tensor, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Tensor functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorLambdaParallel __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                        registerTensorFunctor(
                             gkg::TensorFunctorLambdaParallel::getInstance() );
