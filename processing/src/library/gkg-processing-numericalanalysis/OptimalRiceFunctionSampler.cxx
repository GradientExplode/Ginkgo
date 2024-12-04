#include <gkg-processing-numericalanalysis/OptimalRiceFunctionSampler.h>
#include <gkg-processing-numericalanalysis/RiceFunction.h>
#include <gkg-core-exception/Exception.h>


gkg::OptimalRiceFunctionSampler::OptimalRiceFunctionSampler( 
                                       const gkg::ParametricFunction& function )
                               : OptimalParametricFunctionSampler( function )
{
}


void gkg::OptimalRiceFunctionSampler::get( gkg::Matrix& x ) const
{

  try
  {
  
    const gkg::RiceFunction& riceFunction = 
                           static_cast< const gkg::RiceFunction& >( _function );
  
    int32_t sampleCount = x.getSize1();
    double mean = riceFunction.getApproximateMean();
    double standardDeviation = riceFunction.getApproximateStandardDeviation();
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
  GKG_CATCH( "void gkg::OptimalRiceFunctionSampler::get( "
             "gkg::Matrix& x ) const" );

}
