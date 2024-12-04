#include <gkg-dmri-pdf-functor/EapFunctorEapTextureMap.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorEapTextureMap::EapFunctorEapTextureMap()
                                 : gkg::EapFunctor< gkg::TextureMap<
                                       gkg::OrientationDistributionFunction >,
                                       gkg::OrientationDistributionFunction >()
{
}


gkg::EapFunctorEapTextureMap::~EapFunctorEapTextureMap()
{
}


std::string gkg::EapFunctorEapTextureMap::getName() const
{

  return "eap_texture_map";

}


bool gkg::EapFunctorEapTextureMap::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorEapTextureMap::process(
               gkg::TextureMap< gkg::OrientationDistributionFunction >& output,
               gkg::EapCartesianField& eapCartesianField,
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

    output = eapCartesianField.getItems();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::EapFunctorEapTextureMap::process( "
             "gkg::TextureMap< gkg::OrientationDistributionFunction >& output, "
             "gkg::EapCartesianField& eapCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_EapFunctorEapTextureMap __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::TextureMap<
                       gkg::OrientationDistributionFunction >,
                       gkg::OrientationDistributionFunction >::getInstance().
                                                           registerEapFunctor(
                                 gkg::EapFunctorEapTextureMap::getInstance() );

