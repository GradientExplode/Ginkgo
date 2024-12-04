#include <gkg-processing-numericalanalysis/KalmanFilter.h>
#include <gkg-core-exception/Exception.h>


gkg::KalmanFilter::KalmanFilter( const gkg::Matrix& A )
                 : gkg::Kalman( gkg::RCPointer< gkg::KFFunction >( 
                                        new gkg::KFFunction( A.getSize2() ) ) ),
                   _dataCount( A.getSize1() ),
                   _A( A )
{
}


int32_t gkg::KalmanFilter::getDataCount() const
{

  return _dataCount;

}


const gkg::Matrix& gkg::KalmanFilter::getDesignMatrix() const
{

  return _A;

}


void gkg::KalmanFilter::setSubDesignMatrix( int32_t time )
{

  try
  {

    if ( time >= _dataCount )
    {

      throw std::runtime_error( "there are more iterations than measures" );

    }

    // copying row time of A to a
    int32_t r;
    for ( r = 0; r < _regressorCount; r++ )
    {

      _a( r ) = _A( time, r );

    }

    _K->setParameters( _a );

  }
  GKG_CATCH( "void gkg::KalmanFilter::setSubDesignMatrix( int32_t time )" );

}


void gkg::KalmanFilter::iterate( gkg::Kalman::Parameters& parameters,
                                 double y, 
                                 int32_t time,
                                 bool doUpdate )
{

  try
  {

    // when KalmanFilter is used in a multi-threaded process, doUpdate must
    // be false. The sub-design matrix have to be updated before the threads
    // are launch.
    // For example:
    // 1) No threads:
    //   KalmanFilter kf( B );
    //   kf.iterate( param, y, time )
    // 2) Multi-threaded:
    //   KalmanFilter kf( B );
    //   kf.setSubDesignMatrix( time )
    //   ThreadContext ctxt( kf, y, kalmanLut, time );
    //   ...
    //   // in the thread context
    //   ...
    //   _kf.iterate( param, y, time, false );
    if ( doUpdate )
    {

      if ( time >= _dataCount )
      {

        throw std::runtime_error( "there are more iterations than measures" );

      }

      // copying row time of A to a
      int32_t r;
      for ( r = 0; r < _regressorCount; r++ )
      {

        _a( r ) = _A( time, r );

      }

      _K->setParameters( _a );

    }

    gkg::Kalman::iterate( parameters, y, time );

  }
  GKG_CATCH( "void gkg::KalmanFilter::iterate( "
             "gkg::Kalman::Parameters& parameters, double y, int32_t time )" );

}
