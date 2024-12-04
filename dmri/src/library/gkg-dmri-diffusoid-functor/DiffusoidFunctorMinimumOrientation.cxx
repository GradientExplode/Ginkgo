#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorMinimumOrientation.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor_i.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::DiffusoidFunctorMinimumOrientation::DiffusoidFunctorMinimumOrientation()
                                        : gkg::DiffusoidFunctor<
                                             gkg::Volume< Vector3d< float > >,
                                             gkg::Vector3d< float > >()
{
}


gkg::DiffusoidFunctorMinimumOrientation::~DiffusoidFunctorMinimumOrientation()
{
}


std::string gkg::DiffusoidFunctorMinimumOrientation::getName() const
{

  return "minimum_orientation";

}


void gkg::DiffusoidFunctorMinimumOrientation::processItem(
                 gkg::Vector3d< float >& item,
                 const Diffusoid& diffusoid,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = diffusoid.getMinimumOrientation();

  }
  GKG_CATCH( "void gkg::DiffusoidFunctorMinimumOrientation::processItem( "
             "gkg::Vector3d< float >& item, "
             "const gkg::Diffusoid& diffusoid, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Diffusoid functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_DiffusoidFunctorMinimumOrientation 
                                                       __attribute__((unused)) =
gkg::DiffusoidFunctorFactory<
                      gkg::Volume< gkg::Vector3d< float > >,
                      gkg::Vector3d< float > >::getInstance().
                                                      registerDiffusoidFunctor(
                      gkg::DiffusoidFunctorMinimumOrientation::getInstance() );
