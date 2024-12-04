#include <gkg-dmri-odf-functor/OdfFunctorHinderedDiffusivity.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorHinderedDiffusivity::OdfFunctorHinderedDiffusivity()
                                   : gkg::OdfFunctor< gkg::Volume< float >,
                                                      float >()
{
}


gkg::OdfFunctorHinderedDiffusivity:: ~OdfFunctorHinderedDiffusivity()
{
}


std::string 
gkg::OdfFunctorHinderedDiffusivity::getName() const
{

  return "hindered_diffusivity";

}


bool 
gkg::OdfFunctorHinderedDiffusivity::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorHinderedDiffusivity::process(
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

    gkg::AxCaliberOdfCartesianFieldFunctor*
      axCaliberOdfCartesianFieldFunctor =
      dynamic_cast< gkg::AxCaliberOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    if ( axCaliberOdfCartesianFieldFunctor )
    {

      output = axCaliberOdfCartesianFieldFunctor->getHinderedDiffusivity();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorHinderedDiffusivity::"
             "process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorHinderedDiffusivity
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
          gkg::OdfFunctorHinderedDiffusivity::getInstance() );
