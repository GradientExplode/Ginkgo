#include <gkg-processing-numericalanalysis/OptimalNonCentralChiFunctionSampler.h>
#include <gkg-processing-numericalanalysis/NonCentralChiFunction.h>
#include <gkg-core-exception/Exception.h>


gkg::OptimalNonCentralChiFunctionSampler::OptimalNonCentralChiFunctionSampler( 
                                       const gkg::ParametricFunction& function )
                                        : OptimalParametricFunctionSampler( 
                                                                      function )
{
}


void gkg::OptimalNonCentralChiFunctionSampler::get( gkg::Matrix& x ) const
{

  try
  {
  
    const gkg::NonCentralChiFunction& nonCentralChiFunction =
                  static_cast< const gkg::NonCentralChiFunction& >( _function );

    int32_t sampleCount = x.getSize1();
    double mean = nonCentralChiFunction.getApproximateMean();
    double standardDeviation = 
                        nonCentralChiFunction.getApproximateStandardDeviation();
    double range = 6.0 * standardDeviation;    
    double delta = range / (double)sampleCount;

    x( 0, 0 ) = mean - 0.5 * range;
    
    if ( x( 0, 0 ) < 0.0 )
    {
    
      x( 0, 0 ) = 0.0;
    
    }
    
    int32_t k;
    
    for ( k = 1; k < sampleCount; k++ )
    {
    
      x( k, 0 ) = x( k - 1, 0 ) + delta;
    
    }
  
  }
  GKG_CATCH( "void gkg::OptimalNonCentralChiFunctionSampler::get( "
             "gkg::Matrix& x ) const" );

}
