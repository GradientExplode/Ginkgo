#include <gkg-dmri-pdf-functor/EapFunctorEapDecompositionSH.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-dmri-pdf-dpi/DpiEapCartesianFieldFunctor.h>
//#include <gkg-dmri-pdf-dsi/DsiEapCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorEapDecompositionSH::EapFunctorEapDecompositionSH()
                                 : gkg::EapFunctor< gkg::Volume< float >,
                                                    float >()
{
}


gkg::EapFunctorEapDecompositionSH::~EapFunctorEapDecompositionSH()
{
}


std::string gkg::EapFunctorEapDecompositionSH::getName() const
{

  return "eap_sh";

}


bool gkg::EapFunctorEapDecompositionSH::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorEapDecompositionSH::process(
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

//     gkg::DsiEapCartesianFieldFunctor* dsiEapCartesianFieldFunctor =
//       dynamic_cast< gkg::DsiEapCartesianFieldFunctor* >(
//                                             eapCartesianField.getFunctor() );

    if ( dpiEapCartesianFieldFunctor )
    {

      output =
        dpiEapCartesianFieldFunctor->getEapSphericalHarmonicsCoefficients();

    }
//     else if ( dsiEapCartesianFieldFunctor )
//     {

//       output =
//         dsiEapCartesianFieldFunctor->getEapSphericalHarmonicsCoefficients();

//     }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::EapFunctorEapDecompositionSH::process( "
             "gkg::Volume< float >& output, "
             "gkg::EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_EapFunctorEapDecompositionSH __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
		   registerEapFunctor(
		       gkg::EapFunctorEapDecompositionSH::getInstance() );
