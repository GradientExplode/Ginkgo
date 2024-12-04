#include <gkg-dmri-odf-functor/OdfFunctorAxonDiameter.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEAxonMappingOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEAxonMappingOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorAxonDiameter::OdfFunctorAxonDiameter()
                            : gkg::OdfFunctor< gkg::Volume< float >,
                                               float >()
{
}


gkg::OdfFunctorAxonDiameter::~OdfFunctorAxonDiameter()
{
}


std::string gkg::OdfFunctorAxonDiameter::getName() const
{

  return "axon_diameter";

}


bool gkg::OdfFunctorAxonDiameter::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorAxonDiameter::process(
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

    gkg::PGSEAxonMappingOdfCartesianFieldFunctor*
      pgseAxonMappingOdfCartesianFieldFunctor =
      dynamic_cast< gkg::PGSEAxonMappingOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    if ( pgseAxonMappingOdfCartesianFieldFunctor )
    {

      output = pgseAxonMappingOdfCartesianFieldFunctor->getAxonDiameter();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

    gkg::CTOGSEAxonMappingOdfCartesianFieldFunctor*
      ctogseAxonMappingOdfCartesianFieldFunctor =
      dynamic_cast< gkg::CTOGSEAxonMappingOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    if ( ctogseAxonMappingOdfCartesianFieldFunctor )
    {

      output = ctogseAxonMappingOdfCartesianFieldFunctor->getAxonDiameter();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

    gkg::AxCaliberOdfCartesianFieldFunctor*
      axCaliberOdfCartesianFieldFunctor =
      dynamic_cast< gkg::AxCaliberOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    if ( axCaliberOdfCartesianFieldFunctor )
    {

      output = axCaliberOdfCartesianFieldFunctor->getDiameterMean();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorAxonDiameter::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorAxonDiameter __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
                              gkg::OdfFunctorAxonDiameter::getInstance() );
