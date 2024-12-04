#include <gkg-dmri-pdf-dti/TensorFunctorTensorSiteMap.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorTensorSiteMap::TensorFunctorTensorSiteMap()
                          : gkg::TensorFunctor<
                                        gkg::SiteMap< int32_t, int32_t >,
                                        gkg::Vector3d< int32_t > >()
{
}


gkg::TensorFunctorTensorSiteMap::~TensorFunctorTensorSiteMap()
{
}


std::string gkg::TensorFunctorTensorSiteMap::getName() const
{

  return "tensor_site_map";

}


void gkg::TensorFunctorTensorSiteMap::process(
                        gkg::SiteMap< int32_t, int32_t >& output,
                        TensorCartesianField& tensorCartesianField,
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

    output = tensorCartesianField.getSiteMap();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::TensorFunctorTensorSiteMap::process( "
             "gkg::SiteMap< int32_t, int32_t >& output, "
             "gkg::TensorCartesianField& tensorCartesianField, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorTensorSiteMap __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::SiteMap< int32_t, int32_t >,
                        gkg::Vector3d< int32_t > >::getInstance().
                                                        registerTensorFunctor(
                              gkg::TensorFunctorTensorSiteMap::getInstance() );
