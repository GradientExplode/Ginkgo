#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdf.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberNLPFunction.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainEstimator.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-processing-algobase/MaximumIndexFilter_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


#define EPSILON 1e-6


gkg::AxCaliberOdf::AxCaliberOdf(
     const gkg::OrientationSet* orientationSet,
     const gkg::Volume< float >& t2,
     const gkg::Volume< float >& dw,
     const gkg::Vector3d< int32_t >& site,
     gkg::RCPointer< gkg::AxCaliberMCMCFunction > axCaliberMCMCFunction,
     gkg::RCPointer< gkg::AxCaliberMCMCParameterSet > axCaliberMCMCParameterSet,
     const gkg::Vector& initialParameters,
     bool applyQuickSearchOptimizer,
     int32_t quicksearchStepCount,
     bool applyNlpOptimizer,
     int32_t nlpMaximumIterationCount,
     double nlpMaximumSizeOfTheSimplex,
     bool applyMcmcOptimizer,
     int32_t mcmcBurninCount,
     int32_t mcmcSampleCount,
     int32_t mcmcIntervalCount,
     int32_t mcmcMaximumIterationCount )
                  : gkg::OrientationDistributionFunction(
                                gkg::OrientationDistributionFunction::AxCaliber,
                                orientationSet ),
                    _axCaliberMCMCParameterSet( axCaliberMCMCParameterSet )
{

  try
  {

    if ( _orientationSet )
    {

      int32_t t2Count = t2.getSizeT();
      int32_t measurementCount = dw.getSizeT();
      int32_t t = 0;

      // processing average T2 value
      float averageT2 = ( float )t2( site, 0 );
      for ( t = 1; t < t2Count; t++ )
      {

        averageT2 += ( float )t2( site, t );

      }
      averageT2 /= t2Count;
      if ( averageT2 <= EPSILON )
      {

        averageT2 = EPSILON;

      }

      // building normalized signal vector
      gkg::Vector normalizedSignal( measurementCount );
      for ( t = 0; t < measurementCount; t++ )
      {

        float dwValue = dw( site, t );
        normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;

      }


      //////////////////////////////////////////////////////////////////////////
      // degenerated signal
      //////////////////////////////////////////////////////////////////////////
      if ( averageT2 <= EPSILON )
      {

        _values.resize( 6 );

        int32_t s;
        for ( s = 0; s < 6; s++ )
        {

          _values[ s ] = initialParameters( s );

        }
        // attributing the voxel to hindered compartment
        _values[ 0 ] = 1.0;

      }
      else
      {

        ////////////////////////////////////////////////////////////////////////
        // making a copy if input initial parameters
        ////////////////////////////////////////////////////////////////////////
        gkg::Vector copyOfInitialParameters( initialParameters );


        ////////////////////////////////////////////////////////////////////////
        // creating the NLP function if QuickSearch or NLP optimizer selected
        ////////////////////////////////////////////////////////////////////////
        gkg::RCPointer< gkg::AxCaliberNLPFunction > axCaliberNLPFunction;
        if ( applyQuickSearchOptimizer || applyNlpOptimizer )
        {

          axCaliberNLPFunction.reset( new gkg::AxCaliberNLPFunction(
                                             axCaliberMCMCFunction,
                                             normalizedSignal ) );

        }

        ////////////////////////////////////////////////////////////////////////
        // running the Quick Search  optimizer if selected
        ////////////////////////////////////////////////////////////////////////
        if ( applyQuickSearchOptimizer )
        {

          const gkg::Vector&
            lowerParameters = _axCaliberMCMCParameterSet->getLowerBounds();
          const gkg::Vector&
            upperParameters = _axCaliberMCMCParameterSet->getUpperBounds();

          int32_t p1 = 0;
          int32_t p2 = 0;
          int32_t p3 = 0;
          int32_t p4 = 0;
          double optimalP1 = 0.0;
          double optimalP2 = 0.0;
          double optimalP3 = 0.0;
          double optimalP4 = 0.0;
          double optimalMinusLogLikelihood = 1e38;
          double minusLogLikelihood = 0.0;
      	  double delta1 = ( upperParameters( 0 ) - lowerParameters( 0 ) ) /
                          ( double )quicksearchStepCount;
	        double delta2 = ( upperParameters( 1 ) - lowerParameters( 1 ) ) /
                          ( double )quicksearchStepCount;
	        double delta3 = ( upperParameters( 4 ) - lowerParameters( 4 ) ) /
                          ( double )quicksearchStepCount;
      	  double delta4 = ( upperParameters( 5 ) - lowerParameters( 5 ) ) /
                          ( double )quicksearchStepCount;

          const std::vector< bool >&
            fixedParameters = _axCaliberMCMCParameterSet->getFixed();

          int32_t quicksearchStepCount1 = ( fixedParameters[ 0 ] ? 1:
                                            quicksearchStepCount );
          int32_t quicksearchStepCount2 = ( fixedParameters[ 1 ] ? 1:
                                            quicksearchStepCount );
          int32_t quicksearchStepCount3 = ( fixedParameters[ 4 ] ? 1:
                                            quicksearchStepCount );
          int32_t quicksearchStepCount4 = ( fixedParameters[ 5 ] ? 1:
                                            quicksearchStepCount );

          gkg::Vector parameters( initialParameters );
          for ( p4 = 0; p4 < quicksearchStepCount4; p4++ )
          {

            if ( !fixedParameters[ 5 ] )
            {

              parameters( 5 ) = lowerParameters( 5 ) + p4 * delta4;

            }


            for ( p3 = 0; p3 < quicksearchStepCount3; p3++ )
            {

              if ( !fixedParameters[ 4 ] )
              {

                parameters( 4 ) = lowerParameters( 4 ) + p3 * delta3;

              }

              for ( p2 = 0; p2 < quicksearchStepCount2; p2++ )
              {

                if ( !fixedParameters[ 1 ] )
                {

                  parameters( 1 ) = lowerParameters( 1 ) + p2 * delta2;

                }

                for ( p1 = 0; p1 < quicksearchStepCount1; p1++ )
                {

                  if ( !fixedParameters[ 0 ] )
                  {

                    parameters( 0 ) = lowerParameters( 0 ) + p1 * delta1;

                  }

                  minusLogLikelihood = axCaliberNLPFunction->getValueAt( 
                                                                 parameters );
                  if ( minusLogLikelihood < optimalMinusLogLikelihood )
                  {

                     optimalP1 = parameters( 0 );
                     optimalP2 = parameters( 1 );
                     optimalP3 = parameters( 4 );
                     optimalP4 = parameters( 5 );
                     optimalMinusLogLikelihood = minusLogLikelihood;

                  }

                }

              }

            }

          }
          // replacing initial parameters by those found using the quick 
          // search
          copyOfInitialParameters( 0 ) = optimalP1;
          copyOfInitialParameters( 1 ) = optimalP2;
          copyOfInitialParameters( 4 ) = optimalP3;
          copyOfInitialParameters( 5 ) = optimalP4;

        }

        ////////////////////////////////////////////////////////////////////////
        // running the constrained NLP optimizer if selected
        ////////////////////////////////////////////////////////////////////////
        if ( applyNlpOptimizer )
        {

          gkg::NumericalAnalysisImplementationFactory*
            factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

          const gkg::Vector&
            lowerParameters = _axCaliberMCMCParameterSet->getLowerBounds();
          const gkg::Vector&
            upperParameters = _axCaliberMCMCParameterSet->getUpperBounds();
          const gkg::Vector&
            deltaParameters = _axCaliberMCMCParameterSet->getSteps();
          const std::vector< bool >&
            fixedParameters = _axCaliberMCMCParameterSet->getFixed();

          std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >
            constraints( 6 );

          constraints[ 0 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 0 ),
                                                     upperParameters( 0 ) ) );
          constraints[ 1 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 1 ),
                                                     upperParameters( 1 ) ) );
          constraints[ 2 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 2 ),
                                                     upperParameters( 2 ) ) );
          constraints[ 3 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 3 ),
                                                     upperParameters( 3 ) ) );
          constraints[ 4 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 4 ),
                                                     upperParameters( 4 ) ) );
          constraints[ 5 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 5 ),
                                                     upperParameters( 5 ) ) );

          std::vector< double > nlpOptimizerParameters( 8 );
          nlpOptimizerParameters[ 0 ] = nlpMaximumIterationCount;
          nlpOptimizerParameters[ 1 ] = nlpMaximumSizeOfTheSimplex;
          nlpOptimizerParameters[ 2 ] = ( fixedParameters[ 0 ] ? 0.0 :
                              constraints[ 0 ]->constrainedDeltaParameter(
                                                     deltaParameters( 0 ) ) );
          nlpOptimizerParameters[ 3 ] = ( fixedParameters[ 1 ] ? 0.0 :
                              constraints[ 1 ]->constrainedDeltaParameter(
                                                     deltaParameters( 1 ) ) );
          nlpOptimizerParameters[ 4 ] = ( fixedParameters[ 2 ] ? 0.0 :
                              constraints[ 2 ]->constrainedDeltaParameter(
                                                     deltaParameters( 2 ) ) );
          nlpOptimizerParameters[ 5 ] = ( fixedParameters[ 3 ] ? 0.0 :
                              constraints[ 3 ]->constrainedDeltaParameter(
                                                     deltaParameters( 3 ) ) );
          nlpOptimizerParameters[ 6 ] = ( fixedParameters[ 4 ] ? 0.0 :
                              constraints[ 4 ]->constrainedDeltaParameter(
                                                     deltaParameters( 4 ) ) );
          nlpOptimizerParameters[ 7 ] = ( fixedParameters[ 5 ] ? 0.0 :
                              constraints[ 5 ]->constrainedDeltaParameter(
                                                     deltaParameters( 5 ) ) );

          factory->getConstrainedNonLinearProgrammingOptimumParameters(
            gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
            copyOfInitialParameters,
            constraints,
            *axCaliberNLPFunction,
            nlpOptimizerParameters,
            false );

        }


        ////////////////////////////////////////////////////////////////////////
        // copying intermediate result to _values in case it is the last
        //  optimizer
        ////////////////////////////////////////////////////////////////////////
        _values.resize( 6 );

        int32_t s;
        for ( s = 0; s < 6; s++ )
        {

          _values[ s ] = copyOfInitialParameters( s );

        }


        ////////////////////////////////////////////////////////////////////////
        // running a MCMC estimator to get distributions of the parameters if
        // selected
        ////////////////////////////////////////////////////////////////////////
        if ( applyMcmcOptimizer )
        {

          gkg::Matrix parameterEstimates;
          int32_t updateCallCount = 0;
          double acceptanceRate = 0.0;
          gkg::MonteCarloMarkovChainEstimator
            monteCarloMarkovChainEstimator( axCaliberMCMCFunction,
                                            _axCaliberMCMCParameterSet );

          monteCarloMarkovChainEstimator.estimate( normalizedSignal,
                                                   copyOfInitialParameters,
                                                   parameterEstimates,
                                                   updateCallCount,
                                                   acceptanceRate,
                                                   mcmcBurninCount,
                                                   mcmcSampleCount,
                                                   mcmcIntervalCount,
                                                   1,
                                                   mcmcMaximumIterationCount,
                                                   false );

          // computing median values for each parameter distribution and 
          // storing
          // them into the _values field of the orientation distribution
          // function resizing output vector to parameter count
          int32_t parameterCount = parameterEstimates.getSize1();
          int32_t estimateCount = parameterEstimates.getSize2();
          _values.resize( parameterCount );
          gkg::MedianFilter< std::vector< float >, float > medianFilter;
          int32_t s;
          for ( s = 0; s < parameterCount; s++ )
          {

            std::vector< float > parameterValues( estimateCount );
            int32_t e = 0;
            for ( e = 0; e < estimateCount; e++ )
            {

              parameterValues[ e ] = parameterEstimates( s, e );

            }

            medianFilter.filter( parameterValues, _values[ s ] );

          }

        }

      }

    }

  }
  GKG_CATCH( "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg::RCPointer< gkg::AxCaliberMCMCFunction > "
             "axCaliberMCMCFunction, "
             "gkg::RCPointer< gkg::AxCaliberMCMCParameterSet > "
             "axCaliberMCMCParameterSet, "
             "const gkg::Vector& initialParameters, "
             "int32_t mcmcBurninCount, "
             "int32_t mcmcSampleCount, "
             "int32_t mcmcIntervalCount, "
             "int32_t mcmcMaximumIterationCount" );

}


gkg::AxCaliberOdf::AxCaliberOdf(
     const gkg::OrientationSet* orientationSet,
     const gkg::Volume< float >& t2,
     const gkg::Volume< float >& dw,
     const gkg::Vector3d< float >& site,
     gkg::RCPointer< gkg::AxCaliberMCMCFunction > axCaliberMCMCFunction,
     gkg::RCPointer< gkg::AxCaliberMCMCParameterSet > axCaliberMCMCParameterSet,
     const gkg::Vector& initialParameters,
     bool applyQuickSearchOptimizer,
     int32_t quicksearchStepCount,
     bool applyNlpOptimizer,
     int32_t nlpMaximumIterationCount,
     double nlpMaximumSizeOfTheSimplex,
     bool applyMcmcOptimizer,
     int32_t mcmcBurninCount,
     int32_t mcmcSampleCount,
     int32_t mcmcIntervalCount,
     int32_t mcmcMaximumIterationCount )
                  : gkg::OrientationDistributionFunction(
                                gkg::OrientationDistributionFunction::AxCaliber,
                                orientationSet ),
                    _axCaliberMCMCParameterSet( axCaliberMCMCParameterSet )
{

  try
  {

    if ( _orientationSet )
    {

      // getting pointer to quick partial voluming resampler
      gkg::PartialVolumingQuickResampler< float >& resampler =
        gkg::PartialVolumingQuickResampler< float >::getInstance();

      // collecting resolution of data
      gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
      if ( t2.getHeader().hasAttribute( "resolutionX" ) )
      {

        t2.getHeader().getAttribute( "resolutionX", resolution.x );

      }
      if ( t2.getHeader().hasAttribute( "resolutionY" ) )
      {

        t2.getHeader().getAttribute( "resolutionY", resolution.y );

      }
      if ( t2.getHeader().hasAttribute( "resolutionZ" ) )
      {

        t2.getHeader().getAttribute( "resolutionZ", resolution.z );

      }

      int32_t t2Count = t2.getSizeT();
      int32_t measurementCount = dw.getSizeT();
      int32_t t = 0;

      // processing average T2 value
      float t2Value;
      float averageT2;
      resampler.resample( t2,
                          ( float )0,
                          gkg::Vector3d< double >( site.x, site.y, site.z ),
                          t2Value,
                          &resolution,
                          0 );

      averageT2 = ( float )t2Value;
      for ( t = 1; t < t2Count; t++ )
      {

        resampler.resample( t2,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            t2Value,
                            &resolution,
                            t );
        averageT2 += ( float )t2Value;

      }
      averageT2 /= t2Count;
      if ( averageT2 <= EPSILON )
      {

        averageT2 = EPSILON;

      }

      // building normalized signal vector
      gkg::Vector normalizedSignal( measurementCount );
      float dwValue = 0;
      for ( t = 0; t < measurementCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;

      }

      //////////////////////////////////////////////////////////////////////////
      // degenerated signal
      //////////////////////////////////////////////////////////////////////////
      if ( averageT2 <= EPSILON )
      {

        _values.resize( 6 );

        int32_t s;
        for ( s = 0; s < 6; s++ )
        {

          _values[ s ] = initialParameters( s );

        }
        // attributing the voxel to CSF
        _values[ 0 ] = 1.0;

      }
      else
      {

        ////////////////////////////////////////////////////////////////////////
        // making a copy if input initial parameters
        ////////////////////////////////////////////////////////////////////////
        gkg::Vector copyOfInitialParameters( initialParameters );


        ////////////////////////////////////////////////////////////////////////
        // creating the NLP function if QuickSearch or NLP optimizer selected
        ////////////////////////////////////////////////////////////////////////
        gkg::RCPointer< gkg::AxCaliberNLPFunction > axCaliberNLPFunction;
        if ( applyQuickSearchOptimizer || applyNlpOptimizer )
        {

          axCaliberNLPFunction.reset( new gkg::AxCaliberNLPFunction(
                                             axCaliberMCMCFunction,
                                             normalizedSignal ) );

        }

        ////////////////////////////////////////////////////////////////////////
        // running the Quick Search  optimizer if selected
        ////////////////////////////////////////////////////////////////////////
        if ( applyQuickSearchOptimizer )
        {

          const gkg::Vector&
            lowerParameters = _axCaliberMCMCParameterSet->getLowerBounds();
          const gkg::Vector&
            upperParameters = _axCaliberMCMCParameterSet->getUpperBounds();

          int32_t p1 = 0;
          int32_t p2 = 0;
          int32_t p3 = 0;
          int32_t p4 = 0;
          double optimalP1 = 0.0;
          double optimalP2 = 0.0;
          double optimalP3 = 0.0;
          double optimalP4 = 0.0;
          double optimalMinusLogLikelihood = 1e38;
          double minusLogLikelihood = 0.0;
	  double delta1 = ( upperParameters( 0 ) - lowerParameters( 0 ) ) /
                          ( double )quicksearchStepCount;
	  double delta2 = ( upperParameters( 1 ) - lowerParameters( 1 ) ) /
                          ( double )quicksearchStepCount;
	  double delta3 = ( upperParameters( 4 ) - lowerParameters( 4 ) ) /
                          ( double )quicksearchStepCount;
	  double delta4 = ( upperParameters( 5 ) - lowerParameters( 5 ) ) /
                          ( double )quicksearchStepCount;

          const std::vector< bool >&
            fixedParameters = _axCaliberMCMCParameterSet->getFixed();

          int32_t quicksearchStepCount1 = ( fixedParameters[ 0 ] ? 1:
                                            quicksearchStepCount );
          int32_t quicksearchStepCount2 = ( fixedParameters[ 1 ] ? 1:
                                            quicksearchStepCount );
          int32_t quicksearchStepCount3 = ( fixedParameters[ 4 ] ? 1:
                                            quicksearchStepCount );
          int32_t quicksearchStepCount4 = ( fixedParameters[ 5 ] ? 1:
                                            quicksearchStepCount );

          gkg::Vector parameters( initialParameters );
          for ( p4 = 0; p4 < quicksearchStepCount4; p4++ )
          {

            if ( !fixedParameters[ 5 ] )
            {

              parameters( 5 ) = lowerParameters( 5 ) + p4 * delta4;

            }


            for ( p3 = 0; p3 < quicksearchStepCount3; p3++ )
            {

              if ( !fixedParameters[ 4 ] )
              {

                parameters( 4 ) = lowerParameters( 4 ) + p3 * delta3;

              }

              for ( p2 = 0; p2 < quicksearchStepCount2; p2++ )
              {

                if ( !fixedParameters[ 1 ] )
                {

                  parameters( 1 ) = lowerParameters( 1 ) + p2 * delta2;

                }

                for ( p1 = 0; p1 < quicksearchStepCount1; p1++ )
                {

                  if ( !fixedParameters[ 0 ] )
                  {

                    parameters( 0 ) = lowerParameters( 0 ) + p1 * delta1;

                  }

                  minusLogLikelihood = axCaliberNLPFunction->getValueAt( 
                                                                 parameters );
                  if ( minusLogLikelihood < optimalMinusLogLikelihood )
                  {

                     optimalP1 = parameters( 0 );
                     optimalP2 = parameters( 1 );
                     optimalP3 = parameters( 4 );
                     optimalP4 = parameters( 5 );
                     optimalMinusLogLikelihood = minusLogLikelihood;

                  }

                }

              }

            }

          }
          // replacing initial parameters by those found using the quick 
          // search
          copyOfInitialParameters( 0 ) = optimalP1;
          copyOfInitialParameters( 1 ) = optimalP2;
          copyOfInitialParameters( 4 ) = optimalP3;
          copyOfInitialParameters( 5 ) = optimalP4;

        }


        ////////////////////////////////////////////////////////////////////////
        // running the constrained NLP optimizer if selected
        ////////////////////////////////////////////////////////////////////////
        if ( applyNlpOptimizer )
        {

          gkg::NumericalAnalysisImplementationFactory*
            factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

          const gkg::Vector&
            lowerParameters = _axCaliberMCMCParameterSet->getLowerBounds();
          const gkg::Vector&
            upperParameters = _axCaliberMCMCParameterSet->getUpperBounds();
          const gkg::Vector&
            deltaParameters = _axCaliberMCMCParameterSet->getSteps();
          const std::vector< bool >&
            fixedParameters = _axCaliberMCMCParameterSet->getFixed();

          std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >
            constraints( 6 );

          constraints[ 0 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 0 ),
                                                     upperParameters( 0 ) ) );
          constraints[ 1 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 1 ),
                                                     upperParameters( 1 ) ) );
          constraints[ 2 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 2 ),
                                                     upperParameters( 2 ) ) );
          constraints[ 3 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 3 ),
                                                     upperParameters( 3 ) ) );
          constraints[ 4 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 4 ),
                                                     upperParameters( 4 ) ) );
          constraints[ 5 ].reset(
            new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( 5 ),
                                                     upperParameters( 5 ) ) );

          std::vector< double > nlpOptimizerParameters( 8 );
          nlpOptimizerParameters[ 0 ] = nlpMaximumIterationCount;
          nlpOptimizerParameters[ 1 ] = nlpMaximumSizeOfTheSimplex;
          nlpOptimizerParameters[ 2 ] = ( fixedParameters[ 0 ] ? 0.0 :
                                                       deltaParameters( 0 ) );
          nlpOptimizerParameters[ 3 ] = ( fixedParameters[ 1 ] ? 0.0 :
                                                       deltaParameters( 1 ) );
          nlpOptimizerParameters[ 4 ] = ( fixedParameters[ 2 ] ? 0.0 :
                                                       deltaParameters( 2 ) );
          nlpOptimizerParameters[ 5 ] = ( fixedParameters[ 3 ] ? 0.0 :
                                                       deltaParameters( 3 ) );
          nlpOptimizerParameters[ 6 ] = ( fixedParameters[ 4 ] ? 0.0 :
                                                       deltaParameters( 4 ) );
          nlpOptimizerParameters[ 7 ] = ( fixedParameters[ 5 ] ? 0.0 :
                                                       deltaParameters( 5 ) );

          factory->getConstrainedNonLinearProgrammingOptimumParameters(
            gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
            copyOfInitialParameters,
            constraints,
            *axCaliberNLPFunction,
            nlpOptimizerParameters,
            false );

        }


        ////////////////////////////////////////////////////////////////////////
        // copying intermediate result to _values in case it is the last
        //  optimizer
        ////////////////////////////////////////////////////////////////////////
        _values.resize( 6 );

        int32_t s;
        for ( s = 0; s < 6; s++ )
        {

          _values[ s ] = copyOfInitialParameters( s );

        }


        ////////////////////////////////////////////////////////////////////////
        // running a MCMC estimator to get distributions of the parameters if
        // selected
        ////////////////////////////////////////////////////////////////////////
        if ( applyMcmcOptimizer )
        {

          gkg::Matrix parameterEstimates;
          int32_t updateCallCount = 0;
          double acceptanceRate = 0.0;
          gkg::MonteCarloMarkovChainEstimator
            monteCarloMarkovChainEstimator( axCaliberMCMCFunction,
                                            _axCaliberMCMCParameterSet );

          monteCarloMarkovChainEstimator.estimate( normalizedSignal,
                                                   copyOfInitialParameters,
                                                   parameterEstimates,
                                                   updateCallCount,
                                                   acceptanceRate,
                                                   mcmcBurninCount,
                                                   mcmcSampleCount,
                                                   mcmcIntervalCount,
                                                   1,
                                                   mcmcMaximumIterationCount,
                                                   false );

          // computing median values for each parameter distribution and 
          // storing
          // them into the _values field of the orientation distribution
          // function resizing output vector to parameter count
          int32_t parameterCount = parameterEstimates.getSize1();
          int32_t estimateCount = parameterEstimates.getSize2();
          _values.resize( parameterCount );
          gkg::MedianFilter< std::vector< float >, float > medianFilter;
          int32_t s;
          for ( s = 0; s < parameterCount; s++ )
          {

            std::vector< float > parameterValues( estimateCount );
            int32_t e = 0;
            for ( e = 0; e < estimateCount; e++ )
            {

              parameterValues[ e ] = parameterEstimates( s, e );

            }

            medianFilter.filter( parameterValues, _values[ s ] );

          }

        }

      }

    }

  }
  GKG_CATCH( "gkg::AxCaliberOdf::AxCaliberOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "gkg::RCPointer< gkg::AxCaliberMCMCFunction > "
             "axCaliberMCMCFunction, "
             "gkg::RCPointer< gkg::AxCaliberMCMCParameterSet > "
             "axCaliberMCMCParameterSet, "
             "const gkg::Vector& initialParameters, "
             "int32_t maximumCallaghanModelRootCount, "
             "int32_t maximumCallaghanModelFunctionCount, "
             "int32_t mcmcBurninCount, "
             "int32_t mcmcSampleCount, "
             "int32_t mcmcIntervalCount, "
             "int32_t mcmcMaximumIterationCount )" );

}


gkg::AxCaliberOdf::~AxCaliberOdf()
{
}


const gkg::RCPointer< gkg::AxCaliberMCMCParameterSet >& 
gkg::AxCaliberOdf::getAxCaliberMCMCParameterSet() const
{

  try
  {

    return _axCaliberMCMCParameterSet;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::AxCaliberMCMCParameterSet >& "
             "gkg::AxCaliberOdf::getAxCaliberMCMCParameterSet() const" );

}


#undef EPSILON
