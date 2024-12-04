#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorDiffusoidTextureMap.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor_i.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::DiffusoidFunctorDiffusoidTextureMap::DiffusoidFunctorDiffusoidTextureMap()
                                         : gkg::DiffusoidFunctor<
                                             gkg::TextureMap< gkg::Diffusoid >,
                                             gkg::Diffusoid >()
{
}


gkg::DiffusoidFunctorDiffusoidTextureMap::~DiffusoidFunctorDiffusoidTextureMap()
{
}


std::string gkg::DiffusoidFunctorDiffusoidTextureMap::getName() const
{

  return "diffusoid_texture_map";

}


void gkg::DiffusoidFunctorDiffusoidTextureMap::process(
               gkg::TextureMap< gkg::Diffusoid >& output,
               gkg::DiffusoidCartesianField& diffusoidCartesianField,
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

    output = diffusoidCartesianField.getItems();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DiffusoidFunctorDiffusoidTextureMap::process( "
             "gkg::TextureMap< gkg::Diffusoid >& output, "
             "gkg::DiffusoidCartesianField& diffusoidCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// Diffusoid functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_DiffusoidFunctorDiffusoidTextureMap 
                                                       __attribute__((unused)) =
gkg::DiffusoidFunctorFactory< gkg::TextureMap<
                     gkg::Diffusoid >,
                     gkg::Diffusoid >::getInstance().registerDiffusoidFunctor(
                     gkg::DiffusoidFunctorDiffusoidTextureMap::getInstance() );
