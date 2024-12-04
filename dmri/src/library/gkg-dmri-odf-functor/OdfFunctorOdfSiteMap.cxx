#include <gkg-dmri-odf-functor/OdfFunctorOdfSiteMap.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorOdfSiteMap::OdfFunctorOdfSiteMap()
                          : gkg::OdfFunctor< gkg::SiteMap< int32_t, int32_t >,
                                             gkg::Vector3d< int32_t > >()
{
}


gkg::OdfFunctorOdfSiteMap::~OdfFunctorOdfSiteMap()
{
}


std::string gkg::OdfFunctorOdfSiteMap::getName() const
{

  return "odf_site_map";

}


bool gkg::OdfFunctorOdfSiteMap::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorOdfSiteMap::process(
                             gkg::SiteMap< int32_t, int32_t >& output,
                             OdfCartesianField& odfCartesianField,
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

    output = odfCartesianField.getSiteMap();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorOdfSiteMap::process( "
             "gkg::SiteMap< int32_t, int32_t >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorOdfSiteMap __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                        gkg::Vector3d< int32_t > >::getInstance().
                                                           registerOdfFunctor(
                                    gkg::OdfFunctorOdfSiteMap::getInstance() );
