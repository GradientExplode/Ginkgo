#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryNLPFunction.h>
#include <gkg-core-exception/Exception.h> 
#include <cmath> 


gkg::MultiCompartmentRelaxometryNLPFunction::MultiCompartmentRelaxometryNLPFunction( 
                gkg::RCPointer< gkg::MultiCompartmentRelaxometryMCMCFunction >
                                     multiCompartmentRelaxometryMCMCFunction )
              : gkg::NonLinearProgrammingFunction( 
                 multiCompartmentRelaxometryMCMCFunction->getParameterCount(), 
                 false ), 
                _multiCompartmentRelaxometryMCMCFunction( 
                                    multiCompartmentRelaxometryMCMCFunction ), 
                _measurementCount( 0 )
{
  
  try
  {
  
    //collecting information to speed-up computation
    _t2WeightedVolumeCount = 
           _multiCompartmentRelaxometryMCMCFunction->getT2WeightedVolumeCount();
    _t1WeightedVolumeCount = 
           _multiCompartmentRelaxometryMCMCFunction->getT1WeightedVolumeCount();

    _t2WeightedCounts = 
                _multiCompartmentRelaxometryMCMCFunction->getT2WeightedCounts();
    _t1WeightedCounts = 
                _multiCompartmentRelaxometryMCMCFunction->getT1WeightedCounts();

    _globalT2WeightedCount = 
           _multiCompartmentRelaxometryMCMCFunction->getGlobalT2WeightedCount(); 
    _globalT1WeightedCount = 
           _multiCompartmentRelaxometryMCMCFunction->getGlobalT1WeightedCount(); 

    _t2NoiseVariances = 
                _multiCompartmentRelaxometryMCMCFunction->getT2NoiseVariances();
    _t1NoiseVariances = 
                _multiCompartmentRelaxometryMCMCFunction->getT1NoiseVariances();
    
    
    _twiceT2NoiseVariances = _t2NoiseVariances; 
    std::vector< double >::iterator v = _twiceT2NoiseVariances.begin(),
                                    ve = _twiceT2NoiseVariances.end();
    while ( v != ve )
    {

      *v *= 2.0;
      ++ v;

    }
    
    _twiceT1NoiseVariances = _t1NoiseVariances; 
    v = _twiceT1NoiseVariances.begin();
    ve = _twiceT1NoiseVariances.end();
    while ( v != ve )
    {

      *v *= 2.0;
      ++ v;

    }
    
    _twoPi = 2.0 * M_PI;

  }
  GKG_CATCH( "gkg::MultiCompartmentRelaxometryNLPFunction::"
             "MultiCompartmentRelaxometryNLPFunction( "
             "gkg::RCPointer< gkg::MonteCarloMarkovChainFunction > "
             "monteCarloMarkovChainFunction )" );

}


gkg::MultiCompartmentRelaxometryNLPFunction::~MultiCompartmentRelaxometryNLPFunction()
{
}


void gkg::MultiCompartmentRelaxometryNLPFunction::setRealMeasurements( 
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
  GKG_CATCH( "void gkg::MultiCompartmentRelaxometryNLPFunction::"
             "setRealMeasurements( "
             "const gkg::Vector& realMeasurements )" );

}


double gkg::MultiCompartmentRelaxometryNLPFunction::getValueAt( 
                                           const gkg::Vector& parameters ) const
{
  
  try
  {

    _multiCompartmentRelaxometryMCMCFunction->getValuesAt( parameters, 
                                                         _currentMeasurements );
     
    double L = -this->getLogLikelihood( _currentMeasurements ); 
    
    return L; 
    
  }
  GKG_CATCH( "double gkg::MultiCompartmentRelaxometryNLPFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" ); 

}


double gkg::MultiCompartmentRelaxometryNLPFunction::getLogLikelihood( 
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

      for ( t2Index = 0; 
            t2Index < _t2WeightedCounts[ t2WeightedVolumeIndex ];
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
    
    int32_t t1WeightedVolumeIndex = 0;
    int32_t t1Index = 0;
    
    for ( t1WeightedVolumeIndex = 0;
          t1WeightedVolumeIndex < _t1WeightedVolumeCount;
          t1WeightedVolumeIndex++ )
    {

      for ( t1Index = 0; 
            t1Index < _t1WeightedCounts[ t1WeightedVolumeIndex ];
            t1Index++ )
      {

        logLikelihood += -std::log( std::sqrt( _t1NoiseVariances[
                                                     t1WeightedVolumeIndex ] ) *
                                     std::sqrt( _twoPi ) ) -
                          ( ( *currentMeasurementPointer -
                              *realMeasurementPointer ) *
                            ( *currentMeasurementPointer -
                              *realMeasurementPointer ) ) /
                          ( 2.0 * _t1NoiseVariances[ t1WeightedVolumeIndex ] );

        currentMeasurementPointer ++;
        realMeasurementPointer ++;

      }
    
    }
    
    return logLikelihood; 
    
  }
  GKG_CATCH( "double gkg::MultiCompartmentRelaxometryNLPFunction::"
             "getLoglikelihood( "
             "const gkg::Vector& currentMeasurements ) const " );
}
