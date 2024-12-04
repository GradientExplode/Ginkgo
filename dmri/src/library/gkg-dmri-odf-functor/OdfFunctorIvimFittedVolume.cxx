#include <gkg-dmri-odf-functor/OdfFunctorIvimFittedVolume.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-microstructure-ivim/IvimOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorIvimFittedVolume::OdfFunctorIvimFittedVolume()
                                : gkg::OdfFunctor< gkg::Volume< float >,
                                                   float >()
{
}


gkg::OdfFunctorIvimFittedVolume::~OdfFunctorIvimFittedVolume()
{
}


std::string gkg::OdfFunctorIvimFittedVolume::getName() const
{

  return "ivim_fitted_volume";

}


bool gkg::OdfFunctorIvimFittedVolume::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorIvimFittedVolume::process(
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


    gkg::IvimOdfCartesianFieldFunctor*
      ivimOdfCartesianFieldFunctor =
      dynamic_cast< gkg::IvimOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    if ( ivimOdfCartesianFieldFunctor )
    {

      output = ivimOdfCartesianFieldFunctor->getIvimFittedVolume();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorIvimFittedVolume::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorIvimFittedVolume __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
                          gkg::OdfFunctorIvimFittedVolume::getInstance() );
