#include <gkg-processing-numericalanalysis/ConstrainedGaussianMixtureFunction.h>
#include <gkg-core-exception/Exception.h>


// Sum of gaussian with positivity constraint on all coefficients


gkg::ConstrainedGaussianMixtureFunction::ConstrainedGaussianMixtureFunction( 
                                                 const gkg::Vector& parameters )
                                       : gkg::LevenbergMarquardtFunction( 
				                                    parameters )
{

  try
  {
  
    if ( parameters.getSize() % 3 )
    {
    
      throw std::runtime_error( "invalid number of parameters" );
    
    }
  
  }
  GKG_CATCH( "gkg::ConstrainedGaussianMixtureFunction::"
             "ConstrainedGaussianMixtureFunction( "
             "const gkg::Vector& parameters )" );

}


double gkg::ConstrainedGaussianMixtureFunction::getValueWithParametersAt(
                                                    const gkg::Vector& x, 
		                                    const gkg::Vector& p ) const
{

  try
  {
  
    int32_t k;
    int32_t n = p.getSize();
    double sum = 0.0;
    double sqrtPi = 1.0 / std::sqrt( M_PI );
    
    double x0 = x( 0 );
    
    for ( k = 0; k < n; k += 3 )
    {
    
      double r = p( k + 2 ) * p( k + 2 );
      double p22 = 1.0 / std::sqrt( 2.0 * r * r );
      double xMinusP1OverP22 = p22 * ( x0 - p( k + 1 ) * p( k + 1 ) );
      sum += p( k ) * p( k ) * sqrtPi * std::exp( -xMinusP1OverP22 * 
                                                   xMinusP1OverP22 ) * p22;

    }			       
   
    return sum;

  }
  GKG_CATCH( "double gkg::ConstrainedGaussianMixtureFunction::"
             "getValueWithParametersAt( "
             "const gkg::Vector& x, const gkg::Vector& p ) const" );

}


gkg::Vector 
gkg::ConstrainedGaussianMixtureFunction::getDerivativeWithParametersAt(
                                                    const gkg::Vector& x, 
                                                    const gkg::Vector& p ) const
{

  try
  {
  
    int32_t k;
    int32_t n = p.getSize();
    double sqrt2Pi = 1.0 / std::sqrt( 2.0 * M_PI );
    gkg::Vector derivative( n );

    double x0 = x( 0 );
            
    for ( k = 0; k < n; k += 3 )
    {
      
      double r = p( k + 2 ) * p( k + 2 );
      double xMinusP1 = x0 - p( k + 1 ) * p( k + 1 );
      double p22 = 1.0 / std::sqrt( r * r );
      double p2 = p22 * p22;
      double xMinusP1OverP22 = p2 * xMinusP1 * xMinusP1;
      double g = sqrt2Pi * p22 * std::exp( -0.5 * xMinusP1OverP22 );
      double pg = p( k ) * p( k ) * g;
    
      derivative( k ) = 2.0 * p( k ) * g;
      derivative( k + 1 ) = 2.0 * p( k + 1 ) * p2 * xMinusP1 * pg;
      derivative( k + 2 ) = 2.0 * pg * ( xMinusP1OverP22 - 1.0 ) / p( k + 2 );
      
    }
    
    return derivative;
    
  }
  GKG_CATCH( "gkg::Vector gkg::ConstrainedGaussianMixtureFunction::"
	     "getDerivativeWithParametersAt( "
             "const gkg::Vector& x, const gkg::Vector& p ) const" );

}
