#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorMaximumOrientation.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor_i.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::DiffusoidFunctorMaximumOrientation::DiffusoidFunctorMaximumOrientation()
                                        : gkg::DiffusoidFunctor<
                                             gkg::Volume< Vector3d< float > >,
                                             gkg::Vector3d< float > >()
{
}


gkg::DiffusoidFunctorMaximumOrientation::~DiffusoidFunctorMaximumOrientation()
{
}


std::string gkg::DiffusoidFunctorMaximumOrientation::getName() const
{

  return "maximum_orientation";

}


void gkg::DiffusoidFunctorMaximumOrientation::processItem(
                 gkg::Vector3d< float >& item,
                 const Diffusoid& diffusoid,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = diffusoid.getMaximumOrientation();

  }
  GKG_CATCH( "void gkg::DiffusoidFunctorMaximumOrientation::processItem( "
             "gkg::Vector3d< float >& item, "
             "const gkg::Diffusoid& diffusoid, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Diffusoid functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_DiffusoidFunctorMaximumOrientation 
                                                       __attribute__((unused)) =
gkg::DiffusoidFunctorFactory<
                      gkg::Volume< gkg::Vector3d< float > >,
                      gkg::Vector3d< float > >::getInstance().
                                                      registerDiffusoidFunctor(
                      gkg::DiffusoidFunctorMaximumOrientation::getInstance() );
