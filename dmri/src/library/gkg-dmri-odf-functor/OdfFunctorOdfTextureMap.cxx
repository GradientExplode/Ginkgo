#include <gkg-dmri-odf-functor/OdfFunctorOdfTextureMap.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorOdfTextureMap::OdfFunctorOdfTextureMap()
                                 : gkg::OdfFunctor< gkg::TextureMap<
                                       gkg::OrientationDistributionFunction >,
                                       gkg::OrientationDistributionFunction >()
{
}


gkg::OdfFunctorOdfTextureMap::~OdfFunctorOdfTextureMap()
{
}


std::string gkg::OdfFunctorOdfTextureMap::getName() const
{

  return "odf_texture_map";

}


bool gkg::OdfFunctorOdfTextureMap::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorOdfTextureMap::process(
               gkg::TextureMap< gkg::OrientationDistributionFunction >& output,
               gkg::OdfCartesianField& odfCartesianField,
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

    output = odfCartesianField.getItems();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorOdfTextureMap::process( "
             "gkg::TextureMap< gkg::OrientationDistributionFunction >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorOdfTextureMap __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::TextureMap<
                       gkg::OrientationDistributionFunction >,
                       gkg::OrientationDistributionFunction >::getInstance().
                                                           registerOdfFunctor(
                                 gkg::OdfFunctorOdfTextureMap::getInstance() );
