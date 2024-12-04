#include <gkg-dmri-pdf-functor/EapFunctorEapSiteMap.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorEapSiteMap::EapFunctorEapSiteMap()
                          : gkg::EapFunctor< gkg::SiteMap< int32_t, int32_t >,
                                             gkg::Vector3d< int32_t > >()
{
}


gkg::EapFunctorEapSiteMap::~EapFunctorEapSiteMap()
{
}


std::string gkg::EapFunctorEapSiteMap::getName() const
{

  return "eap_site_map";

}


bool gkg::EapFunctorEapSiteMap::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorEapSiteMap::process(
			  gkg::SiteMap< int32_t, int32_t >& output,
			  EapCartesianField& eapCartesianField,
			  const std::vector< double >& /*scalarParameters*/,
			  const std::vector< std::string >&/*stringParameters*/,
			  bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "processing " << this->getName() << " : " << std::flush;

    }

    output = eapCartesianField.getSiteMap();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::EapFunctorEapSiteMap::process( "
             "gkg::SiteMap< int32_t, int32_t >& output, "
             "gkg::EapCartesianField& eapCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_EapFunctorEapSiteMap __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                        gkg::Vector3d< int32_t > >::getInstance().
                                                           registerEapFunctor(
                                    gkg::EapFunctorEapSiteMap::getInstance() );

