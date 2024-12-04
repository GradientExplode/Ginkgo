#include <gkg-dmri-pdf-dti/TensorFunctorTensorTextureMap.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorTensorTextureMap::TensorFunctorTensorTextureMap()
                                   : gkg::TensorFunctor<
                                       gkg::TextureMap< gkg::Tensor >,
                                       gkg::Tensor >()
{
}


gkg::TensorFunctorTensorTextureMap::~TensorFunctorTensorTextureMap()
{
}


std::string gkg::TensorFunctorTensorTextureMap::getName() const
{

  return "tensor_texture_map";

}


void gkg::TensorFunctorTensorTextureMap::process(
               gkg::TextureMap< gkg::Tensor >& output,
               gkg::TensorCartesianField& tensorCartesianField,
               const std::vector< double >& /*scalarParameters*/,
               const std::vector< std::string >& /*stringParameters*/,
               bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    output = tensorCartesianField.getItems();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::TensorFunctorTensorTextureMap::process( "
             "gkg::TextureMap< gkg::Tensor >& output, "
             "gkg::TensorCartesianField& tensorCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// Tensor functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorTensorTextureMap __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::TextureMap<
                           gkg::Tensor >,
                           gkg::Tensor >::getInstance().registerTensorFunctor(
                           gkg::TensorFunctorTensorTextureMap::getInstance() );
