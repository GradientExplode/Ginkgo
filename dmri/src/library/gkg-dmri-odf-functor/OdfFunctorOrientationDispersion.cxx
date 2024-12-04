#include <gkg-dmri-odf-functor/OdfFunctorOrientationDispersion.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-microstructure-noddi/NoddiOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEAxonMappingOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEAxonMappingOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorOrientationDispersion::OdfFunctorOrientationDispersion()
                                     : gkg::OdfFunctor< gkg::Volume< float >,
                                                        float >()
{
}


gkg::OdfFunctorOrientationDispersion::~OdfFunctorOrientationDispersion()
{
}


std::string gkg::OdfFunctorOrientationDispersion::getName() const
{

  return "orientation_dispersion";

}


bool gkg::OdfFunctorOrientationDispersion::isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorOrientationDispersion::process(
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

    gkg::NoddiOdfCartesianFieldFunctor*
      noddiOdfCartesianFieldFunctor =
      dynamic_cast< gkg::NoddiOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    if ( noddiOdfCartesianFieldFunctor )
    {

      output = noddiOdfCartesianFieldFunctor->getOrientationDispersion();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

    gkg::PGSEAxonMappingOdfCartesianFieldFunctor*
      pgseAxonMappingOdfCartesianFieldFunctor =
      dynamic_cast< gkg::PGSEAxonMappingOdfCartesianFieldFunctor* >(
                                               odfCartesianField.getFunctor() );

    if ( pgseAxonMappingOdfCartesianFieldFunctor )
    {

      output = pgseAxonMappingOdfCartesianFieldFunctor->
                                                     getOrientationDispersion();
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

      output = ctogseAxonMappingOdfCartesianFieldFunctor->
                                                     getOrientationDispersion();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorOrientationDispersion::process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorOrientationDispersion 
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
                          gkg::OdfFunctorOrientationDispersion::getInstance() );
