#include <gkg-dmri-odf-functor/OdfFunctorIvimFraction.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-microstructure-ivim/IvimOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorIvimFraction::OdfFunctorIvimFraction()
                            : gkg::OdfFunctor< gkg::Volume< float >,
                                               float >()
{
}


gkg::OdfFunctorIvimFraction::~OdfFunctorIvimFraction()
{
}


std::string gkg::OdfFunctorIvimFraction::getName() const
{

  return "ivim_fraction";

}


bool gkg::OdfFunctorIvimFraction::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorIvimFraction::process(
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

      output = ivimOdfCartesianFieldFunctor->getIvimFraction();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorIvimFraction::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorIvimFraction __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
                          gkg::OdfFunctorIvimFraction::getInstance() );
