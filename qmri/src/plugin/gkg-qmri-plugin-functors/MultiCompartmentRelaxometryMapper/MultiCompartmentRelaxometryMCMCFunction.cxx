#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMCMCFunction.h>
#include <gkg-core-exception/Exception.h> 
#include <cmath>


gkg::MultiCompartmentRelaxometryMCMCFunction::MultiCompartmentRelaxometryMCMCFunction(
                         const bool& t2Relaxometry, 
                         const bool& t1Relaxometry, 
                         const bool& competitiveT2, 
                         const bool& competitiveT1, 
                         const std::vector< gkg::Vector >& echoTimes, 
                         const gkg::Vector& flipAngles, 
                         const std::vector< gkg::Vector >& repetitionTimes, 
                         const std::vector< double >& t2NoiseStandardDeviations, 
                         const std::vector< double >& t1NoiseStandardDeviations, 
                         const int32_t& t2WeightedVolumeCount, 
                         const int32_t& t1WeightedVolumeCount, 
                         const std::vector< int32_t >& t2WeightedCounts, 
                         const std::vector< int32_t >& t1WeightedCounts, 
                         const int32_t& globalT2WeightedCount, 
                         const int32_t& globalT1WeightedCount, 
                         const int32_t& parameterCount, 
                         const int32_t& arteryCount,
                         const bool& hasCSF )
    : gkg::MonteCarloMarkovChainFunction(), 
           _t2Relaxometry( t2Relaxometry ), 
           _t1Relaxometry( t1Relaxometry ), 
           _competitiveT2( competitiveT2 ), 
           _competitiveT1( competitiveT1 ), 
           _echoTimes( echoTimes ), 
           _flipAngles( flipAngles ), 
           _repetitionTimes( repetitionTimes ), 
           _t2NoiseStandardDeviations( t2NoiseStandardDeviations ), 
           _t1NoiseStandardDeviations( t1NoiseStandardDeviations ), 
           _t2WeightedVolumeCount( t2WeightedVolumeCount ), 
           _t1WeightedVolumeCount( t1WeightedVolumeCount ), 
           _t2WeightedCounts( t2WeightedCounts ), 
           _t1WeightedCounts( t1WeightedCounts ), 
           _globalT2WeightedCount( globalT2WeightedCount ), 
           _globalT1WeightedCount( globalT1WeightedCount ), 
           _parameterCount( parameterCount ), 
           _arteryCount( arteryCount ),
           _hasCSF( hasCSF ), 
           _b1Scaling( 1.0f ),
           _coilScaling( 1.0f )
{

  try 
  {
    
    _t2NoiseVariances.resize( _t2WeightedVolumeCount );

    int32_t t2WeightedVolumeIndex = 0;
    for ( t2WeightedVolumeIndex = 0;
          t2WeightedVolumeIndex < _t2WeightedVolumeCount;
          t2WeightedVolumeIndex++ )
    {

      _t2NoiseVariances[ t2WeightedVolumeIndex ] = 
                           _t2NoiseStandardDeviations[ t2WeightedVolumeIndex ] *
                           _t2NoiseStandardDeviations[ t2WeightedVolumeIndex ];

    }
    
    _t1NoiseVariances.resize( _t1WeightedVolumeCount );

    int32_t t1WeightedVolumeIndex = 0;
    for ( t1WeightedVolumeIndex = 0;
          t1WeightedVolumeIndex < _t1WeightedVolumeCount;
          t1WeightedVolumeIndex++ )
    {

      _t1NoiseVariances[ t1WeightedVolumeIndex ] = 
                           _t1NoiseStandardDeviations[ t1WeightedVolumeIndex ] *
                           _t1NoiseStandardDeviations[ t1WeightedVolumeIndex ];

    }
  
  }
  GKG_CATCH( "gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "MultiCompartmentRelaxometryMCMCFunction( "
             "const bool& t2Relaxometry, "
             "const bool& t1Relaxometry, "
             "const bool& competitiveT2, "
             "const bool& competitiveT1, "
             "const std::vector< gkg::Vector >& echoTimes, "
             "const gkg::Vector& flipAngles, "
             "const std::vector< gkg::Vector > >& repetitionTimes, "
             "const std::vector< double >& t2NoiseStandardDeviations, "
             "const std::vector< double >& t1NoiseStandardDeviations, "
             "const int32_t& t2WeightedVolumeCount, "
             "const int32_t& t1WeightedVolumeCount, "
             "const std::vector< int32_t >& t2WeightedCounts, "
             "const std::vector< int32_t >& t1WeightedCounts, "
             "const int32_t& globalT2WeightedCount, "
             "const int32_t& globalT1WeightedCount, "
             "const int32_t& parameterCount, "
             "const int32_t& arteryCount )" ); 

}


gkg::MultiCompartmentRelaxometryMCMCFunction::~MultiCompartmentRelaxometryMCMCFunction()
{
}


double gkg::MultiCompartmentRelaxometryMCMCFunction::getLikelihoodRatio( 
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
    
    int32_t t1Index = 0;
    int32_t t1WeightedVolumeIndex = 0;
    for ( t1WeightedVolumeIndex = 0;
          t1WeightedVolumeIndex < _t1WeightedVolumeCount;
          t1WeightedVolumeIndex++ )
    {

      for ( t1Index = 0; t1Index < _t1WeightedCounts[ t1WeightedVolumeIndex ]; 
            t1Index++ )
      {
 
        logLikelihood1 += -std::log( _t1NoiseStandardDeviations[
                                                       t1WeightedVolumeIndex ] *
                                     std::sqrt( 2 * M_PI ) ) -
                          ( ( currentMeasurements( m ) -
                              realMeasurements( m ) ) *
                            ( currentMeasurements( m ) -
                              realMeasurements( m ) ) ) /
                          ( 2.0 * _t1NoiseVariances[ t1WeightedVolumeIndex ] );
 
        logLikelihood2 += -std::log( _t1NoiseStandardDeviations[
                                                       t1WeightedVolumeIndex ] *
                                     std::sqrt( 2 * M_PI ) ) -
                          ( ( newMeasurements( m ) -
                              realMeasurements( m ) ) *
                            ( newMeasurements( m ) -
                              realMeasurements( m ) ) ) /
                          ( 2.0 * _t1NoiseVariances[ t1WeightedVolumeIndex ] );

        ++ m;
 
      }

    }

    return std::exp( logLikelihood2 - logLikelihood1 ); 

  }
  GKG_CATCH( "double gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getLikelihoodRation( "
             "const gkg::Vector& realMeasurements, "
             "const gkg::Vector& currentMeasurements, "
             "const gkg::Vector& newMeasurements ) const" );

}


bool gkg::MultiCompartmentRelaxometryMCMCFunction::getT2Relaxometry() const
{

  try
  {
  
    return _t2Relaxometry;
  
  }
  GKG_CATCH( "bool gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT2Relaxometry() const" );
             
}


bool gkg::MultiCompartmentRelaxometryMCMCFunction::getT1Relaxometry() const
{

  try
  {
  
    return _t1Relaxometry;
  
  }
  GKG_CATCH( "bool gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT1Relaxometry() const" );
             
}


int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::getT2WeightedVolumeCount() const
{

  try
  {

    return _t2WeightedVolumeCount;

  }
  GKG_CATCH( "int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT2WeightedVolumeCount() const" );

}


int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::getT1WeightedVolumeCount() const
{

  try
  {

    return _t1WeightedVolumeCount;

  }
  GKG_CATCH( "int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT1WeightedVolumeCount() const" );

}


const std::vector< int32_t >&
 gkg::MultiCompartmentRelaxometryMCMCFunction::getT2WeightedCounts() const
{

  try
  {

    return _t2WeightedCounts;

  }
  GKG_CATCH( "const std::vector< int32_t >& "
             "gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT2WeightedCount() const" );

}


const std::vector< int32_t >&
 gkg::MultiCompartmentRelaxometryMCMCFunction::getT1WeightedCounts() const
{

  try
  {

    return _t1WeightedCounts;

  }
  GKG_CATCH( "const std::vector< int32_t >& "
             "gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT1WeightedCount() const" );

}


int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::getT2WeightedCount(
                                           int32_t t2WeightedVolumeIndex ) const
{

  try
  {

    return _t2WeightedCounts[ t2WeightedVolumeIndex ];

  }
  GKG_CATCH( "int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT2WeightedCount( int32_t t2WeightedVolumeIndex ) const" );

}


int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::getT1WeightedCount(
                                           int32_t t1WeightedVolumeIndex ) const
{

  try
  {

    return _t1WeightedCounts[ t1WeightedVolumeIndex ];

  }
  GKG_CATCH( "int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT1WeightedCount( int32_t t1WeightedVolumeIndex ) const" );

}


int32_t 
gkg::MultiCompartmentRelaxometryMCMCFunction::getGlobalT2WeightedCount() const
{

  try
  {

    return _globalT2WeightedCount;

  }
  GKG_CATCH( "int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getGlobalT2WeightedCount() const" );

}


int32_t 
gkg::MultiCompartmentRelaxometryMCMCFunction::getGlobalT1WeightedCount() const
{

  try
  {

    return _globalT1WeightedCount;

  }
  GKG_CATCH( "int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getGlobalT1WeightedCount() const" );

}


const std::vector< double >& 
gkg::MultiCompartmentRelaxometryMCMCFunction::getT2NoiseStandardDeviations() const
{

  try
  {

    return _t2NoiseStandardDeviations;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT2NoiseStandardDeviations() const" );

}


const std::vector< double >& 
gkg::MultiCompartmentRelaxometryMCMCFunction::getT1NoiseStandardDeviations() const
{

  try
  {

    return _t1NoiseStandardDeviations;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT1NoiseStandardDeviations() const" );

}


double gkg::MultiCompartmentRelaxometryMCMCFunction::getT2NoiseStandardDeviation(
                                           int32_t t2WeightedVolumeIndex ) const
{

  try
  {

    return _t2NoiseStandardDeviations[ t2WeightedVolumeIndex ];

  }
  GKG_CATCH( "double gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT2NoiseStandardDeviation( "
             "int32_t t2WeightedVolumeIndex ) const" );

}


double gkg::MultiCompartmentRelaxometryMCMCFunction::getT1NoiseStandardDeviation(
                                           int32_t t1WeightedVolumeIndex ) const
{

  try
  {

    return _t1NoiseStandardDeviations[ t1WeightedVolumeIndex ];

  }
  GKG_CATCH( "double gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT1NoiseStandardDeviation( "
             "int32_t t1WeightedVolumeIndex ) const" );

}


const std::vector< double >& 
gkg::MultiCompartmentRelaxometryMCMCFunction::getT2NoiseVariances() const
{

  try
  {

    return _t2NoiseVariances;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT2NoiseVariances() const" );

}


const std::vector< double >& 
gkg::MultiCompartmentRelaxometryMCMCFunction::getT1NoiseVariances() const
{

  try
  {

    return _t1NoiseVariances;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT1NoiseVariances() const" );

}


double gkg::MultiCompartmentRelaxometryMCMCFunction::getT2NoiseVariance(
                                           int32_t t2WeightedVolumeIndex ) const
{

  try
  {

    return _t2NoiseVariances[ t2WeightedVolumeIndex ];

  }
  GKG_CATCH( "double gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT2NoiseVariance( "
             "int32_t t2WeightedVolumeIndex ) const" );

}


double gkg::MultiCompartmentRelaxometryMCMCFunction::getT1NoiseVariance(
                                           int32_t t1WeightedVolumeIndex ) const
{

  try
  {

    return _t1NoiseVariances[ t1WeightedVolumeIndex ];

  }
  GKG_CATCH( "double gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getT2NoiseVariance( "
             "int32_t t1WeightedVolumeIndex ) const" );

}


int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::getParameterCount() const
{

  try
  {

    return _parameterCount;

  }
  GKG_CATCH( "int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getParameterCount() const" );

}


int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::getArteryCount() const
{

  try
  {
  
    return _arteryCount; 
  
  }
  GKG_CATCH( "int32_t gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getArteryCount() const" );

}


void gkg::MultiCompartmentRelaxometryMCMCFunction::setB1Scaling( float b1Scaling )
{
  
  try
  {
  
    _b1Scaling = b1Scaling;
  
  }
  GKG_CATCH( "void gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "setB1Scaling( float b1Scaling )" );

}


void gkg::MultiCompartmentRelaxometryMCMCFunction::setCoilScaling( 
                                                             float coilScaling )
{
  
  try
  {
    
    _coilScaling = coilScaling;
    
  }
  GKG_CATCH( "void gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "setCoilScaling( float coilScaling )" );

}


void gkg::MultiCompartmentRelaxometryMCMCFunction::getValuesAt( 
                                                  const gkg::Vector& parameters,
                                                  gkg::Vector& values ) const
{
  
  try
  {
    
    int32_t a = 0;
    int32_t t1WeightedVolumeIndex = 0;
    int32_t t2WeightedVolumeIndex = 0; 
    
    float parenchymaProtonDensity = ( float )parameters( 0 );
    
    float parenchymaT1 = 1.0f;
    float parenchymaT2 = 1.0f;
    float parenchymaFraction = 1.0f;
    
    float csfProtonDensity = 0.0f;
    float csfT1 = 1.0f;
    float csfT2 = 1.0f;
    float csfFraction = 0.0f;
    
    float arteriesProtonDensity = 0.0f;
    std::vector< float > arteriesFractions;
    std::vector< float > arteriesT1s;
    std::vector< float > arteriesT2s;
    
    std::vector< float > t2WeightedScalings;
    std::vector< float > t1WeightedScalings;
    
    if ( _t2Relaxometry && _t1Relaxometry )
    {
      
      parenchymaT1 = ( float )parameters( 1 );
      parenchymaT2 = ( float )parameters( 2 );
      
      if ( _hasCSF )
      {
        
        parenchymaFraction = ( float )parameters( 3 );
        csfProtonDensity = ( float )parameters( 4 );
        csfT1 = ( float )parameters( 5 );
        csfT2 = ( float )parameters( 6 );
        
        if ( _arteryCount > 0 )
        {
          
          csfFraction = ( float )parameters( 7 );
          arteriesProtonDensity = ( float )parameters( 8 );
          arteriesT1s.resize( _arteryCount );
          arteriesT2s.resize( _arteryCount );
          arteriesFractions.resize( _arteryCount );
          
          for ( a = 0; a < _arteryCount - 1; a++ )
          {
            
            arteriesT1s[ a ] = ( float )parameters( 5 + 3 * ( a + 1 ) + 1 );
            arteriesT2s[ a ] = ( float )parameters( 5 + 3 * ( a + 1 ) + 2 );
            arteriesFractions[ a ] = 
                                   ( float )parameters( 5 + 3 * ( a + 1 ) + 3 );
            
          }
          
          arteriesT1s[ _arteryCount - 1 ] = 
                            ( float )parameters( 5 + 3 * ( _arteryCount ) + 1 );
          arteriesT2s[ _arteryCount - 1 ] = 
                            ( float )parameters( 5 + 3 * ( _arteryCount ) + 2 );
          
        }
        
      }
      
      else
      {
        
        if ( _arteryCount > 0 )
        {
          
          parenchymaFraction = ( float )parameters( 3 );
          arteriesProtonDensity = ( float )parameters( 4 );
          arteriesT1s.resize( _arteryCount );
          arteriesT2s.resize( _arteryCount );
          arteriesFractions.resize( _arteryCount );
          
          for ( a = 0; a < _arteryCount - 1; a++ )
          {
            
            arteriesT1s[ a ] = ( float )parameters( 1 + 3 * ( a + 1 ) + 1 );
            arteriesT2s[ a ] = ( float )parameters( 1 + 3 * ( a + 1 ) + 2 );
            arteriesFractions[ a ] = 
                                   ( float )parameters( 1 + 3 * ( a + 1 ) + 3 );
            
          }
          
          arteriesT1s[ _arteryCount - 1 ] = 
                            ( float )parameters( 1 + 3 * ( _arteryCount ) + 1 );
          arteriesT2s[ _arteryCount - 1 ] = 
                            ( float )parameters( 1 + 3 * ( _arteryCount ) + 2 );
          
        }
        
      }
      
      t2WeightedScalings.resize( _t2WeightedVolumeCount ); 
      t2WeightedScalings[ 0 ] = 1.0f; 
      for ( t2WeightedVolumeIndex = 0; 
            t2WeightedVolumeIndex < _t2WeightedVolumeCount - 1; 
            t2WeightedVolumeIndex++ )
      {
        
        t2WeightedScalings[ t2WeightedVolumeIndex + 1 ] = 
                             ( float )parameters( _parameterCount -
                                                  ( _t2WeightedVolumeCount - 1 ) 
                                                  - ( _t1WeightedVolumeCount ) +
                                                  t2WeightedVolumeIndex );
      
      }      
      
      t1WeightedScalings.resize( _t1WeightedVolumeCount ); 
      int32_t t1WeightedVolumeIndex = 0; 
      for ( t1WeightedVolumeIndex = 0; 
            t1WeightedVolumeIndex < _t1WeightedVolumeCount; 
            t1WeightedVolumeIndex++ )
      {
        
        t1WeightedScalings[ t1WeightedVolumeIndex ] = 
                                ( float )parameters( _parameterCount - 
                                                    ( _t1WeightedVolumeCount ) +
                                                     t1WeightedVolumeIndex );
      
      }      
      
    }
    
    else if ( _t2Relaxometry )
    {
      
      parenchymaT2 = ( float ) parameters( 1 );
      
      if ( _hasCSF )
      {
        
        parenchymaFraction = ( float )parameters( 2 );
        csfProtonDensity = ( float )parameters( 3 );
        csfT2 = ( float )parameters( 4 );
        
        if ( _arteryCount > 0 )
        {
          
          csfFraction = ( float )parameters( 5 );
          arteriesProtonDensity = ( float )parameters( 6 );
          arteriesT2s.resize( _arteryCount );
          arteriesFractions.resize( _arteryCount );
          
          for ( a = 0; a < _arteryCount - 1; a++ )
          {
            
            arteriesT2s[ a ] = ( float )parameters( 5 + 2 * ( a + 1 ) );
            arteriesFractions[ a ] = 
                                   ( float )parameters( 5 + 2 * ( a + 1 ) + 1 );
            
          }
          
          arteriesT2s[ _arteryCount - 1 ] = 
                                ( float )parameters( 5 + 2 * ( _arteryCount ) );
          
        }
        
      }
      
      else
      {
        
        if ( _arteryCount > 0 )
        {
          
          parenchymaFraction = ( float )parameters( 2 );
          arteriesProtonDensity = ( float )parameters( 3 );
          arteriesT2s.resize( _arteryCount );
          arteriesFractions.resize( _arteryCount );
          
          for ( a = 0; a < _arteryCount - 1; a++ )
          {
            
            arteriesT2s[ a ] = ( float )parameters( 2 + 2 * ( a + 1 ) );
            arteriesFractions[ a ] = 
                                   ( float )parameters( 2 + 2 * ( a + 1 ) + 1 );
            
          }
          
          arteriesT2s[ _arteryCount - 1 ] = 
                                ( float )parameters( 2 + 2 * ( _arteryCount ) );
          
        }
        
      }
      
      t2WeightedScalings.resize( _t2WeightedVolumeCount ); 
      t2WeightedScalings[ 0 ] = 1.0f; 
      for ( t2WeightedVolumeIndex = 0; 
            t2WeightedVolumeIndex < _t2WeightedVolumeCount - 1; 
            t2WeightedVolumeIndex++ )
      {
        
        t2WeightedScalings[ t2WeightedVolumeIndex + 1 ] = 
                              ( float )parameters( _parameterCount - 
                                                  ( _t2WeightedVolumeCount - 1 )
                                                   + t2WeightedVolumeIndex );
      
      }
      
    }
    
    else if ( _t1Relaxometry )
    {
      
      parenchymaT1 = ( float ) parameters( 1 );
      
      if ( _hasCSF )
      {
        
        parenchymaFraction = ( float )parameters( 2 );
        csfProtonDensity = ( float )parameters( 3 );
        csfT1 = ( float )parameters( 4 );
        
        if ( _arteryCount > 0 )
        {
          
          csfFraction = ( float )parameters( 5 );
          arteriesProtonDensity = ( float )parameters( 6 );
          arteriesT1s.resize( _arteryCount );
          arteriesFractions.resize( _arteryCount );
          
          for ( a = 0; a < _arteryCount - 1; a++ )
          {
            
            arteriesT1s[ a ] = ( float )parameters( 5 + 2 * ( a + 1 ) );
            arteriesFractions[ a ] = 
                                   ( float )parameters( 5 + 2 * ( a + 1 ) + 1 );
            
          }
          
          arteriesT1s[ _arteryCount - 1 ] = 
                                ( float )parameters( 5 + 2 * ( _arteryCount ) );
          
        }
        
      }
      
      else
      {
        
        if ( _arteryCount > 0 )
        {
          
          parenchymaFraction = ( float )parameters( 2 );
          arteriesProtonDensity = ( float )parameters( 3 );
          arteriesT1s.resize( _arteryCount );
          arteriesFractions.resize( _arteryCount );
          
          for ( a = 0; a < _arteryCount - 1; a++ )
          {
            
            arteriesT1s[ a ] = ( float )parameters( 2 + 2 * ( a + 1 ) );
            arteriesFractions[ a ] = 
                                   ( float )parameters( 2 + 2 * ( a + 1 ) + 1 );
            
          }
          
          arteriesT1s[ _arteryCount - 1 ] = 
                                ( float )parameters( 2 + 2 * ( _arteryCount ) );
          
        }
        
      }
      
      t1WeightedScalings.resize( _t1WeightedVolumeCount ); 
      t1WeightedScalings[ 0 ] = 1.0f; 
      for ( t1WeightedVolumeIndex = 0; 
            t1WeightedVolumeIndex < _t1WeightedVolumeCount - 1; 
            t1WeightedVolumeIndex++ )
      {
        
        t1WeightedScalings[ t1WeightedVolumeIndex + 1 ] = 
                              ( float )parameters( _parameterCount - 
                                                  ( _t1WeightedVolumeCount - 1 )
                                                   + t1WeightedVolumeIndex );
      
      }
      
    }
    
    values.fill( 0.0f );
    float negativeProduct = 1.0f;
    float arteriesNegativeProduct = 1.0f; 

    if ( _t2Relaxometry && _competitiveT2 )
    {
    
      int32_t globalT2Index = 0; 
      int32_t t2Index = 0; 
      float t2Ponderation = 0.0f; 
      for ( t2WeightedVolumeIndex = 0;
            t2WeightedVolumeIndex < _t2WeightedVolumeCount;
            t2WeightedVolumeIndex++ )
      {

        for ( t2Index = 0; t2Index < _t2WeightedCounts[ t2WeightedVolumeIndex ];
              t2Index++ )
        {
        
          negativeProduct = 1.0f; 
          arteriesNegativeProduct = 1.0f;
          t2Ponderation = 0.0f; 
          
          values( globalT2Index ) += 
                                 t2WeightedScalings[ t2WeightedVolumeIndex ] *
                                 parenchymaProtonDensity * _coilScaling *
                                 parenchymaFraction *
                                 std::exp( - _echoTimes[ t2WeightedVolumeIndex](
                                                     t2Index ) / parenchymaT2 );
          negativeProduct *= ( 1.0f - parenchymaFraction );
          
          if ( _hasCSF )
          {
            
            values( globalT2Index ) += 
                                t2WeightedScalings[ t2WeightedVolumeIndex ] *
                                csfProtonDensity * _coilScaling * 
                                negativeProduct * csfFraction *
                                std::exp( - _echoTimes[ t2WeightedVolumeIndex ](
                                                            t2Index ) / csfT2 );
            negativeProduct *= ( 1.0f - csfFraction );
            
          }
          
          if ( _arteryCount > 0 )
          {
          
            for ( a = 0; a < _arteryCount - 1; a++ )
            {
              
              t2Ponderation += arteriesNegativeProduct * 
                                      arteriesFractions[ a ] / arteriesT2s[ a ];
              arteriesNegativeProduct *= ( 1.0f - arteriesFractions[ a ] );
              
            }
            
            t2Ponderation += 
                      arteriesNegativeProduct / arteriesT2s[ _arteryCount - 1 ];
            
            values( globalT2Index ) += 
                                   t2WeightedScalings[ t2WeightedVolumeIndex ] * 
                                   arteriesProtonDensity * _coilScaling *
                                   negativeProduct *
                                   std::exp( 
                                           - _echoTimes[ t2WeightedVolumeIndex](
                                                    t2Index ) * t2Ponderation ); 
            
          }
          
          ++ globalT2Index;
          
        }
      
      }  
    
    }
    else if ( _t2Relaxometry )
    {
    
      int32_t globalT2Index = 0; 
      int32_t t2Index = 0; 
      for ( t2WeightedVolumeIndex = 0;
            t2WeightedVolumeIndex < _t2WeightedVolumeCount;
            t2WeightedVolumeIndex++ )
      {

        for ( t2Index = 0; t2Index < _t2WeightedCounts[ t2WeightedVolumeIndex ];
              t2Index++ )
        {
        
          negativeProduct = 1.0f;
          
          values( globalT2Index ) += 
                                   t2WeightedScalings[ t2WeightedVolumeIndex ] * 
                                   parenchymaProtonDensity * _coilScaling *
                                   parenchymaFraction * 
                                   std::exp( 
                                          - _echoTimes[ t2WeightedVolumeIndex ]( 
                                                     t2Index ) / parenchymaT2 ); 
          negativeProduct *= ( 1.0f - parenchymaFraction );
          
          if ( _hasCSF )
          {
            
            values( globalT2Index ) += 
                                   t2WeightedScalings[ t2WeightedVolumeIndex ] *
                                   csfProtonDensity * _coilScaling *
                                   negativeProduct * csfFraction * 
                                   std::exp( 
                                          - _echoTimes[ t2WeightedVolumeIndex ](
                                                            t2Index ) / csfT2 );
            negativeProduct *= ( 1.0f - csfFraction );
            
          }
          
          if ( _arteryCount > 0 )
          {
            
            for ( a = 0; a < _arteryCount - 1; a++ )
            {
              
              values( globalT2Index ) += 
                                   t2WeightedScalings[ t2WeightedVolumeIndex ] * 
                                   arteriesProtonDensity * _coilScaling *
                                   negativeProduct * arteriesFractions [ a ] * 
                                   std::exp( 
                                          - _echoTimes[ t2WeightedVolumeIndex ]( 
                                                 t2Index ) / arteriesT2s[ a ] );
              negativeProduct *= ( 1.0f - arteriesFractions[ a ] );
              
            }
            
            values( globalT2Index ) += 
                          t2WeightedScalings[ t2WeightedVolumeIndex ] * 
                          arteriesProtonDensity * _coilScaling *
                          negativeProduct * std::exp( 
                              - _echoTimes[ t2WeightedVolumeIndex ]( t2Index ) / 
                              arteriesT2s[ _arteryCount - 1 ] );
            
            
          }
          
          ++ globalT2Index; 
          
        }
      
      }

    }
    
    if ( _t1Relaxometry && _competitiveT1 )
    {
    
      int32_t globalT1Index = 0; 
      int32_t t1Index = 0; 
      float correctedFlipAngle = 0.0f; 
      float t1Ponderation = 0.0f; 
      for ( t1WeightedVolumeIndex = 0;
            t1WeightedVolumeIndex < _t1WeightedVolumeCount;
            t1WeightedVolumeIndex++ )
      {

        for ( t1Index = 0; t1Index < _t1WeightedCounts[ t1WeightedVolumeIndex ];
              t1Index++ )
        {
        
          correctedFlipAngle = _flipAngles( t1Index ) * _b1Scaling * 
                                                                   M_PI / 180.0;
          negativeProduct = 1.0f; 
          arteriesNegativeProduct = 1.0f;
          t1Ponderation = 0.0f; 
          
          values( _globalT2WeightedCount + globalT1Index ) += 
                                   t1WeightedScalings[ t1WeightedVolumeIndex ] *
                                   parenchymaProtonDensity * _coilScaling * 
                                   parenchymaFraction *
                                   ( float )std::sin( correctedFlipAngle ) * 
                                   ( 1.0f - ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) / parenchymaT1 ) ) / 
                                   ( 1.0f - ( float )std::cos( 
                                                          correctedFlipAngle ) *
                                   ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) / parenchymaT1 ) );
          negativeProduct *= ( 1.0f - parenchymaFraction );
          
          if ( _hasCSF )
          {
            
            values( _globalT2WeightedCount + globalT1Index ) += 
                                   t1WeightedScalings[ t1WeightedVolumeIndex ] *
                                   csfProtonDensity * _coilScaling *
                                   negativeProduct * csfFraction * 
                                   ( float )std::sin( correctedFlipAngle ) * 
                                   ( 1.0f - ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) / csfT1 ) ) / 
                                   ( 1.0f - ( float )std::cos( 
                                                          correctedFlipAngle ) *
                                   ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) / csfT1 ) );
            negativeProduct *= ( 1.0f - csfFraction );
            
          }
          
          if ( _arteryCount > 0 )
          {
            
            for ( a = 0; a < _arteryCount - 1; a++ )
            {
              
              t1Ponderation += arteriesNegativeProduct * 
                                      arteriesFractions[ a ] / arteriesT1s[ a ];
              arteriesNegativeProduct *= ( 1.0f - arteriesFractions[ a ] );
              
            }
            
            t1Ponderation += 
                      arteriesNegativeProduct / arteriesT1s[ _arteryCount - 1 ];
            
            values( _globalT2WeightedCount + globalT1Index ) += 
                                   t1WeightedScalings[ t1WeightedVolumeIndex ] * 
                                   arteriesProtonDensity * _coilScaling *
                                   negativeProduct *
                                   ( float )std::sin( correctedFlipAngle ) * 
                                   ( 1.0f - ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) * t1Ponderation ) ) / 
                                   ( 1.0f - ( float )std::cos( 
                                                          correctedFlipAngle ) *
                                   ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) * t1Ponderation ) );
            
          }
          
          ++ globalT1Index; 
        
        }
        
      }
        
    }
    else if ( _t1Relaxometry )
    {

      int32_t globalT1Index = 0; 
      int32_t t1Index = 0; 
      float correctedFlipAngle = 0.0f; 
      for ( t1WeightedVolumeIndex = 0;
            t1WeightedVolumeIndex < _t1WeightedVolumeCount;
            t1WeightedVolumeIndex++ )
      {

        for ( t1Index = 0; t1Index < _t1WeightedCounts[ t1WeightedVolumeIndex ];
              t1Index++ )
        {
        
          correctedFlipAngle = _flipAngles( t1Index ) * _b1Scaling * 
                                                                   M_PI / 180.0;
          negativeProduct = 1.0f;
          
          values( _globalT2WeightedCount + globalT1Index ) += 
                                   t1WeightedScalings[ t1WeightedVolumeIndex ] * 
                                   parenchymaProtonDensity * _coilScaling * 
                                   parenchymaFraction * 
                                   ( float )std::sin( correctedFlipAngle ) * 
                                   ( 1.0f - ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) / parenchymaT1 ) ) / 
                                   ( 1.0f - ( float )std::cos( 
                                                          correctedFlipAngle ) *
                                   ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) / parenchymaT1 ) );
          negativeProduct *= ( 1.0f - parenchymaFraction );
          
          if ( _hasCSF )
          {
            
            values( _globalT2WeightedCount + globalT1Index ) +=
                                   t1WeightedScalings[ t1WeightedVolumeIndex ] * 
                                   csfProtonDensity * _coilScaling * 
                                   negativeProduct * csfFraction *
                                   ( float )std::sin( correctedFlipAngle ) * 
                                   ( 1.0f - ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) / csfT1 ) ) / 
                                   ( 1.0f - ( float )std::cos( 
                                                          correctedFlipAngle ) *
                                   ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) / csfT1 ) );
            negativeProduct *= ( 1.0f - csfFraction );
            
          }
          
          if ( _arteryCount > 0 )
          {
            
            for ( a = 0; a < _arteryCount - 1; a++ )
            {
              
              values( _globalT2WeightedCount + globalT1Index ) += 
                                   t1WeightedScalings[ t1WeightedVolumeIndex ] * 
                                   arteriesProtonDensity * _coilScaling *
                                   negativeProduct * arteriesFractions [ a ] * 
                                   ( float )std::sin( correctedFlipAngle ) * 
                                   ( 1.0f - ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) / arteriesT1s[ a ] ) ) / 
                                   ( 1.0f - ( float )std::cos( 
                                                          correctedFlipAngle ) *
                                   ( float )std::exp( - 
                                      _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                      t1Index ) / arteriesT1s[ a ] ) );
              negativeProduct *= ( 1.0f - arteriesFractions[ a ] );
              
            }
          
            values( _globalT2WeightedCount + globalT1Index ) += 
                            t1WeightedScalings[ t1WeightedVolumeIndex ] * 
                            arteriesProtonDensity * _coilScaling *
                            negativeProduct *  
                            ( float )std::sin( correctedFlipAngle ) * 
                            ( 1.0f - ( float )std::exp( - 
                               _repetitionTimes[ t1WeightedVolumeIndex ]( 
                               t1Index ) / arteriesT1s[ _arteryCount - 1 ] ) ) / 
                            ( 1.0f - ( float )std::cos( 
                                                          correctedFlipAngle ) *
                            ( float )std::exp( - 
                                _repetitionTimes[ t1WeightedVolumeIndex ]( 
                                t1Index ) / arteriesT1s[ _arteryCount - 1 ] ) );
            
          }
          
          ++ globalT1Index; 
          
        }
      
      }
      
    }

  }
  GKG_CATCH( "void gkg::MultiCompartmentRelaxometryMCMCFunction::"
             "getValuesAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& values ) const" );

}

