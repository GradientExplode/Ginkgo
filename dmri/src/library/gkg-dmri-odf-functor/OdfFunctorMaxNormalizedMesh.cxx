#include <gkg-dmri-odf-functor/OdfFunctorMaxNormalizedMesh.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorMaxNormalizedMesh::OdfFunctorMaxNormalizedMesh()
                                    : gkg::OdfFunctor<
                                           gkg::MeshMap< int32_t, float, 3U >,
                                           std::vector< float > >()
{
}


gkg::OdfFunctorMaxNormalizedMesh::~OdfFunctorMaxNormalizedMesh()
{
}


std::string gkg::OdfFunctorMaxNormalizedMesh::getName() const
{

  return "max_normalized_mesh";

}


bool 
gkg::OdfFunctorMaxNormalizedMesh::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorMaxNormalizedMesh::processItem(
                 std::vector< float >& item,
                 const gkg::OrientationDistributionFunction& odf,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // getting mesh scale
    float meshScale = scalarParameters[ 1 ];

    // preparing max filter
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
    float maximum;

    // processing max scaling
    item = odf.getOrientationProbabilities();

    // processing max value
    maximumFilter.filter( item, maximum );

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( 0.0, maximum,
                0.0, meshScale );

    rescaler.rescale( item, item );

  }
  GKG_CATCH( "void gkg::OdfFunctorMaxNormalizedMesh::processItem( "
             "std::vector< float >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorMaxNormalizedMesh __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                        std::vector< float > >::getInstance().
                                                           registerOdfFunctor(
                          gkg::OdfFunctorMaxNormalizedMesh::getInstance() );
