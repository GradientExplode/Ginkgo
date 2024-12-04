#include <gkg-dmri-odf-charmed/CharmedOdf.h>
#include <gkg-dmri-odf-charmed/CharmedNLPFunction.h>
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


gkg::CharmedOdf::CharmedOdf(
         const gkg::OrientationSet* orientationSet,
         const gkg::Volume< float >& t2,
         const gkg::Volume< float >& dw,
         const gkg::Vector3d< int32_t >& site,
         gkg::RCPointer< gkg::CharmedMCMCFunction > charmedMCMCFunction,
         gkg::RCPointer< gkg::CharmedMCMCParameterSet > charmedMCMCParameterSet,
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
                                               charmedMCMCFunction->getType(),
                                               orientationSet ),
            _charmedMCMCParameterSet( charmedMCMCParameterSet )
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
      if ( averageT2 <= 0 )
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
      // making a copy if input initial parameters
      //////////////////////////////////////////////////////////////////////////
      gkg::Vector copyOfInitialParameters( initialParameters );


      //////////////////////////////////////////////////////////////////////////
      // creating the NLP function if QuickSearch and/or NLP optimizer selected
      //////////////////////////////////////////////////////////////////////////
      gkg::RCPointer< gkg::CharmedNLPFunction > charmedNLPFunction;
      if ( applyQuickSearchOptimizer || applyNlpOptimizer )
      {

        charmedNLPFunction.reset( new gkg::CharmedNLPFunction( 
                                                           charmedMCMCFunction,
                                                           normalizedSignal ) );

      }

      //////////////////////////////////////////////////////////////////////////
      // running the Quick Search  optimizer if selected
      //////////////////////////////////////////////////////////////////////////
      if ( applyQuickSearchOptimizer )
      {

        const gkg::Vector&
          lowerParameters = _charmedMCMCParameterSet->getLowerBounds();
        const gkg::Vector&
          upperParameters = _charmedMCMCParameterSet->getUpperBounds();

        int32_t p1 = 0;
        int32_t p2 = 0;
        int32_t p3 = 0;
        double optimalP1;
        double optimalP2;
        double optimalP3;
        double optimalMinusLogLikelihood = 1e38;
        double minusLogLikelihood = 0.0;

        if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 1 )
        {

          for ( p2 = 0; p2 < quicksearchStepCount; p2++ )
          {

            for ( p1 = 0; p1 < quicksearchStepCount; p1++ )
            {

              gkg::Vector parameters( initialParameters );
              // isotropic fraction
              parameters( 1 ) = lowerParameters( 1 ) +
                                  p1 * ( upperParameters( 1 ) -
                                         lowerParameters( 1 ) ) /
                                  ( double )quicksearchStepCount;
              // restricted fraction                  
              parameters( 5 ) = lowerParameters( 5 ) +
                                p2 * ( upperParameters( 5 ) -
                                       lowerParameters( 5 ) ) /
                                ( double )quicksearchStepCount;

              minusLogLikelihood = charmedNLPFunction->getValueAt( parameters );
              if ( minusLogLikelihood < optimalMinusLogLikelihood )
              {

                 optimalP1 = parameters( 1 );
                 optimalP2 = parameters( 5 );
                 optimalMinusLogLikelihood = minusLogLikelihood;

              }

            }

          }

          // replacing initial parameters by those found using the quick search
          copyOfInitialParameters( 1 ) = optimalP1;
          copyOfInitialParameters( 5 ) = optimalP2;

        }
        else if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 2 )
        {

          for ( p3 = 0; p3 < quicksearchStepCount; p3++ )
          {

            for ( p2 = 0; p2 < quicksearchStepCount; p2++ )
            {

              for ( p1 = 0; p1 < quicksearchStepCount; p1++ )
              {

                gkg::Vector parameters( initialParameters );
                // isotropic fraction
                parameters( 1 ) = lowerParameters( 1 ) +
                                  p1 * ( upperParameters( 1 ) -
                                         lowerParameters( 1 ) ) /
                                  ( double )quicksearchStepCount;
                // restricted fraction of 1st compartment
                parameters( 5 ) = lowerParameters( 5 ) +
                                  p2 * ( upperParameters( 5 ) -
                                         lowerParameters( 5 ) ) /
                                  ( double )quicksearchStepCount;
                // restricted fraction of 2nd compartment
                parameters( 8 ) = lowerParameters( 8 ) +
                                  p3 * ( upperParameters( 8 ) -
                                         lowerParameters( 8 ) ) /
                                  ( double )quicksearchStepCount;

                minusLogLikelihood = charmedNLPFunction->getValueAt( parameters );
                if ( minusLogLikelihood < optimalMinusLogLikelihood )
                {

                   optimalP1 = parameters( 1 );
                   optimalP2 = parameters( 5 );
                   optimalP3 = parameters( 8 );
                   optimalMinusLogLikelihood = minusLogLikelihood;

                }

              }

            }

          }

          // replacing initial parameters by those found using the quick search
          copyOfInitialParameters( 1 ) = optimalP1;
          copyOfInitialParameters( 5 ) = optimalP2;
          copyOfInitialParameters( 8 ) = optimalP3;

        }

      }


      //////////////////////////////////////////////////////////////////////////
      // running the constrained NLP optimizer if selected
      //////////////////////////////////////////////////////////////////////////
      if ( applyNlpOptimizer )
      {
        
        gkg::NumericalAnalysisImplementationFactory*
          factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

        const gkg::Vector&
          lowerParameters = _charmedMCMCParameterSet->getLowerBounds();
        const gkg::Vector&
          upperParameters = _charmedMCMCParameterSet->getUpperBounds();
        const gkg::Vector&
          deltaParameters = _charmedMCMCParameterSet->getSteps();
        const std::vector< bool >&
          fixedParameters = _charmedMCMCParameterSet->getFixed();
        
        if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 1 )
        {

          std::vector< gkg::RCPointer< gkg::OptimizerConstraint > > 
            constraints( 8 );

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

          std::vector< double > nlpOptimizerParameters( 10 );
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
        
          factory->getConstrainedNonLinearProgrammingOptimumParameters(
            gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
            copyOfInitialParameters,
            constraints,
            *charmedNLPFunction,
            nlpOptimizerParameters,
            false );

        }
        else if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 2 )
        {

          std::vector< gkg::RCPointer< gkg::OptimizerConstraint > > 
            constraints( 11 );

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

          std::vector< double > nlpOptimizerParameters( 13 );
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
        
          factory->getConstrainedNonLinearProgrammingOptimumParameters(
            gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
            copyOfInitialParameters,
            constraints,
            *charmedNLPFunction,
            nlpOptimizerParameters,
            false );

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // copying intermediate result to _values in case it is the last optimizer
      //////////////////////////////////////////////////////////////////////////

      if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 1 )
      {

        _values.resize( 8 );

        int32_t s;
        for ( s = 0; s < 8; s++ )
        {

          _values[ s ] = copyOfInitialParameters( s );

        }

      }
      else if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 2 )
      {

        _values.resize( 11 );

        int32_t s;
        for ( s = 0; s < 11; s++ )
        {

          _values[ s ] = copyOfInitialParameters( s );

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // running a MCMC estimator to get distributions of the parameters if
      // selected
      //////////////////////////////////////////////////////////////////////////
      if ( applyMcmcOptimizer )
      {

        gkg::Matrix parameterEstimates;
        int32_t updateCallCount = 0;
        double acceptanceRate = 0.0;
        gkg::MonteCarloMarkovChainEstimator 
          monteCarloMarkovChainEstimator( charmedMCMCFunction,
                                          _charmedMCMCParameterSet );

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

        // computing median values for each parameter distribution and storing
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
  GKG_CATCH( "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg::RCPointer< gkg::CharmedMCMCFunction > charmedMCMCFunction, "
             "gkg::RCPointer< gkg::CharmedMCMCParameterSet > "
             "charmedMCMCParameterSet, "
             "const gkg::Vector& initialParameters, "
             "int32_t mcmcBurninCount, "
             "int32_t mcmcSampleCount, "
             "int32_t mcmcIntervalCount, "
             "int32_t mcmcMaximumIterationCount" );

}


gkg::CharmedOdf::CharmedOdf(
             const gkg::OrientationSet* orientationSet,
             const gkg::Volume< float >& t2,
             const gkg::Volume< float >& dw,
             const gkg::Vector3d< float >& site,
             gkg::RCPointer< gkg::CharmedMCMCFunction > charmedMCMCFunction,
             gkg::RCPointer< gkg::CharmedMCMCParameterSet > charmedMCMCParameterSet,
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
                                                   charmedMCMCFunction->getType(),
                                                   orientationSet ),
                _charmedMCMCParameterSet( charmedMCMCParameterSet )
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
      if ( averageT2 <= 0 )
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
      // making a copy if input initial parameters
      //////////////////////////////////////////////////////////////////////////
      gkg::Vector copyOfInitialParameters( initialParameters );


      //////////////////////////////////////////////////////////////////////////
      // creating the NLP function if QuickSearch and/or NLP optimizer selected
      //////////////////////////////////////////////////////////////////////////
      gkg::RCPointer< gkg::CharmedNLPFunction > charmedNLPFunction;
      if ( applyQuickSearchOptimizer || applyNlpOptimizer )
      {

        charmedNLPFunction.reset( new gkg::CharmedNLPFunction(
                                                           charmedMCMCFunction,
                                                           normalizedSignal ) );

      }

      //////////////////////////////////////////////////////////////////////////
      // running the Quick Search  optimizer if selected
      //////////////////////////////////////////////////////////////////////////
      if ( applyQuickSearchOptimizer )
      {

        const gkg::Vector&
          lowerParameters = _charmedMCMCParameterSet->getLowerBounds();
        const gkg::Vector&
          upperParameters = _charmedMCMCParameterSet->getUpperBounds();

        int32_t p1 = 0;
        int32_t p2 = 0;
        int32_t p3 = 0;
        double optimalP1;
        double optimalP2;
        double optimalP3;
        double optimalMinusLogLikelihood = 1e38;
        double minusLogLikelihood = 0.0;

        if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 1 )
        {

          for ( p2 = 0; p2 < quicksearchStepCount; p2++ )
          {

            for ( p1 = 0; p1 < quicksearchStepCount; p1++ )
            {

              gkg::Vector parameters( initialParameters );
              // isotropic fraction
              parameters( 1 ) = lowerParameters( 1 ) +
                                  p1 * ( upperParameters( 1 ) -
                                         lowerParameters( 1 ) ) /
                                  ( double )quicksearchStepCount;
              // restricted fraction                  
              parameters( 5 ) = lowerParameters( 5 ) +
                                p2 * ( upperParameters( 5 ) -
                                       lowerParameters( 5 ) ) /
                                ( double )quicksearchStepCount;

              minusLogLikelihood = charmedNLPFunction->getValueAt( parameters );
              if ( minusLogLikelihood < optimalMinusLogLikelihood )
              {

                 optimalP1 = parameters( 1 );
                 optimalP2 = parameters( 5 );
                 optimalMinusLogLikelihood = minusLogLikelihood;

              }

            }

          }

          // replacing initial parameters by those found using the quick search
          copyOfInitialParameters( 1 ) = optimalP1;
          copyOfInitialParameters( 5 ) = optimalP2;

        }
        else if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 2 )
        {

          for ( p3 = 0; p3 < quicksearchStepCount; p3++ )
          {

            for ( p2 = 0; p2 < quicksearchStepCount; p2++ )
            {

              for ( p1 = 0; p1 < quicksearchStepCount; p1++ )
              {

                gkg::Vector parameters( initialParameters );
                // isotropic fraction
                parameters( 1 ) = lowerParameters( 1 ) +
                                  p1 * ( upperParameters( 1 ) -
                                         lowerParameters( 1 ) ) /
                                  ( double )quicksearchStepCount;
                // restricted fraction of 1st compartment
                parameters( 5 ) = lowerParameters( 5 ) +
                                  p2 * ( upperParameters( 5 ) -
                                         lowerParameters( 5 ) ) /
                                  ( double )quicksearchStepCount;
                // restricted fraction of 2nd compartment
                parameters( 8 ) = lowerParameters( 8 ) +
                                  p3 * ( upperParameters( 8 ) -
                                         lowerParameters( 8 ) ) /
                                  ( double )quicksearchStepCount;

                minusLogLikelihood = charmedNLPFunction->getValueAt( parameters );
                if ( minusLogLikelihood < optimalMinusLogLikelihood )
                {

                   optimalP1 = parameters( 1 );
                   optimalP2 = parameters( 5 );
                   optimalP3 = parameters( 8 );
                   optimalMinusLogLikelihood = minusLogLikelihood;

                }

              }

            }

          }

          // replacing initial parameters by those found using the quick search
          copyOfInitialParameters( 1 ) = optimalP1;
          copyOfInitialParameters( 5 ) = optimalP2;
          copyOfInitialParameters( 8 ) = optimalP3;

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // running the constrained NLP optimizer if selected
      //////////////////////////////////////////////////////////////////////////
      if ( applyNlpOptimizer )
      {
        
        gkg::NumericalAnalysisImplementationFactory*
          factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

        const gkg::Vector&
          lowerParameters = _charmedMCMCParameterSet->getLowerBounds();
        const gkg::Vector&
          upperParameters = _charmedMCMCParameterSet->getUpperBounds();
        const gkg::Vector&
          deltaParameters = _charmedMCMCParameterSet->getSteps();
        const std::vector< bool >&
          fixedParameters = _charmedMCMCParameterSet->getFixed();
        
        if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 1 )
        {

          std::vector< gkg::RCPointer< gkg::OptimizerConstraint > > 
            constraints( 8 );

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

          std::vector< double > nlpOptimizerParameters( 10 );
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
        
          factory->getConstrainedNonLinearProgrammingOptimumParameters(
            gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
            copyOfInitialParameters,
            constraints,
            *charmedNLPFunction,
            nlpOptimizerParameters,
            false );

        }
        else if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 2 )
        {

          std::vector< gkg::RCPointer< gkg::OptimizerConstraint > > 
            constraints( 11 );

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

          std::vector< double > nlpOptimizerParameters( 13 );
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
        
          factory->getConstrainedNonLinearProgrammingOptimumParameters(
            gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
            copyOfInitialParameters,
            constraints,
            *charmedNLPFunction,
            nlpOptimizerParameters,
            false );

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // copying intermediate result to _values in case it is the last optimizer
      //////////////////////////////////////////////////////////////////////////

      if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 1 )
      {

        _values.resize( 8 );

        int32_t s;
        for ( s = 0; s < 8; s++ )
        {

          _values[ s ] = copyOfInitialParameters( s );

        }

      }
      else if ( charmedMCMCFunction->getRestrictedCompartmentCount() == 2 )
      {

        _values.resize( 11 );

        int32_t s;
        for ( s = 0; s < 11; s++ )
        {

          _values[ s ] = copyOfInitialParameters( s );

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // running a MCMC estimator to get distributions of the parameters if
      // selected
      //////////////////////////////////////////////////////////////////////////
      if ( applyMcmcOptimizer )
      {

        gkg::Matrix parameterEstimates;
        int32_t updateCallCount = 0;
        double acceptanceRate = 0.0;
        gkg::MonteCarloMarkovChainEstimator 
          monteCarloMarkovChainEstimator( charmedMCMCFunction,
                                          _charmedMCMCParameterSet );

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

        // computing median values for each parameter distribution and storing
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
  GKG_CATCH( "gkg::CharmedOdf::CharmedOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "gkg::RCPointer< gkg::CharmedMCMCFunction > charmedMCMCFunction, "
             "gkg::RCPointer< gkg::CharmedMCMCParameterSet > "
             "charmedMCMCParameterSet, "
             "const gkg::Vector& initialParameters, "
             "int32_t mcmcBurninCount, "
             "int32_t mcmcSampleCount, "
             "int32_t mcmcIntervalCount, "
             "int32_t mcmcMaximumIterationCount )" );

}


gkg::CharmedOdf::~CharmedOdf()
{
}


const gkg::RCPointer< gkg::CharmedMCMCParameterSet >& 
gkg::CharmedOdf::getCharmedMCMCParameterSet() const
{

  try
  {

    return _charmedMCMCParameterSet;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::CharmedMCMCParameterSet >& "
             "gkg::CharmedOdf::getCharmedMCMCParameterSet() const" );

}


#undef EPSILON
