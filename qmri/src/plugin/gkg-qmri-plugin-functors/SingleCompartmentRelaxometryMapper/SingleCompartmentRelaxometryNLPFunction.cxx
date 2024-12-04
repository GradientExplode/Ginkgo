#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryNLPFunction.h>
#include <gkg-core-exception/Exception.h> 
#include <cmath> 


gkg::SingleCompartmentRelaxometryNLPFunction::
                                        SingleCompartmentRelaxometryNLPFunction( 
                gkg::RCPointer< gkg::SingleCompartmentRelaxometryMCMCFunction >
                                      singleCompartmentRelaxometryMCMCFunction )
    : gkg::NonLinearProgrammingFunction( 
                  singleCompartmentRelaxometryMCMCFunction->getParameterCount(), 
                  false ), 
      _singleCompartmentRelaxometryMCMCFunction( 
                                     singleCompartmentRelaxometryMCMCFunction )
{
}


gkg::SingleCompartmentRelaxometryNLPFunction::
                                      ~SingleCompartmentRelaxometryNLPFunction()
{
}


void gkg::SingleCompartmentRelaxometryNLPFunction::setRealMeasurements( 
                                           const gkg::Vector& realMeasurements )
{
  
  try
  {
    
    _realMeasurements = realMeasurements; 
    if ( _currentMeasurements.getSize() != _realMeasurements.getSize() )
    {

      _currentMeasurements.reallocate( _realMeasurements.getSize() );

    }

  }
  GKG_CATCH( "void gkg::SingleCompartmentRelaxometryNLPFunction::"
             "setRealMeasurements( "
             "const gkg::Vector& realMeasurements )" );

}


double gkg::SingleCompartmentRelaxometryNLPFunction::getValueAt( 
                                           const gkg::Vector& parameters ) const
{
  
  try
  {

    _singleCompartmentRelaxometryMCMCFunction->getValuesAt(
                                                         parameters, 
                                                         _currentMeasurements );    
    return -this->getLogLikelihood( _currentMeasurements ); 
    
  }
  GKG_CATCH( "double gkg::SingleCompartmentRelaxometryNLPFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" ); 

}


double gkg::SingleCompartmentRelaxometryNLPFunction::getLogLikelihood( 
                                  const gkg::Vector& currentMeasurements ) const
{
  
  try 
  {
    
    _currentMeasurements = currentMeasurements; 
    
    double logLikelihood = 0.0; 
    
    const double* currentMeasurementPointer = &_currentMeasurements( 0 ); 
    const double* realMeasurementPointer = &_realMeasurements( 0 ); 

    const std::vector< int32_t >& measurementCounts = 
              _singleCompartmentRelaxometryMCMCFunction->getMeasurementCounts();
    const std::vector< double >& noiseVariances = 
                 _singleCompartmentRelaxometryMCMCFunction->getNoiseVariances();
    double sqrtTwoPi = std::sqrt( 2.0 * M_PI );

    int32_t inputVolumeCount = ( int32_t )measurementCounts.size();
    int32_t inputVolumeIndex = 0;
    int32_t measurementIndex = 0;

    for ( inputVolumeIndex = 0;
          inputVolumeIndex < inputVolumeCount;
          inputVolumeIndex++ )
    {

      for ( measurementIndex = 0; 
            measurementIndex < measurementCounts[ inputVolumeIndex ];
            measurementIndex++ )
      {

        const double& noiseVariance = noiseVariances[ inputVolumeIndex ];

        logLikelihood += -std::log( std::sqrt( noiseVariance ) * sqrtTwoPi ) -
                         ( ( *currentMeasurementPointer -
                             *realMeasurementPointer ) *
                           ( *currentMeasurementPointer -
                             *realMeasurementPointer ) ) /
                         ( 2.0 * noiseVariance );

        currentMeasurementPointer ++;
        realMeasurementPointer ++;

      }
    
    }
    
    return logLikelihood; 
    
  }
  GKG_CATCH( "double gkg::SingleCompartmentRelaxometryNLPFunction::"
             "getLoglikelihood( "
             "const gkg::Vector& currentMeasurements ) const " );
}
