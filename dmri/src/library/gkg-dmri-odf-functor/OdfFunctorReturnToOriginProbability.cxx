#include <gkg-dmri-odf-functor/OdfFunctorReturnToOriginProbability.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-odf-dsi/DsiOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorReturnToOriginProbability::OdfFunctorReturnToOriginProbability()
                                 : gkg::OdfFunctor< gkg::Volume< float >,
                                                    float >()
{
}


gkg::OdfFunctorReturnToOriginProbability::~OdfFunctorReturnToOriginProbability()
{
}


std::string gkg::OdfFunctorReturnToOriginProbability::getName() const
{

  return "return_to_origin_probability";

}


bool gkg::OdfFunctorReturnToOriginProbability::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorReturnToOriginProbability::process(
                             gkg::Volume< float >& output,
                             OdfCartesianField& odfCartesianField,
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

    gkg::DsiOdfCartesianFieldFunctor* dsiOdfCartesianFieldFunctor =
      dynamic_cast< gkg::DsiOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );


    if ( dsiOdfCartesianFieldFunctor )
    {

      output =
        dsiOdfCartesianFieldFunctor->getReturnToOriginProbability();

    }
    else
    {
      
      std::cout << "Functor does not exist for this type of ODF. Continuing...";
      
    }


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorReturnToOriginProbability::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorReturnToOriginProbability 
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerOdfFunctor(
                      gkg::OdfFunctorReturnToOriginProbability::getInstance() );
