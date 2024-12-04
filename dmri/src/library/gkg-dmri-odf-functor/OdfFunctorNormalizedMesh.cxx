#include <gkg-dmri-odf-functor/OdfFunctorNormalizedMesh.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorNormalizedMesh::OdfFunctorNormalizedMesh()
                              : gkg::OdfFunctor<
                                           gkg::MeshMap< int32_t, float, 3U >,
                                           std::vector< float > >()
{
}


gkg::OdfFunctorNormalizedMesh::~OdfFunctorNormalizedMesh()
{
}


std::string gkg::OdfFunctorNormalizedMesh::getName() const
{

  return "normalized_mesh";

}


bool 
gkg::OdfFunctorNormalizedMesh::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorNormalizedMesh::processItem(
                 std::vector< float >& item,
                 const gkg::OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // getting mesh scale
    float meshScale = scalarParameters[ 1 ];

    // processing orientation probabilities
    item = odf.getOrientationProbabilities();

    // rescaling probabilities
    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( 0.0, 1.0,
                0.0, meshScale );

    rescaler.rescale( item, item );

  }
  GKG_CATCH( "void gkg::OdfFunctorNormalizedMesh::processItem( "
             "std::vector< float >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorNormalizedMesh __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                        std::vector< float > >::getInstance().
                                                           registerOdfFunctor(
                          gkg::OdfFunctorNormalizedMesh::getInstance() );
