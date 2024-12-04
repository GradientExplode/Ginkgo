#include <gkg-dmri-odf-functor/OdfFunctorMeanSquaredDisplacement.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-odf-dsi/DsiOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorMeanSquaredDisplacement::OdfFunctorMeanSquaredDisplacement()
                                 : gkg::OdfFunctor< gkg::Volume< float >,
                                                    float >()
{
}


gkg::OdfFunctorMeanSquaredDisplacement::~OdfFunctorMeanSquaredDisplacement()
{
}


std::string gkg::OdfFunctorMeanSquaredDisplacement::getName() const
{

  return "mean_squared_displacement";

}


bool gkg::OdfFunctorMeanSquaredDisplacement::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorMeanSquaredDisplacement::process(
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
        dsiOdfCartesianFieldFunctor->getMeanSquaredDisplacement();

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
  GKG_CATCH( "void gkg::OdfFunctorMeanSquaredDisplacement::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorMeanSquaredDisplacement 
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                           registerOdfFunctor(
                          gkg::OdfFunctorMeanSquaredDisplacement::getInstance() );


