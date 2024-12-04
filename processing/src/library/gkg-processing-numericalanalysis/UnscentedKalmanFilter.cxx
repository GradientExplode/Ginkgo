#include <gkg-processing-numericalanalysis/UnscentedKalmanFilter.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


gkg::UnscentedKalmanFilter::UnscentedKalmanFilter( 
                                            gkg::RCPointer< gkg::KFFunction > K,
                                            double k )
                          : gkg::Kalman( K ),
                            _scale( 0.0 )
{

  try
  {

    if ( _regressorCount > 0 )
    {

      double den = double( _regressorCount ) + k;
      int32_t i;

      _sigmaPointCount = 2 * _regressorCount + 1;
      _scale = std::sqrt( den );

      _w.reallocate( _sigmaPointCount );
      _w( 0 ) = k / den;

      for ( i = 1; i < _sigmaPointCount; i++ )
      {

        _w( i ) = 0.5 / den;

      }

    }

  }
  GKG_CATCH( "gkg::UnscentedKalmanFilter::UnscentedKalmanFilter( "
             "int32_t regressorCount, double k )" );

}


void gkg::UnscentedKalmanFilter::iterate( gkg::Kalman::Parameters& parameters,
                                          double y, 
                                          int32_t t )
{

  try
  {

    gkg::Vector xHat( _regressorCount );
    gkg::Vector Y( _sigmaPointCount );
    gkg::Vector wY( _sigmaPointCount );
    gkg::Matrix sp( _regressorCount, _sigmaPointCount );
    int32_t i, j;

    getSigmaPoints( parameters, sp );

    for ( j = 0; j < _regressorCount; j++ )
    {

      xHat( j ) = _w( 0 ) * sp( j, 0 );

      for ( i = 1; i < _sigmaPointCount; i++ )
      {

        xHat( j ) += _w( i ) * sp( j, i );

      }

    }

    _K->getValuesAt( sp, Y );

    for ( j = 0; j < _regressorCount; j++ )
    {

      for ( i = 0; i < _sigmaPointCount; i++ )
      {

        sp( j, i ) -= xHat( j );

      }

    }

    int32_t r1, r2;
    for ( r1 = 0; r1 < _regressorCount; r1++ )
    {

      for ( r2 = 0; r2 < _regressorCount; r2++ )
      {

        double c = 0.0;

        for ( i = 0; i < _sigmaPointCount; i++ )
        {

          c += _w( i ) * sp( r1, i ) * sp( r2, i );

        }

        parameters.varianceOfX( r1, r2 ) = c;

      }

    }

    double yHat = 0.0;

    for ( i = 0; i < _sigmaPointCount; i++ )
    {

      yHat += _w( i ) * Y( i );

    }

    for ( i = 0; i < _sigmaPointCount; i++ )
    {

      Y( i ) -= yHat;
      wY( i ) = _w( i ) * Y( i );

    }

    // processing innovation
    double innovation = y - yHat;

    // processing covariance between the regressor X and the data
    Vector covarianceOfAY = sp.getComposition( wY );

    double inverseVarianceOfY = 1.0 / Y.getComposition( wY );

    update( parameters, innovation, inverseVarianceOfY, covarianceOfAY, t );

  }
  GKG_CATCH( "gkg::UnscentedKalmanFilter::iterate( "
             "gkg::Kalman::Parameters& parameters, double y, int32_t time )" );

}


void gkg::UnscentedKalmanFilter::getSigmaPoints(
                                            gkg::Kalman::Parameters& parameters,
                                            gkg::Matrix& sp )
{

  try
  {

    gkg::Matrix L( _regressorCount, _regressorCount );
    gkg::Matrix& varX = parameters.varianceOfX;
    gkg::Vector& X = parameters.X;
    int32_t i, j, k, l;
    double sum, sl;

    // Cholesky decomposition of the covariance matrix (Doolittle algorithm)
    // varianceOfX = L.L^T
    for ( i = 0, l = _regressorCount + 1; i < _regressorCount; i++, l++ )
    {

      sum = varX( i, i );

      for ( j = i + 1, k = _regressorCount + i + 2;
            j < _regressorCount; 
            j++, k++ )
      {

        sp( i, j + 1 ) = X( i );
        sp( i, k ) = X( i );

      }

      for ( k = i - 1; k >= 0; k-- )
      {

        sum -= L( i, k ) * L( i, k );

      }

      L( i, i ) = std::sqrt( sum );
      sl = _scale * L( i, i );
      sp( i, 0 ) = X( i );
      sp( i, i + 1 ) = X( i ) + sl;
      sp( i, l ) = X( i ) - sl;

      for ( j = i + 1; j < _regressorCount; j++ )
      {

        sum = varX( i, j );

        for ( k = i - 1; k >= 0; k-- )
        {

          sum -= L( i, k ) * L( j, k );

        }

        L( j, i ) = sum / L( i, i );
        sl = _scale * L( j, i );
        sp( j, i + 1 ) = X( j ) + sl;
        sp( j, l ) = X( j ) - sl;

      }

    }

  }
  GKG_CATCH( "void gkg::UnscentedKalmanFilter::getSigmaPoints( "
             "gkg::Kalman::Parameters& parameters )" );

}
