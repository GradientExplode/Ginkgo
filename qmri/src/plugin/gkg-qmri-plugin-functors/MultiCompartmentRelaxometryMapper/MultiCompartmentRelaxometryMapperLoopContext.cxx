#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperLoopContext.h>
#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperGauge.h>
#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryNLPFunction.h>
#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMCMCParameterSet.h>
#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMCMCFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainEstimator.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::MultiCompartmentRelaxometryMapperLoopContext::
                                   MultiCompartmentRelaxometryMapperLoopContext(
   gkg::MultiCompartmentRelaxometryMapperGauge& gauge, 
   const std::vector< gkg::Vector3d< int32_t > >& sites, 
   const bool& t2Relaxometry, 
   const bool& t1Relaxometry, 
   const bool& competitiveT2, 
   const bool& competitiveT1, 
   const std::vector< gkg::Volume< float > >& t2WeightedVolumes, 
   const std::vector< gkg::Volume< float > >& t1WeightedVolumes, 
   const gkg::RCPointer< gkg::Volume< float > >& b1MappingVolumePointer,
   const gkg::RCPointer< gkg::Volume< float > >& hcMappingVolumePointer,
   const gkg::RCPointer< gkg::Volume< float > >& bcMappingVolumePointer, 
   const std::vector< gkg::Vector >& echoTimes, 
   const gkg::Vector& flipAngles, 
   const std::vector< gkg::Vector >& repetitionTimes, 
   const std::vector< double >& t2NoiseStandardDeviations, 
   const std::vector< double >& t1NoiseStandardDeviations, 
   const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >& constraints, 
   const std::vector< double >& nlpOptimizerParameters, 
   const std::vector< double >& initialParameters, 
   const gkg::Vector& lowerParameterBoundaries, 
   const gkg::Vector& upperParameterBoundaries, 
   const std::vector< bool >& isFixedParameters, 
   const gkg::Vector& deltaParameters, 
   const bool& applyMCMC, 
   const int32_t& mcmcBurninCount, 
   const int32_t& mcmcSampleCount, 
   const int32_t& mcmcIntervalCount, 
   const int32_t& mcmcMaximumIterationCount, 
   gkg::Volume< float >& parenchymaFractionVolume, 
   gkg::Volume< float >& parenchymaProtonDensityVolume, 
   gkg::Volume< float >& parenchymaT1Volume, 
   gkg::Volume< float >& parenchymaT2Volume, 
   std::vector< gkg::Volume< float > >& arteriesFractionVolumes,
   gkg::Volume< float >& arteriesProtonDensityVolume, 
   std::vector< gkg::Volume< float > >& arteriesT1Volumes,
   std::vector< gkg::Volume< float > >& arteriesT2Volumes,
   gkg::Volume< float >& csfFractionVolume,
   gkg::Volume< float >& csfProtonDensityVolume,
   gkg::Volume< float >& csfT1Volume, 
   gkg::Volume< float >& csfT2Volume, 
   gkg::Volume< float >& t2WeightedScalingVolume, 
   gkg::Volume< float >& t1WeightedScalingVolume, 
   const bool& computeFittedData, 
   gkg::Volume< float >& fittedDataVolume, 
   const int32_t& t2WeightedVolumeCount, 
   const int32_t& t1WeightedVolumeCount, 
   const int32_t& globalT2WeightedMeasurementCount, 
   const int32_t& globalT1WeightedMeasurementCount, 
   const std::vector< int32_t >& t2WeightedMeasurementCounts, 
   const std::vector< int32_t >& t1WeightedMeasurementCounts, 
   const int32_t& arteryCount, 
   const int32_t& parameterCount, 
   const bool& hasCSF, 
   const bool& verbose )
        : gkg::LoopContext< int32_t >( &gauge ),
          _sites( sites ), 
          _t2Relaxometry( t2Relaxometry ), 
          _t1Relaxometry( t1Relaxometry ), 
          _competitiveT2( competitiveT2 ), 
          _competitiveT1( competitiveT1 ), 
          _t2WeightedVolumes( t2WeightedVolumes ),
          _t1WeightedVolumes( t1WeightedVolumes ), 
          _b1MappingVolumePointer( b1MappingVolumePointer ), 
          _hcMappingVolumePointer( hcMappingVolumePointer ),
          _bcMappingVolumePointer( bcMappingVolumePointer ),
          _echoTimes( echoTimes ), 
          _flipAngles( flipAngles ), 
          _repetitionTimes( repetitionTimes ),
          _t2NoiseStandardDeviations( t2NoiseStandardDeviations ), 
          _t1NoiseStandardDeviations( t1NoiseStandardDeviations ), 
          _constraints( constraints ), 
          _nlpOptimizerParameters( nlpOptimizerParameters ), 
          _initialParameters( initialParameters ), 
          _lowerParameterBoundaries( lowerParameterBoundaries ), 
          _upperParameterBoundaries( upperParameterBoundaries ), 
          _isFixedParameters( isFixedParameters ), 
          _deltaParameters( deltaParameters ), 
          _applyMCMC( applyMCMC ), 
          _mcmcBurninCount( mcmcBurninCount ), 
          _mcmcSampleCount( mcmcSampleCount ), 
          _mcmcIntervalCount( mcmcIntervalCount ), 
          _mcmcMaximumIterationCount( mcmcMaximumIterationCount ), 
          _parenchymaFractionVolume( parenchymaFractionVolume ), 
          _parenchymaProtonDensityVolume( parenchymaProtonDensityVolume ), 
          _parenchymaT1Volume( parenchymaT1Volume ), 
          _parenchymaT2Volume( parenchymaT2Volume ), 
          _arteriesFractionVolumes( arteriesFractionVolumes ), 
          _arteriesProtonDensityVolume( arteriesProtonDensityVolume ), 
          _arteriesT1Volumes( arteriesT1Volumes ), 
          _arteriesT2Volumes( arteriesT2Volumes ), 
          _csfFractionVolume( csfFractionVolume ),
          _csfProtonDensityVolume( csfProtonDensityVolume ),
          _csfT1Volume( csfT1Volume ),
          _csfT2Volume( csfT2Volume ),
          _t2WeightedScalingVolume( t2WeightedScalingVolume ), 
          _t1WeightedScalingVolume( t1WeightedScalingVolume ), 
          _computeFittedData( computeFittedData ), 
          _fittedDataVolume( fittedDataVolume ), 
          _t2WeightedVolumeCount( t2WeightedVolumeCount ), 
          _t1WeightedVolumeCount( t1WeightedVolumeCount ), 
          _globalT2WeightedMeasurementCount( globalT2WeightedMeasurementCount ), 
          _globalT1WeightedMeasurementCount( globalT1WeightedMeasurementCount ), 
          _t2WeightedMeasurementCounts( t2WeightedMeasurementCounts ),
          _t1WeightedMeasurementCounts( t1WeightedMeasurementCounts ),
          _arteryCount( arteryCount ), 
          _parameterCount( parameterCount ), 
          _hasCSF( hasCSF ), 
          _verbose( verbose ) 
{

  try
  {
  
    ////////////////////////////////////////////////////////////////////////////
    // getting access to the numerical analysis factory 
    ////////////////////////////////////////////////////////////////////////////
    
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();     
  }
  GKG_CATCH( "gkg::MultiCompartmentRelaxometryMapperLoopContext::"
             "MultiCompartmentRelaxometryMapperLoopContext( "
             "gkg::MultiCompartmentRelaxometryMapperLoopGauge& gauge, "
             "const std::vector< gkg::Vector3d< int32_t > >& sites, "
             "const bool& t2Relaxometry, "
             "const bool& t1Relaxometry, "
             "const bool& competitiveT2, "
             "const bool& competitiveT1, "
             "const std::vector< gkg::Volume< float > >& t2WeightedVolumes, "
             "const std::vector< gkg::Volume< float > >& t1WeightedVolumes, "
             "const gkg::RCPointer< "
             "gkg::Volume< float > >& b1MappingVolumePointer, "
             "const gkg::RCPointer< "
             "gkg::Volume< float > >& hcMappingVolumePointer, "
             "const gkg::RCPointer< "
             "gkg::Volume< float > >& bcMappingVolumePointer, "
             "const std::vector< gkg::Vector >& echoTimes, "
             "const gkg::Vector& flipAngles, "
             "const std::vector< gkg::Vector >& repetitionTimes, "
             "const std::vector< double >& t2NoiseStandardDeviations, "
             "const std::vector< double >& t1NoiseStandardDeviations, "
             "const std::vector< gkg::RCPointer< "
             "gkg::OptimizerConstraint > >& constraints, "
             "const std::vector< double >& nlpOptimizerParameters, "
             "const std::vector< double >& initialParameters, "
             "const gkg::Vector& lowerParameterBoundaries, "
             "const gkg::Vector& upperParameterBoundaries, "
             "const std::vector< bool >& isFixedParameters, "
             "const gkg::Vector& deltaParameters, "
             "const bool& applyMCMC, "
             "const int32_t& mcmcBurninCount, "
             "const int32_t& mcmcSampleCount, "
             "const int32_t& mcmcIntervalCount, "
             "const int32_t& mcmcMaximumIterationCount, "
             "gkg::Volume< float >& parenchymaFractionVolume, "
             "gkg::Volume< float >& parenchymaProtonDensityVolume, "
             "gkg::Volume< float >& parenchymaT1Volume, "
             "gkg::Volume< float >& parenchymaT2Volume, "
             "std::vector< gkg::Volume< float > >& arteriesFractionVolumes, "
             "gkg::Volume< float >& arteriesProtonDensityVolume, "
             "std::vector< gkg::Volume< float > >& arteriesT1Volumes, "
             "std::vector< gkg::Volume< float > >& arteriesT2Volumes, "
             "gkg::Volume< float >& t2WeightedScalingVolume, "
             "gkg::Volume< float >& t1WeightedScalingVolume, "
             "const bool& computeFittedData, "
             "gkg::Volume< float >& fittedDataVolume, "
             "const int32_t& t2WeightedVolumeCount, "
             "const int32_t& t1WeightedVolumeCount, "
             "const int32_t& globalT2WeightedMeasurementCount, "
             "const int32_t& globalT1WeightedMeasurementCount, "
             "const std::vector< int32_t >& t2WeightedMeasurementCounts, "
             "const std::vector< int32_t >& t1WeightedMeasurementCounts, "
             "const int32_t& arteryCount, "
             "const int32_t& parameterCount, "
             "const bool hasCSF, "
             "const bool& verbose )" );

}


gkg::MultiCompartmentRelaxometryMapperLoopContext::
                                      ~MultiCompartmentRelaxometryMapperLoopContext()
{
}


void gkg::MultiCompartmentRelaxometryMapperLoopContext::doIt( int32_t startIndex,
                                                         int32_t count )
{

  try
  {
    
    ////////////////////////////////////////////////////////////////////////////
    // allocating a MCMC function
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::MultiCompartmentRelaxometryMCMCFunction > 
      multiCompartmentRelaxometryMCMCFunction( 
        new gkg::MultiCompartmentRelaxometryMCMCFunction ( 
                                              _t2Relaxometry, 
                                              _t1Relaxometry,
                                              _competitiveT2,
                                              _competitiveT1,
                                              _echoTimes, 
                                              _flipAngles, 
                                              _repetitionTimes, 
                                              _t2NoiseStandardDeviations, 
                                              _t1NoiseStandardDeviations, 
                                              _t2WeightedVolumeCount, 
                                              _t1WeightedVolumeCount, 
                                              _t2WeightedMeasurementCounts, 
                                              _t1WeightedMeasurementCounts, 
                                              _globalT2WeightedMeasurementCount,
                                              _globalT1WeightedMeasurementCount,
                                              _parameterCount,
                                              _arteryCount, 
                                              _hasCSF ) ); 
    

    ////////////////////////////////////////////////////////////////////////////
    // allocating a MCMC parameter set 
    ////////////////////////////////////////////////////////////////////////////
        
    gkg::RCPointer< gkg::MultiCompartmentRelaxometryMCMCParameterSet > 
      multiCompartmentRelaxometryMCMCParameterSet( 
        new gkg::MultiCompartmentRelaxometryMCMCParameterSet( 
                                                      _lowerParameterBoundaries,
                                                      _upperParameterBoundaries,
                                                      _isFixedParameters, 
                                                      _deltaParameters ) ); 
    
    
    ////////////////////////////////////////////////////////////////////////////
    // allocating an MCMC estimator 
    ////////////////////////////////////////////////////////////////////////////
    
    gkg::MonteCarloMarkovChainEstimator 
      monteCarloMarkovChainEstimator( multiCompartmentRelaxometryMCMCFunction, 
                                      multiCompartmentRelaxometryMCMCParameterSet ); 
    
    
    ////////////////////////////////////////////////////////////////////////////
    // allocating a NLP function from the MCMC function 
    ////////////////////////////////////////////////////////////////////////////
    
    gkg::MultiCompartmentRelaxometryNLPFunction 
      multiCompartmentRelaxometryNLPFunction( multiCompartmentRelaxometryMCMCFunction ); 
    
    
    ////////////////////////////////////////////////////////////////////////////
    // allocating temporary memory for results 
    ////////////////////////////////////////////////////////////////////////////
    
    int32_t a = 0; 
    std::vector< float > parenchymaProtonDensities( count ); 
    std::vector< float > parenchymaFractions( count );
    std::vector< float > parenchymaT1s( count );
    std::vector< float > parenchymaT2s( count ); 
    
    std::vector< float > csfFractions( count );
    std::vector< float > csfProtonDensities( count );
    std::vector< float > csfT1s( count );
    std::vector< float > csfT2s( count );
    
    std::vector< float > arteriesProtonDensities( count );
    std::vector< std::vector< float > > arteriesFractions( _arteryCount );
    std::vector< std::vector< float > > arteriesT1s( _arteryCount );
    std::vector< std::vector< float > > arteriesT2s( _arteryCount );
    
    for ( a = 0; a < _arteryCount; a++ )
    {
      
      arteriesFractions[ a ].resize( count );
      arteriesT1s[ a ].resize( count );
      arteriesT2s[ a ].resize( count );
      
    }
    
    std::vector< float > t2WeightedScalings;
    if ( _t2Relaxometry )
    {
      
      t2WeightedScalings.resize( count * ( _t2WeightedVolumeCount - 1 ) );
      
    }
    
    std::vector< float > t1WeightedScalings;
    
    if ( _t2Relaxometry && _t1Relaxometry )
    {
      
      t1WeightedScalings.resize( count * ( _t1WeightedVolumeCount ) );
      
    }
    else if ( _t1Relaxometry )
    {
      
      t1WeightedScalings.resize( count * ( _t1WeightedVolumeCount - 1 ) );
      
    }
        
    std::vector< float > fittedDatas( count );
    std::vector< gkg::Vector > fittedMeasurements( count );

    
    ////////////////////////////////////////////////////////////////////////////
    // looping over site(s) 
    ////////////////////////////////////////////////////////////////////////////
    
    const gkg::Vector3d< int32_t >* sitePtr = 
    		       ( const gkg::Vector3d< int32_t >* )&_sites[ startIndex ];

    int32_t s = 0;
    int32_t t2WeightedVolumeIndex = 0;
    int32_t t2Index = 0;
    int32_t t1WeightedVolumeIndex = 0;
    int32_t t1Index = 0;
    gkg::Vector realMeasurements( _globalT2WeightedMeasurementCount +
                                  _globalT1WeightedMeasurementCount );
    int32_t updateCallCount = 0;
    double acceptanceRate = 0.0;
    gkg::Matrix parameterEstimates;
    gkg::MedianFilter< std::vector< float >, float > medianFilter;
    std::vector< float > optimalParameters( _parameterCount );
    std::vector< float > parameterValues( _mcmcSampleCount );
    int32_t p = 0;
    int32_t e = 0;

    for ( s = 0; s < count; s++ )
    {

      if ( _verbose && _gauge )
      {

        if ( s && !( s % 100 ) )
        {

          lock();
          _gauge->add( 100 );
          unlock();

        }

      }

      const gkg::Vector3d< int32_t >& site = sitePtr[ s ];


      //////////////////////////////////////////////////////////////////////////
      // collecting the T2-weighted measurements
      //////////////////////////////////////////////////////////////////////////

      if ( _t2Relaxometry )
      {
        
        int32_t globalT2Index = 0;
        for ( t2WeightedVolumeIndex = 0;
              t2WeightedVolumeIndex < _t2WeightedVolumeCount;
              t2WeightedVolumeIndex++ )
        {

          for ( t2Index = 0;
                t2Index < _t2WeightedMeasurementCounts[ t2WeightedVolumeIndex ];
                t2Index++ )
          {

            realMeasurements( globalT2Index ) =
                   _t2WeightedVolumes[ t2WeightedVolumeIndex ]( site, t2Index );

            ++ globalT2Index;

          }

        }
      
      }
      
      
      //////////////////////////////////////////////////////////////////////////
      // collecting the T1-weighted measurements
      //////////////////////////////////////////////////////////////////////////

      if ( _t1Relaxometry )
      { 
      
        int32_t globalT1Index = 0;
        for ( t1WeightedVolumeIndex = 0;
              t1WeightedVolumeIndex < _t1WeightedVolumeCount;
              t1WeightedVolumeIndex++ )
        {

          for ( t1Index = 0;
                t1Index < _t1WeightedMeasurementCounts[ t1WeightedVolumeIndex ];
                t1Index++ )
          {

            realMeasurements( _globalT2WeightedMeasurementCount + 
                                                               globalT1Index ) =
                   _t1WeightedVolumes[ t1WeightedVolumeIndex ]( site, t1Index );

            ++ globalT1Index;

          }

        }
      
      }
      
      
      //////////////////////////////////////////////////////////////////////////
      // feeding the NLP function with the real measurements
      //////////////////////////////////////////////////////////////////////////

      multiCompartmentRelaxometryNLPFunction.setRealMeasurements( realMeasurements ); 
      
      
      //////////////////////////////////////////////////////////////////////////
      // computing the b1 scaling 
      //////////////////////////////////////////////////////////////////////////
      
      if ( _t1Relaxometry && !_b1MappingVolumePointer.isNull() )
      {
        
        static_cast< gkg::MultiCompartmentRelaxometryMCMCFunction* >( 
         multiCompartmentRelaxometryMCMCFunction.getPointer() )->setB1Scaling(
                                ( *_b1MappingVolumePointer )( site ) / 900.0f );
      
      }
      
      
      //////////////////////////////////////////////////////////////////////////
      // computing the coil scaling 
      //////////////////////////////////////////////////////////////////////////
      
      if ( !_hcMappingVolumePointer.isNull() && 
           !_bcMappingVolumePointer.isNull() )
      {
        
        static_cast< gkg::MultiCompartmentRelaxometryMCMCFunction* >( 
          multiCompartmentRelaxometryMCMCFunction.getPointer() )->setCoilScaling( 
                                         ( *_hcMappingVolumePointer )( site ) / 
                                         ( *_bcMappingVolumePointer )( site ) );
        
      }
      
      //////////////////////////////////////////////////////////////////////////
      // initializing the parameters 
      //////////////////////////////////////////////////////////////////////////

      gkg::Vector copyOfInitialParameters( _initialParameters );
       
      
      //////////////////////////////////////////////////////////////////////////
      // optimizing the parameters using a NLP optimizer
      //////////////////////////////////////////////////////////////////////////

      _factory->getConstrainedNonLinearProgrammingOptimumParameters(
                gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
                copyOfInitialParameters,
                _constraints,
                multiCompartmentRelaxometryNLPFunction,
                _nlpOptimizerParameters,
                false );


      //////////////////////////////////////////////////////////////////////////
      // sampling the parameters using a MCMC optimizer
      //////////////////////////////////////////////////////////////////////////

      if ( _applyMCMC )
      {
      
        updateCallCount = 0;
        acceptanceRate = 0.0;
        monteCarloMarkovChainEstimator.estimate( realMeasurements,
                                                 copyOfInitialParameters,
                                                 parameterEstimates,
                                                 updateCallCount,
                                                 acceptanceRate,
                                                 _mcmcBurninCount,
                                                 _mcmcSampleCount,
                                                 _mcmcIntervalCount,
                                                 1,
                                                 _mcmcMaximumIterationCount,
                                                 false );


        ////////////////////////////////////////////////////////////////////////
        // computing median values for each parameter distribution
        ////////////////////////////////////////////////////////////////////////

        for ( p = 0; p < _parameterCount; p++ )
        {
          
          for ( e = 0; e < _mcmcSampleCount; e++ )
          {
            
            parameterValues[ e ] = parameterEstimates( p, e ); 
            
          }
          medianFilter.filter( parameterValues, optimalParameters[ p ] ); 
          
        }
      
      }
      else
      {
        
        for ( p = 0; p < _parameterCount; p++ )
        {
          
          optimalParameters[ p ] = copyOfInitialParameters( p ); 
          
        }
        
      }
      
      
      //////////////////////////////////////////////////////////////////////////
      // copying results to vectors 
      //////////////////////////////////////////////////////////////////////////
      
      float negativeProduct = 1.0f; 
      parenchymaProtonDensities[ s ] = optimalParameters[ 0 ];
      
      if ( _t2Relaxometry && _t1Relaxometry )
      {
        
        parenchymaT1s[ s ] = optimalParameters[ 1 ];
        parenchymaT2s[ s ] = optimalParameters[ 2 ];
        
        if ( _hasCSF )
        {
          
          parenchymaFractions[ s ] = optimalParameters[ 3 ];
          negativeProduct *= ( 1.0f - optimalParameters[ 3 ] );
          csfProtonDensities[ s ] = optimalParameters[ 4 ];
          csfT1s[ s ] = optimalParameters[ 5 ];
          csfT2s[ s ] = optimalParameters[ 6 ];
          
          if ( _arteryCount > 0 )
          {
            
            csfFractions[ s ] = negativeProduct * optimalParameters[ 7 ];
            negativeProduct *= ( 1.0f - optimalParameters[ 7 ] );
            arteriesProtonDensities[ s ] = optimalParameters[ 8 ];
            for ( a = 0; a < _arteryCount - 1; a++ )
            {
              
              arteriesT1s[ a ][ s ] = 
                                     optimalParameters[ 5 + 3 * ( a + 1 ) + 1 ];
              arteriesT2s[ a ][ s ] = 
                                     optimalParameters[ 5 + 3 * ( a + 1 ) + 2 ];
              arteriesFractions[ a ][ s ] = negativeProduct * 
                                     optimalParameters[ 5 + 3 * ( a + 1 ) + 3 ];
              negativeProduct *= ( 1.0f - 
                                   optimalParameters[ 5 + 3 * ( a + 1 ) + 3 ] );
              
            }
            
            arteriesT1s[ _arteryCount - 1 ][ s ] = 
                              optimalParameters[ 5 + 3 * ( _arteryCount ) + 1 ];
            arteriesT2s[ _arteryCount - 1 ][ s ] = 
                              optimalParameters[ 5 + 3 * ( _arteryCount ) + 2 ];
            arteriesFractions[ _arteryCount - 1 ][ s ] = negativeProduct;
            
          }
          else
          {
            
            csfFractions[ s ] = negativeProduct;
            
          }
          
        }
         
        else 
        {
          
          if ( _arteryCount > 0 )
          {
            
            parenchymaFractions[ s ] = optimalParameters[ 3 ];
            negativeProduct *= ( 1.0f - optimalParameters[ 3 ] );
            arteriesProtonDensities[ s ] = optimalParameters[ 4 ];
            for ( a = 0; a < _arteryCount - 1; a++ )
            {
            
              arteriesT1s[ a ][ s ] = 
                                     optimalParameters[ 1 + 3 * ( a + 1 ) + 1 ];
              arteriesT2s[ a ][ s ] = 
                                     optimalParameters[ 1 + 3 * ( a + 1 ) + 2 ];
              arteriesFractions[ a ][ s ] = negativeProduct * 
                                     optimalParameters[ 1 + 3 * ( a + 1 ) + 3 ];
              negativeProduct *= ( 1.0f - 
                                   optimalParameters[ 1 + 3 * ( a + 1 ) + 3 ] );
              
            }
            
            arteriesT1s[ _arteryCount - 1 ][ s ] = 
                              optimalParameters[ 1 + 3 * ( _arteryCount ) + 1 ];
            arteriesT2s[ _arteryCount - 1 ][ s ] = 
                              optimalParameters[ 1 + 3 * ( _arteryCount ) + 2 ];
            arteriesFractions[ _arteryCount - 1 ][ s ] = negativeProduct;
            
          }
          
        }
        
        for ( t2WeightedVolumeIndex = 0; 
              t2WeightedVolumeIndex < _t2WeightedVolumeCount - 1; 
              t2WeightedVolumeIndex++ )
        {
          
          t2WeightedScalings[ t2WeightedVolumeIndex * count + s ] = 
                               optimalParameters[ _parameterCount - 
                                                  ( _t2WeightedVolumeCount - 1 )
                                                  - ( _t1WeightedVolumeCount ) + 
                                                  t2WeightedVolumeIndex ];
        
        }
        for ( t1WeightedVolumeIndex = 0; 
              t1WeightedVolumeIndex < _t1WeightedVolumeCount; 
              t1WeightedVolumeIndex++ )
        {
          
          t1WeightedScalings[ t1WeightedVolumeIndex * count + s ] = 
                               optimalParameters[ _parameterCount - 
                                                        _t1WeightedVolumeCount + 
                                                        t1WeightedVolumeIndex ];
        
        }
        
      }
      
      else if ( _t2Relaxometry ) 
      {
        
        parenchymaT2s[ s ] = optimalParameters[ 1 ];

        if ( _hasCSF )
        {
          
          parenchymaFractions[ s ] = optimalParameters[ 2 ];
          negativeProduct *= ( 1.0f - optimalParameters[ 2 ] );
          csfProtonDensities[ s ] = optimalParameters[ 3 ];
          csfT2s[ s ] = optimalParameters[ 4 ];
          if ( _arteryCount > 0 )
          {
            
            csfFractions[ s ] = negativeProduct * optimalParameters[ 5 ];
            negativeProduct *= ( 1.0f - optimalParameters[ 5 ] );
            arteriesProtonDensities[ s ] = optimalParameters[ 6 ];
            for ( a = 0; a < _arteryCount - 1; a++ )
            {
              
              arteriesT2s[ a ][ s ] = optimalParameters[ 5 + 2 * ( a + 1 ) ];
              arteriesFractions[ a ][ s ] = negativeProduct * 
                                     optimalParameters[ 5 + 2 * ( a + 1 ) + 1 ];
              negativeProduct *= ( 1.0f - 
                                   optimalParameters[ 5 + 2 * ( a + 1 ) + 1 ] );
              
            }
            
            arteriesT2s[ _arteryCount - 1 ][ s ] = 
                                  optimalParameters[ 5 + 2 * ( _arteryCount ) ];
            arteriesFractions[ _arteryCount - 1 ][ s ] = negativeProduct;
            
          }
          else
          {
            
            csfFractions[ s ] = negativeProduct;
            
          }
          
        }
         
        else 
        {
          
          if ( _arteryCount > 0 )
          {
            
            parenchymaFractions[ s ] = optimalParameters[ 2 ];
            negativeProduct *= ( 1.0f - optimalParameters[ 2 ] );
            arteriesProtonDensities[ s ] = optimalParameters[ 3 ];
            for ( a = 0; a < _arteryCount - 1; a++ )
            {
            
              arteriesT2s[ a ][ s ] = optimalParameters[ 2 + 2 * ( a + 1 ) ];
              arteriesFractions[ a ][ s ] = negativeProduct * 
                                     optimalParameters[ 2 + 2 * ( a + 1 ) + 1 ];
              negativeProduct *= ( 1.0f - 
                                   optimalParameters[ 2 + 2 * ( a + 1 ) + 1 ] );
              
            }
            
            arteriesT2s[ _arteryCount - 1 ][ s ] = 
                                  optimalParameters[ 2 + 2 * ( _arteryCount ) ];
            arteriesFractions[ _arteryCount - 1 ][ s ] = negativeProduct;
            
          }
          
        }
        
        for ( t2WeightedVolumeIndex = 0; 
              t2WeightedVolumeIndex < _t2WeightedVolumeCount - 1; 
              t2WeightedVolumeIndex++ )
        {
          
          t2WeightedScalings[ t2WeightedVolumeIndex * count + s ] = 
                               optimalParameters[ _parameterCount - 
                                                  ( _t2WeightedVolumeCount - 1 )
                                                  + t2WeightedVolumeIndex ];
        
        }
        
      }
      
      else if ( _t1Relaxometry )
      {
        
        parenchymaT1s[ s ] = optimalParameters[ 1 ];

        if ( _hasCSF )
        {
          
          parenchymaFractions[ s ] = optimalParameters[ 2 ];
          negativeProduct *= ( 1.0f - optimalParameters[ 2 ] );
          csfProtonDensities[ s ] = optimalParameters[ 3 ];
          csfT1s[ s ] = optimalParameters[ 4 ];
          if ( _arteryCount > 0 )
          {
            
            csfFractions[ s ] = negativeProduct * optimalParameters[ 5 ];
            negativeProduct *= ( 1.0f - optimalParameters[ 5 ] );
            arteriesProtonDensities[ s ] = optimalParameters[ 6 ];
            for ( a = 0; a < _arteryCount - 1; a++ )
            {
              
              arteriesT2s[ a ][ s ] = optimalParameters[ 5 + 2 * ( a + 1 ) ];
              arteriesFractions[ a ][ s ] = negativeProduct * 
                                     optimalParameters[ 5 + 2 * ( a + 1 ) + 1 ];
              negativeProduct *= ( 1.0f - 
                                   optimalParameters[ 5 + 2 * ( a + 1 ) + 1 ] );
              
            }
            
            arteriesT1s[ _arteryCount - 1 ][ s ] = 
                                  optimalParameters[ 5 + 2 * ( _arteryCount ) ];
            arteriesFractions[ _arteryCount - 1 ][ s ] = negativeProduct;
            
          }
          else
          {
            
            csfFractions[ s ] = negativeProduct;
            
          }
          
        }
         
        else 
        {
          
          if ( _arteryCount > 0 )
          {
            
            parenchymaFractions[ s ] = optimalParameters[ 2 ];
            negativeProduct *= ( 1.0f - optimalParameters[ 2 ] );
            arteriesProtonDensities[ s ] = optimalParameters[ 3 ];
            for ( a = 0; a < _arteryCount - 1; a++ )
            {
            
              arteriesT1s[ a ][ s ] = optimalParameters[ 2 + 2 * ( a + 1 ) ];
              arteriesFractions[ a ][ s ] = negativeProduct * 
                                     optimalParameters[ 2 + 2 * ( a + 1 ) + 1 ];
              negativeProduct *= ( 1.0f - 
                                   optimalParameters[ 2 + 2 * ( a + 1 ) + 1 ] );
              
            }
            
            arteriesT1s[ _arteryCount - 1 ][ s ] = 
                                  optimalParameters[ 2 + 2 * ( _arteryCount ) ];
            arteriesFractions[ _arteryCount - 1 ][ s ] = negativeProduct;
            
          }
          
        }
        
        for ( t1WeightedVolumeIndex = 0; 
              t1WeightedVolumeIndex < _t1WeightedVolumeCount - 1; 
              t1WeightedVolumeIndex++ )
        {
          
          t1WeightedScalings[ t1WeightedVolumeIndex * count + s ] = 
                               optimalParameters[ _parameterCount - 
                                                  ( _t1WeightedVolumeCount - 1 )
                                                  + t1WeightedVolumeIndex ];
        
        }
        
      }
      
      
      //////////////////////////////////////////////////////////////////////////
      // computing fitted data 
      //////////////////////////////////////////////////////////////////////////
      
      if ( _computeFittedData ) 
      {
        
        fittedMeasurements[ s ].reallocate( _globalT2WeightedMeasurementCount +
                                             _globalT1WeightedMeasurementCount);
        multiCompartmentRelaxometryMCMCFunction->getValuesAt( 
                                               gkg::Vector( optimalParameters ), 
                                               fittedMeasurements[ s ] );
      
      }
      
    }
    

    ////////////////////////////////////////////////////////////////////////////
    // copying results to output volumes 
    ////////////////////////////////////////////////////////////////////////////
    
    lock(); 
    
    int32_t m = 0; 
    int32_t measurementCount = _globalT2WeightedMeasurementCount + 
                               _globalT1WeightedMeasurementCount; 
    for ( s = 0; s < count; s++ )
    {
      
      const gkg::Vector3d< int32_t >& site = sitePtr[ s ];
      _parenchymaFractionVolume( site ) = parenchymaFractions[ s ];
      _parenchymaProtonDensityVolume( site ) = parenchymaProtonDensities[ s ];
      
      if ( _t2Relaxometry && _t1Relaxometry )
      {
        
        _parenchymaT1Volume( site ) = parenchymaT1s[ s ];
        _parenchymaT2Volume( site ) = parenchymaT2s[ s ];
        
        if ( _hasCSF )
        {
          
          _csfFractionVolume( site ) = csfFractions[ s ];
          _csfProtonDensityVolume( site ) = csfProtonDensities[ s ];
          _csfT1Volume( site ) = csfT1s[ s ];
          _csfT2Volume( site ) = csfT2s[ s ];
          
          if ( _arteryCount > 0 )
          {
            
            _arteriesProtonDensityVolume( site ) = arteriesProtonDensities[ s ];
            for ( a = 0; a < _arteryCount; a++ )
            {
              
              _arteriesFractionVolumes[ a ]( site ) = 
                                                    arteriesFractions[ a ][ s ];
              _arteriesT1Volumes[ a ]( site ) = arteriesT1s[ a ][ s ];
              _arteriesT2Volumes[ a ]( site ) = arteriesT2s[ a ][ s ];
              
            }
            
          }
          
        }
        
        else
        {
          
          if ( _arteryCount > 0 )
          {
            
            _arteriesProtonDensityVolume( site ) = arteriesProtonDensities[ s ];
            for ( a = 0; a < _arteryCount; a++ )
            {
              
              _arteriesFractionVolumes[ a ]( site ) = 
                                                    arteriesFractions[ a ][ s ];
              _arteriesT1Volumes[ a ]( site ) = arteriesT1s[ a ][ s ];
              _arteriesT2Volumes[ a ]( site ) = arteriesT2s[ a ][ s ];
              
            }
            
          }          
          
        }
        for ( t2WeightedVolumeIndex = 0; 
              t2WeightedVolumeIndex < _t2WeightedVolumeCount - 1; 
              t2WeightedVolumeIndex++ )
        {
          
          _t2WeightedScalingVolume( site, t2WeightedVolumeIndex ) = 
                        t2WeightedScalings[ t2WeightedVolumeIndex * count + s ];
        
        }
        for ( t1WeightedVolumeIndex = 0; 
              t1WeightedVolumeIndex < _t1WeightedVolumeCount; 
              t1WeightedVolumeIndex++ )
        {
          
          _t1WeightedScalingVolume( site, t1WeightedVolumeIndex ) = 
                        t1WeightedScalings[ t1WeightedVolumeIndex * count + s ];
        
        }
        
      }
      
      else if ( _t2Relaxometry )
      {
        
        _parenchymaT2Volume( site ) = parenchymaT2s[ s ];
        
        if ( _hasCSF )
        {
          
          _csfFractionVolume( site ) = csfFractions[ s ];
          _csfProtonDensityVolume( site ) = csfProtonDensities[ s ];
          _csfT2Volume( site ) = csfT2s[ s ];
          
          if ( _arteryCount > 0 )
          {
            
            _arteriesProtonDensityVolume( site ) = arteriesProtonDensities[ s ];
            for ( a = 0; a < _arteryCount; a++ )
            {
              
              _arteriesFractionVolumes[ a ]( site ) = 
                                                    arteriesFractions[ a ][ s ];
              _arteriesT2Volumes[ a ]( site ) = arteriesT2s[ a ][ s ];
              
            }
            
          }
          
        }
        
        else
        {
          
          if ( _arteryCount > 0 )
          {
            
            _arteriesProtonDensityVolume( site ) = arteriesProtonDensities[ s ];
            for ( a = 0; a < _arteryCount; a++ )
            {
              
              _arteriesFractionVolumes[ a ]( site ) = 
                                                    arteriesFractions[ a ][ s ];
              _arteriesT2Volumes[ a ]( site ) = arteriesT2s[ a ][ s ];
              
            }
            
          }          
          
        }
        for ( t2WeightedVolumeIndex = 0; 
              t2WeightedVolumeIndex < _t2WeightedVolumeCount - 1; 
              t2WeightedVolumeIndex++ )
        {
          
          _t2WeightedScalingVolume( site, t2WeightedVolumeIndex ) = 
                        t2WeightedScalings[ t2WeightedVolumeIndex * count + s ];
        
        }
        
      }
      
      else if ( _t1Relaxometry )
      {
        
        _parenchymaT1Volume( site ) = parenchymaT1s[ s ];
        
        if ( _hasCSF )
        {
          
          _csfFractionVolume( site ) = csfFractions[ s ];
          _csfProtonDensityVolume( site ) = csfProtonDensities[ s ];
          _csfT1Volume( site ) = csfT1s[ s ];
          
          if ( _arteryCount > 0 )
          {
            
            _arteriesProtonDensityVolume( site ) = arteriesProtonDensities[ s ];
            for ( a = 0; a < _arteryCount; a++ )
            {
              
              _arteriesFractionVolumes[ a ]( site ) = 
                                                    arteriesFractions[ a ][ s ];
              _arteriesT1Volumes[ a ]( site ) = arteriesT1s[ a ][ s ];
              
            }
            
          }
          
        }
        
        else
        {
          
          if ( _arteryCount > 0 )
          {
            
            _arteriesProtonDensityVolume( site ) = arteriesProtonDensities[ s ];
            for ( a = 0; a < _arteryCount; a++ )
            {
              
              _arteriesFractionVolumes[ a ]( site ) = 
                                                    arteriesFractions[ a ][ s ];
              _arteriesT1Volumes[ a ]( site ) = arteriesT1s[ a ][ s ];
              
            }
            
          }          
          
        }
        for ( t1WeightedVolumeIndex = 0; 
              t1WeightedVolumeIndex < _t1WeightedVolumeCount - 1; 
              t1WeightedVolumeIndex++ )
        {
          
          _t1WeightedScalingVolume( site, t1WeightedVolumeIndex ) = 
                        t1WeightedScalings[ t1WeightedVolumeIndex * count + s ];
        
        }
        
      }
      
      // computed fitted data
      if ( _computeFittedData )
      {
        
        const gkg::Vector& currentFittedMeasurements = fittedMeasurements[ s ];
        for ( m = 0; m < measurementCount; m++ )
        {
          
          _fittedDataVolume( site, m ) = 
                                        ( float )currentFittedMeasurements( m );
        
        }
      
      }
      
    }
    
    if ( _verbose && _gauge )
    {
      
      _gauge->add( count % 100 );
      
    }
    
    unlock();
    
  }
  GKG_CATCH( "void gkg::MultiCompartmentRelaxometryMapperLoopContext::doIt( "
             "int32_t startIndex, "
             "int32_t count )" );

}

