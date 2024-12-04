#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionMCMCFunction.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::MyelinWaterFractionMCMCFunction::MyelinWaterFractionMCMCFunction(
                         const std::vector< gkg::Vector >& echoTimes,
                         const std::vector< double >& t2NoiseStandardDeviations,
                         int32_t t1WeightedCount,
                         double t1NoiseStandardDeviation )
                                     : gkg::MonteCarloMarkovChainFunction(),
                                       _t2NoiseStandardDeviations(
                                                    t2NoiseStandardDeviations ),
                                       _t1WeightedCount( t1WeightedCount ),
                                       _t1NoiseStandardDeviation(
                                                     t1NoiseStandardDeviation ),
                                       _t1NoiseVariance(
                                                     t1NoiseStandardDeviation *
                                                     t1NoiseStandardDeviation )
{

  try
  {

    _t2WeightedVolumeCount = ( int32_t )echoTimes.size();
    _t2WeightedCounts.resize( _t2WeightedVolumeCount );
    _t2NoiseVariances.resize( _t2WeightedVolumeCount );
    _globalT2WeightedCount = 0;

    int32_t t2WeightedVolumeIndex = 0;
    for ( t2WeightedVolumeIndex = 0;
          t2WeightedVolumeIndex < _t2WeightedVolumeCount;
          t2WeightedVolumeIndex++ )
    {

      _t2WeightedCounts[ t2WeightedVolumeIndex ] =
                                   echoTimes[ t2WeightedVolumeIndex ].getSize();
      _globalT2WeightedCount += _t2WeightedCounts[ t2WeightedVolumeIndex ];
      _t2NoiseVariances[ t2WeightedVolumeIndex ] = 
                           _t2NoiseStandardDeviations[ t2WeightedVolumeIndex ] *
                           _t2NoiseStandardDeviations[ t2WeightedVolumeIndex ];

    }

  }
  GKG_CATCH( "gkg::MyelinWaterFractionMCMCFunction::"
             "MyelinWaterFractionMCMCFunction( "
             "const std::vector< gkg::Vector >& echoTimes, "
             "const std::vector< double >& t2NoiseStandardDeviations, "
             "int32_t t1WeightedCount, "
             "double t1NoiseStandardDeviation )" );

}


gkg::MyelinWaterFractionMCMCFunction::~MyelinWaterFractionMCMCFunction()
{
}


double gkg::MyelinWaterFractionMCMCFunction::getLikelihoodRatio(
                                      const gkg::Vector& realMeasurements,
                                      const gkg::Vector& currentMeasurements,
                                      const gkg::Vector& newMeasurements ) const
{

  try
  {

    // Gaussian case
     
    double logLikelihood1 = 0.0;
    double logLikelihood2 = 0.0;
     
    int32_t m = 0;
    int32_t measurementCount = realMeasurements.getSize();

    int32_t t2Index = 0;
    int32_t t2WeightedVolumeIndex = 0;
    for ( t2WeightedVolumeIndex = 0;
          t2WeightedVolumeIndex < _t2WeightedVolumeCount;
          t2WeightedVolumeIndex++ )
    {


      for ( t2Index = 0; t2Index < _t2WeightedCounts[ t2WeightedVolumeIndex ]; 
            t2Index++ )
      {
 
        logLikelihood1 += -std::log( _t2NoiseStandardDeviations[
                                                       t2WeightedVolumeIndex ] *
                                     std::sqrt( 2 * M_PI ) ) -
                          ( ( currentMeasurements( m ) -
                              realMeasurements( m ) ) *
                            ( currentMeasurements( m ) -
                              realMeasurements( m ) ) ) /
                          ( 2.0 * _t2NoiseVariances[ t2WeightedVolumeIndex ] );
 
        logLikelihood2 += -std::log( _t2NoiseStandardDeviations[
                                                       t2WeightedVolumeIndex ] *
                                     std::sqrt( 2 * M_PI ) ) -
                          ( ( newMeasurements( m ) -
                              realMeasurements( m ) ) *
                            ( newMeasurements( m ) -
                              realMeasurements( m ) ) ) /
                          ( 2.0 * _t2NoiseVariances[ t2WeightedVolumeIndex ] );

        ++ m;
 
      }

    }

    for ( m = _globalT2WeightedCount; m < measurementCount; m++ )
    {
    
      logLikelihood1 += -std::log( _t1NoiseStandardDeviation *
                                   std::sqrt( 2 * M_PI ) ) -
                        ( ( currentMeasurements( m ) -
                            realMeasurements( m ) ) *
                          ( currentMeasurements( m ) -
                            realMeasurements( m ) ) ) / 
                        ( 2.0 * _t1NoiseVariance );
    
      logLikelihood2 += -std::log( _t1NoiseStandardDeviation *
                                   std::sqrt( 2 * M_PI ) ) -
                        ( ( newMeasurements( m ) -
                            realMeasurements( m ) ) *
                          ( newMeasurements( m ) -
                            realMeasurements( m ) ) ) / 
                        ( 2.0 * _t1NoiseVariance );
    
    }
    
    return std::exp( logLikelihood2 - logLikelihood1 );

  }
  GKG_CATCH( "double gkg::MyelinWaterFractionMCMCFunction::"
             "getLikelihoodRatio( "
             "const gkg::Vector& realMeasurements, "
             "const gkg::Vector& currentMeasurements, "
             "const gkg::Vector& newMeasurements ) const" );

}


int32_t gkg::MyelinWaterFractionMCMCFunction::getT2WeightedVolumeCount() const
{

  try
  {

    return _t2WeightedVolumeCount;

  }
  GKG_CATCH( "int32_t gkg::MyelinWaterFractionMCMCFunction::"
             "getT2WeightedVolumeCount() const" );

}


const std::vector< int32_t >&
 gkg::MyelinWaterFractionMCMCFunction::getT2WeightedCounts() const
{

  try
  {

    return _t2WeightedCounts;

  }
  GKG_CATCH( "const std::vector< int32_t >& "
             "gkg::MyelinWaterFractionMCMCFunction::"
             "getT2WeightedCount() const" );

}


int32_t gkg::MyelinWaterFractionMCMCFunction::getT2WeightedCount(
                                           int32_t t2WeightedVolumeIndex ) const
{

  try
  {

    return _t2WeightedCounts[ t2WeightedVolumeIndex ];

  }
  GKG_CATCH( "int32_t gkg::MyelinWaterFractionMCMCFunction::"
             "getT2WeightedCount( int32_t t2WeightedVolumeIndex ) const" );

}


int32_t 
gkg::MyelinWaterFractionMCMCFunction::getGlobalT2WeightedCount() const
{

  try
  {

    return _globalT2WeightedCount;

  }
  GKG_CATCH( "int32_t gkg::MyelinWaterFractionMCMCFunction::"
             "getGlobalT2WeightedCount() const" );

}


const std::vector< double >& 
gkg::MyelinWaterFractionMCMCFunction::getT2NoiseStandardDeviations() const
{

  try
  {

    return _t2NoiseStandardDeviations;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::MyelinWaterFractionMCMCFunction::"
             "getT2NoiseStandardDeviations() const" );

}


double gkg::MyelinWaterFractionMCMCFunction::getT2NoiseStandardDeviation(
                                           int32_t t2WeightedVolumeIndex ) const
{

  try
  {

    return _t2NoiseStandardDeviations[ t2WeightedVolumeIndex ];

  }
  GKG_CATCH( "double gkg::MyelinWaterFractionMCMCFunction::"
             "getT2NoiseStandardDeviation( "
             "int32_t t2WeightedVolumeIndex ) const" );

}


const std::vector< double >& 
gkg::MyelinWaterFractionMCMCFunction::getT2NoiseVariances() const
{

  try
  {

    return _t2NoiseVariances;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::MyelinWaterFractionMCMCFunction::"
             "getT2NoiseVariances() const" );

}


double gkg::MyelinWaterFractionMCMCFunction::getT2NoiseVariance(
                                           int32_t t2WeightedVolumeIndex ) const
{

  try
  {

    return _t2NoiseVariances[ t2WeightedVolumeIndex ];

  }
  GKG_CATCH( "double gkg::MyelinWaterFractionMCMCFunction::"
             "getT2NoiseVariance( "
             "int32_t t2WeightedVolumeIndex ) const" );

}


int32_t gkg::MyelinWaterFractionMCMCFunction::getT1WeightedCount() const
{

  try
  {

    return _t1WeightedCount;

  }
  GKG_CATCH( "int32_t gkg::MyelinWaterFractionMCMCFunction::"
             "getT1WeightedCount() const" );

}


double gkg::MyelinWaterFractionMCMCFunction::getT1NoiseStandardDeviation() const
{

  try
  {

    return _t1NoiseStandardDeviation;

  }
  GKG_CATCH( "double gkg::MyelinWaterFractionMCMCFunction::"
             "getT1NoiseStandardDeviation() const" );

}


double gkg::MyelinWaterFractionMCMCFunction::getT1NoiseVariance() const
{

  try
  {

    return _t1NoiseVariance;

  }
  GKG_CATCH( "double gkg::MyelinWaterFractionMCMCFunction::"
             "getT1NoiseVariance() const" );

}


int32_t gkg::MyelinWaterFractionMCMCFunction::getParameterCount() const
{

  try
  {

    return 10 + _t2WeightedVolumeCount - 1;

  }
  GKG_CATCH( "int32_t gkg::MyelinWaterFractionMCMCFunction::"
             "getParameterCount() const" );


}
