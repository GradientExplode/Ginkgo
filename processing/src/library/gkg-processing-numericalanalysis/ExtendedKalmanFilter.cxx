#include <gkg-processing-numericalanalysis/ExtendedKalmanFilter.h>
#include <gkg-core-exception/Exception.h>


gkg::ExtendedKalmanFilter::ExtendedKalmanFilter( 
                                           gkg::RCPointer< gkg::KFFunction > K )
                         : gkg::Kalman( K )
		  
{
}


void gkg::ExtendedKalmanFilter::iterate( gkg::Kalman::Parameters& parameters,
                                         double y, 
                                         int32_t t )
{

  try
  {

    // get signal estimate and function jacobian
    double yHat = 0.0;
    Vector J( _regressorCount );

    _K->getValueAt( parameters.X, yHat );
    _K->getJacobian( parameters.X, J );

    // processing innovation
    double innovation = y - yHat;

    // processing covariance between the regressor X and the data
    Vector covarianceOfAY = parameters.varianceOfX.getComposition( J );

    double inverseVarianceOfY = 1.0 /
                                ( J.getComposition( covarianceOfAY ) + 1.0 );

    update( parameters, innovation, inverseVarianceOfY, covarianceOfAY, t );

  }
  GKG_CATCH( "gkg::ExtendedKalmanFilter::iterate( "
             "gkg::Kalman::Parameters& parameters, double y, int32_t t )" );

}
