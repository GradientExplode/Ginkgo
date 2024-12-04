#include <gkg-processing-numericalanalysis/RiceFunction.h>
#include <gkg-processing-numericalanalysis/OptimalRiceFunctionSampler.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


gkg::RiceFunction::RiceFunction( const gkg::Vector& parameters )
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
  
    _factory =
      gkg::NumericalAnalysisSelector::getInstance(). getImplementationFactory();

    if ( _factory )
    {

      double value = _parameters( 0 );
      double sigma = _parameters( 1 );
      double value2 = value * value;
      double sigma2 = 2.0 * sigma * sigma;
      double v = -value2 / sigma2 ;

      _approximateMean = sigma * std::sqrt( M_PI_2 ) *
                         _factory->getHypergeometricFunction1F1( -0.5, 1, v );
      _approximateStandardDeviation = std::sqrt( sigma2 + value2 - 
                                          _approximateMean * _approximateMean );

    }

  }
  GKG_CATCH( "gkg::RiceFunction::RiceFunction( "
             "const gkg::Vector& parameters )" );
  
}


void gkg::RiceFunction::getValueAt( const gkg::Vector& x, gkg::Vector& y ) const
{

  try
  {
  
    y( 0 ) = getValueAt( x( 0 ) );

  }
  GKG_CATCH( "void gkg::RiceFunction::getValueAt( "
             "const gkg::Vector& x, gkg::Vector& y ) const" );

}


void gkg::RiceFunction::getValuesAt( gkg::Matrix& x, gkg::Matrix& y ) const
{

  try
  {
  
    int32_t k;
    int32_t sampleCount = x.getSize1();
    gkg::OptimalRiceFunctionSampler sampler( *this );
  
    sampler.get( x );
    
    for ( k = 0; k < sampleCount; k++ )
    {
    
      y( k, 0 ) = getValueAt( x( k, 0 ) );
    
    }
  
  }
  GKG_CATCH( "void gkg::RiceFunction::getValuesAt( "
             "gkg::Matrix& x, gkg::Matrix& y )" );

}


double gkg::RiceFunction::getApproximateMean() const
{

  return _approximateMean;
  
}


double gkg::RiceFunction::getApproximateStandardDeviation() const
{

  return _approximateStandardDeviation;

}
  

double gkg::RiceFunction::getValueAt( double x ) const
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
    double sigma2 = sigma * sigma;
    double xOverSigma2 = x / sigma2;

    if ( ( value / sigma ) <= 8.0 )
    {

      double zi = value * xOverSigma2;
      double I0 = _factory->getRegularBesselFunctionI0( zi );
    
      result = xOverSigma2 * I0 * 
               std::exp( -0.5 * ( x * x + value * value ) / sigma2 );

    }
    else
    {

      double s2 = _approximateStandardDeviation * _approximateStandardDeviation;
      double zi = x - _approximateMean;

      result = std::exp( -0.5 * zi * zi / s2 ) / std::sqrt( 2.0 * M_PI * s2 );

    }

    return result;

  }
  GKG_CATCH( "double gkg::RiceFunction::getValueAt( double x ) const" );

}
