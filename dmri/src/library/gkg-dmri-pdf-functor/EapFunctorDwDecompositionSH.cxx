#include <gkg-dmri-pdf-functor/EapFunctorDwDecompositionSH.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-dmri-pdf-dpi/DpiEapCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorDwDecompositionSH::EapFunctorDwDecompositionSH()
                                 : gkg::EapFunctor< gkg::Volume< float >,
                                                    float >()
{
}


gkg::EapFunctorDwDecompositionSH::~EapFunctorDwDecompositionSH()
{
}


std::string gkg::EapFunctorDwDecompositionSH::getName() const
{

  return "dw_sh";

}


bool gkg::EapFunctorDwDecompositionSH::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorDwDecompositionSH::process(
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
//                                        eapCartesianField.getFunctor() );

    if ( dpiEapCartesianFieldFunctor )
    {

      output =
        dpiEapCartesianFieldFunctor->getDwSphericalHarmonicsCoefficients();

    }
    // else if ( dsiEapCartesianFieldFunctor )
//     {

//       output =
//         dsiEapCartesianFieldFunctor->getDwSphericalHarmonicsCoefficients();

//     }
    
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::EapFunctorDwDecompositionSH::process( "
             "gkg::Volume< float >& output, "
             "gkg::EapCartesianField& eapCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_EapFunctorDwDecompositionSH __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::Volume< float >, float >::getInstance().
		   registerEapFunctor(
			      gkg::EapFunctorDwDecompositionSH::getInstance() );

