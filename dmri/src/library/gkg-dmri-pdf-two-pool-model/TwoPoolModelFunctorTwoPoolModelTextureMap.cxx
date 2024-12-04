#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorTwoPoolModelTextureMap.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TwoPoolModelFunctorTwoPoolModelTextureMap::
                                     TwoPoolModelFunctorTwoPoolModelTextureMap()
               : gkg::TwoPoolModelFunctor< gkg::TextureMap< gkg::TwoPoolModel >,
                 gkg::TwoPoolModel >()
{
}


gkg::TwoPoolModelFunctorTwoPoolModelTextureMap::
                                    ~TwoPoolModelFunctorTwoPoolModelTextureMap()
{
}


std::string gkg::TwoPoolModelFunctorTwoPoolModelTextureMap::getName() const
{

  return "two_pool_model_texture_map";

}


void gkg::TwoPoolModelFunctorTwoPoolModelTextureMap::process(
               gkg::TextureMap< gkg::TwoPoolModel >& output,
               gkg::TwoPoolModelCartesianField& twoPoolModelCartesianField,
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

    output = twoPoolModelCartesianField.getItems();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::TwoPoolModelFunctorTwoPoolModelTextureMap::process( "
             "gkg::TextureMap< gkg::TwoPoolModel >& output, "
             "gkg::TwoPoolModelCartesianField& twoPoolModelCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// TwoPoolModel functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TwoPoolModelFunctorTwoPoolModelTextureMap 
                                                       __attribute__((unused)) =
gkg::TwoPoolModelFunctorFactory< gkg::TextureMap<
                gkg::TwoPoolModel >,
                gkg::TwoPoolModel >::getInstance().registerTwoPoolModelFunctor(
                gkg::TwoPoolModelFunctorTwoPoolModelTextureMap::getInstance() );
