#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorMinMaxNormalizedMesh.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor_i.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::DiffusoidFunctorMinMaxNormalizedMesh::
                                         DiffusoidFunctorMinMaxNormalizedMesh()
                                          : gkg::DiffusoidFunctor<
                                            gkg::MeshMap< int32_t, float, 3U >,
                                              std::vector< float > >()
{
}


gkg::DiffusoidFunctorMinMaxNormalizedMesh::~DiffusoidFunctorMinMaxNormalizedMesh()
{
}


std::string gkg::DiffusoidFunctorMinMaxNormalizedMesh::getName() const
{

  return "minmax_normalized_mesh";

}


void gkg::DiffusoidFunctorMinMaxNormalizedMesh::processItem(
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
    gkg::MinimumFilter< std::vector< float >, float > minimumFilter;
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
    float minimum;
    float maximum;

    // processing min-max scaling
    item = diffusoid.getMagnitudes();

    // processing min & max value(s)
    minimumFilter.filter( item, minimum );
    maximumFilter.filter( item, maximum );

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( minimum, maximum,
                0.0, meshScale );
    rescaler.rescale( item, item );

  }
  GKG_CATCH( "void gkg::DiffusoidFunctorMinMaxNormalizedMesh::processItem( "
             "std::vector< float >& item, "
             "const gkg::Diffusoid& diffusoid, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Diffusoid functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_DiffusoidFunctorMinMaxNormalizedMesh
                                                       __attribute__((unused)) =
gkg::DiffusoidFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                              std::vector< float > >::getInstance().
                                                     registerDiffusoidFunctor(
                    gkg::DiffusoidFunctorMinMaxNormalizedMesh::getInstance() );
