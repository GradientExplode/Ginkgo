#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorMaxNormalizedMesh.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor_i.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::DiffusoidFunctorMaxNormalizedMesh::DiffusoidFunctorMaxNormalizedMesh()
                                       : gkg::DiffusoidFunctor<
                                           gkg::MeshMap< int32_t, float, 3U >,
                                           std::vector< float > >()
{
}


gkg::DiffusoidFunctorMaxNormalizedMesh::~DiffusoidFunctorMaxNormalizedMesh()
{
}


std::string gkg::DiffusoidFunctorMaxNormalizedMesh::getName() const
{

  return "max_normalized_mesh";

}


void gkg::DiffusoidFunctorMaxNormalizedMesh::processItem(
                 std::vector< float >& item,
                 const gkg::Diffusoid& diffusoid,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // getting mesh scale
    float meshScale = scalarParameters[ 1 ];

    // preparing min max filter(s)
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
    float maximum;

    // processing min-max scaling
    item = diffusoid.getMagnitudes();

    // processing max value
    maximumFilter.filter( item, maximum );

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( 0.0, maximum,
                0.0, meshScale );
    rescaler.rescale( item, item );

  }
  GKG_CATCH( "void gkg::DiffusoidFunctorMaxNormalizedMesh::processItem( "
             "std::vector< float >& item, "
             "const gkg::Diffusoid& diffusoid, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Diffusoid functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_DiffusoidFunctorMaxNormalizedMesh 
                                                       __attribute__((unused)) =
gkg::DiffusoidFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                              std::vector< float > >::getInstance().
                                                     registerDiffusoidFunctor(
                       gkg::DiffusoidFunctorMaxNormalizedMesh::getInstance() );
