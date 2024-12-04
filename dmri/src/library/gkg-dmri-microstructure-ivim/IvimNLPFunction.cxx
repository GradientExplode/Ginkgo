#include <gkg-dmri-microstructure-ivim/IvimNLPFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <boost/math/tools/precision.hpp>
#include <boost/math/special_functions/detail/bessel_i0.hpp>


gkg::IvimNLPFunction::IvimNLPFunction(
                                      const gkg::Vector& realMeasurements,
                                      double noiseVariance,
                                      const std::vector< float >& shellBValues,
                                      int32_t maximumShellIndex,
                                      double parenchymaDiffusivity )
                     : gkg::NonLinearProgrammingFunction( 2, false ),
                       _realMeasurements( realMeasurements ),
                       _noiseVariance( noiseVariance ),
                       _shellBValues( shellBValues ),
                       _maximumShellIndex( maximumShellIndex ),
                       _parenchymaDiffusivity( parenchymaDiffusivity )
{

  try
  {

    // collecting information to speed-up computation 
    _twiceNoiseVariance = 2.0 * _noiseVariance;
    _twoPi = 2.0 * M_PI;
    _measurementCount = _maximumShellIndex + 1;
    _currentMeasurements.reallocate( _measurementCount );

  }
  GKG_CATCH( "gkg::IvimNLPFunction::IvimNLPFunction( "
             "const gkg::Vector& realMeasurements, "
             "double noiseVariance, "
             "const std::vector< float >& shellBValues, "
             "int32_t maximumShellIndex )" );

}


gkg::IvimNLPFunction::~IvimNLPFunction()
{
}
    

void 
gkg::IvimNLPFunction::getValuesAt( const gkg::Vector& parameters, 
                                   gkg::Vector& values ) const
{

  try
  {

    // parameters( 0 ) -> FIVIM
    // parameters( 1 ) -> IVIM diffusivity D*

    int32_t m = 0;
    for ( m = 0; m < _measurementCount; m++ )
    {


      _currentMeasurements( m ) = parameters( 0 ) *
                                  std::exp( -_shellBValues[ m ] * 1e6 * 
                                            parameters( 1 ) ) +
                                  ( 1.0 - parameters( 0 ) ) *
                                  std::exp( -_shellBValues[ m ] * 1e6 * 
                                            _parenchymaDiffusivity );

      if ( std::isnan( _currentMeasurements( m ) ) )
      {

        std::cout << "----------------->nan" << std::endl;

      }

    }

    values = _currentMeasurements;

  }
  GKG_CATCH( "double gkg::IvimNLPFunction::getValuesAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& values ) const" );

}


double 
gkg::IvimNLPFunction::getValueAt( const gkg::Vector& parameters ) const
{

  try
  {

    this->getValuesAt( parameters, _currentMeasurements );

    double L = -this->getLogLikelihood( _currentMeasurements );

    if ( std::isnan( L ) )
    {

      std::cout << "----------------> L = " << L << std::endl;

    }
    return L;

  }
  GKG_CATCH( "double gkg::IvimNLPFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


double gkg::IvimNLPFunction::getLogLikelihood(
                            const gkg::Vector& /* currentMeasurements */ ) const
{

  try
  {

    double logLikelihood = 0.0;

    const double* currentMeasurementPointer = &_currentMeasurements( 0 );
    const double* realMeasurementPointer = &_realMeasurements( 0 );

    double a = 0.0;

    int32_t m = 0;
    for ( m = 0; m < _measurementCount; m++ )
    {

      a = ( *realMeasurementPointer * *currentMeasurementPointer ) /
            _noiseVariance;

      if ( a < 700.0 )
      {

        logLikelihood += 
                     std::log( *currentMeasurementPointer *
                     boost::math::detail::bessel_i0( a ) /
		     _noiseVariance ) -
                     ( *currentMeasurementPointer * *currentMeasurementPointer +
                       *realMeasurementPointer * *realMeasurementPointer ) /
                     _twiceNoiseVariance;

      }
      else
      {

        logLikelihood += 
                     std::log( *currentMeasurementPointer / _noiseVariance /
		     std::sqrt( _twoPi * a ) ) -
                     ( *currentMeasurementPointer * *currentMeasurementPointer +
                       *realMeasurementPointer * *realMeasurementPointer ) /
                     _twiceNoiseVariance + a;

      }

      currentMeasurementPointer ++;
      realMeasurementPointer ++;

    }

/*
    int32_t m = 0;
    for ( m = 0; m < _measurementCount; m++ )
    {

      logLikelihood += -std::log( std::sqrt( _noiseVariance ) *
                                   std::sqrt( _twoPi ) ) -
                        ( ( *currentMeasurementPointer -
                            *realMeasurementPointer ) *
                          ( *currentMeasurementPointer -
                            *realMeasurementPointer ) ) / 
                        ( 2.0 * _noiseVariance );

      currentMeasurementPointer ++;
      realMeasurementPointer ++;
    
    }
*/
    return logLikelihood;

  }
  GKG_CATCH( "double gkg::IvimNLPFunction::getLogLikelihood( "
             "const gkg::Vector& currentMeasurements ) const" );

}

