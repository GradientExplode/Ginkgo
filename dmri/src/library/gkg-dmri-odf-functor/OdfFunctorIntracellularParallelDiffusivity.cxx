#include <gkg-dmri-odf-functor/OdfFunctorIntracellularParallelDiffusivity.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdfCartesianFieldFunctor.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorIntracellularParallelDiffusivity::
                               OdfFunctorIntracellularParallelDiffusivity()
                                                : gkg::OdfFunctor<
                                                           gkg::Volume< float >,
                                                           float >()
{
}


gkg::OdfFunctorIntracellularParallelDiffusivity::
                              ~OdfFunctorIntracellularParallelDiffusivity()
{
}


std::string 
gkg::OdfFunctorIntracellularParallelDiffusivity::getName() const
{

  return "intracellular_parallel_diffusivity";

}


bool 
gkg::OdfFunctorIntracellularParallelDiffusivity::
                                                 isStandardBasisRequired() const
{

  return false;

}


void gkg::OdfFunctorIntracellularParallelDiffusivity::process(
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

      output = axCaliberOdfCartesianFieldFunctor->
                                          getIntracellularParallelDiffusivity();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      return;

    }

  }
  GKG_CATCH( "void gkg::OdfFunctorIntracellularParallelDiffusivity::"
             "process( "
             "gkg::Volume< float >& output, "
             "gkg::OdfCartesianField& odfCartesianField, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "bool verbose ) const" );

}



////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorIntracellularParallelDiffusivity
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                             registerOdfFunctor(
          gkg::OdfFunctorIntracellularParallelDiffusivity::getInstance() );
