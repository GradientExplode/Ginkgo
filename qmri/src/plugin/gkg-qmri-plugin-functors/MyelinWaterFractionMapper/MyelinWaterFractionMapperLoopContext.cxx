#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionMapperLoopContext.h>
#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionMapperGauge.h>
#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/VFASPGRBasedMyelinWaterFractionMCMCFunction.h>
#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/IRSEBasedMyelinWaterFractionMCMCFunction.h>
#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionNLPFunction.h>
#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionMCMCParameterSet.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainEstimator.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::MyelinWaterFractionMapperLoopContext::MyelinWaterFractionMapperLoopContext( 
           gkg::MyelinWaterFractionMapperGauge& gauge,
           const std::vector< Vector3d< int32_t > >& sites,
           const std::vector< gkg::Volume< float > >& t2WeightedVolumes,
           const gkg::Volume< float >& t1WeightedVolume,
           const bool& t1MappingUsingSPGR,
           const gkg::RCPointer< gkg::Volume< float > >& b1MappingVolumePointer,
           const std::vector< gkg::Vector >& echoTimes,
           const gkg::Vector& flipAngles,
           const gkg::Vector& repetitionTimes,
           const gkg::Vector& inversionTimes,
           const std::vector< double >& t2NoiseStandardDeviations,
           const double& t1NoiseStandardDeviation,
           const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >& 
                                                                    constraints,
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
           gkg::Volume< float >& myelinWaterFractionVolume,
           gkg::Volume< float >& myelinWaterT1Volume,
           gkg::Volume< float >& myelinWaterT2Volume,
           gkg::Volume< float >& grayWhiteFractionVolume,
           gkg::Volume< float >& grayWhiteT1Volume,
           gkg::Volume< float >& grayWhiteT2Volume,
           gkg::Volume< float >& csfFractionVolume,
           gkg::Volume< float >& csfT1Volume,
           gkg::Volume< float >& csfT2Volume,
           gkg::Volume< float >& protonDensityVolume,
           gkg::Volume< float >& protonDensityScalingVolume,
           gkg::Volume< float >& t2WeightedScalingVolume,
           const bool& computeFittedData,
           gkg::Volume< float >& fittedDataVolume,
           const bool& verbose )
                                       : gkg::LoopContext< int32_t >( &gauge ),
                                         _sites( sites ),
                                         _t2WeightedVolumes(
                                                            t2WeightedVolumes ),
                                         _t1WeightedVolume( t1WeightedVolume ),
                                         _t1MappingUsingSPGR( 
                                                           t1MappingUsingSPGR ),
                                         _b1MappingVolumePointer( 
                                                       b1MappingVolumePointer ),
                                         _echoTimes( echoTimes ),
                                         _flipAngles( flipAngles ),
                                         _repetitionTimes( repetitionTimes ),
                                         _inversionTimes( inversionTimes ),
                                         _t2NoiseStandardDeviations(
                                                    t2NoiseStandardDeviations ),
                                         _t1NoiseStandardDeviation(
                                                     t1NoiseStandardDeviation ),
                                         _constraints( constraints ),
                                         _nlpOptimizerParameters(
                                                       nlpOptimizerParameters ),
                                         _initialParameters(
                                                            initialParameters ),
                                         _lowerParameterBoundaries(
                                                     lowerParameterBoundaries ),
                                         _upperParameterBoundaries(
                                                     upperParameterBoundaries ),
                                         _isFixedParameters(
                                                            isFixedParameters ),
                                         _deltaParameters( deltaParameters ),
                                         _applyMCMC( applyMCMC ),
                                         _mcmcBurninCount( mcmcBurninCount ),
                                         _mcmcSampleCount( mcmcSampleCount ),
                                         _mcmcIntervalCount(
                                                            mcmcIntervalCount ),
                                         _mcmcMaximumIterationCount(
                                                    mcmcMaximumIterationCount ),

                                         _myelinWaterFractionVolume(
                                                    myelinWaterFractionVolume ),
                                         _myelinWaterT1Volume(
                                                          myelinWaterT1Volume ),
                                         _myelinWaterT2Volume(
                                                          myelinWaterT2Volume ),
                                         _grayWhiteFractionVolume(
                                                      grayWhiteFractionVolume ),
                                         _grayWhiteT1Volume(
                                                            grayWhiteT1Volume ),
                                         _grayWhiteT2Volume(
                                                            grayWhiteT2Volume ),
                                         _csfFractionVolume( 
                                                            csfFractionVolume ),
                                         _csfT1Volume( csfT1Volume ),
                                         _csfT2Volume( csfT2Volume ),
                                         _protonDensityVolume(
                                                          protonDensityVolume ),
                                         _protonDensityScalingVolume(
                                                   protonDensityScalingVolume ),
                                         _t2WeightedScalingVolume(
                                                      t2WeightedScalingVolume ),
                                         _computeFittedData(
                                                            computeFittedData ),
                                         _fittedDataVolume( fittedDataVolume ),
                                         _verbose( verbose )
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // precomputing T2-weighted and T1-weighted measurement count
    ////////////////////////////////////////////////////////////////////////////

    _t2WeightedVolumeCount = ( int32_t )t2WeightedVolumes.size();
    _t2WeightedMeasurementCounts.resize( _t2WeightedVolumeCount );
    _globalT2WeightedMeasurementCount = 0;
    int32_t t2WeightedVolumeIndex = 0;
    for ( t2WeightedVolumeIndex = 0;
          t2WeightedVolumeIndex < _t2WeightedVolumeCount;
          t2WeightedVolumeIndex++ )
    {

      _t2WeightedMeasurementCounts[ t2WeightedVolumeIndex ] =
                         _t2WeightedVolumes[ t2WeightedVolumeIndex ].getSizeT();
      _globalT2WeightedMeasurementCount +=
                          _t2WeightedMeasurementCounts[ t2WeightedVolumeIndex ];

    }
    _t1WeightedMeasurementCount = _t1WeightedVolume.getSizeT();


    ////////////////////////////////////////////////////////////////////////////
    // getting access to the numerical analysis factory
    ////////////////////////////////////////////////////////////////////////////

    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

  }
  GKG_CATCH( "gkg::MyelinWaterFractionMapperLoopContext::"
             "MyelinWaterFractionMapperLoopContext( "
             "gkg::MyelinWaterFractionMapperGauge& gauge, "
             "const std::vector< Vector3d< int32_t > >& sites, "
             "const std::vector< gkg::Volume< float > >& t2WeightedVolumes, "
             "const gkg::Volume< float >& t1WeightedVolume, "
             "const bool& t1MappingUsingSPGR, "
             "const gkg::RCPointer< gkg::Volume< float > >& "
             " b1MappingVolumePointer, "
             "const std::vector< gkg::Vector >& echoTimes, "
             "const gkg::Vector& flipAngles, "
             "const gkg::Vector& repetitionTimes, "
             "const gkg::Vector& inversionTimes, "
             "const std::vector< float >& t2NoiseStandardDeviations, "
             "const float& t1NoiseStandardDeviation, "
             "const std::vector< gkg::RCPointer< "
             " gkg::OptimizerConstraint > >& constraints, "
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
             "gkg::Volume< float >& myelinWaterFractionVolume, "
             "gkg::Volume< float >& myelinWaterT1Volume, "
             "gkg::Volume< float >& myelinWaterT2Volume, "
             "gkg::Volume< float >& grayWhiteFractionVolume, "
             "gkg::Volume< float >& grayWhiteT1Volume, "
             "gkg::Volume< float >& grayWhiteT2Volume, "
             "gkg::Volume< float >& csfFractionVolume, "
             "gkg::Volume< float >& csfT1Volume, "
             "gkg::Volume< float >& csfT2Volume, "
             "gkg::Volume< float >& protonDensityVolume, "
             "gkg::Volume< float >& protonDensityScalingVolume, "
             "gkg::Volume< float >& t2WeightedScalingVolume, "
             "const bool& computeFittedData, "
             "gkg::Volume< float >& fittedDataVolume, "
             "const bool& verbose )" );

}


gkg::MyelinWaterFractionMapperLoopContext::
                                         ~MyelinWaterFractionMapperLoopContext()
{



}


void gkg::MyelinWaterFractionMapperLoopContext::doIt( int32_t startIndex,
                                                      int32_t count )
{

  try
  {

    //////////////////////////////////////////////////////////////////////////
    // allocating a MCMC function
    //////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::MyelinWaterFractionMCMCFunction >
                                                myelinWaterFractionMCMCFunction;
    if ( _t1MappingUsingSPGR )
    {

      // in case of variable flip angle SPGR sequence
      myelinWaterFractionMCMCFunction.reset(
                new gkg::VFASPGRBasedMyelinWaterFractionMCMCFunction(
                                                  _echoTimes,
                                                  _flipAngles,
                                                  _repetitionTimes,
                                                  _t2NoiseStandardDeviations,
                                                  _t1NoiseStandardDeviation ) );

    }
    // in case of IR-SE sequence
    else
    {

      // in case of IR-SE sequence
      myelinWaterFractionMCMCFunction.reset(
                new gkg::IRSEBasedMyelinWaterFractionMCMCFunction(
                                                  _echoTimes,
                                                  _inversionTimes,
                                                  _t2NoiseStandardDeviations,
                                                  _t1NoiseStandardDeviation ) );

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating a MCMC parameter set
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::MyelinWaterFractionMCMCParameterSet >
      myelinWaterFractionMCMCParameterSet(
        new gkg::MyelinWaterFractionMCMCParameterSet( _lowerParameterBoundaries,
                                                      _upperParameterBoundaries,
                                                      _isFixedParameters,
                                                      _deltaParameters ) ); 


    ////////////////////////////////////////////////////////////////////////////
    // allocating an MCMC estimator
    ////////////////////////////////////////////////////////////////////////////

    gkg::MonteCarloMarkovChainEstimator
      monteCarloMarkovChainEstimator( myelinWaterFractionMCMCFunction,
                                      myelinWaterFractionMCMCParameterSet );


    ////////////////////////////////////////////////////////////////////////////
    // allocating a NLF function from the MCMC function
    ////////////////////////////////////////////////////////////////////////////

    gkg::MyelinWaterFractionNLPFunction 
              myelinWaterFractionNLPFunction( myelinWaterFractionMCMCFunction );


    ////////////////////////////////////////////////////////////////////////////
    // allocating temporary memory for results
    ////////////////////////////////////////////////////////////////////////////

    std::vector< float > myelinWaterFractions( count );
    std::vector< float > myelinWaterT1s( count );
    std::vector< float > myelinWaterT2s( count );
    std::vector< float > grayWhiteFractions( count );
    std::vector< float > grayWhiteT1s( count );
    std::vector< float > grayWhiteT2s( count );
    std::vector< float > csfFractions( count );
    std::vector< float > csfT1s( count );
    std::vector< float > csfT2s( count );
    std::vector< float > protonDensities( count );
    std::vector< float > protonDensityScalings( count );
    std::vector< float > t2WeightedScalings( count *
                                             ( _t2WeightedVolumeCount - 1 ) );
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
    int32_t t1Index = 0;
    gkg::Vector realMeasurements( _globalT2WeightedMeasurementCount +
                                  _t1WeightedMeasurementCount );
    int32_t updateCallCount = 0;
    double acceptanceRate = 0.0;
    gkg::Matrix parameterEstimates;
    gkg::MedianFilter< std::vector< float >, float > medianFilter;
    std::vector< float > optimalParameters( 10 + _t2WeightedVolumeCount - 1 );
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


      //////////////////////////////////////////////////////////////////////////
      // collecting the T1-weighted measurements
      //////////////////////////////////////////////////////////////////////////

      for ( t1Index = 0; t1Index < _t1WeightedMeasurementCount; t1Index++ )
      {

        realMeasurements( _globalT2WeightedMeasurementCount + t1Index ) =
                                           _t1WeightedVolume( site, t1Index );

      }


      //////////////////////////////////////////////////////////////////////////
      // feeding the NLP function with the real measurements
      //////////////////////////////////////////////////////////////////////////

      myelinWaterFractionNLPFunction.setRealMeasurements( realMeasurements );


      //////////////////////////////////////////////////////////////////////////
      // computing the b1 scaling
      //////////////////////////////////////////////////////////////////////////

      if ( _t1MappingUsingSPGR )
      {

        if ( !_b1MappingVolumePointer.isNull() )
        {

          static_cast< gkg::VFASPGRBasedMyelinWaterFractionMCMCFunction* >(
            myelinWaterFractionMCMCFunction.getPointer() )->setB1Scaling(
                                ( *_b1MappingVolumePointer )( site ) / 900.0f );

        }

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
        myelinWaterFractionNLPFunction,
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

        for ( p = 0; p < 10 + _t2WeightedVolumeCount - 1; p++ )
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

        for ( p = 0; p < 10 + _t2WeightedVolumeCount - 1; p++ )
        {

          optimalParameters[ p ] = copyOfInitialParameters( p );

        }

      }

      // copying results to output volumes
      myelinWaterFractions[ s ] = ( 1.0f - optimalParameters[ 5 ] ) *
                                  optimalParameters[ 0 ];
      myelinWaterT1s[ s ] = optimalParameters[ 1 ];
      myelinWaterT2s[ s ] = optimalParameters[ 2 ];
      grayWhiteFractions[ s ] = ( 1.0f - optimalParameters[ 5 ] ) *
                                ( 1.0f - optimalParameters[ 0 ] );
      grayWhiteT1s[ s ] = optimalParameters[ 3 ];
      grayWhiteT2s[ s ] = optimalParameters[ 4 ];
      csfFractions[ s ] = optimalParameters[ 5 ];
      csfT1s[ s ] = optimalParameters[ 6 ];
      csfT2s[ s ] = optimalParameters[ 7 ];
      protonDensities[ s ] = optimalParameters[ 8 ];
      protonDensityScalings[ s ] = optimalParameters[ 9 ];

      for ( t2WeightedVolumeIndex = 0;
            t2WeightedVolumeIndex < _t2WeightedVolumeCount - 1;
            t2WeightedVolumeIndex++ )
      {

        t2WeightedScalings[ t2WeightedVolumeIndex * count + s ] =
                                optimalParameters[ 10 + t2WeightedVolumeIndex ];

      }

      // computing fitted data
      if ( _computeFittedData )
      {

        fittedMeasurements[ s ].reallocate( _globalT2WeightedMeasurementCount +
                                            _t1WeightedMeasurementCount );
        myelinWaterFractionMCMCFunction->getValuesAt(
                                               gkg::Vector( optimalParameters ),
                                               fittedMeasurements[ s ] );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // copying results to output volumes
    ////////////////////////////////////////////////////////////////////////////

    lock();

    int32_t m =0;
    int32_t measurementCount = _globalT2WeightedMeasurementCount +
                               _t1WeightedMeasurementCount;
    for ( s = 0; s < count; s++ )
    {

      const gkg::Vector3d< int32_t >& site = sitePtr[ s ];

//      if ( ( myelinWaterT1s[ s ] > grayWhiteT1s[ s ] ) ||
//           ( myelinWaterT2s[ s ] > grayWhiteT2s[ s ] ) )

      if ( myelinWaterFractions[ s ] > grayWhiteFractions[ s ] )
      {

        _myelinWaterFractionVolume( site ) = grayWhiteFractions[ s ];
        _myelinWaterT1Volume( site ) = grayWhiteT1s[ s ];
        _myelinWaterT2Volume( site ) = grayWhiteT2s[ s ];
        _grayWhiteFractionVolume( site ) = myelinWaterFractions[ s ];
        _grayWhiteT1Volume( site ) = myelinWaterT1s[ s ];
        _grayWhiteT2Volume( site ) = myelinWaterT2s[ s ];
        _csfFractionVolume( site ) = csfFractions[ s ];
        _csfT1Volume( site ) = csfT1s[ s ];
        _csfT2Volume( site ) = csfT2s[ s ];
        _protonDensityVolume( site ) = protonDensities[ s ];
        _protonDensityScalingVolume( site ) = protonDensityScalings[ s ];
        for ( t2WeightedVolumeIndex = 0;
              t2WeightedVolumeIndex < _t2WeightedVolumeCount - 1;
              t2WeightedVolumeIndex++ )
        {

          _t2WeightedScalingVolume( site, t2WeightedVolumeIndex ) =
                        t2WeightedScalings[ t2WeightedVolumeIndex * count + s ];

        }

      }
      else
      {

        _myelinWaterFractionVolume( site ) = myelinWaterFractions[ s ];
        _myelinWaterT1Volume( site ) = myelinWaterT1s[ s ];
        _myelinWaterT2Volume( site ) = myelinWaterT2s[ s ];
        _grayWhiteFractionVolume( site ) = grayWhiteFractions[ s ];
        _grayWhiteT1Volume( site ) = grayWhiteT1s[ s ];
        _grayWhiteT2Volume( site ) = grayWhiteT2s[ s ];
        _csfFractionVolume( site ) = csfFractions[ s ];
        _csfT1Volume( site ) = csfT1s[ s ];
        _csfT2Volume( site ) = csfT2s[ s ];
        _protonDensityVolume( site ) = protonDensities[ s ];
        _protonDensityScalingVolume( site ) = protonDensityScalings[ s ];
        for ( t2WeightedVolumeIndex = 0;
              t2WeightedVolumeIndex < _t2WeightedVolumeCount - 1;
              t2WeightedVolumeIndex++ )
        {

          _t2WeightedScalingVolume( site, t2WeightedVolumeIndex ) =
                        t2WeightedScalings[ t2WeightedVolumeIndex * count + s ];

        }

      }

      // computing fitted data
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
  GKG_CATCH( "void gkg::MyelinWaterFractionMapperLoopContext::doIt( "
             "int32_t startIndex, "
             "int32_t count )" );

}

