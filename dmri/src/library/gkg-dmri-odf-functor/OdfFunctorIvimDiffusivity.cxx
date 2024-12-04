#include <gkg-dmri-odf-functor/OdfFunctorIvimDiffusivity.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-microstructure-ivim/IvimOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorIvimDiffusivity::OdfFunctorIvimDiffusivity()
                               : gkg::OdfFunctor< gkg::Volume< float >,
                                                  float >()
{
}


gkg::OdfFunctorIvimDiffusivity::~OdfFunctorIvimDiffusivity()
{
}


std::string gkg::OdfFunctorIvimDiffusivity::getName() const
{

  return "ivim_diffusivity";

}


bool gkg::OdfFunctorIvimDiffusivity::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorIvimDiffusivity::process(
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

      output = ivimOdfCartesianFieldFunctor->getIvimDiffusivity();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorIvimDiffusivity::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorIvimDiffusivity __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
                          gkg::OdfFunctorIvimDiffusivity::getInstance() );
