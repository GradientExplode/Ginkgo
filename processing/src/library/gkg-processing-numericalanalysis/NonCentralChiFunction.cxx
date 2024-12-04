#include <gkg-processing-numericalanalysis/NonCentralChiFunction.h>
#include <gkg-processing-numericalanalysis/OptimalNonCentralChiFunctionSampler.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


#define EPSILON 1e-03


gkg::NonCentralChiFunction::NonCentralChiFunction( 
                                                 const gkg::Vector& parameters )
                          : gkg::ParametricFunction( parameters ),
                            _approximateMean( 0.0 ),
                            _approximateStandardDeviation( 1.0 )
{

  try
  {
  
    if ( _parameters( 0 ) < 0.0 )
    {
    
      throw std::runtime_error( "parameter value must be positive" );
    
    }
  
    if ( _parameters( 1 ) < 0.0 )
    {
    
      throw std::runtime_error( "parameter sigma must be positive" );
    
    }
  
    if ( _parameters( 2 ) < 0.0 )
    {
    
      throw std::runtime_error( "parameter channelCount must be positive" );
    
    }
  
    _factory =
      gkg::NumericalAnalysisSelector::getInstance(). getImplementationFactory();

    if ( _factory )
    {

      double value = _parameters( 0 );
      double sigma = _parameters( 1 );
      double n = _parameters( 2 );
      double value2 = value * value;
      double sigma2 = 2.0 * sigma * sigma;
      double v = -value2 / sigma2;
    
      _approximateMean = _factory->getPochhammer( n, 0.5 ) * M_SQRT2 * sigma *
                         _factory->getHypergeometricFunction1F1( -0.5, n, v );
      _approximateStandardDeviation = std::sqrt( n * sigma2 + value2 - 
                                          _approximateMean * _approximateMean );


    }
  
  }
  GKG_CATCH( "gkg::NonCentralChiFunction::NonCentralChiFunction( "
             "const gkg::Vector& parameters )" );
  
}


void gkg::NonCentralChiFunction::getValueAt( const gkg::Vector& x, 
                                             gkg::Vector& y ) const
{

  try
  {
  
    y( 0 ) = getValueAt( x( 0 ) );

  }
  GKG_CATCH( "void gkg::NonCentralChiFunction::getValueAt( "
             "const gkg::Vector& x, gkg::Vector& y ) const" );

}


void gkg::NonCentralChiFunction::getValuesAt( gkg::Matrix& x, 
                                              gkg::Matrix& y ) const
{

  try
  {
  
    int32_t k;
    int32_t sampleCount = x.getSize1();
    gkg::OptimalNonCentralChiFunctionSampler sampler( *this );
  
    sampler.get( x );
    
    for ( k = 0; k < sampleCount; k++ )
    {
    
      y( k, 0 ) = getValueAt( x( k, 0 ) );
    
    }
  
  }
  GKG_CATCH( "void gkg::NonCentralChiFunction::getValuesAt( "
             "gkg::Matrix& x, gkg::Matrix& y )" );

}


double gkg::NonCentralChiFunction::getApproximateMean() const
{

  return _approximateMean;
  
}


double gkg::NonCentralChiFunction::getApproximateStandardDeviation() const
{

  return _approximateStandardDeviation;

}


double gkg::NonCentralChiFunction::getValueAt( double x ) const
{

  try
  {
  
    if ( !_factory )
    {
    
      throw std::runtime_error( "numerical analysis implementation not found" );
    
    }
    
    double result = 0.0;
    double value = _parameters( 0 );
    double sigma = _parameters( 1 );
    double n = _parameters( 2 );
    double sigma2 = sigma * sigma;
    double xOverSigma2 = x / sigma2;
    double nMinus1 = n - 1.0;

    if ( ( value / sigma ) <= 8.0 )
    {

      if ( value > EPSILON )
      {

        double zi = value * xOverSigma2;

        if ( n > 1.0 )
        {

          xOverSigma2 *= std::pow( x / value, nMinus1 );

        }

        double In = _factory->getRegularBesselFunctionIn( (int32_t)nMinus1,
                                                          zi );
    
        result = xOverSigma2 * In * 
                 std::exp( -0.5 * ( x * x + value * value ) / sigma2 );

      }
      else
      {

        if ( n > 1.0 )
        {

          xOverSigma2 *= std::pow( 0.5 * x * xOverSigma2, nMinus1 ) /
                         _factory->getGammaFunction( n );

        }

        result = xOverSigma2 * std::exp( -0.5 * x * x / sigma2 );

      }

    }
    else
    {

      double s2 = _approximateStandardDeviation * _approximateStandardDeviation;
      double zi = x - _approximateMean;

      result = std::exp( -0.5 * zi * zi / s2 ) / std::sqrt( 2.0 * M_PI * s2 );

    }

    return result;

  }
  GKG_CATCH( "double gkg::NonCentralChiFunction::getValueAt( "
             "double x ) const" );

}
