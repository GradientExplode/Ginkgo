#include <gkg-dmri-microstructure-axon-mapping/AxonMappingOdf.h>
#include <gkg-dmri-microstructure-axon-mapping/AxonMappingNLPFunction.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEDefines.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEDefines.h>
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
#include <gkg-core-io/Writer_i.h>
#include <cmath>

#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-core-io/StringConverter.h>
#include <fstream>


////////////////////////////////////////////////////////////////////////////////
// QUICKSEARCH with 4 parameters
////////////////////////////////////////////////////////////////////////////////

#define QUICK_SEARCH_FOUR_PARAMETERS( P1, P2, P3, P4 ) \
 \
int32_t p1 = 0; \
int32_t p2 = 0; \
int32_t p3 = 0; \
int32_t p4 = 0; \
double optimalP1; \
double optimalP2; \
double optimalP3; \
double optimalP4; \
double optimalMinusLogLikelihood = 1e38; \
double minusLogLikelihood = 0.0; \
double delta1 = ( upperParameters( P1 ) - lowerParameters( P1 ) ) / \
                ( double )quicksearchStepCount; \
double delta2 = ( upperParameters( P2 ) - lowerParameters( P2 ) ) / \
                ( double )quicksearchStepCount; \
double delta3 = ( upperParameters( P3 ) - lowerParameters( P3 ) ) / \
                ( double )quicksearchStepCount; \
double delta4 = ( upperParameters( P4 ) - lowerParameters( P4 ) ) / \
                ( double )quicksearchStepCount; \
 \
const std::vector< bool >&  \
  fixedParameters = _axonMappingMCMCParameterSet->getFixed(); \
 \
int32_t quicksearchStepCount1 = ( fixedParameters[ P1 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount2 = ( fixedParameters[ P2 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount3 = ( fixedParameters[ P3 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount4 = ( fixedParameters[ P4 ] ? 0: \
                                  quicksearchStepCount ); \
 \
gkg::Vector parameters( initialParameters ); \
for ( p4 = 0; p4 <= quicksearchStepCount4; p4++ ) \
{ \
 \
  if ( !fixedParameters[ P4 ] ) \
  { \
 \
    parameters( P4 ) = lowerParameters( P4 ) + p4 * delta4; \
 \
  } \
  for ( p3 = 0; p3 <= quicksearchStepCount3; p3++ ) \
  { \
 \
    if ( !fixedParameters[ P3 ] ) \
    { \
 \
      parameters( P3 ) = lowerParameters( P3 ) + p3 * delta3; \
 \
    } \
    for ( p2 = 0; p2 <= quicksearchStepCount2; p2++ ) \
    { \
 \
      if ( !fixedParameters[ P2 ] ) \
      { \
 \
        parameters( P2 ) = lowerParameters( P2 ) + p2 * delta2; \
 \
      } \
      for ( p1 = 0; p1 <= quicksearchStepCount1; p1++ ) \
      { \
 \
        if ( !fixedParameters[ P1 ] ) \
        { \
 \
          parameters( P1 ) = lowerParameters( P1 ) + p1 * delta1; \
 \
        } \
 \
        minusLogLikelihood = axonMappingNLPFunction->getValueAt( \
                                                       parameters ); \
 \
        if ( minusLogLikelihood < optimalMinusLogLikelihood ) \
        { \
 \
          optimalP1 = parameters( P1 ); \
          optimalP2 = parameters( P2 ); \
          optimalP3 = parameters( P3 ); \
          optimalP4 = parameters( P4 ); \
          optimalMinusLogLikelihood = minusLogLikelihood; \
 \
        } \
 \
      } \
 \
    } \
 \
  } \
 \
} \
copyOfInitialParameters( P1 ) = optimalP1; \
copyOfInitialParameters( P2 ) = optimalP2; \
copyOfInitialParameters( P2 ) = optimalP3; \
copyOfInitialParameters( P4 ) = optimalP4


////////////////////////////////////////////////////////////////////////////////
// QUICKSEARCH with 5 parameters
////////////////////////////////////////////////////////////////////////////////

#define QUICK_SEARCH_FIVE_PARAMETERS( P1, P2, P3, P4, P5 ) \
 \
int32_t p1 = 0; \
int32_t p2 = 0; \
int32_t p3 = 0; \
int32_t p4 = 0; \
int32_t p5 = 0; \
double optimalP1 = 0.0; \
double optimalP2 = 0.0; \
double optimalP3 = 0.0; \
double optimalP4 = 0.0; \
double optimalP5 = 0.0; \
double optimalMinusLogLikelihood = 1e38; \
double minusLogLikelihood = 0.0; \
double delta1 = ( upperParameters( P1 ) - lowerParameters( P1 ) ) / \
                ( double )quicksearchStepCount; \
double delta2 = ( upperParameters( P2 ) - lowerParameters( P2 ) ) / \
                ( double )quicksearchStepCount; \
double delta3 = ( upperParameters( P3 ) - lowerParameters( P3 ) ) / \
                ( double )quicksearchStepCount; \
double delta4 = ( upperParameters( P4 ) - lowerParameters( P4 ) ) / \
                ( double )quicksearchStepCount; \
double delta5 = ( upperParameters( P5 ) - lowerParameters( P5 ) ) / \
                ( double )quicksearchStepCount; \
 \
const std::vector< bool >&  \
  fixedParameters = _axonMappingMCMCParameterSet->getFixed(); \
 \
int32_t quicksearchStepCount1 = ( fixedParameters[ P1 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount2 = ( fixedParameters[ P2 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount3 = ( fixedParameters[ P3 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount4 = ( fixedParameters[ P4 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount5 = ( fixedParameters[ P5 ] ? 0: \
                                  quicksearchStepCount ); \
 \
gkg::Vector parameters( initialParameters ); \
for ( p5 = 0; p5 <= quicksearchStepCount5; p5++ ) \
{ \
 \
  if ( !fixedParameters[ P5 ] ) \
  { \
 \
    parameters( P5 ) = lowerParameters( P5 ) + p5 * delta5; \
 \
  } \
  for ( p4 = 0; p4 <= quicksearchStepCount4; p4++ ) \
  { \
   \
    if ( !fixedParameters[ P4 ] ) \
    { \
   \
      parameters( P4 ) = lowerParameters( P4 ) + p4 * delta4; \
   \
    } \
    for ( p3 = 0; p3 <= quicksearchStepCount3; p3++ ) \
    { \
   \
      if ( !fixedParameters[ P3 ] ) \
      { \
   \
        parameters( P3 ) = lowerParameters( P3 ) + p3 * delta3; \
   \
      } \
      for ( p2 = 0; p2 <= quicksearchStepCount2; p2++ ) \
      { \
   \
        if ( !fixedParameters[ P2 ] ) \
        { \
   \
          parameters( P2 ) = lowerParameters( P2 ) + p2 * delta2; \
   \
        } \
        for ( p1 = 0; p1 <= quicksearchStepCount1; p1++ ) \
        { \
   \
          if ( !fixedParameters[ P1 ] ) \
          { \
   \
            parameters( P1 ) = lowerParameters( P1 ) + p1 * delta1; \
   \
          } \
   \
          minusLogLikelihood = axonMappingNLPFunction->getValueAt( \
                                                         parameters ); \
   \
          if ( minusLogLikelihood < optimalMinusLogLikelihood ) \
          { \
   \
            optimalP1 = parameters( P1 ); \
            optimalP2 = parameters( P2 ); \
            optimalP3 = parameters( P3 ); \
            optimalP4 = parameters( P4 ); \
            optimalMinusLogLikelihood = minusLogLikelihood; \
   \
          } \
   \
        } \
   \
      } \
   \
    } \
   \
  } \
} \
copyOfInitialParameters( P1 ) = optimalP1; \
copyOfInitialParameters( P2 ) = optimalP2; \
copyOfInitialParameters( P2 ) = optimalP3; \
copyOfInitialParameters( P4 ) = optimalP4; \
copyOfInitialParameters( P4 ) = optimalP5


////////////////////////////////////////////////////////////////////////////////
// QUICKSEARCH with 6 parameters
////////////////////////////////////////////////////////////////////////////////

#define QUICK_SEARCH_SIX_PARAMETERS( P1, P2, P3, P4, P5, P6 ) \
 \
int32_t p1 = 0; \
int32_t p2 = 0; \
int32_t p3 = 0; \
int32_t p4 = 0; \
int32_t p5 = 0; \
int32_t p6 = 0; \
double optimalP1; \
double optimalP2; \
double optimalP3; \
double optimalP4; \
double optimalP5; \
double optimalP6; \
double optimalMinusLogLikelihood = 1e38; \
double minusLogLikelihood = 0.0; \
double delta1 = ( upperParameters( P1 ) - lowerParameters( P1 ) ) / \
                ( double )quicksearchStepCount; \
double delta2 = ( upperParameters( P2 ) - lowerParameters( P2 ) ) / \
                ( double )quicksearchStepCount; \
double delta3 = ( upperParameters( P3 ) - lowerParameters( P3 ) ) / \
                ( double )quicksearchStepCount; \
double delta4 = ( upperParameters( P4 ) - lowerParameters( P4 ) ) / \
                ( double )quicksearchStepCount; \
double delta5 = ( upperParameters( P5 ) - lowerParameters( P5 ) ) / \
                ( double )quicksearchStepCount; \
double delta5 = ( upperParameters( P6 ) - lowerParameters( P6 ) ) / \
                ( double )quicksearchStepCount; \
 \
const std::vector< bool >&  \
  fixedParameters = _axonMappingMCMCParameterSet->getFixed(); \
 \
int32_t quicksearchStepCount1 = ( fixedParameters[ P1 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount2 = ( fixedParameters[ P2 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount3 = ( fixedParameters[ P3 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount4 = ( fixedParameters[ P4 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount5 = ( fixedParameters[ P5 ] ? 0: \
                                  quicksearchStepCount ); \
int32_t quicksearchStepCount6 = ( fixedParameters[ P6 ] ? 0: \
                                  quicksearchStepCount ); \
 \
gkg::Vector parameters( initialParameters ); \
for ( p6 = 0; p6 <= quicksearchStepCount6; p6++ ) \
{ \
 \
  if ( !fixedParameters[ P6 ] ) \
  { \
 \
    parameters( P6 ) = lowerParameters( P6 ) + p6 * delta6; \
 \
  } \
  for ( p5 = 0; p5 <= quicksearchStepCount5; p5++ ) \
  { \
   \
    if ( !fixedParameters[ P5 ] ) \
    { \
   \
      parameters( P5 ) = lowerParameters( P5 ) + p5 * delta5; \
   \
    } \
    for ( p4 = 0; p4 <= quicksearchStepCount4; p4++ ) \
    { \
     \
      if ( !fixedParameters[ P4 ] ) \
      { \
     \
        parameters( P4 ) = lowerParameters( P4 ) + p4 * delta4; \
     \
      } \
      for ( p3 = 0; p3 <= quicksearchStepCount3; p3++ ) \
      { \
     \
        if ( !fixedParameters[ P3 ] ) \
        { \
     \
          parameters( P3 ) = lowerParameters( P3 ) + p3 * delta3; \
     \
        } \
        for ( p2 = 0; p2 <= quicksearchStepCount2; p2++ ) \
        { \
     \
          if ( !fixedParameters[ P2 ] ) \
          { \
     \
            parameters( P2 ) = lowerParameters( P2 ) + p2 * delta2; \
     \
          } \
          for ( p1 = 0; p1 <= quicksearchStepCount1; p1++ ) \
          { \
     \
            if ( !fixedParameters[ P1 ] ) \
            { \
     \
              parameters( P1 ) = lowerParameters( P1 ) + p1 * delta1; \
     \
            } \
     \
            minusLogLikelihood = axonMappingNLPFunction->getValueAt( \
                                                           parameters ); \
     \
            if ( minusLogLikelihood < optimalMinusLogLikelihood ) \
            { \
     \
              optimalP1 = parameters( P1 ); \
              optimalP2 = parameters( P2 ); \
              optimalP3 = parameters( P3 ); \
              optimalP4 = parameters( P4 ); \
              optimalMinusLogLikelihood = minusLogLikelihood; \
     \
            } \
     \
          } \
     \
        } \
     \
      } \
     \
    } \
  } \
} \
copyOfInitialParameters( P1 ) = optimalP1; \
copyOfInitialParameters( P2 ) = optimalP2; \
copyOfInitialParameters( P2 ) = optimalP3; \
copyOfInitialParameters( P4 ) = optimalP4; \
copyOfInitialParameters( P5 ) = optimalP5; \
copyOfInitialParameters( P6 ) = optimalP6;


////////////////////////////////////////////////////////////////////////////////
// piece of code to save parameter histograms
////////////////////////////////////////////////////////////////////////////////

//               {
// 
//                 const gkg::Vector&
//                   deltaParameters = _axonMappingMCMCParameterSet->getSteps();
//                 const std::vector< bool >&
//                  fixedParameters = _axonMappingMCMCParameterSet->getFixed();
// 
//                 if ( !fixedParameters[ s ] )
//                 {
// 
//                   std::vector< double > histogram( 100 );
//                   double lowerBoundary = 0.0;
//                   double upperBoundary = 0.0;
// 
//                   gkg::HistogramAnalyzer::getInstance().getHistogram(
//           	                                  parameterValues,
//         					  histogram,
//         					  lowerBoundary,
//         					  upperBoundary,
//         					  deltaParameters( s ) / 1.0 );
// 
//                   std::ofstream os( ( std::string( "/tmp/parameter" ) + '-' +
//                                       gkg::StringConverter::toString( s ) +
//                                       ".txt" ).c_str() );
//                   for ( int ss = 0; ss < 100; ss++ )
//                   {
// 
//                      os << lowerBoundary +
//  	    	           ( ( upperBoundary - lowerBoundary ) /
//  	    	           ( double )( 100 - 1 ) ) * ( double ) ss
//                         << "\t"
//                         << histogram[ ss ]
//                         << std::endl;
// 
//                   }
//                   os.close();
// 
//                 }
// 
//               }


////////////////////////////////////////////////////////////////////////////////
// class AxonMappingOdf
////////////////////////////////////////////////////////////////////////////////

template < class S >
gkg::AxonMappingOdf::AxonMappingOdf(
 const gkg::OrientationSet* orientationSet,
 const gkg::Volume< float >& t2,
 const gkg::Volume< float >& dw,
 const gkg::Vector3d< S >& site,
 gkg::RCPointer< gkg::AxonMappingMCMCFunction > axonMappingMCMCFunction,
 gkg::RCPointer< gkg::AxonMappingMCMCParameterSet > axonMappingMCMCParameterSet,
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
                                             axonMappingMCMCFunction->getType(),
                                             orientationSet ),
                      _axonMappingMCMCParameterSet(
                                                   axonMappingMCMCParameterSet )
{

  try
  {

    if ( _orientationSet )
    {

      //////////////////////////////////////////////////////////////////////////
      // computing average T2 value and normalized signal vector
      //////////////////////////////////////////////////////////////////////////
      float averageT2 = 0.0;
      gkg::Vector normalizedSignal;
      bool isOk = this->computeNormalizedSignal( t2,
                                                 dw,
                                                 site,
                                                 averageT2,
                                                 normalizedSignal );

      if ( !isOk )
      {

        if ( axonMappingMCMCFunction->getType() ==
             gkg::OrientationDistributionFunction::PGSEAxonMapping )
        {

          _values.resize( 11 );
          _values[ PGSE_IsotropicFraction ] = 1.0f;
          _values[ PGSE_IsotropicDiffusivity ] = 3e-9;
          _values[ PGSE_ExtraPerpendicularDiffusivity ] = 0.4e-9;
          _values[ PGSE_StationaryFraction ] = 0.0f;
          _values[ PGSE_IntracellularFraction ] = 0.0f;
          _values[ PGSE_Kappa ] = 0.01f;
          _values[ PGSE_ParallelDiffusivity ] = 1.7e-9;
          _values[ PGSE_AxonDiameter ] = 0.01f;
          _values[ PGSE_PrincipalOrientation ] = 1.0f;
          _values[ PGSE_PrincipalOrientation + 1 ] = 1.0f;
          _values[ PGSE_PrincipalOrientation + 2 ] = 1.0f;

        }
        else if ( axonMappingMCMCFunction->getType() ==
                  gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
        {

          _values.resize( 11 );
          _values[ CTOGSE_IsotropicFraction ] = 1.0f;
          _values[ CTOGSE_IsotropicDiffusivity ] = 3e-9;
          _values[ CTOGSE_ExtraPerpendicularDiffusivity ] = 0.4e-9;
          _values[ CTOGSE_StationaryFraction ] = 0.0f;
          _values[ CTOGSE_IntracellularFraction ] = 0.0f;
          _values[ CTOGSE_Kappa ] = 0.01f;
          _values[ CTOGSE_ParallelDiffusivity ] = 1.7e-9;
          _values[ CTOGSE_AxonDiameter ] = 0.01f;
          _values[ CTOGSE_PrincipalOrientation ] = 1.0f;
          _values[ CTOGSE_PrincipalOrientation + 1 ] = 1.0f;
          _values[ CTOGSE_PrincipalOrientation + 2 ] = 1.0f;

        }


      }
      else
      {

        ////////////////////////////////////////////////////////////////////////
        // making a copy if input initial parameters
        ////////////////////////////////////////////////////////////////////////

        gkg::Vector copyOfInitialParameters( initialParameters );

        ////////////////////////////////////////////////////////////////////////
        // getting the parameters count
        ////////////////////////////////////////////////////////////////////////

        int32_t parameterCount = 0;
        parameterCount = _axonMappingMCMCParameterSet->getParameterCount();


        ////////////////////////////////////////////////////////////////////////
        // creating the NLP function if QuickSearch and/or
        // NLP optimizer selected
        ////////////////////////////////////////////////////////////////////////
        gkg::RCPointer< gkg::AxonMappingNLPFunction > axonMappingNLPFunction;

        if ( applyQuickSearchOptimizer || applyNlpOptimizer )
        {

          axonMappingNLPFunction.reset(
                       new gkg::AxonMappingNLPFunction( axonMappingMCMCFunction,
                                                        normalizedSignal ) );

        }


        ////////////////////////////////////////////////////////////////////////
        // running the Quick Search  optimizer if selected
        ////////////////////////////////////////////////////////////////////////

        if ( applyQuickSearchOptimizer )
        {

          const gkg::Vector&
            lowerParameters = _axonMappingMCMCParameterSet->getLowerBounds();
          const gkg::Vector&
            upperParameters = _axonMappingMCMCParameterSet->getUpperBounds();

          if ( axonMappingMCMCFunction->getType() ==
               gkg::OrientationDistributionFunction::PGSEAxonMapping )
          {                                         

            QUICK_SEARCH_FIVE_PARAMETERS( PGSE_IsotropicFraction,
                                          PGSE_StationaryFraction,
                                          PGSE_IntracellularFraction,
                                          PGSE_Kappa,
                                          PGSE_AxonDiameter );

          }
          else if ( axonMappingMCMCFunction->getType() ==
                    gkg::OrientationDistributionFunction::CTOGSEAxonMapping )
          {                                         

            QUICK_SEARCH_FIVE_PARAMETERS( CTOGSE_IsotropicFraction,
                                          CTOGSE_StationaryFraction,
                                          CTOGSE_IntracellularFraction,
                                          CTOGSE_Kappa,
                                          CTOGSE_AxonDiameter );

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
            lowerParameters = _axonMappingMCMCParameterSet->getLowerBounds();
          const gkg::Vector&
            upperParameters = _axonMappingMCMCParameterSet->getUpperBounds();
          const gkg::Vector&
            deltaParameters = _axonMappingMCMCParameterSet->getSteps();
          const std::vector< bool >&
            fixedParameters = _axonMappingMCMCParameterSet->getFixed();

          std::vector< gkg::RCPointer< gkg::OptimizerConstraint > > 
            constraints( parameterCount );

          int32_t p = 0;
          for ( p = 0; p < parameterCount; p++ )
          {

            constraints[ p ].reset(
              new gkg::ClosedIntervalOptimizerConstraint(
                                                     lowerParameters( p ),
                                                     upperParameters( p ) ) );

          }

          std::vector< double > nlpOptimizerParameters( parameterCount + 2 );
          nlpOptimizerParameters[ 0 ] = nlpMaximumIterationCount;
          nlpOptimizerParameters[ 1 ] = nlpMaximumSizeOfTheSimplex;

          for ( p = 0; p < parameterCount; p++ )
          {

            nlpOptimizerParameters[ p + 2 ] = ( fixedParameters[ p ] ? 0.0 :
                              constraints[ p ]->constrainedDeltaParameter(
                                                     deltaParameters( p ) ) );

          }

          factory->getConstrainedNonLinearProgrammingOptimumParameters(
            gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
            copyOfInitialParameters,
            constraints,
            *axonMappingNLPFunction,
            nlpOptimizerParameters,
            false );

        }


        ////////////////////////////////////////////////////////////////////////
        // copying intermediate result to _values in case it is the last 
        // optimizer
        ////////////////////////////////////////////////////////////////////////
        _values.resize( parameterCount );

        int32_t s;
        for ( s = 0; s < parameterCount; s++ )
        {

          _values[ s ] = copyOfInitialParameters( s );

        }

        if ( ( axonMappingMCMCFunction->getType() ==
               gkg::OrientationDistributionFunction::PGSEAxonMapping ) ||
             ( axonMappingMCMCFunction->getType() ==
               gkg::OrientationDistributionFunction::CTOGSEAxonMapping ) )
        {

          gkg::Vector3d< float > 
            principalOrientation( _values[ parameterCount - 3 ],
                                  _values[ parameterCount - 2 ],
                                  _values[ parameterCount - 1 ] );

          principalOrientation.normalize();
          _values[ parameterCount - 3 ] = principalOrientation.x;
          _values[ parameterCount - 2 ] = principalOrientation.y;
          _values[ parameterCount - 1 ] = principalOrientation.z;

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
            monteCarloMarkovChainEstimator( axonMappingMCMCFunction,
                                            _axonMappingMCMCParameterSet );

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
          // storing them into the _values field of the orientation 
          // distribution function resizing output vector to parameter count
          _values.resize( parameterCount );
          gkg::MedianFilter< std::vector< float >, float > medianFilter;
          int32_t estimateCount = parameterEstimates.getSize2();
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

          if ( ( axonMappingMCMCFunction->getType() ==
                 gkg::OrientationDistributionFunction::PGSEAxonMapping ) ||
               ( axonMappingMCMCFunction->getType() ==
                 gkg::OrientationDistributionFunction::CTOGSEAxonMapping ) )
          {

            gkg::Vector3d< float >
              principalOrientation( _values[ parameterCount - 3 ],
                                    _values[ parameterCount - 2 ],
                                    _values[ parameterCount - 1 ] );
            principalOrientation.normalize();
            _values[ parameterCount - 3 ] = principalOrientation.x;
            _values[ parameterCount - 2 ] = principalOrientation.y;
            _values[ parameterCount - 1 ] = principalOrientation.z;

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class S > "
             "gkg::AxonMappingOdf::AxonMappingOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< S >& site, "
             "gkg::RCPointer< gkg::AxonMappingMCMCFunction > "
             "axonMappingMCMCFunction, "
             "gkg::RCPointer< gkg::AxonMappingMCMCParameterSet > "
             "axonMappingMCMCParameterSet, "
             "const gkg::Vector& initialParameters, "
             "bool applyQuickSearchOptimizer, "
             "int32_t quicksearchStepCount, "
             "bool applyNlpOptimizer, "
             "int32_t nlpMaximumIterationCount, "
             "double nlpMaximumSizeOfTheSimplex, "
             "bool applyMcmcOptimizer, "
             "int32_t mcmcBurninCount, "
             "int32_t mcmcSampleCount, "
             "int32_t mcmcIntervalCount, "
             "int32_t mcmcMaximumIterationCount )" );

}


gkg::AxonMappingOdf::~AxonMappingOdf()
{
}


const gkg::RCPointer< gkg::AxonMappingMCMCParameterSet >& 
gkg::AxonMappingOdf::getAxonMappingMCMCParameterSet() const
{

  try
  {

    return _axonMappingMCMCParameterSet;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::AxonMappingMCMCParameterSet >& "
             "gkg::AxonMappingOdf::getAxonMappingMCMCParameterSet() const" );

}


template
gkg::AxonMappingOdf::AxonMappingOdf(
 const gkg::OrientationSet* orientationSet,
 const gkg::Volume< float >& t2,
 const gkg::Volume< float >& dw,
 const gkg::Vector3d< int32_t >& site,
 gkg::RCPointer< gkg::AxonMappingMCMCFunction > axonMappingMCMCFunction,
 gkg::RCPointer< gkg::AxonMappingMCMCParameterSet > axonMappingMCMCParameterSet,
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
 int32_t mcmcMaximumIterationCount );

template
gkg::AxonMappingOdf::AxonMappingOdf(
 const gkg::OrientationSet* orientationSet,
 const gkg::Volume< float >& t2,
 const gkg::Volume< float >& dw,
 const gkg::Vector3d< float >& site,
 gkg::RCPointer< gkg::AxonMappingMCMCFunction > axonMappingMCMCFunction,
 gkg::RCPointer< gkg::AxonMappingMCMCParameterSet > axonMappingMCMCParameterSet,
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
 int32_t mcmcMaximumIterationCount );

