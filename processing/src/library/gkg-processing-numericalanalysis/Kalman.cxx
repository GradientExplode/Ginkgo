#include <gkg-processing-numericalanalysis/Kalman.h>
#include <gkg-core-exception/Exception.h>



#define GKG_INITAL_KALMAN_FILTER_VARIANCE_Of_X   1e7


//
// class Kalman::Parameters
//

gkg::Kalman::Parameters::Parameters( int32_t regressorCount )
                       : X( regressorCount ),
                         varianceOfX( regressorCount, regressorCount )
{

  try
  {

    X.setZero();
    varianceOfX.setDiagonal( GKG_INITAL_KALMAN_FILTER_VARIANCE_Of_X );

    sumOfSquareResiduals = 0.0;
    squareScaleParameter = 0.0;

  }
  GKG_CATCH( "gkg::Kalman::Parameters::Parameters( "
             "int32_t regressorCount )" );

}


gkg::Kalman::Parameters::Parameters( const gkg::Kalman::Parameters& other )
                       : X( other.X ),
                         varianceOfX( other.varianceOfX ),
                         sumOfSquareResiduals( other.sumOfSquareResiduals ),
                         squareScaleParameter( other.squareScaleParameter )
{
}


void gkg::Kalman::Parameters::sanityCheck( int32_t regressorCount ) const
{

  try
  {

    if ( X.getSize() != regressorCount )
    {

      throw std::runtime_error( "bad X size" );

    }

    if ( ( varianceOfX.getSize1() != regressorCount ) ||
         ( varianceOfX.getSize2() != regressorCount ) )
    {

      throw std::runtime_error( "bad viarianceOfX size" );

    }

  }
  GKG_CATCH( "void gkg::Kalman::Parameters::sanityCheck( "
             "int32_t regressorCount ) const" );

}


//
// class Kalman
//

gkg::Kalman::Kalman( gkg::RCPointer< gkg::KFFunction > K )
           : _regressorCount( K->getParameterCount() ),
             _K( K )
{

  try
  {

    if ( _regressorCount > 0 )
    {

      // preallocated also for speeding up processing
      _a.reallocate( _regressorCount );

    }

  }
  GKG_CATCH( "gkg::Kalman::Kalman( int32_t dataCount, "
             "int32_t regressorCount )"  );

}


gkg::Kalman::~Kalman()
{
}


void gkg::Kalman::setSubDesignMatrix( const gkg::Vector& subDesignMatrix )
{

  try
  {

    if ( subDesignMatrix.getSize() != _regressorCount )
    {

      throw std::runtime_error( "bad subDesignMatrix size" );

    }

    _a = subDesignMatrix;
    _K->setParameters( _a );

  }
  GKG_CATCH( "void gkg::Kalman::setSubDesignMatrix( "
             "const gkg::Vector& subDesignMatrix )" );

}


int32_t gkg::Kalman::getRegressorCount() const
{

  return _regressorCount;

}


const gkg::Vector& gkg::Kalman::getSubDesignMatrix() const
{

  return _a;

}


void gkg::Kalman::iterate( gkg::Kalman::Parameters& parameters,
                           double y,
                           int32_t time )
{

  try
  {

    // sanity checks
    parameters.sanityCheck( _regressorCount );

    // get signal estimate
    double yHat = 0.0;
    _K->getValueAt( parameters.X, yHat );

    // processing innovation
    double innovation = y - yHat;

    // processing covariance between the regressor X and the data
    Vector covarianceOfAY = parameters.varianceOfX.getComposition( _a );

    double inverseVarianceOfY = 1.0 /
                                ( _a.getComposition( covarianceOfAY ) + 1.0 );

    update( parameters, innovation, inverseVarianceOfY, covarianceOfAY, time );

  }
  GKG_CATCH( "void gkg::Kalman::iterate( "
             "gkg::Kalman::Parameters& parameters, "
             "double y, "
             "int32_t time )" );

}


void gkg::Kalman::update( gkg::Kalman::Parameters& parameters,
                          double innovation,
                          double inverseVarianceOfY,
                          gkg::Vector& covarianceOfAY,
                          int32_t time )
{

  try
  {

    double product = innovation * inverseVarianceOfY;
    int32_t r1, r2;

    for ( r1 = 0; r1 < _regressorCount; r1++ )
    {

      double covR1 = covarianceOfAY( r1 );
      double varTimesCov = inverseVarianceOfY * covR1;
      parameters.X( r1 ) += product * covR1;

      for ( r2 = 0; r2 < _regressorCount; r2++ )
      {

        parameters.varianceOfX( r1, r2 ) -= varTimesCov * covarianceOfAY( r2 );

      }

    }

    parameters.sumOfSquareResiduals += innovation * product;
    parameters.squareScaleParameter = parameters.sumOfSquareResiduals /
                                      ( ( double )time + 1.0 );

  }
  GKG_CATCH( "void gkg::Kalman::update( gkg::Kalman::Parameters& parameters, "
             "double innovation, double inverseVarianceOfY, "
             "int32_t time )" );

}
