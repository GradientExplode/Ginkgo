#include <gkg-dmri-odf-functor/OdfFunctorMaxNormalizedPositiveMesh.h>
#include <gkg-dmri-odf-functor/OdfFunctor_i.h>
#include <gkg-dmri-odf-functor/OdfFunctorFactory_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::OdfFunctorMaxNormalizedPositiveMesh::
                                       OdfFunctorMaxNormalizedPositiveMesh()
                       : gkg::OdfFunctor< gkg::MeshMap< int32_t, float, 3U >,
                                          std::vector< float > >()
{
}


gkg::OdfFunctorMaxNormalizedPositiveMesh::
                                      ~OdfFunctorMaxNormalizedPositiveMesh()
{
}


std::string gkg::OdfFunctorMaxNormalizedPositiveMesh::getName() const
{

  return "max_normalized_positive_mesh";

}


bool 
gkg::OdfFunctorMaxNormalizedPositiveMesh::isStandardBasisRequired() const
{

  return true;

}


void gkg::OdfFunctorMaxNormalizedPositiveMesh::processItem(
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

    // processing max value
    maximumFilter.filter( item, maximum );

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( 0.0, maximum,
                0.0, meshScale );
    rescaler.rescale( item, item );

  }
  GKG_CATCH( "void gkg::OdfFunctorMaxNormalizedPositiveMesh::processItem( "
             "std::vector< float >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_OdfFunctorMaxNormalizedPositiveMesh 
                                                       __attribute__((unused)) =
gkg::OdfFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                        std::vector< float > >::getInstance().
                                                           registerOdfFunctor(
                  gkg::OdfFunctorMaxNormalizedPositiveMesh::getInstance() );
