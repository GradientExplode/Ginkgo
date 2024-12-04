#include <gkg-dmri-pdf-functor/EapFunctorOdfMinMaxNormalizedMesh.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorOdfMinMaxNormalizedMesh::EapFunctorOdfMinMaxNormalizedMesh()
                                    : gkg::EapFunctor<
                                           gkg::MeshMap< int32_t, float, 3U >,
                                           std::vector< float > >()
{
}


gkg::EapFunctorOdfMinMaxNormalizedMesh::~EapFunctorOdfMinMaxNormalizedMesh()
{
}


std::string gkg::EapFunctorOdfMinMaxNormalizedMesh::getName() const
{

  return "odf_mesh";

}


bool 
gkg::EapFunctorOdfMinMaxNormalizedMesh::isStandardBasisRequired() const
{

  return false;

}


void gkg::EapFunctorOdfMinMaxNormalizedMesh::processItem(
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
    int32_t count = 0;
    for ( o = 0; o < orientationCount; o++ )
    {
	
      if ( item[ o ] < 0 )
	{
	  
	  item[ o ] = 0.0;
	  count++;
	  
	}
      
    }
    

    // processing min & max value(s)
    minimumFilter.filter( item, minimum );
    maximumFilter.filter( item, maximum );

    //std::cout << "min,max" << minimum << "," << maximum << "\n";

    gkg::Rescaler< std::vector< float >, std::vector< float > >
      rescaler( minimum, maximum,
                0.0, meshScale );

    rescaler.rescale( item, item );

  }
  GKG_CATCH( "void gkg::EapFunctorOdfMinMaxNormalizedMesh::processItem( "
             "std::vector< float >& item, "
             "const gkg::OrientationDistributionFunction& odf, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// ODF functor registration
////////////////////////////////////////////////////////////////////////////////

static bool register_gkg_EapFunctorOdfMinMaxNormalizedMesh 
                                                       __attribute__((unused)) =
gkg::EapFunctorFactory< gkg::MeshMap< int32_t, float, 3U >,
                        std::vector< float > >::getInstance().
                                                           registerEapFunctor(
                        gkg::EapFunctorOdfMinMaxNormalizedMesh::getInstance() );
