#include <gkg-dmri-pdf-functor/EapFunctorGFAScaledEapMeshesMaxNormalized.h>
#include <gkg-dmri-pdf-functor/EapFunctor_i.h>
#include <gkg-dmri-pdf-functor/EapFunctorFactory_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-exception/Exception.h>


gkg::EapFunctorGFAScaledEapMeshesMaxNormalized::EapFunctorGFAScaledEapMeshesMaxNormalized()
                                    : gkg::EapFunctor<
                              std::vector< gkg::MeshMap< int32_t, float, 3U > >,
                              std::vector< float > >()
{
}


gkg::EapFunctorGFAScaledEapMeshesMaxNormalized::~EapFunctorGFAScaledEapMeshesMaxNormalized()
{
}


std::string gkg::EapFunctorGFAScaledEapMeshesMaxNormalized::getName() const
{

  return "gfa_scaled_eap_meshes_max_normalized";

}


bool 
gkg::EapFunctorGFAScaledEapMeshesMaxNormalized::isStandardBasisRequired() const
{

  return false;  // for now, the change of basis will be done as we process 

}


void gkg::EapFunctorGFAScaledEapMeshesMaxNormalized::processItem(
                 std::vector< float >& item,
                 const gkg::OrientationDistributionFunction& eap,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    bool filterNegative = true;

    // getting mesh scale
    float meshScale = scalarParameters[ 1 ];

    // obtaining threshold GFA
    float gfa = eap.getGeneralizedFractionalAnisotropy();
    float lowerGFA = scalarParameters[ 2 ];
    float upperGFA = scalarParameters[ 3 ];
    if ( gfa < lowerGFA )
    {

      gfa = 0.0;

    }
    else if ( gfa > upperGFA )
    {

      gfa = 1.0;

    }
    else
    {

      gfa = ( gfa - lowerGFA ) / ( upperGFA- lowerGFA );

    }

    // preparing min max filter(s)
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;
    float maximum;

    // processing min-max scaling
    item = eap.getOrientationProbabilities();

    if ( filterNegative )
    {
    
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

    }

    if( !scalarParameters[ 6 ] )  // if set opacity, no min-max normalization
    {
    
      //processing min & max value(s)
      maximumFilter.filter( item, maximum );
            
      gkg::Rescaler< std::vector< float >, std::vector< float > >
	rescaler( 0.0, maximum,
		  0.0, meshScale * gfa );
      
      //std::cout << "Min, Max : " << minimum << "," << maximum << "\n";
      rescaler.rescale( item, item );

    }
      
  }
  GKG_CATCH( 
            "void gkg::EapFunctorGFAScaledEapMeshesMaxNormalized::processItem( "
            "std::vector< float >& item, "
            "const gkg::OrientationDistributionFunction& eap, "
            "const std::vector< double >& scalarParameters, "
            "const std::vector< std::string >& /*stringParameters*/ ) "
            "const" );

}


////////////////////////////////////////////////////////////////////////////////
// EAP functor registration
////////////////////////////////////////////////////////////////////////////////
static bool register_gkg_EapFunctorGFAScaledEapMeshesMaxNormalized 
                                                       __attribute__((unused)) =
  gkg::EapFunctorFactory< std::vector< gkg::MeshMap< int32_t, float, 3U > >,
			  std::vector< float > >::getInstance().
                                                             registerEapFunctor(
                gkg::EapFunctorGFAScaledEapMeshesMaxNormalized::getInstance() );
