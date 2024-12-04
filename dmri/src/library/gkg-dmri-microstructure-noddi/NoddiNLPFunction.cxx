#include <gkg-dmri-microstructure-noddi/NoddiNLPFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <boost/math/tools/precision.hpp>
#include <boost/math/special_functions/detail/bessel_i0.hpp>


gkg::NoddiNLPFunction::NoddiNLPFunction( 
                     gkg::RCPointer< gkg::NoddiMCMCFunction > noddiMCMCFunction,
                     const gkg::Vector& realMeasurements )
                      : gkg::NonLinearProgrammingFunction( 
                                         noddiMCMCFunction->getParameterCount(),
                                         false ),
                        _noddiMCMCFunction( noddiMCMCFunction ),
                        _realMeasurements( realMeasurements )
{

  try
  {

    // getting a pointer to the numerical analysis factory
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    // collecting information to speed-up computation 
    _noiseVariance = _noddiMCMCFunction->getNoiseVariance();
    _twiceNoiseVariance = 2.0 * _noiseVariance;
    _twoPi = 2.0 * M_PI;
    _measurementCount = _realMeasurements.getSize();
    _currentMeasurements.reallocate( _measurementCount );


  }
  GKG_CATCH( "gkg::NoddiNLPFunction::NoddiNLPFunction( "
             "gkg::RCPointer< gkg::NoddiMCMCFunction > noddiMCMCFunction, "
             "const gkg::Vector& realMeasurements)" );

}


gkg::NoddiNLPFunction::~NoddiNLPFunction()
{
}


double 
gkg::NoddiNLPFunction::getValueAt( const gkg::Vector& parameters ) const
{

  try
  {

    _noddiMCMCFunction->getValuesAt( parameters,
                                     _currentMeasurements );
    int32_t i = 0;

    for ( i = 0; i < _currentMeasurements.getSize(); i++ )
    {

      if ( std::isnan( _currentMeasurements( i ) ) )
      {

        std::cout << "----------------->nan" << std::endl;

      }

    }

    double L = -this->getLogLikelihood( _currentMeasurements );

    if ( std::isnan( L ) )
    {

      std::cout << "----------------> L = " << L << std::endl;

    }
    return L;

    //return -this->getLogLikelihood( _currentMeasurements );

  }
  GKG_CATCH( "double "
             "gkg::NoddiNLPWatsonFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}
    

double gkg::NoddiNLPFunction::getLogLikelihood(
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
/*
		     _factory->getRegularBesselFunctionI0( a ) / */
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
  GKG_CATCH( "double gkg::NoddiNLPFunction::getLogLikelihood( "
             "const gkg::Vector& currentMeasurements ) const" );

}

