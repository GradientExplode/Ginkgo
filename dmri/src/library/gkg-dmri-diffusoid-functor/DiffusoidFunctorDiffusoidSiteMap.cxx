#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorDiffusoidSiteMap.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor_i.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::DiffusoidFunctorDiffusoidSiteMap::DiffusoidFunctorDiffusoidSiteMap()
                                      : gkg::DiffusoidFunctor<
                                          gkg::SiteMap< int32_t, int32_t >,
                                          gkg::Vector3d< int32_t > >()
{
}


gkg::DiffusoidFunctorDiffusoidSiteMap::~DiffusoidFunctorDiffusoidSiteMap()
{
}


std::string gkg::DiffusoidFunctorDiffusoidSiteMap::getName() const
{

  return "diffusoid_site_map";

}


void gkg::DiffusoidFunctorDiffusoidSiteMap::process(
                        gkg::SiteMap< int32_t, int32_t >& output,
                        DiffusoidCartesianField& diffusoidCartesianField,
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

    output = diffusoidCartesianField.getSiteMap();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DiffusoidFunctorDiffusoidSiteMap::process( "
             "gkg::SiteMap< int32_t, int32_t >& output, "
             "gkg::DiffusoidCartesianField& diffusoidCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_DiffusoidFunctorDiffusoidSiteMap 
                                                       __attribute__((unused)) =
gkg::DiffusoidFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                        gkg::Vector3d< int32_t > >::getInstance().
                                                     registerDiffusoidFunctor(
                        gkg::DiffusoidFunctorDiffusoidSiteMap::getInstance() );
