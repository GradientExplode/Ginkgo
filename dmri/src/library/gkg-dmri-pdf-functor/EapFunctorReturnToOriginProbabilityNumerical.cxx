#include <gkg-dmri-pdf-functor/EapFunctorReturnToOriginProbabilityNumerical.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-dmri-pdf-dpi/DpiEapCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorReturnToOriginProbabilityNumerical::EapFunctorReturnToOriginProbabilityNumerical()
                                 : gkg::EapFunctor< gkg::Volume< float >,
                                                    float >()
{
}


gkg::EapFunctorReturnToOriginProbabilityNumerical::~EapFunctorReturnToOriginProbabilityNumerical()
{
}


std::string gkg::EapFunctorReturnToOriginProbabilityNumerical::getName() const
{

  return "return_to_origin_probability_numerical";

}


bool gkg::EapFunctorReturnToOriginProbabilityNumerical::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorReturnToOriginProbabilityNumerical::process(
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
      output =
	dpiEapCartesianFieldFunctor->getAverageNormalizedDwSignal();
      
      // ANALYTICAL
//       output =
// 	dpiEapCartesianFieldFunctor->getAnalyticalEapAtZero();
	
    }


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "void gkg::EapFunctorReturnToOriginProbabilityNumerical::process( "
             "gkg::Volume< float >& output, "
             "gkg::EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////
static bool register_gkg_EapFunctorReturnToOriginProbabilityNumerical 
                                                       __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
		   registerEapFunctor(
	     gkg::EapFunctorReturnToOriginProbabilityNumerical::getInstance() );
