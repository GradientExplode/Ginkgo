#include <gkg-processing-numericalanalysis/RefinedKalmanFilter.h>
#include <gkg-processing-numericalanalysis/KalmanFilter.h>
#include <gkg-core-exception/Exception.h>




//
// class RefinedKalmanFilter::Parameters
//

gkg::RefinedKalmanFilter::Parameters::Parameters( int32_t regressorCount )
                              : kalmanFilterParameters( regressorCount ),
                                hessianOfSSR( regressorCount, regressorCount ),
                                sumOfPairProducts( 0.0 ),
                                gradientOfSPP( regressorCount ),
                                hessianOfSPP( regressorCount, regressorCount ),
                                autocorrelation( 0.0 )
{

  try
  {

    hessianOfSSR.setZero();
    gradientOfSPP.setZero();
    hessianOfSPP.setZero();

  }
  GKG_CATCH( "gkg::RefinedKalmanFilter::Parameters::Parameters( "
             "int32_t regressorCount )" );

}


gkg::RefinedKalmanFilter::Parameters::Parameters(
                             const gkg::RefinedKalmanFilter::Parameters& other )
                       : kalmanFilterParameters( other.kalmanFilterParameters ),
                         hessianOfSSR( other.hessianOfSSR ),
                         sumOfPairProducts( other.sumOfPairProducts ),
                         gradientOfSPP( other.gradientOfSPP ),
                         hessianOfSPP( other.hessianOfSPP ),
                         autocorrelation( other.autocorrelation )
{
}


void gkg::RefinedKalmanFilter::Parameters::sanityCheck(
                                                  int32_t regressorCount ) const
{

  try
  {

    kalmanFilterParameters.sanityCheck( regressorCount );

    if ( ( hessianOfSSR.getSize1() != regressorCount ) ||
         ( hessianOfSSR.getSize2() != regressorCount ) )
    {

      throw std::runtime_error( "bad hessianOfSSR size" );

    }

    if ( gradientOfSPP.getSize() != regressorCount )
    {

      throw std::runtime_error( "bad gradientOfSPP size" );

    }

    if ( ( hessianOfSPP.getSize1() != regressorCount ) ||
         ( hessianOfSPP.getSize2() != regressorCount ) )
    {

      throw std::runtime_error( "bad hessianOfSPP size" );

    }

  }
  GKG_CATCH( "void gkg::RefinedKalmanFilter::Parameters::sanityCheck( "
             "int32_t regressorCount ) const" );

}


//
// class RefinedKalmanFilter
//

gkg::RefinedKalmanFilter::RefinedKalmanFilter( const gkg::Matrix& A,
                                               int32_t iterationCount )
                         : _kalmanFilter( A ),
                           _iterationCount( iterationCount )
{

  try
  {

    // preallocated for speeding up processing
    _previousA.reallocate( _kalmanFilter.getRegressorCount() );
    _previousX.reallocate( _kalmanFilter.getRegressorCount() );
    _deltaX.reallocate( _kalmanFilter.getRegressorCount() );

  }
  GKG_CATCH( "gkg::RefinedKalmanFilter::RefinedKalmanFilter( "
             "const gkg::Matrix& A )" );

}


gkg::RefinedKalmanFilter::~RefinedKalmanFilter()
{
}


void gkg::RefinedKalmanFilter::iterate(
                               gkg::RefinedKalmanFilter::Parameters& parameters,
                               double y,
                               double previousY,
                               int32_t time )
{

  try
  {

    int32_t regressorCount = _kalmanFilter.getRegressorCount();
    int32_t dataCount = _kalmanFilter.getDataCount();

    // sanity checks
    parameters.sanityCheck( regressorCount );

    if ( time >= dataCount )
    {

      throw std::runtime_error( "there are more iterations than measures" );

    }

    // obtaining a reference to design matrix A and a
    const gkg::Matrix& A = _kalmanFilter.getDesignMatrix();
    const gkg::Vector& a = _kalmanFilter.getSubDesignMatrix();

    // obtaining a reference to Kalman's parameters
    gkg::Kalman::Parameters&
      parametersK = parameters.kalmanFilterParameters;

    // copying previous row of A to previousA
    int32_t r;
    if ( time > 0 )
    {

      for ( r = 0; r < regressorCount; r++ )
      {

        _previousA( r ) = A( time - 1, r );

      }

    }
    else
    {

      _previousA.setZero();

    }

    // storing previous least square estimate
    _previousX = parametersK.X;

    // processing standard Kalman's filter estimate
    _kalmanFilter.iterate( parametersK, y, time );

    // processing least square estimate variation    
    for ( r = 0; r < regressorCount; r++ )
    {

      _deltaX( r ) = parametersK.X( r ) - _previousX( r );

    }

    // processing hessian of the sum of square residuals
    int32_t r1, r2;
    for ( r1 = 0; r1 < regressorCount; r1++ )
    {

      for ( r2 = 0; r2 < regressorCount; r2++ )
      {

        parameters.hessianOfSSR( r1, r2 ) += a( r1 ) * a( r2 );

      }

    }

    // updating bias correction factor
    if ( time != 0 )
    {


      // updating  sum of pair products
      double residual = y - a.getComposition( parametersK.X );
      double previousResidual = previousY - _previousA.getComposition( 
                                                                parametersK.X );
      gkg::Vector v = parameters.hessianOfSPP.getComposition( _deltaX );

      parameters.sumOfPairProducts +=
                      2.0 * parameters.gradientOfSPP.getComposition( _deltaX ) +
                      _deltaX.getComposition( v ) +
                      residual * previousResidual;

      // updating SPP gradient
      for ( r = 0; r < regressorCount; r++ )
      {

        parameters.gradientOfSPP( r ) += v( r )
                                         - 0.5 * previousResidual * a( r ) 
                                         - 0.5 * residual * _previousA( r );

      }

      // updating SPP hessian
      for ( r1 = 0; r1 < regressorCount; r1++ )
      {

        for ( r2 = 0; r2 < regressorCount; r2++ )
        {

          parameters.hessianOfSPP( r1, r2 ) += 0.5 * (
                                               a( r1 ) * _previousA( r2 ) +
                                               _previousA( r1 ) * a( r2 ) );

        }

      }

      // processing bias correction factor
      double correction = ( double )( time + 1 ) / ( double )time; 

      // updating autocorrelation
      parameters.autocorrelation = correction * parameters.sumOfPairProducts /
                                   parametersK.sumOfSquareResiduals;

      // refinement loop during the last iteration
      if ( time == dataCount - 1 )
      {

        int32_t iteration;
        double auxiliary1;
        double auxiliary2;
        for ( iteration = 0; iteration < _iterationCount; iteration++ )
        {

          auxiliary1 = 1.0 / ( 1.0 + parameters.autocorrelation *
                                     parameters.autocorrelation );
          auxiliary2 = 2.0 * correction * parameters.autocorrelation;

          // updating covariance varX = aux1*varX +
          //                            aux1*aux2*varX*hessianSPP*varX
          // which is a simplification of:
          // varX = aux1 * pseudoInv( I - aux1*aux2*varXd*hessianSPP ) * varXd
          gkg::Matrix
            tmpVarX = parametersK.varianceOfX.getComposition(
                                     parameters.hessianOfSPP ).getComposition(
                                                      parametersK.varianceOfX );
          for ( r1 = 0; r1 < regressorCount; r1++ )
          {

            for ( r2 = 0; r2 < regressorCount; r2++ )
            {

              parametersK.varianceOfX( r1, r2 ) =
                                           auxiliary1 *
                                           ( parametersK.varianceOfX( r1, r2 ) +
                                             auxiliary2 * tmpVarX( r1, r2 ) );

            }

          }

          // updating estimate
          _deltaX = parametersK.varianceOfX.getComposition(
                                                     parameters.gradientOfSPP );
          for ( r = 0; r < regressorCount; r++ )
          {

            parametersK.X( r ) += auxiliary2 * _deltaX( r );

          }

          // processing refined sum of pair products:
          // rSPP = SPP + 2*gradientOfSPP._deltaX + deltaX^T.hessianOfSPP.deltaX
          parameters.sumOfPairProducts +=
                 2.0 * parameters.gradientOfSPP.getComposition( _deltaX ) +
                 _deltaX.getComposition( parameters.hessianOfSPP.getComposition(
                                                                    _deltaX ) );

          // processing refined sum of square residuals:
          // rSSR = SSR + deltaX^T.hessianOfSSR.deltaX
          parametersK.sumOfSquareResiduals +=
                 _deltaX.getComposition( parameters.hessianOfSSR.getComposition(
                                                                    _deltaX ) );

          // updating autocorrelation
          parameters.autocorrelation = correction *
                                       parameters.sumOfPairProducts /
                                       parametersK.sumOfSquareResiduals;

          // updating square scale
          parametersK.squareScaleParameter =
             ( 1.0 - parameters.autocorrelation * parameters.autocorrelation ) *
             parametersK.sumOfSquareResiduals / ( ( double )time + 1.0 );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::RefinedKalmanFilter::iterate( "
             "gkg::RefinedKalmanFilter::Parameters& parameters, "
             "double y, "
             "double previousY, "
             "int32_t time )" );

}
