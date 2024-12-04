#include <gkg-dmri-odf-functor/OdfFunctorAxonDiameterStdDev.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorAxonDiameterStdDev::OdfFunctorAxonDiameterStdDev()
                                  : gkg::OdfFunctor< gkg::Volume< float >,
                                                     float >()
{
}


gkg::OdfFunctorAxonDiameterStdDev::~OdfFunctorAxonDiameterStdDev()
{
}


std::string gkg::OdfFunctorAxonDiameterStdDev::getName() const
{

  return "axon_diameter_stddev";

}


bool gkg::OdfFunctorAxonDiameterStdDev::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorAxonDiameterStdDev::process(
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

      output = axCaliberOdfCartesianFieldFunctor->getDiameterStdDev();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorAxonDiameterStdDev::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorAxonDiameterStdDev __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
                             gkg::OdfFunctorAxonDiameterStdDev::getInstance() );
