#include <gkg-dmri-pdf-dti/TensorFunctorFA.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorFA::TensorFunctorFA()
                     : gkg::TensorFunctor< gkg::Volume< float >, float >()
{
}


gkg::TensorFunctorFA::~TensorFunctorFA()
{
}


std::string gkg::TensorFunctorFA::getName() const
{

  return "fa";

}


void gkg::TensorFunctorFA::processItem(
                 float& item,
                 const gkg::Tensor& tensor,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = tensor.getFractionalAnisotropy();

  }
  GKG_CATCH( "void gkg::TensorFunctorFA::processItem( "
             "float& item, "
             "const gkg::Tensor& tensor, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Tensor functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorFA __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                          registerTensorFunctor(
                                          gkg::TensorFunctorFA::getInstance() );
