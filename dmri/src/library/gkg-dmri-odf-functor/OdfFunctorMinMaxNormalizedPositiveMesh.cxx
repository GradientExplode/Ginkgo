#include <gkg-dmri-odf-functor/OdfFunctorMinMaxNormalizedPositiveMesh.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorMinMaxNormalizedPositiveMesh::
                                       OdfFunctorMinMaxNormalizedPositiveMesh()
                       : gkg::OdfFunctor< gkg::MeshMap< int32_t, float, 3U >,
                                          std::vector< float > >()
{
}


gkg::OdfFunctorMinMaxNormalizedPositiveMesh::
                                      ~OdfFunctorMinMaxNormalizedPositiveMesh()
{
}


std::string gkg::OdfFunctorMinMaxNormalizedPositiveMesh::getName() const
{

  return "minmax_normalized_positive_mesh";

}


bool 
gkg::OdfFunctorMinMaxNormalizedPositiveMesh::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorMinMaxNormalizedPositiveMesh::processItem(
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

    // setting negative value(s) to zero
    int32_t o = 0;
    int32_t orientationCount = ( int32_t )item.size();
    for ( o = 0; o < orientationCount; o++ )
    {

      if ( item[ o ] < 0 )
      {

        item[ o ] = 0.0;

      }

    }

    // processing min & max value(s)
    minimumFilter.filter( item, minimum );
    maximumFilter.filter( item, maximum );

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( minimum, maximum,
                0.0, meshScale );
    rescaler.rescale( item, item );

  }
  GKG_CATCH( "void gkg::OdfFunctorMinMaxNormalizedPositiveMesh::processItem( "
             "std::vector< float >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorMinMaxNormalizedPositiveMesh 
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                        std::vector< float > >::getInstance().
                                                           registerOdfFunctor(
                  gkg::OdfFunctorMinMaxNormalizedPositiveMesh::getInstance() );
