#include <gkg-dmri-pdf-functor/EapFunctorReturnToOriginProbability.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-dmri-pdf-dpi/DpiEapCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorReturnToOriginProbability::EapFunctorReturnToOriginProbability()
                                 : gkg::EapFunctor< gkg::Volume< float >,
                                                    float >()
{
}


gkg::EapFunctorReturnToOriginProbability::~EapFunctorReturnToOriginProbability()
{
}


std::string gkg::EapFunctorReturnToOriginProbability::getName() const
{

  return "return_to_origin_probability";

}


bool gkg::EapFunctorReturnToOriginProbability::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorReturnToOriginProbability::process(
                           gkg::Volume< float >& output,
			   EapCartesianField& eapCartesianField,
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

    
    gkg::DpiEapCartesianFieldFunctor* dpiEapCartesianFieldFunctor =
      dynamic_cast< gkg::DpiEapCartesianFieldFunctor* >(
						 eapCartesianField.getFunctor() );

    
    if ( dpiEapCartesianFieldFunctor )
    {

      // NUMERICAL RTO
//    output =
//  	dpiEapCartesianFieldFunctor->getAverageNormalizedDwSignal();

      // ANALYTICAL
      output =
	dpiEapCartesianFieldFunctor->getAnalyticalEapAtZero();
	
    }


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "void gkg::EapFunctorReturnToOriginProbability::process( "
             "gkg::Volume< float >& output, "
             "gkg::EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////
static bool register_gkg_EapFunctorReturnToOriginProbability 
                                                       __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
		   registerEapFunctor(
		      gkg::EapFunctorReturnToOriginProbability::getInstance() );


