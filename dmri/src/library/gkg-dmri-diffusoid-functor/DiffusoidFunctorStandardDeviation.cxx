#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorStandardDeviation.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor_i.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::DiffusoidFunctorStandardDeviation::DiffusoidFunctorStandardDeviation()
                                 : gkg::DiffusoidFunctor< gkg::Volume< float >,
                                                          float >()
{
}


gkg::DiffusoidFunctorStandardDeviation::~DiffusoidFunctorStandardDeviation()
{
}


std::string gkg::DiffusoidFunctorStandardDeviation::getName() const
{

  return "standard_deviation";

}


void gkg::DiffusoidFunctorStandardDeviation::processItem(
                 float& item,
                 const gkg::Diffusoid& diffusoid,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = diffusoid.getStandardDeviation();

  }
  GKG_CATCH( "void gkg::DiffusoidFunctorStandardDeviation::processItem( "
             "float& item, "
             "const gkg::Diffusoid& diffusoid, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// Diffusoid functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_DiffusoidFunctorStandardDeviation 
                                                       __attribute__((unused)) =
gkg::DiffusoidFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                     registerDiffusoidFunctor(
                       gkg::DiffusoidFunctorStandardDeviation::getInstance() );
