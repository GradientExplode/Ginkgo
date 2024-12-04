#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionNLPFunction.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::MyelinWaterFractionNLPFunction::MyelinWaterFractionNLPFunction(
                          gkg::RCPointer< gkg::MyelinWaterFractionMCMCFunction >
                                               myelinWaterFractionMCMCFunction )
                                    : gkg::NonLinearProgrammingFunction( 
                                         myelinWaterFractionMCMCFunction->
                                                            getParameterCount(),
                                         false ),
                                      _myelinWaterFractionMCMCFunction( 
                                              myelinWaterFractionMCMCFunction ),
                                      _measurementCount( 0 )
{

  try
  {

    // collecting information to speed-up computation
    _t2WeightedVolumeCount =
                   _myelinWaterFractionMCMCFunction->getT2WeightedVolumeCount();
    _t2WeightedCounts = _myelinWaterFractionMCMCFunction->getT2WeightedCounts();
    _globalT2WeightedCount =
                   _myelinWaterFractionMCMCFunction->getGlobalT2WeightedCount();
    _t2NoiseVariances = _myelinWaterFractionMCMCFunction->getT2NoiseVariances();


    _twiceT2NoiseVariances = _t2NoiseVariances;
    std::vector< double >::iterator v = _twiceT2NoiseVariances.begin(),
                                    ve = _twiceT2NoiseVariances.end();
    while ( v != ve )
    {

      *v *= 2.0;
      ++ v;

    }

    _t1WeightedCount = _myelinWaterFractionMCMCFunction->getT1WeightedCount();
    _t1NoiseVariance = _myelinWaterFractionMCMCFunction->getT1NoiseVariance();
    _twiceT1NoiseVariance = 2.0 * _t1NoiseVariance;
    _twoPi = 2.0 * M_PI;

  }
  GKG_CATCH( "gkg::MyelinWaterFractionNLPFunction::"
             "MyelinWaterFractionNLPFunction( "
             "gkg::RCPointer< gkg::MonteCarloMarkovChainFunction > "
             "monteCarloMarkovChainFunction )" );

}


gkg::MyelinWaterFractionNLPFunction::~MyelinWaterFractionNLPFunction()
{
}


void gkg::MyelinWaterFractionNLPFunction::setRealMeasurements(
                                           const gkg::Vector& realMeasurements )
{

  try
  {


    _realMeasurements = realMeasurements;    
    _measurementCount = _realMeasurements.getSize();
    if ( _currentMeasurements.getSize() != _measurementCount )
    {

      _currentMeasurements.reallocate( _measurementCount );

    }

  }
  GKG_CATCH( "void gkg::MyelinWaterFractionNLPFunction::setRealMeasurements( "
             "const gkg::Vector& realMeasurements )" );

}


double gkg::MyelinWaterFractionNLPFunction::getValueAt( 
                                           const gkg::Vector& parameters ) const
{

  try
  {

    _myelinWaterFractionMCMCFunction->getValuesAt( parameters,
                                                   _currentMeasurements );

    double L = -this->getLogLikelihood( _currentMeasurements );

    return L;

  }
  GKG_CATCH( "double gkg::MyelinWaterFractionNLPFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


double gkg::MyelinWaterFractionNLPFunction::getLogLikelihood( 
                                  const gkg::Vector& currentMeasurements ) const
{

  try
  {

    _currentMeasurements = currentMeasurements;

    double logLikelihood = 0.0;

    const double* currentMeasurementPointer = &_currentMeasurements( 0 );
    const double* realMeasurementPointer = &_realMeasurements( 0 );


    int32_t t2WeightedVolumeIndex = 0;
    int32_t t2Index = 0;
    for ( t2WeightedVolumeIndex = 0;
          t2WeightedVolumeIndex < _t2WeightedVolumeCount;
          t2WeightedVolumeIndex++ )
    {

      for ( t2Index = 0; t2Index < _t2WeightedCounts[ t2WeightedVolumeIndex ];
            t2Index++ )
      {

        logLikelihood += -std::log( std::sqrt( _t2NoiseVariances[
                                                     t2WeightedVolumeIndex ] ) *
                                     std::sqrt( _twoPi ) ) -
                          ( ( *currentMeasurementPointer -
                              *realMeasurementPointer ) *
                            ( *currentMeasurementPointer -
                              *realMeasurementPointer ) ) /
                          ( 2.0 * _t2NoiseVariances[ t2WeightedVolumeIndex ] );

        currentMeasurementPointer ++;
        realMeasurementPointer ++;

      }
    
    }

    int32_t m = 0;
    for ( m = _globalT2WeightedCount; m < _measurementCount; m++ )
    {

      logLikelihood += -std::log( std::sqrt( _t1NoiseVariance ) *
                                   std::sqrt( _twoPi ) ) -
                        ( ( *currentMeasurementPointer -
                            *realMeasurementPointer ) *
                          ( *currentMeasurementPointer -
                            *realMeasurementPointer ) ) / 
                        ( 2.0 * _t1NoiseVariance );

      currentMeasurementPointer ++;
      realMeasurementPointer ++;
    
    }

    return logLikelihood;

  }
  GKG_CATCH( "double gkg::MyelinWaterFractionNLPFunction::getLogLikelihood( "
             "const gkg::Vector& currentMeasurements ) const" );


}
