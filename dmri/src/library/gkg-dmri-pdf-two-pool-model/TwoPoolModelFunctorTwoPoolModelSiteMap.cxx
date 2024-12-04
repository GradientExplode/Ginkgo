#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorTwoPoolModelSiteMap.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctor_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelFunctorFactory_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TwoPoolModelFunctorTwoPoolModelSiteMap::
                                        TwoPoolModelFunctorTwoPoolModelSiteMap()
                          : gkg::TwoPoolModelFunctor<
                                        gkg::SiteMap< int32_t, int32_t >,
                                        gkg::Vector3d< int32_t > >()
{
}


gkg::TwoPoolModelFunctorTwoPoolModelSiteMap::
                                       ~TwoPoolModelFunctorTwoPoolModelSiteMap()
{
}


std::string gkg::TwoPoolModelFunctorTwoPoolModelSiteMap::getName() const
{

  return "two_pool_model_site_map";

}


void gkg::TwoPoolModelFunctorTwoPoolModelSiteMap::process(
                        gkg::SiteMap< int32_t, int32_t >& output,
                        TwoPoolModelCartesianField& twoPoolModelCartesianField,
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

    output = twoPoolModelCartesianField.getSiteMap();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::TwoPoolModelFunctorTwoPoolModelSiteMap::process( "
             "gkg::SiteMap< int32_t, int32_t >& output, "
             "gkg::TwoPoolModelCartesianField& twoPoolModelCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TwoPoolModelFunctorTwoPoolModelSiteMap 
                                                       __attribute__((unused)) =
gkg::TwoPoolModelFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                   gkg::Vector3d< int32_t > >::getInstance().
                                                    registerTwoPoolModelFunctor(
                   gkg::TwoPoolModelFunctorTwoPoolModelSiteMap::getInstance() );
