#include <gkg-processing-numericalanalysis/ParallelKalmanFilter.h>
#include <gkg-core-exception/Exception.h>


gkg::ParallelKalmanFilter::ParallelKalmanFilter( const gkg::Matrix& A,
                                                 int32_t gaussianCount )
                         : gkg::KalmanFilter( A ),
			   _gaussianCount( gaussianCount )
{

  try
  {

    // preallocated for speeding up processing
    _y.reallocate( _gaussianCount );
    _covariance.reallocate( _gaussianCount );
    _weight.reallocate( _gaussianCount );

  }
  GKG_CATCH( "gkg::ParallelKalmanFilter::ParallelKalmanFilter( "
             "const gkg::Matrix& A )" );

}


void gkg::ParallelKalmanFilter::iterate( gkg::Kalman::Parameters& parameters,
				         gkg::Vector& gaussianMixtureNoise,
                                         double y,
                                         int32_t time )
{

  try
  {

    // sanity checks
    parameters.sanityCheck( _regressorCount );

    if ( time >= _dataCount )
    {

      throw std::runtime_error( "there are more iterations than measures" );

    }

    if ( ( gaussianMixtureNoise.getSize() / 3 ) != _gaussianCount )
    {

      throw std::runtime_error( "bad number of parameters" );

    }

    // copying row time of A to 
    int32_t r;
    for ( r = 0; r < _regressorCount; r++ )
    {

      _a( r ) = _A( time, r );

    }
    
    // processing covariance between the regressor X and the data
    Vector covarianceOfAY = parameters.varianceOfX.getComposition( _a );
    _varianceOfY = _a.getComposition( covarianceOfAY );

    // processing channels
    int32_t c;
    double sumOfWeights = 0.0;
    for ( c = 0; c < _gaussianCount; c++ )
    {
    
      double yc = _a.getComposition( parameters.X ) + 
                  gaussianMixtureNoise( 3 * c + 1 );
      _y( c ) = yc;
		
      double sigma = gaussianMixtureNoise( 3 * c + 2 );
      double pm = _varianceOfY + sigma * sigma;
      _covariance( c ) = pm;
      
      double yMinusyc = y - yc;
      double w = gaussianMixtureNoise( 3 * c ) * 
                 std::exp( -0.5 * yMinusyc * yMinusyc / pm ) /
		 std::sqrt( 2.0 * M_PI * pm );
		 
      sumOfWeights += w;
      _weight( c ) = w;
    
    }
    
    double estimatedY = 0.0;
    for ( c = 0; c < _gaussianCount; c++ )
    {
    
      _weight( c ) /= sumOfWeights;
      estimatedY += _weight( c ) * _y( c );
    
    }
    
    double covariance = 0.0;
    for ( c = 0; c < _gaussianCount; c++ )
    {
    
      double estimatedYMinusyc = estimatedY - _y( c );
      covariance += _weight( c ) * ( _covariance( c ) + 
                                     estimatedYMinusyc * estimatedYMinusyc );
    
    }

    // processing innovation
    double innovation = y - estimatedY;
    
    for ( r = 0; r < _regressorCount; r++ )
    {

      parameters.X( r ) += innovation * covarianceOfAY( r ) / covariance;

    }

    int r1, r2;
    for ( r1 = 0; r1 < _regressorCount; r1++ )
    {

      for ( r2 = 0; r2 < _regressorCount; r2++ )
      {

        parameters.varianceOfX( r1, r2 ) -= covarianceOfAY( r1 ) *
                                            covarianceOfAY( r2 ) /
					    covariance;

      }

    }

  }
  GKG_CATCH( "void gkg::ParallelKalmanFilter::iterate( "
             "gkg::Kalman::Parameters& parameters, "
	     "gkg::Vector& gaussianMixtureNoise, "
             "double y, "
             "int32_t time )" );

}
