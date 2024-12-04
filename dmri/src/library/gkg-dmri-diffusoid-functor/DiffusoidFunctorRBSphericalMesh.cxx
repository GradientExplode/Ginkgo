#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorRBSphericalMesh.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctor_i.h>
#include <gkg-dmri-diffusoid-functor/DiffusoidFunctorFactory_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::DiffusoidFunctorRBSphericalMesh::DiffusoidFunctorRBSphericalMesh()
                                     : gkg::DiffusoidFunctor<
                                        gkg::MeshMap< int32_t, float, 3U >,
                                        std::vector< std::vector< float > > >()
{
}


gkg::DiffusoidFunctorRBSphericalMesh::~DiffusoidFunctorRBSphericalMesh()
{
}


std::string gkg::DiffusoidFunctorRBSphericalMesh::getName() const
{

  return "rb_spherical_mesh";

}


void gkg::DiffusoidFunctorRBSphericalMesh::processItem(
                 std::vector< std::vector< float > >& item,
                 const gkg::Diffusoid& diffusoid,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // getting mesh scale
    float rgbScale = scalarParameters[ 0 ];

    // getting mesh scale
    float meshScale = scalarParameters[ 1 ];

    // collecting magnitude(s)
    std::vector< float > magnitudes;
    magnitudes = diffusoid.getMagnitudes();

    // allocating mesh scaling
    item[ 0 ] = std::vector< float >( magnitudes.size(), meshScale );

    // getting maximum magnitude
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
    float maximum;
    maximumFilter.filter( magnitudes, maximum );

    // rescaling magnitude
    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( 0.0, maximum,
                0.0, rgbScale );
    rescaler.rescale( magnitudes, magnitudes );

    // building red/green/blue texture scalings
    item[ 1 ] = magnitudes;
    item[ 2 ] = std::vector< float >( magnitudes.size(), 0.5f );
    item[ 3 ] = std::vector< float >( magnitudes.size() );
    int32_t o;
    for ( o = 0; o < ( int32_t )magnitudes.size(); o++ )
    {

      item[ 3 ][ o ] = 1.0 - magnitudes[ o ];
      if ( item[ 3 ][ o ] < 0.0 )
      {

        item[ 3 ][ o ] = 0.0;

      }

    }

  }
  GKG_CATCH( "void gkg::DiffusoidFunctorRBSphericalMesh::processItem( "
             "std::vector< std::vector< float > >& item, "
             "const gkg::Diffusoid& diffusoid, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Diffusoid functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_DiffusoidFunctorRBSphericalMesh 
                                                       __attribute__((unused)) =
  gkg::DiffusoidFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                        std::vector< std::vector< float > > >::getInstance().
                                                     registerDiffusoidFunctor(
                         gkg::DiffusoidFunctorRBSphericalMesh::getInstance() );
