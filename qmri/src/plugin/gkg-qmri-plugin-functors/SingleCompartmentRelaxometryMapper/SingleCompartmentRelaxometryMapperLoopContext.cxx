#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperLoopContext.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperGauge.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCFunctionFactory.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMCMCParameterSet.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryNLPFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainEstimator.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>



gkg::SingleCompartmentRelaxometryMapperLoopContext::
                                  SingleCompartmentRelaxometryMapperLoopContext(
  gkg::SingleCompartmentRelaxometryMapperGauge& gauge, 
  const std::vector< gkg::Volume< float > >& inputVolumes,
  const std::vector< gkg::Vector3d< int32_t > >& sites,
  const std::string& qMriMappingType,
  const std::vector< double >& optimizerParameters,
  const std::vector< double >& scalarParameters,
  const gkg::RCPointer< gkg::AcquisitionParameterSet >& acquisitionParameterSet,
  const int32_t& inputVolumeCount,
  const std::vector< int32_t >& measurementCounts,
  const int32_t& globalMeasurementCount,
  const bool& verbose,
  gkg::Volume< float >& protonDensityVolume,
  gkg::Volume< float >& relaxationTimeVolume,
  gkg::Volume< float >& scalingVolume,
  const bool& computeFittedMeasurements,
  gkg::Volume< float >& fittedMeasurementVolume )
    : gkg::LoopContext< int32_t >( &gauge ),
      _inputVolumes( inputVolumes ),
      _sites( sites ),
      _qMriMappingType( qMriMappingType ),
      _optimizerParameters( optimizerParameters ),
      _scalarParameters( scalarParameters ),
      _acquisitionParameterSet( acquisitionParameterSet ),
      _inputVolumeCount( inputVolumeCount ),
      _measurementCounts( measurementCounts ),
      _globalMeasurementCount( globalMeasurementCount ),
      _verbose( verbose ),
      _protonDensityVolume( protonDensityVolume ),
      _relaxationTimeVolume( relaxationTimeVolume ),
      _scalingVolume( scalingVolume ),
      _computeFittedMeasurements( computeFittedMeasurements ),
      _fittedMeasurementVolume( fittedMeasurementVolume )
{

  try
  {
  
    ////////////////////////////////////////////////////////////////////////////
    // getting access to the numerical analysis factory 
    ////////////////////////////////////////////////////////////////////////////
    
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();


    ////////////////////////////////////////////////////////////////////////////
    // initializing parameter boundaries, step and fixed information
    ////////////////////////////////////////////////////////////////////////////
    
    _initialParameters.reallocate( 2 + _inputVolumeCount - 1 );
    _initialParameters( 0 ) = _scalarParameters[ 0 ];
    _initialParameters( 1 ) = _scalarParameters[ 1 ];
    int32_t s = 0;
    for ( s = 1; s < _inputVolumeCount; s++ )
    {

      _initialParameters( 1 + s ) = _scalarParameters[ 1 + s ];


    }

    _lowerParameterBoundaries.reallocate( 2 + _inputVolumeCount - 1 );
    _lowerParameterBoundaries( 0 ) = _scalarParameters[ 2 +
                                                        _inputVolumeCount - 1 ];
    _lowerParameterBoundaries( 1 ) = _scalarParameters[ 3 +
                                                        _inputVolumeCount - 1 ];
    for ( s = 1; s < _inputVolumeCount; s++ )
    {

      _lowerParameterBoundaries( 1 + s ) =
                             _scalarParameters[ 3 + _inputVolumeCount - 1 + s ];


    }

    _upperParameterBoundaries.reallocate( 2 + _inputVolumeCount - 1 );
    _upperParameterBoundaries( 0 ) = _scalarParameters[ 
                                            4 + 2 * ( _inputVolumeCount - 1 ) ];
    _upperParameterBoundaries( 1 ) = _scalarParameters[
                                            5 + 2 * ( _inputVolumeCount - 1 ) ];
    for ( s = 1; s < _inputVolumeCount; s++ )
    {

      _upperParameterBoundaries( 1 + s ) =
                     _scalarParameters[ 5 + 2 * ( _inputVolumeCount - 1 ) + s ];


    }


    _deltaParameters.reallocate( 2 + _inputVolumeCount - 1 );
    _deltaParameters( 0 ) = _scalarParameters[
                                            6 + 3 * ( _inputVolumeCount - 1 ) ];
    _deltaParameters( 1 ) = _scalarParameters[
                                            7 + 3 * ( _inputVolumeCount - 1 ) ];
    for ( s = 1; s < _inputVolumeCount; s++ )
    {

      _deltaParameters( 1 + s ) =
                     _scalarParameters[ 7 + 3 * ( _inputVolumeCount - 1 ) + s ];


    }

    _isFixedParameters.resize( 2U + _inputVolumeCount - 1 );
    _isFixedParameters[ 0 ] = false; // force estimation of proton density
    _isFixedParameters[ 1 ] = false; // force estimation of relaxation time
    for ( s = 1; s < _inputVolumeCount; s++ )
    {

      _isFixedParameters[ 1 + s ] =
                     _scalarParameters[ 7 + 4 * ( _inputVolumeCount - 1 ) + s ];


    }


    ////////////////////////////////////////////////////////////////////////////
    // preparing NLP constraints and optimizer parameters
    ////////////////////////////////////////////////////////////////////////////
    
    _constraints.resize( 2U + _inputVolumeCount - 1 ); 
    _nlpOptimizerParameters.resize( 4U + _inputVolumeCount - 1 ); 

    _nlpOptimizerParameters[ 0 ] = _optimizerParameters[ 0 ]; 
    _nlpOptimizerParameters[ 1 ] = _optimizerParameters[ 1 ]; 

    int32_t p = 0; 
    for ( p = 0 ; p < 2 + _inputVolumeCount - 1; p++ )
    {
    
      _constraints[ p ].reset( new gkg::ClosedIntervalOptimizerConstraint( 
                                             _lowerParameterBoundaries( p ), 
                                             _upperParameterBoundaries( p ) ) );

      if ( _isFixedParameters[ p ] )
      {

        _nlpOptimizerParameters[ 2 + p ] = 
          _constraints[ p ]->constrainedDeltaParameter( 0.0 );

      }
      else
      {

        _nlpOptimizerParameters[ 2 + p ] = 
          _constraints[ p ]->constrainedDeltaParameter( _deltaParameters( p ) );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // preparing MCMC optimizer parameters
    ////////////////////////////////////////////////////////////////////////////
    
    _applyMCMC = ( ( ( int32_t )( _optimizerParameters[ 2 ] + 0.5 ) == 1 ) ? 
                                                                 true : false );
    _mcmcBurninCount = ( int32_t )( _optimizerParameters[ 3 ] + 0.5 );
    _mcmcSampleCount = ( int32_t )( _optimizerParameters[ 4 ] + 0.5 ); 
    _mcmcIntervalCount = ( int32_t )( _optimizerParameters[ 5 ] + 0.5 ); 
    _mcmcMaximumIterationCount = ( int32_t )( _optimizerParameters[ 6 ] + 0.5 );

  }
  GKG_CATCH( "gkg::SingleCompartmentRelaxometryMapperLoopContext::"
             "SingleCompartmentRelaxometryMapperLoopContext( "
             "gkg::SingleCompartmentRelaxometryMapperGauge& gauge, "
             "const std::vector< gkg::Volume< float > >& inputVolumes, "
             "const std::vector< gkg::Vector3d< int32_t > >& sites, "
             "const std::string& qMriMappingType, "
             "const std::vector< double >& optimizerParameters, "
             "const std::vector< double >& scalarParameters, "
             "const gkg::RCPointer< gkg::AcquisitionParameterSet >& "
             "acquisitionParameterSet, "
             "const bool& verbose )" );

}


gkg::SingleCompartmentRelaxometryMapperLoopContext::
                                ~SingleCompartmentRelaxometryMapperLoopContext()
{
}


void gkg::SingleCompartmentRelaxometryMapperLoopContext::doIt(
                                                             int32_t startIndex,
                                                             int32_t count )
{

  try
  {
    
    
    
    ////////////////////////////////////////////////////////////////////////////
    // allocating the adequate MCMC function
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double > 
      noiseStandardDeviations( _scalarParameters.begin() +
                               8U + 5 * ( _inputVolumeCount - 1 ),
                               _scalarParameters.end() );

    gkg::RCPointer< gkg::SingleCompartmentRelaxometryMCMCFunction > 
      singleCompartmentRelaxometryMCMCFunction(
        gkg::SingleCompartmentRelaxometryMCMCFunctionFactory::getInstance().
          create( _qMriMappingType,
                  _measurementCounts,
                  noiseStandardDeviations,
                  _acquisitionParameterSet ) );


    ////////////////////////////////////////////////////////////////////////////
    // allocating a MCMC parameter set 
    ////////////////////////////////////////////////////////////////////////////
        
    gkg::RCPointer< gkg::SingleCompartmentRelaxometryMCMCParameterSet > 
      singleCompartmentRelaxometryMCMCParameterSet( 
        new gkg::SingleCompartmentRelaxometryMCMCParameterSet( 
                                                      _lowerParameterBoundaries,
                                                      _upperParameterBoundaries,
                                                      _isFixedParameters, 
                                                      _deltaParameters ) ); 

    ////////////////////////////////////////////////////////////////////////////
    // allocating an MCMC estimator 
    ////////////////////////////////////////////////////////////////////////////
    
    gkg::MonteCarloMarkovChainEstimator 
      monteCarloMarkovChainEstimator(
                                 singleCompartmentRelaxometryMCMCFunction, 
                                 singleCompartmentRelaxometryMCMCParameterSet );
    
    ////////////////////////////////////////////////////////////////////////////
    // allocating a NLP function from the MCMC function 
    ////////////////////////////////////////////////////////////////////////////
    
    gkg::SingleCompartmentRelaxometryNLPFunction 
      singleCompartmentRelaxometryNLPFunction(
                                     singleCompartmentRelaxometryMCMCFunction );
    
    ////////////////////////////////////////////////////////////////////////////
    // allocating temporary memory for results 
    ////////////////////////////////////////////////////////////////////////////

    std::vector< float > protonDensities( count );
    std::vector< float > relaxationTimes( count );
    std::vector< std::vector< float > > scalings;

    if ( _inputVolumeCount > 1 )
    {

      scalings.resize( count );

    }
    std::vector< gkg::Vector > fittedMeasurements;
    if ( _computeFittedMeasurements ) 
    {

      fittedMeasurements.resize( count );

    }

    ////////////////////////////////////////////////////////////////////////////
    // looping over site(s) 
    ////////////////////////////////////////////////////////////////////////////
    
    int32_t inputVolumeIndex = 0;
    int32_t measurementIndex = 0;
    gkg::Vector realMeasurements( _globalMeasurementCount );

    int32_t updateCallCount = 0;
    double acceptanceRate = 0.0;
    gkg::Matrix parameterEstimates;
    gkg::MedianFilter< gkg::Vector, double > medianFilter;
    gkg::Vector optimalParameters( 2 +  _inputVolumeCount - 1 );
    std::vector< double > parameterValues( _mcmcSampleCount );
    int32_t p = 0;
    int32_t e = 0;
    int32_t i = 0;

    int32_t m = 0;
    int32_t s = 0;
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

      //////////////////////////////////////////////////////////////////////////
      // setting the current site
      //////////////////////////////////////////////////////////////////////////

      const gkg::Vector3d< int32_t >& site = _sites[ startIndex + s ];
      singleCompartmentRelaxometryMCMCFunction->setSite( site );


      //////////////////////////////////////////////////////////////////////////
      // collecting the measurements
      //////////////////////////////////////////////////////////////////////////

      int32_t m = 0;
      for ( inputVolumeIndex = 0;
            inputVolumeIndex < _inputVolumeCount;
            inputVolumeIndex++ )
      {

        for ( measurementIndex = 0;
              measurementIndex < _measurementCounts[ inputVolumeIndex ];
              measurementIndex++ )
        {

          realMeasurements( m ) =
                    _inputVolumes[ inputVolumeIndex ]( site, measurementIndex );

          ++ m;

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // feeding the NLP function with the real measurements
      //////////////////////////////////////////////////////////////////////////

      singleCompartmentRelaxometryNLPFunction.setRealMeasurements(
                                                             realMeasurements );
      

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
                singleCompartmentRelaxometryNLPFunction,
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

        for ( p = 0; p < 2 + _inputVolumeCount - 1; p++ )
        {
          
          for ( e = 0; e < _mcmcSampleCount; e++ )
          {
            
            parameterValues[ e ] = parameterEstimates( p, e ); 
            
          }
          medianFilter.filter( parameterValues, optimalParameters( p ) ); 
          
        }
      
      }
      else
      {
        
        for ( p = 0; p < 2 + _inputVolumeCount - 1; p++ )
        {
          
          optimalParameters( p ) = copyOfInitialParameters( p ); 
          
        }
        
      }


      //////////////////////////////////////////////////////////////////////////
      // copying results to vectors 
      //////////////////////////////////////////////////////////////////////////
      
      protonDensities[ s ] = ( float )optimalParameters( 0 );
      relaxationTimes[ s ] = ( float )optimalParameters( 1 );
      if ( _inputVolumeCount > 1 )
      {

        scalings[ s ].resize( _inputVolumeCount - 1 );
        for ( i = 0; i < _inputVolumeCount - 1; i++ )
        {

          scalings[ s ][ i ] = ( float )optimalParameters( 2 + i );

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // computing fitted data 
      //////////////////////////////////////////////////////////////////////////
      
      if ( _computeFittedMeasurements ) 
      {
        
        fittedMeasurements[ s ].reallocate( _globalMeasurementCount );
        singleCompartmentRelaxometryMCMCFunction->getValuesAt( 
                                               optimalParameters, 
                                               fittedMeasurements[ s ] );
      
      }
      
    }


    ////////////////////////////////////////////////////////////////////////////
    // copying results to output volumes 
    ////////////////////////////////////////////////////////////////////////////
    
    lock(); 
    
    for ( s = 0; s < count; s++ )
    {
      
      const gkg::Vector3d< int32_t >& site = _sites[ startIndex + s ];

      _protonDensityVolume( site ) = protonDensities[ s ];
      _relaxationTimeVolume( site ) = relaxationTimes[ s ];
      if ( _inputVolumeCount > 1 )
      {

        for ( i = 0; i < _inputVolumeCount - 1; i++ )
        {

          _scalingVolume( site, i ) = scalings[ s ][ i ];

        }

      }
      if ( _computeFittedMeasurements )
      {

        const gkg::Vector& currentFittedMeasurements = fittedMeasurements[ s ];
        for ( m = 0; m < _globalMeasurementCount; m++ )
        {
          
          _fittedMeasurementVolume( site, m ) = 
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
