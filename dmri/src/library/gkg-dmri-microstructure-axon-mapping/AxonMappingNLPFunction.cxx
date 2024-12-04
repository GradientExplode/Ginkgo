#include <gkg-dmri-microstructure-axon-mapping/AxonMappingNLPFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::AxonMappingNLPFunction::AxonMappingNLPFunction( 
         gkg::RCPointer< gkg::AxonMappingMCMCFunction > axonMappingMCMCFunction,
         const gkg::Vector& realMeasurements )
                            : gkg::NonLinearProgrammingFunction( 
                                   axonMappingMCMCFunction->getParameterCount(),
                                   false ),
                              _axonMappingMCMCFunction(
                                                      axonMappingMCMCFunction ),
                              _realMeasurements( realMeasurements )
{

  try
  {

    // getting a pointer to the numerical analysis factory
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    // collecting information to speed-up computation 
    _noiseVariance = _axonMappingMCMCFunction->getNoiseVariance();
    _twiceNoiseVariance = 2.0 * _noiseVariance;
    _twoPi = 2.0 * M_PI;
    _measurementCount = _realMeasurements.getSize();
    _currentMeasurements.reallocate( _measurementCount );


  }
  GKG_CATCH( "gkg::AxonMappingNLPFunction::AxonMappingNLPFunction( "
             "gkg::RCPointer< gkg::AxonMappingMCMCFunction > "
             "axonMappingMCMCFunction, "
             "const gkg::Vector& realMeasurements)" );

}


gkg::AxonMappingNLPFunction::~AxonMappingNLPFunction()
{
}


double 
gkg::AxonMappingNLPFunction::getValueAt( const gkg::Vector& parameters ) const
{

  try
  {

    _axonMappingMCMCFunction->getValuesAt( parameters,
                                           _currentMeasurements );

    return -this->getLogLikelihood( _currentMeasurements );

  }
  GKG_CATCH( "double "
             "gkg::AxonMappingNLPWatsonFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}
    

double gkg::AxonMappingNLPFunction::getLogLikelihood(
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
		     _factory->getRegularBesselFunctionI0( a ) / 
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

    return logLikelihood;

  }
  GKG_CATCH( "double gkg::AxonMappingNLPFunction::getLogLikelihood( "
             "const gkg::Vector& currentMeasurements ) const" );

}

