#include <gkg-dmri-microstructure-noddi/NoddiOdf.h>
#include <gkg-dmri-microstructure-noddi/NoddiNLPFunction.h>
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


gkg::NoddiOdf::NoddiOdf(
             const gkg::OrientationSet* orientationSet,
             const gkg::Volume< float >& t2,
             const gkg::Volume< float >& dw,
             const gkg::Vector3d< int32_t >& site,
             gkg::RCPointer< gkg::NoddiMCMCFunction > noddiMCMCFunction,
             gkg::RCPointer< gkg::NoddiMCMCParameterSet > noddiMCMCParameterSet,
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
                                                   noddiMCMCFunction->getType(),
                                                   orientationSet ),
                _noddiMCMCParameterSet( noddiMCMCParameterSet )
{

  try
  {

    if ( _orientationSet )
    {

      int32_t t2Count = t2.getSizeT();
      int32_t orientationCount = dw.getSizeT();
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
      gkg::Vector normalizedSignal( orientationCount );
      for ( t = 0; t < orientationCount; t++ )
      {

        float dwValue = dw( site, t );
        normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;

      }


      //////////////////////////////////////////////////////////////////////////
      // degenerated signal
      //////////////////////////////////////////////////////////////////////////
      if ( averageT2 <= EPSILON )
      {

        if ( noddiMCMCFunction->getType() ==
             gkg::OrientationDistributionFunction::NoddiWatson )
        {

          _values.resize( 8 );

          int32_t s;
          for ( s = 0; s < 8; s++ )
          {

            _values[ s ] = initialParameters( s );

          }
          // attributing the voxel to CSF
          _values[ 0 ] = 1.0;
          _values[ 1 ] = 0.0;

        }
        else if ( noddiMCMCFunction->getType() ==
                  gkg::OrientationDistributionFunction::NoddiBingham )
        {

          _values.resize( 12 );

          int32_t s;
          for ( s = 0; s < 12; s++ )
          {

            _values[ s ] = initialParameters( s );

          }
          // attributing the voxel to CSF
          _values[ 0 ] = 1.0;
          _values[ 1 ] = 0.0;

        }

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
        gkg::RCPointer< gkg::NoddiNLPFunction > noddiNLPFunction;
        if ( applyQuickSearchOptimizer || applyNlpOptimizer )
        {

          noddiNLPFunction.reset( new gkg::NoddiNLPFunction(
                                                           noddiMCMCFunction,
                                                           normalizedSignal ) );

        }

        ////////////////////////////////////////////////////////////////////////
        // running the Quick Search  optimizer if selected
        ////////////////////////////////////////////////////////////////////////
        if ( applyQuickSearchOptimizer )
        {

          const gkg::Vector&
            lowerParameters = _noddiMCMCParameterSet->getLowerBounds();
          const gkg::Vector&
            upperParameters = _noddiMCMCParameterSet->getUpperBounds();

          if ( noddiMCMCFunction->getType() ==
               gkg::OrientationDistributionFunction::NoddiWatson )
          {

            int32_t p1 = 0;
            int32_t p2 = 0;
            int32_t p3 = 0;
            double optimalP1 = 0.0;
            double optimalP2 = 0.0;
            double optimalP3 = 0.0;
            double optimalMinusLogLikelihood = 1e38;
            double minusLogLikelihood = 0.0;
	    double delta0 = ( upperParameters( 0 ) - lowerParameters( 0 ) ) /
                            ( double )quicksearchStepCount;
	    double delta1 = ( upperParameters( 1 ) - lowerParameters( 1 ) ) /
                            ( double )quicksearchStepCount;
	    double delta2 = ( upperParameters( 2 ) - lowerParameters( 2 ) ) /
                            ( double )quicksearchStepCount;

            const std::vector< bool >&
              fixedParameters = _noddiMCMCParameterSet->getFixed();

            int32_t quicksearchStepCount1 = ( fixedParameters[ 0 ] ? 0:
                                              quicksearchStepCount );
            int32_t quicksearchStepCount2 = ( fixedParameters[ 1 ] ? 0:
                                              quicksearchStepCount );
            int32_t quicksearchStepCount3 = ( fixedParameters[ 2 ] ? 0:
                                              quicksearchStepCount );

            gkg::Vector parameters( initialParameters );
            for ( p3 = 0; p3 < quicksearchStepCount3; p3++ )
            {

              if ( !fixedParameters[ 2 ] )
              {

                parameters( 2 ) = lowerParameters( 2 ) + p3 * delta2;

              }

              for ( p2 = 0; p2 < quicksearchStepCount2; p2++ )
              {

                if ( !fixedParameters[ 1 ] )
                {

                  parameters( 1 ) = lowerParameters( 1 ) + p2 * delta1;

                }

                for ( p1 = 0; p1 < quicksearchStepCount1; p1++ )
                {

                  if ( !fixedParameters[ 0 ] )
                  {

                    parameters( 0 ) = lowerParameters( 0 ) + p1 * delta0;

                  }

                  minusLogLikelihood = noddiNLPFunction->getValueAt( 
                                                                   parameters );
                  if ( minusLogLikelihood < optimalMinusLogLikelihood )
                  {

                     optimalP1 = parameters( 0 );
                     optimalP2 = parameters( 1 );
                     optimalP3 = parameters( 2 );
                     optimalMinusLogLikelihood = minusLogLikelihood;

                  }

                }

              }

            }
            // replacing initial parameters by those found using the quick 
            // search
            copyOfInitialParameters( 0 ) = optimalP1;
            copyOfInitialParameters( 1 ) = optimalP2;
            copyOfInitialParameters( 2 ) = optimalP3;

          }
          else if ( noddiMCMCFunction->getType() ==
                    gkg::OrientationDistributionFunction::NoddiBingham )
          {

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
	    double delta0 = ( upperParameters( 0 ) - lowerParameters( 0 ) ) /
                            ( double )quicksearchStepCount;
	    double delta1 = ( upperParameters( 1 ) - lowerParameters( 1 ) ) /
                            ( double )quicksearchStepCount;
	    double delta2 = ( upperParameters( 2 ) - lowerParameters( 2 ) ) /
                            ( double )quicksearchStepCount;
	    double delta3 = ( upperParameters( 3 ) - lowerParameters( 3 ) ) /
                            ( double )quicksearchStepCount;

            const std::vector< bool >&
              fixedParameters = _noddiMCMCParameterSet->getFixed();

            int32_t quicksearchStepCount1 = ( fixedParameters[ 0 ] ? 0:
                                              quicksearchStepCount );
            int32_t quicksearchStepCount2 = ( fixedParameters[ 1 ] ? 0:
                                              quicksearchStepCount );
            int32_t quicksearchStepCount3 = ( fixedParameters[ 2 ] ? 0:
                                              quicksearchStepCount );
            int32_t quicksearchStepCount4 = ( fixedParameters[ 3 ] ? 0:
                                              quicksearchStepCount );

            gkg::Vector parameters( initialParameters );
            for ( p4 = 0; p4 < quicksearchStepCount4; p4++ )
            {

              if ( !fixedParameters[ 3 ] )
              {

                parameters( 3 ) = lowerParameters( 3 ) + p4 * delta3;

              }

              for ( p3 = 0; p3 < quicksearchStepCount3; p3++ )
              {

                if ( !fixedParameters[ 2 ] )
                {

                  parameters( 2 ) = lowerParameters( 2 ) + p3 * delta2;

                }

                for ( p2 = 0; p2 < quicksearchStepCount2; p2++ )
                {

                  if ( !fixedParameters[ 1 ] )
                  {

                    parameters( 1 ) = lowerParameters( 1 ) + p2 * delta1;

                  }

                  for ( p1 = 0; p1 < quicksearchStepCount1; p1++ )
                  {

                    if ( !fixedParameters[ 0 ] )
                    {

                      parameters( 0 ) = lowerParameters( 0 ) + p1 * delta0;

                    }

                    minusLogLikelihood = noddiNLPFunction->getValueAt(
                                                                   parameters );
                    if ( minusLogLikelihood < optimalMinusLogLikelihood )
                    {

                       optimalP1 = parameters( 0 );
                       optimalP2 = parameters( 1 );
                       optimalP3 = parameters( 2 );
                       optimalP4 = parameters( 3 );
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
            copyOfInitialParameters( 2 ) = optimalP3;
            copyOfInitialParameters( 3 ) = optimalP4;

          }

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
            lowerParameters = _noddiMCMCParameterSet->getLowerBounds();
          const gkg::Vector&
            upperParameters = _noddiMCMCParameterSet->getUpperBounds();
          const gkg::Vector&
            deltaParameters = _noddiMCMCParameterSet->getSteps();
          const std::vector< bool >&
            fixedParameters = _noddiMCMCParameterSet->getFixed();

          if ( noddiMCMCFunction->getType() ==
               gkg::OrientationDistributionFunction::NoddiWatson )
          {

            std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >
              constraints( 9 );

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
            constraints[ 6 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 6 ),
                                                       upperParameters( 6 ) ) );
            constraints[ 7 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 7 ),
                                                       upperParameters( 7 ) ) );
            constraints[ 8 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 8 ),
                                                       upperParameters( 8 ) ) );

            std::vector< double > nlpOptimizerParameters( 11 );
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
            nlpOptimizerParameters[ 8 ] = ( fixedParameters[ 6 ] ? 0.0 :
                                constraints[ 6 ]->constrainedDeltaParameter(
                                                       deltaParameters( 6 ) ) );
            nlpOptimizerParameters[ 9 ] = ( fixedParameters[ 7 ] ? 0.0 :
                                constraints[ 7 ]->constrainedDeltaParameter(
                                                       deltaParameters( 7 ) ) );
            nlpOptimizerParameters[ 10 ] = ( fixedParameters[ 8 ] ? 0.0 :
                                constraints[ 8 ]->constrainedDeltaParameter(
                                                       deltaParameters( 8 ) ) );

            factory->getConstrainedNonLinearProgrammingOptimumParameters(
              gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
              copyOfInitialParameters,
              constraints,
              *noddiNLPFunction,
              nlpOptimizerParameters,
              false );

          }
          else if ( noddiMCMCFunction->getType() ==
                    gkg::OrientationDistributionFunction::NoddiBingham )
          {

            std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >
              constraints( 13 );

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
            constraints[ 6 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 6 ),
                                                       upperParameters( 6 ) ) );
            constraints[ 7 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 7 ),
                                                       upperParameters( 7 ) ) );
            constraints[ 8 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 8 ),
                                                       upperParameters( 8 ) ) );
            constraints[ 9 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 9 ),
                                                       upperParameters( 9 ) ) );
            constraints[ 10 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                      lowerParameters( 10 ),
                                                      upperParameters( 10 ) ) );
            constraints[ 11 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                      lowerParameters( 11 ),
                                                      upperParameters( 11 ) ) );
            constraints[ 12 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                      lowerParameters( 12 ),
                                                      upperParameters( 12 ) ) );


            std::vector< double > nlpOptimizerParameters( 15 );
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
            nlpOptimizerParameters[ 8 ] = ( fixedParameters[ 6 ] ? 0.0 :
                                constraints[ 6 ]->constrainedDeltaParameter(
                                                       deltaParameters( 6 ) ) );
            nlpOptimizerParameters[ 9 ] = ( fixedParameters[ 7 ] ? 0.0 :
                                constraints[ 7 ]->constrainedDeltaParameter(
                                                       deltaParameters( 7 ) ) );
            nlpOptimizerParameters[ 10 ] = ( fixedParameters[ 8 ] ? 0.0 :
                                constraints[ 8 ]->constrainedDeltaParameter(
                                                       deltaParameters( 8 ) ) );
            nlpOptimizerParameters[ 11 ] = ( fixedParameters[ 9 ] ? 0.0 :
                                constraints[ 9 ]->constrainedDeltaParameter(
                                                       deltaParameters( 9 ) ) );
            nlpOptimizerParameters[ 12 ] = ( fixedParameters[ 10 ] ? 0.0 :
                                constraints[ 10 ]->constrainedDeltaParameter(
                                                      deltaParameters( 10 ) ) );
            nlpOptimizerParameters[ 13 ] = ( fixedParameters[ 11 ] ? 0.0 :
                                constraints[ 11 ]->constrainedDeltaParameter(
                                                      deltaParameters( 11 ) ) );
            nlpOptimizerParameters[ 14 ] = ( fixedParameters[ 12 ] ? 0.0 :
                                constraints[ 12 ]->constrainedDeltaParameter(
                                                      deltaParameters( 12 ) ) );

            factory->getConstrainedNonLinearProgrammingOptimumParameters(
              gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
              copyOfInitialParameters,
              constraints,
              *noddiNLPFunction,
              nlpOptimizerParameters,
              false );

          }

        }


        ////////////////////////////////////////////////////////////////////////
        // copying intermediate result to _values in case it is the last
        //  optimizer
        ////////////////////////////////////////////////////////////////////////
        if ( noddiMCMCFunction->getType() ==
             gkg::OrientationDistributionFunction::NoddiWatson )
        {

          _values.resize( 9 );

          int32_t s;
          for ( s = 0; s < 9; s++ )
          {

            _values[ s ] = copyOfInitialParameters( s );

          }
          gkg::Vector3d< float > principalOrientation( _values[ 3 ],
                                                       _values[ 4 ],
                                                       _values[ 5 ] );
          principalOrientation.normalize();
          _values[ 3 ] = principalOrientation.x;
          _values[ 4 ] = principalOrientation.y;
          _values[ 5 ] = principalOrientation.z;

        }
        else if ( noddiMCMCFunction->getType() ==
                  gkg::OrientationDistributionFunction::NoddiBingham )
        {

          _values.resize( 13 );

          int32_t s;
          for ( s = 0; s < 13; s++ )
          {

            _values[ s ] = copyOfInitialParameters( s );

          }
          gkg::Vector3d< float > principalOrientation( _values[ 3 ],
                                                       _values[ 4 ],
                                                       _values[ 5 ] );
          principalOrientation.normalize();
          _values[ 3 ] = principalOrientation.x;
          _values[ 4 ] = principalOrientation.y;
          _values[ 5 ] = principalOrientation.z;

          gkg::Vector3d< float > secondaryOrientation( _values[ 7 ],
                                                       _values[ 8 ],
                                                       _values[ 9 ] );
          secondaryOrientation.normalize();
          _values[ 7 ] = secondaryOrientation.x;
          _values[ 8 ] = secondaryOrientation.y;
          _values[ 9 ] = secondaryOrientation.z;

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
            monteCarloMarkovChainEstimator( noddiMCMCFunction,
                                            _noddiMCMCParameterSet );

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

          if ( noddiMCMCFunction->getType() ==
               gkg::OrientationDistributionFunction::NoddiWatson )
          {

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
            gkg::Vector3d< float > principalOrientation( _values[ 3 ],
                                                         _values[ 4 ],
                                                         _values[ 5 ] );
            principalOrientation.normalize();
            _values[ 3 ] = principalOrientation.x;
            _values[ 4 ] = principalOrientation.y;
            _values[ 5 ] = principalOrientation.z;

          }
          else if ( noddiMCMCFunction->getType() ==
                    gkg::OrientationDistributionFunction::NoddiBingham )
          {

            // computing median values for each parameter distribution and 
            // storing them into the _values field of the orientation 
            // distribution function resizing output vector to parameter count
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

            gkg::Vector3d< float > principalOrientation( _values[ 4 ],
                                                         _values[ 5 ],
                                                         _values[ 6 ] );
            principalOrientation.normalize();
            _values[ 4 ] = principalOrientation.x;
            _values[ 5 ] = principalOrientation.y;
            _values[ 6 ] = principalOrientation.z;

            gkg::Vector3d< float > secondaryOrientation( _values[ 7 ],
                                                         _values[ 8 ],
                                                         _values[ 9 ] );
            secondaryOrientation.normalize();
            _values[ 7 ] = secondaryOrientation.x;
            _values[ 8 ] = secondaryOrientation.y;
            _values[ 9 ] = secondaryOrientation.z;

          }

        }

      }

    }

  }
  GKG_CATCH( "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg::RCPointer< gkg::NoddiMCMCFunction > noddiMCMCFunction, "
             "gkg::RCPointer< gkg::NoddiMCMCParameterSet > "
             "noddiMCMCParameterSet, "
             "const gkg::Vector& initialParameters, "
             "int32_t mcmcBurninCount, "
             "int32_t mcmcSampleCount, "
             "int32_t mcmcIntervalCount, "
             "int32_t mcmcMaximumIterationCount" );

}


gkg::NoddiOdf::NoddiOdf(
             const gkg::OrientationSet* orientationSet,
             const gkg::Volume< float >& t2,
             const gkg::Volume< float >& dw,
             const gkg::Vector3d< float >& site,
             gkg::RCPointer< gkg::NoddiMCMCFunction > noddiMCMCFunction,
             gkg::RCPointer< gkg::NoddiMCMCParameterSet > noddiMCMCParameterSet,
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
                                                   noddiMCMCFunction->getType(),
                                                   orientationSet ),
                _noddiMCMCParameterSet( noddiMCMCParameterSet )
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
      int32_t orientationCount = dw.getSizeT();
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
      gkg::Vector normalizedSignal( orientationCount );
      float dwValue = 0;
      for ( t = 0; t < orientationCount; t++ )
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

        if ( noddiMCMCFunction->getType() ==
             gkg::OrientationDistributionFunction::NoddiWatson )
        {

          _values.resize( 9 );

          int32_t s;
          for ( s = 0; s < 9; s++ )
          {

            _values[ s ] = initialParameters( s );

          }
          // attributing the voxel to CSF
          _values[ 0 ] = 1.0;
          _values[ 1 ] = 0.0;

        }
        else if ( noddiMCMCFunction->getType() ==
                  gkg::OrientationDistributionFunction::NoddiBingham )
        {

          _values.resize( 13 );

          int32_t s;
          for ( s = 0; s < 13; s++ )
          {

            _values[ s ] = initialParameters( s );

          }
          // attributing the voxel to CSF
          _values[ 0 ] = 1.0;
          _values[ 1 ] = 0.0;

        }

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
        gkg::RCPointer< gkg::NoddiNLPFunction > noddiNLPFunction;
        if ( applyQuickSearchOptimizer || applyNlpOptimizer )
        {

          noddiNLPFunction.reset( new gkg::NoddiNLPFunction(
                                                           noddiMCMCFunction,
                                                           normalizedSignal ) );

        }

        ////////////////////////////////////////////////////////////////////////
        // running the Quick Search  optimizer if selected
        ////////////////////////////////////////////////////////////////////////
        if ( applyQuickSearchOptimizer )
        {

          const gkg::Vector&
            lowerParameters = _noddiMCMCParameterSet->getLowerBounds();
          const gkg::Vector&
            upperParameters = _noddiMCMCParameterSet->getUpperBounds();

          if ( noddiMCMCFunction->getType() ==
               gkg::OrientationDistributionFunction::NoddiWatson )
          {

            int32_t p1 = 0;
            int32_t p2 = 0;
            int32_t p3 = 0;
            double optimalP1 = 0.0;
            double optimalP2 = 0.0;
            double optimalP3 = 0.0;
            double optimalMinusLogLikelihood = 1e38;
            double minusLogLikelihood = 0.0;
	    double delta0 = ( upperParameters( 0 ) - lowerParameters( 0 ) ) /
                            ( double )quicksearchStepCount;
	    double delta1 = ( upperParameters( 1 ) - lowerParameters( 1 ) ) /
                            ( double )quicksearchStepCount;
	    double delta2 = ( upperParameters( 2 ) - lowerParameters( 2 ) ) /
                            ( double )quicksearchStepCount;

            for ( p3 = 0; p3 < quicksearchStepCount; p3++ )
            {

              for ( p2 = 0; p2 < quicksearchStepCount; p2++ )
              {

                for ( p1 = 0; p1 < quicksearchStepCount; p1++ )
                {

                  gkg::Vector parameters( initialParameters );
                  parameters( 0 ) = lowerParameters( 0 ) + p1 * delta0;
                  parameters( 1 ) = lowerParameters( 1 ) + p2 * delta1;
                  parameters( 2 ) = lowerParameters( 2 ) + p3 * delta2;

                  minusLogLikelihood = noddiNLPFunction->getValueAt( 
                                                                   parameters );
                  if ( minusLogLikelihood < optimalMinusLogLikelihood )
                  {

                     optimalP1 = parameters( 0 );
                     optimalP2 = parameters( 1 );
                     optimalP3 = parameters( 2 );
                     optimalMinusLogLikelihood = minusLogLikelihood;

                  }

                }

              }

            }
            // replacing initial parameters by those found using the quick 
            // search
            copyOfInitialParameters( 0 ) = optimalP1;
            copyOfInitialParameters( 1 ) = optimalP2;
            copyOfInitialParameters( 2 ) = optimalP3;

          }
          else if ( noddiMCMCFunction->getType() ==
                    gkg::OrientationDistributionFunction::NoddiBingham )
          {

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
	    double delta0 = ( upperParameters( 0 ) - lowerParameters( 0 ) ) /
                            ( double )quicksearchStepCount;
	    double delta1 = ( upperParameters( 1 ) - lowerParameters( 1 ) ) /
                            ( double )quicksearchStepCount;
	    double delta2 = ( upperParameters( 2 ) - lowerParameters( 2 ) ) /
                            ( double )quicksearchStepCount;
	    double delta3 = ( upperParameters( 3 ) - lowerParameters( 3 ) ) /
                            ( double )quicksearchStepCount;

            for ( p4 = 0; p4 < quicksearchStepCount; p4++ )
            {

              for ( p3 = 0; p3 < quicksearchStepCount; p3++ )
              {

                for ( p2 = 0; p2 < quicksearchStepCount; p2++ )
                {

                  for ( p1 = 0; p1 < quicksearchStepCount; p1++ )
                  {

                    gkg::Vector parameters( initialParameters );
                    parameters( 0 ) = lowerParameters( 0 ) + p1 * delta0;
                    parameters( 1 ) = lowerParameters( 1 ) + p2 * delta1;
                    parameters( 2 ) = lowerParameters( 2 ) + p3 * delta2;
                    parameters( 3 ) = lowerParameters( 3 ) + p4 * delta3;

                    minusLogLikelihood = noddiNLPFunction->getValueAt(
                                                                   parameters );
                    if ( minusLogLikelihood < optimalMinusLogLikelihood )
                    {

                       optimalP1 = parameters( 0 );
                       optimalP2 = parameters( 1 );
                       optimalP3 = parameters( 2 );
                       optimalP4 = parameters( 3 );
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
            copyOfInitialParameters( 2 ) = optimalP3;
            copyOfInitialParameters( 3 ) = optimalP4;

          }

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
            lowerParameters = _noddiMCMCParameterSet->getLowerBounds();
          const gkg::Vector&
            upperParameters = _noddiMCMCParameterSet->getUpperBounds();
          const gkg::Vector&
            deltaParameters = _noddiMCMCParameterSet->getSteps();
          const std::vector< bool >&
            fixedParameters = _noddiMCMCParameterSet->getFixed();

          if ( noddiMCMCFunction->getType() ==
               gkg::OrientationDistributionFunction::NoddiWatson )
          {

            std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >
              constraints( 9 );

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
            constraints[ 6 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 6 ),
                                                       upperParameters( 6 ) ) );
            constraints[ 7 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 7 ),
                                                       upperParameters( 7 ) ) );
            constraints[ 8 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 8 ),
                                                       upperParameters( 8 ) ) );

            std::vector< double > nlpOptimizerParameters( 11 );
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
            nlpOptimizerParameters[ 8 ] = ( fixedParameters[ 6 ] ? 0.0 :
                                                         deltaParameters( 6 ) );
            nlpOptimizerParameters[ 9 ] = ( fixedParameters[ 7 ] ? 0.0 :
                                                         deltaParameters( 7 ) );

            nlpOptimizerParameters[ 10 ] = ( fixedParameters[ 8 ] ? 0.0 :
                                                         deltaParameters( 8 ) );
            factory->getConstrainedNonLinearProgrammingOptimumParameters(
              gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
              copyOfInitialParameters,
              constraints,
              *noddiNLPFunction,
              nlpOptimizerParameters,
              false );

          }
          else if ( noddiMCMCFunction->getType() ==
                    gkg::OrientationDistributionFunction::NoddiBingham )
          {

            std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >
              constraints( 13 );

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
            constraints[ 6 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 6 ),
                                                       upperParameters( 6 ) ) );
            constraints[ 7 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 7 ),
                                                       upperParameters( 7 ) ) );
            constraints[ 8 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 8 ),
                                                       upperParameters( 8 ) ) );
            constraints[ 9 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 9 ),
                                                       upperParameters( 9 ) ) );
            constraints[ 10 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                      lowerParameters( 10 ),
                                                      upperParameters( 10 ) ) );
            constraints[ 11 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                      lowerParameters( 11 ),
                                                      upperParameters( 11 ) ) );
            constraints[ 12 ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                      lowerParameters( 12 ),
                                                      upperParameters( 12 ) ) );

            std::vector< double > nlpOptimizerParameters( 15 );
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
            nlpOptimizerParameters[ 8 ] = ( fixedParameters[ 6 ] ? 0.0 :
                                                        deltaParameters( 6 ) );
            nlpOptimizerParameters[ 9 ] = ( fixedParameters[ 7 ] ? 0.0 :
                                                        deltaParameters( 7 ) );
            nlpOptimizerParameters[ 10 ] = ( fixedParameters[ 8 ] ? 0.0 :
                                                        deltaParameters( 8 ) );
            nlpOptimizerParameters[ 11 ] = ( fixedParameters[ 9 ] ? 0.0 :
                                                        deltaParameters( 9 ) );
            nlpOptimizerParameters[ 12 ] = ( fixedParameters[ 10 ] ? 0.0 :
                                                        deltaParameters( 10 ) );
            nlpOptimizerParameters[ 13 ] = ( fixedParameters[ 11 ] ? 0.0 :
                                                        deltaParameters( 11 ) );
            nlpOptimizerParameters[ 14 ] = ( fixedParameters[ 12 ] ? 0.0 :
                                                        deltaParameters( 12 ) );

            factory->getConstrainedNonLinearProgrammingOptimumParameters(
              gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
              copyOfInitialParameters,
              constraints,
              *noddiNLPFunction,
              nlpOptimizerParameters,
              false );

          }

        }


        ////////////////////////////////////////////////////////////////////////
        // copying intermediate result to _values in case it is the last
        //  optimizer
        ////////////////////////////////////////////////////////////////////////
        if ( noddiMCMCFunction->getType() ==
             gkg::OrientationDistributionFunction::NoddiWatson )
        {

          _values.resize( 9 );

          int32_t s;
          for ( s = 0; s < 9; s++ )
          {

            _values[ s ] = copyOfInitialParameters( s );

          }
          gkg::Vector3d< float > principalOrientation( _values[ 3 ],
                                                       _values[ 4 ],
                                                       _values[ 5 ] );
          principalOrientation.normalize();
          _values[ 3 ] = principalOrientation.x;
          _values[ 4 ] = principalOrientation.y;
          _values[ 5 ] = principalOrientation.z;

        }
        else if ( noddiMCMCFunction->getType() ==
                  gkg::OrientationDistributionFunction::NoddiBingham )
        {

          _values.resize( 12 );

          int32_t s;
          for ( s = 0; s < 12; s++ )
          {

            _values[ s ] = copyOfInitialParameters( s );

          }
          gkg::Vector3d< float > principalOrientation( _values[ 3 ],
                                                       _values[ 4 ],
                                                       _values[ 5 ] );
          principalOrientation.normalize();
          _values[ 3 ] = principalOrientation.x;
          _values[ 4 ] = principalOrientation.y;
          _values[ 5 ] = principalOrientation.z;

          gkg::Vector3d< float > secondaryOrientation( _values[ 7 ],
                                                       _values[ 8 ],
                                                       _values[ 9 ] );
          secondaryOrientation.normalize();
          _values[ 7 ] = secondaryOrientation.x;
          _values[ 8 ] = secondaryOrientation.y;
          _values[ 9 ] = secondaryOrientation.z;

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
            monteCarloMarkovChainEstimator( noddiMCMCFunction,
                                            _noddiMCMCParameterSet );

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

          if ( noddiMCMCFunction->getType() ==
               gkg::OrientationDistributionFunction::NoddiWatson )
          {

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
            gkg::Vector3d< float > principalOrientation( _values[ 3 ],
                                                         _values[ 4 ],
                                                         _values[ 5 ] );
            principalOrientation.normalize();
            _values[ 3 ] = principalOrientation.x;
            _values[ 4 ] = principalOrientation.y;
            _values[ 5 ] = principalOrientation.z;

          }
          else if ( noddiMCMCFunction->getType() ==
                    gkg::OrientationDistributionFunction::NoddiBingham )
          {

            // computing median values for each parameter distribution and 
            // storing them into the _values field of the orientation 
            // distribution function resizing output vector to parameter count
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

            gkg::Vector3d< float > principalOrientation( _values[ 4 ],
                                                         _values[ 5 ],
                                                         _values[ 6 ] );
            principalOrientation.normalize();
            _values[ 4 ] = principalOrientation.x;
            _values[ 5 ] = principalOrientation.y;
            _values[ 6 ] = principalOrientation.z;

            gkg::Vector3d< float > secondaryOrientation( _values[ 7 ],
                                                         _values[ 8 ],
                                                         _values[ 9 ] );
            secondaryOrientation.normalize();
            _values[ 7 ] = secondaryOrientation.x;
            _values[ 8 ] = secondaryOrientation.y;
            _values[ 9 ] = secondaryOrientation.z;

          }

        }

      }

    }

  }
  GKG_CATCH( "gkg::NoddiOdf::NoddiOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "gkg::RCPointer< gkg::NoddiMCMCFunction > noddiMCMCFunction, "
             "gkg::RCPointer< gkg::NoddiMCMCParameterSet > "
             "noddiMCMCParameterSet, "
             "const gkg::Vector& initialParameters, "
             "int32_t mcmcBurninCount, "
             "int32_t mcmcSampleCount, "
             "int32_t mcmcIntervalCount, "
             "int32_t mcmcMaximumIterationCount )" );

}


gkg::NoddiOdf::~NoddiOdf()
{
}


const gkg::RCPointer< gkg::NoddiMCMCParameterSet >& 
gkg::NoddiOdf::getNoddiMCMCParameterSet() const
{

  try
  {

    return _noddiMCMCParameterSet;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::NoddiMCMCParameterSet >& "
             "gkg::NoddiOdf::getNoddiMCMCParameterSet() const" );

}


#undef EPSILON
