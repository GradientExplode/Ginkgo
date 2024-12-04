#include <gkg-dmri-odf-functor/OdfFunctorIsotropicDiffusivity.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-microstructure-noddi/NoddiOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEAxonMappingOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEAxonMappingOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorIsotropicDiffusivity::OdfFunctorIsotropicDiffusivity()
                                    : gkg::OdfFunctor< gkg::Volume< float >,
                                                       float >()
{
}


gkg::OdfFunctorIsotropicDiffusivity::~OdfFunctorIsotropicDiffusivity()
{
}


std::string gkg::OdfFunctorIsotropicDiffusivity::getName() const
{

  return "isotropic_diffusivity";

}


bool gkg::OdfFunctorIsotropicDiffusivity::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorIsotropicDiffusivity::process(
                         gkg::Volume< float >& output,
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

    gkg::NoddiOdfCartesianFieldFunctor*
      noddiOdfCartesianFieldFunctor =
      dynamic_cast< gkg::NoddiOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    if ( noddiOdfCartesianFieldFunctor )
    {

      output = noddiOdfCartesianFieldFunctor->getIsotropicDiffusivity();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

    gkg::PGSEAxonMappingOdfCartesianFieldFunctor*
      pgseAxonMappingOdfCartesianFieldFunctor =
      dynamic_cast< gkg::PGSEAxonMappingOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    if ( pgseAxonMappingOdfCartesianFieldFunctor )
    {

      output = pgseAxonMappingOdfCartesianFieldFunctor->
                                                     getIsotropicDiffusivity();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

    gkg::CTOGSEAxonMappingOdfCartesianFieldFunctor*
      ctogseAxonMappingOdfCartesianFieldFunctor =
      dynamic_cast< gkg::CTOGSEAxonMappingOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    if ( ctogseAxonMappingOdfCartesianFieldFunctor )
    {

      output = ctogseAxonMappingOdfCartesianFieldFunctor->
                                                     getIsotropicDiffusivity();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorIsotropicDiffusivity::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorIsotropicDiffusivity
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
                           gkg::OdfFunctorIsotropicDiffusivity::getInstance() );
