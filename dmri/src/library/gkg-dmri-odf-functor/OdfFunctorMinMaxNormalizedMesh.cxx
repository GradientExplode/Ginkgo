#include <gkg-dmri-odf-functor/OdfFunctorMinMaxNormalizedMesh.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorMinMaxNormalizedMesh::OdfFunctorMinMaxNormalizedMesh()
                                    : gkg::OdfFunctor<
                                           gkg::MeshMap< int32_t, float, 3U >,
                                           std::vector< float > >()
{
}


gkg::OdfFunctorMinMaxNormalizedMesh::~OdfFunctorMinMaxNormalizedMesh()
{
}


std::string gkg::OdfFunctorMinMaxNormalizedMesh::getName() const
{

  return "minmax_normalized_mesh";

}


bool 
gkg::OdfFunctorMinMaxNormalizedMesh::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorMinMaxNormalizedMesh::processItem(
                 std::vector< float >& item,
                 const gkg::OrientationDistributionFunction& odf,
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
    item = odf.getOrientationProbabilities();

    // processing min & max value(s)
    minimumFilter.filter( item, minimum );
    maximumFilter.filter( item, maximum );

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( minimum, maximum,
                0.0, meshScale );

    rescaler.rescale( item, item );

  }
  GKG_CATCH( "void gkg::OdfFunctorMinMaxNormalizedMesh::processItem( "
             "std::vector< float >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorMinMaxNormalizedMesh 
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                        std::vector< float > >::getInstance().
                                                           registerOdfFunctor(
                          gkg::OdfFunctorMinMaxNormalizedMesh::getInstance() );
