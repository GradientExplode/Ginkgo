#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunction.h>
#include <gkg-core-exception/Exception.h> 
#include <cmath>


gkg::SingleCompartmentRelaxometryMCMCFunction::
                                       SingleCompartmentRelaxometryMCMCFunction(
                          const std::vector< int32_t >& measurementCounts,
                          const std::vector< double >& noiseStandardDeviations )
    : gkg::MonteCarloMarkovChainFunction(),
      _measurementCounts( measurementCounts ),
      _noiseStandardDeviations( noiseStandardDeviations ),
      _parameterCount( 2 + ( int32_t )measurementCounts.size() - 1 )
{

  try
  {

    int32_t inputVolumeCount = ( int32_t )_measurementCounts.size();
    if ( ( int32_t )_noiseStandardDeviations.size() != inputVolumeCount )
    {

      throw std::runtime_error(
           "noise standard deviation count does not match input volume count" );

    }
    _noiseVariances.resize( inputVolumeCount );

    int32_t inputVolumeIndex = 0;
    for ( inputVolumeIndex = 0;
          inputVolumeIndex < inputVolumeCount;
          inputVolumeIndex++ )
    {

      _noiseVariances[ inputVolumeIndex ] = 
                                  _noiseStandardDeviations[ inputVolumeIndex ] *
                                  _noiseStandardDeviations[ inputVolumeIndex ];

    }

  }
  GKG_CATCH( "gkg::SingleCompartmentRelaxometryMCMCFunction:: "
             "SingleCompartmentRelaxometryMCMCFunction( "
             "const std::vector< int32_t >& measurementCounts, "
             "const std::double< double >& noiseStandardDeviations )" );


}


gkg::SingleCompartmentRelaxometryMCMCFunction::
                                     ~SingleCompartmentRelaxometryMCMCFunction()
{
}


double gkg::SingleCompartmentRelaxometryMCMCFunction::getLikelihoodRatio( 
                                      const gkg::Vector& realMeasurements, 
                                      const gkg::Vector& currentMeasurements, 
                                      const gkg::Vector& newMeasurements ) const
{

  try
  {
    
    int32_t inputVolumeCount = ( int32_t )_measurementCounts.size();

    // Gaussian case     
    double logLikelihood1 = 0.0;
    double logLikelihood2 = 0.0;
     
    int32_t m = 0;
    
    int32_t measurementIndex = 0;
    int32_t inputVolumeIndex = 0;
    for ( inputVolumeIndex = 0;
          inputVolumeIndex < inputVolumeCount;
          inputVolumeIndex++ )
    {

      for ( measurementIndex = 0;
            measurementIndex < _measurementCounts[ inputVolumeIndex ]; 
            measurementIndex++ )
      {
 
        logLikelihood1 += -std::log( _noiseStandardDeviations[
                                                            inputVolumeIndex ] *
                                     std::sqrt( 2 * M_PI ) ) -
                          ( ( currentMeasurements( m ) -
                              realMeasurements( m ) ) *
                            ( currentMeasurements( m ) -
                              realMeasurements( m ) ) ) /
                          ( 2.0 * _noiseVariances[ inputVolumeIndex ] );
 
        logLikelihood2 += -std::log( _noiseStandardDeviations[
                                                            inputVolumeIndex ] *
                                     std::sqrt( 2 * M_PI ) ) -
                          ( ( newMeasurements( m ) -
                              realMeasurements( m ) ) *
                            ( newMeasurements( m ) -
                              realMeasurements( m ) ) ) /
                          ( 2.0 * _noiseVariances[ inputVolumeIndex ] );

        ++ m;
 
      }

    }
    
    return std::exp( logLikelihood2 - logLikelihood1 ); 

  }
  GKG_CATCH( "double gkg::SingleCompartmentRelaxometryMCMCFunction::"
             "getLikelihoodRation( "
             "const gkg::Vector& realMeasurements, "
             "const gkg::Vector& currentMeasurements, "
             "const gkg::Vector& newMeasurements ) const" );

}


const std::vector< int32_t >&
gkg::SingleCompartmentRelaxometryMCMCFunction::getMeasurementCounts() const
{

  try
  {

    return _measurementCounts;

  }
  GKG_CATCH( "const std::vector< int32_t >& "
             "gkg::SingleCompartmentRelaxometryMCMCFunction::"
             "getMeasurementCounts() const" );

}


int32_t
gkg::SingleCompartmentRelaxometryMCMCFunction::getMeasurementCount(
                                                int32_t inputVolumeIndex ) const
{

  try
  {

    return _measurementCounts[ inputVolumeIndex ];

  }
  GKG_CATCH( "int32_t "
             "gkg::SingleCompartmentRelaxometryMCMCFunction::"
             "getMeasurementCount( "
             "int32_t inputVolumeIndex ) const" );

}


const std::vector< double >& 
gkg::SingleCompartmentRelaxometryMCMCFunction::
                                              getNoiseStandardDeviations() const
{

  try
  {

    return _noiseStandardDeviations;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::SingleCompartmentRelaxometryMCMCFunction::"
             "getNoiseStandardDeviations() const" );

}


double
gkg::SingleCompartmentRelaxometryMCMCFunction::
                     getNoiseStandardDeviation( int32_t inputVolumeIndex ) const
{

  try
  {

    return _noiseStandardDeviations[ inputVolumeIndex ];

  }
  GKG_CATCH( "double "
             "gkg::SingleCompartmentRelaxometryMCMCFunction::"
             "getNoiseStandardDeviation( "
             "int32_t inputVolumeIndex ) const" );

}


const std::vector< double >& 
gkg::SingleCompartmentRelaxometryMCMCFunction::getNoiseVariances() const
{

  try
  {

    return _noiseVariances;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::SingleCompartmentRelaxometryMCMCFunction::"
             "getNoiseVariancess() const" );

}


double
gkg::SingleCompartmentRelaxometryMCMCFunction::
                              getNoiseVariance( int32_t inputVolumeIndex ) const
{

  try
  {

    return _noiseVariances[ inputVolumeIndex ];

  }
  GKG_CATCH( "double "
             "gkg::SingleCompartmentRelaxometryMCMCFunction::"
             "getNoiseVariance( "
             "int32_t inputVolumeIndex ) const" );

}


int32_t 
gkg::SingleCompartmentRelaxometryMCMCFunction::getParameterCount() const
{

  try
  {

    return _parameterCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::SingleCompartmentRelaxometryMCMCFunction::"
             "getParameterCount() const" );

}


void gkg::SingleCompartmentRelaxometryMCMCFunction::setSite(
                                          const gkg::Vector3d< int32_t >& site )
{

  try
  {

    _site = site;

  }
  GKG_CATCH( "void gkg::SingleCompartmentRelaxometryMCMCFunction::setSite( "
             "const gkg::Vector3d< int32_t >& site )" );


}
