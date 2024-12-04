#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCWatsonFunction.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCParameterSet.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCWatsonParameterSet.h>
#include <gkg-dmri-microstructure-noddi/NoddiNLPFunction.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainEstimator.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <gkg-core-io/StringConverter.h>
#include <fstream>



#define STEP_COUNT_P1 10
#define STEP_COUNT_P2 10
#define STEP_COUNT_P3 10



double 
getLogLikelihood( const gkg::Vector& realMeasurements,
                  const gkg::Vector& currentMeasurements,
                  double noiseStandardDeviation,
                  const gkg::NumericalAnalysisImplementationFactory* factory )
{

  try
  {

    double logLikelihood = 0.0;
    double noiseVariance = noiseStandardDeviation * noiseStandardDeviation;
    double twiceNoiseVariance = 2.0 * noiseVariance;
    double twoPi = 2.0 * M_PI;

    int32_t m = 0;
    int32_t measurementCount = realMeasurements.getSize();

    for ( m = 0; m < measurementCount; m++ )
    {

      double arg = ( realMeasurements( m ) * currentMeasurements( m ) ) /
                   noiseVariance;

      if ( arg < 700.0 )
      {

        logLikelihood += std::log( currentMeasurements( m ) *
                                  factory->getRegularBesselFunctionI0( arg ) / 
                                   noiseVariance ) -
                        ( currentMeasurements( m ) * currentMeasurements( m ) +
                          realMeasurements( m ) * realMeasurements( m ) ) /
                        twiceNoiseVariance;

      }
      else
      {

        logLikelihood += std::log( currentMeasurements( m ) / 
                                   noiseVariance / 
                                   std::sqrt( twoPi * arg ) ) -
                        ( currentMeasurements( m ) * currentMeasurements( m ) +
                          realMeasurements( m ) * realMeasurements( m ) ) /
                        twiceNoiseVariance +
                        arg;

      }

    }

    return logLikelihood;

  }
  GKG_CATCH( "double "
             "getLikelihood( const gkg::Vector& realMeasurements, "
             "const gkg::Vector& currentMeasurements, "
             "double noiseStandardDeviation, "
             "const gkg::NumericalAnalysisImplementationFactory* factory )" );

}





int main( int32_t argc, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameDWRef;
    
    // parameters to create synthetic data
    int32_t outputOrientationCount = 1000;
    double isotropicFraction = 0.2;
    double intracellularFraction = 0.6;
    double kappa = 4.0;
    double principalOrientationX = 1.0;
    double principalOrientationY = 0.0;
    double principalOrientationZ = 0.0;
    double parallelDiffusivity = 1.7e-9;
    double isotropicDiffusivity = 3.0e-9;
    double stationaryFraction = 0.0;
    double noiseStandardDeviation = 100;
    std::string fileNameBaseOutputVolume;
 
    // parameters to decode NODDI model from synthetic data using MCMC
    std::vector< double > vectorOfInitialParameters;
    std::vector< double > vectorOfLowerParameters;
    std::vector< double > vectorOfUpperParameters;
    std::vector< double > vectorOfDeltaParameters;
    std::vector< double > vectorOfFixedParameters;
    std::vector< double > vectorOfMCMCParameters;     
    std::string fileNameBaseDistribution;

    bool applyQuickSearch = false;
    bool applyNLP = false;
    bool applyMCMC = false;

    bool verbose = false;
    
    // managing the command line options
    gkg::Application application( argc, argv,
                                  "Noddi image estimator testing" );
    application.addSingleOption( "-dwRef",
                                 "DW reference filename",
                                 fileNameDWRef );
    application.addSingleOption( "-outputOrientationCount",
                                 "Output orientation count (default=500)",
                                 outputOrientationCount,
                                 true );
    application.addSingleOption( "-isotropicFraction",
                                 "Isotropic fraction (default=0.2)",
                                 isotropicFraction,
                                 true );
    application.addSingleOption( "-intracellularFraction",
                                 "Intracellular fraction (default=0.6)",
                                 intracellularFraction,
                                 true );
    application.addSingleOption( "-kappa",
                                 "Kappa (default=3.0)",
                                 kappa,
                                 true );
    application.addSingleOption( "-principalOrientationX",
                                 "Principal Orientation X (default=1.0)",
                                 principalOrientationX,
                                 true );
    application.addSingleOption( "-principalOrientationY",
                                 "Principal Orientation Y (default=0.0)",
                                 principalOrientationY,
                                 true );
    application.addSingleOption( "-principalOrientationZ",
                                 "Principal Orientation Z (default=0.0)",
                                 principalOrientationZ,
                                 true );
    application.addSingleOption( "-parallelDiffusivity",
                                 "Parallel diffusivity (default=1.7e-9)",
                                 parallelDiffusivity,
                                 true );
    application.addSingleOption( "-isotropicDiffusivity",
                                 "Isotropic diffusivity (default=3.0e-9)",
                                 isotropicDiffusivity,
                                 true );
    application.addSingleOption( "-stationaryFraction",
                                 "Stationary fraction (default=0.0)",
                                 stationaryFraction,
                                 true );
    application.addSingleOption( "-noiseStandardDeviation",
                                 "Noise Standard Deviation (default=100.0)",
                                 noiseStandardDeviation,
                                 true );
    
    application.addSeriesOption( "-initialParameters",
                                 "Initial parameters",
                                 vectorOfInitialParameters,
                                 9, 9 );
    application.addSeriesOption( "-lowerParameters",
                                 "Lower bound of parameters",
                                 vectorOfLowerParameters,
                                 9, 9 );
    application.addSeriesOption( "-upperParameters",
                                 "Upper bound of parameters",
                                 vectorOfUpperParameters,
                                 9, 9 );
    application.addSeriesOption( "-deltaParameters",
                                 "Variation of parameters",
                                 vectorOfDeltaParameters,
                                 9, 9 );    
    application.addSeriesOption( "-fixedParameters",
                                 "Fixed parameters",
                                 vectorOfFixedParameters,
                                 9, 9 );    
    application.addSeriesOption( "-mcmcParameters",
                                 "Burn-in count / sample count / "
                                 "interval count / maximum iteration count",
                                 vectorOfMCMCParameters,
                                 4, 4 );

    application.addSingleOption( "-ov",
                                 "Output volume base file name",
                                 fileNameBaseOutputVolume );
    application.addSingleOption( "-od",
                                 "Output distribution base file name",
                                 fileNameBaseDistribution );

    application.addSingleOption( "-applyQuickSearch",
                                 "Apply quick search optimisation "
                                 " (default=false)",
                                 applyQuickSearch,
                                 true );
    application.addSingleOption( "-applyNLP",
                                 "Apply NLP optimisation "
                                 " (default=false)",
                                 applyNLP,
                                 true );
    application.addSingleOption( "-applyMCMC",
                                 "Apply MCMC optimisation "
                                 " (default=false)",
                                 applyMCMC,
                                 true );

    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();
    
    //////////////////////////////////////////////////////////////////////////
    // getting numerical analysis implementation factory
    //////////////////////////////////////////////////////////////////////////
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                 getImplementationFactory();

    // initializing random generator 
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    
    //////////////////////////////////////////////////////////////////////////
    // generating synthetic data
    //////////////////////////////////////////////////////////////////////////
    
    // setting the parameters vector
    gkg::Vector parameters( 9 );
    parameters( 0 ) = isotropicFraction;
    parameters( 1 ) = intracellularFraction;
    parameters( 2 ) = kappa;
    parameters( 3 ) = principalOrientationX; 
    parameters( 4 ) = principalOrientationY; 
    parameters( 5 ) = principalOrientationZ; 
    parameters( 6 ) = parallelDiffusivity;
    parameters( 7 ) = isotropicDiffusivity;
    parameters( 8 ) = stationaryFraction;

    std::cout << "parameters : " << parameters << std::endl;
    std::cout << "noiseStandardDeviation : " 
              << noiseStandardDeviation << std::endl;

    // reading the reference DW just to copy the .minf part
    gkg::RCPointer< gkg::Volume< float > >
      dwReference( new gkg::Volume< float > );
    gkg::TypedVolumeReaderProcess< float >
      dwVolumeReaderProcess( *dwReference );
    dwVolumeReaderProcess.execute( fileNameDWRef );


    // creating an input orientation set
    gkg::ElectrostaticOrientationSet 
      outputOrientationSet( outputOrientationCount );

    // creating the Watson function
    gkg::RCPointer< gkg::NoddiMCMCWatsonFunction >
      noddiMCMCWatsonFunction( new 
                   gkg::NoddiMCMCWatsonFunction( dwReference,
                                                 outputOrientationSet,
                                                 noiseStandardDeviation /
                                                 1000.0 ) );

    // collecting attenuation values corresponding to input orientations
    gkg::Vector values( dwReference->getSizeT() );
    noddiMCMCWatsonFunction->getValuesAt( parameters, values );
  
    // creating a T2 (b=0) volume
    gkg::Volume< float > t2( 1, 1, 1 );
    t2( 0, 0, 0 ) = 1000.0;
   
    // creating a DW volume and filling it with attenuations corrupted by
    // Rician noise
    gkg::Volume< float > dw( 1, 1, 1, dwReference->getSizeT() );
    dw.getHeader() = dwReference->getHeader();
    dw.getHeader()[ "resolutionX" ] = 1.0;
    dw.getHeader()[ "resolutionY" ] = 1.0;
    dw.getHeader()[ "resolutionZ" ] = 1.0;
    dw.getHeader()[ "resolutionT" ] = 1.0;
    dw.getHeader()[ "sizeX" ] = 1;
    dw.getHeader()[ "sizeY" ] = 1;
    dw.getHeader()[ "sizeZ" ] = 1;

    
    int32_t o =0; 
    for ( o = 0; o < noddiMCMCWatsonFunction->getInputOrientationCount(); o++ )
    {
    
      double noiseFreeSignal =( double )t2( 0, 0, 0 ) * values( o );

      // adding Rician noise to noise free signal
      double noiseRealChannel = 
                   factory->getGaussianRandomNumber( randomGenerator,
                                                      0.0,
                                                      noiseStandardDeviation );
                   
      double noiseImaginaryChannel = 
                   factory->getGaussianRandomNumber( randomGenerator,
                                                      0.0,
                                                      noiseStandardDeviation );
      
      dw( 0, 0, 0, o ) = std::sqrt( ( noiseFreeSignal + noiseRealChannel ) *
                                    ( noiseFreeSignal + noiseRealChannel ) +
                                    ( noiseImaginaryChannel * 
                                      noiseImaginaryChannel ) );  

      if ( dw( 0, 0, 0, o ) <= 0 )
      {

        dw( 0, 0, 0, o ) = 1;

      }
    
    }
    
    // creating mask
    gkg::Volume< int16_t > mask( 1, 1, 1 );
    mask( 0, 0, 0 ) = 1;
    
    // saving results to disk
    gkg::Writer::getInstance().write( fileNameBaseOutputVolume + "_t2.ima",
                                      t2 );
    gkg::Writer::getInstance().write( fileNameBaseOutputVolume + "_dw.ima",
                                      dw );
    gkg::Writer::getInstance().write( fileNameBaseOutputVolume + "_mask.ima",
                                      mask );


    //////////////////////////////////////////////////////////////////////////
    // preparing normalizedSignal
    //////////////////////////////////////////////////////////////////////////
    
    gkg::Vector
    normalizedSignal( noddiMCMCWatsonFunction->getInputOrientationCount() );
    for ( o = 0; o < noddiMCMCWatsonFunction->getInputOrientationCount(); o++ )
    {
      
      normalizedSignal( o ) = ( double )dw( 0, 0, 0, o ) / 
                              ( double )t2( 0, 0, 0 );
                              
    } 
    
    //////////////////////////////////////////////////////////////////////////
    // preparing a NODDI MCMC parameter set
    //////////////////////////////////////////////////////////////////////////
    
    gkg::Vector initialParameters( 9 );
    // initial isotropic fraction
    initialParameters( 0 ) = vectorOfInitialParameters[ 0 ]; 
    // initial intracellular fraction
    initialParameters( 1 ) = vectorOfInitialParameters[ 1 ]; 
    // initial kappa
    initialParameters( 2 ) = vectorOfInitialParameters[ 2 ]; 
    // orientation x
    initialParameters( 3 ) = vectorOfInitialParameters[ 3 ]; 
    // orientation y
    initialParameters( 4 ) = vectorOfInitialParameters[ 4 ]; 
    // orientation z
    initialParameters( 5 ) = vectorOfInitialParameters[ 5 ]; 
    // initial parallel diffusivity
    initialParameters( 6 ) = vectorOfInitialParameters[ 6 ]; 
    // initial isotropic diffusivity
    initialParameters( 7 ) = vectorOfInitialParameters[ 7 ]; 
    // initial stationary fraction
    initialParameters( 8 ) = vectorOfInitialParameters[ 8 ]; 
    
    gkg::Vector lowerParameters( 9 );
    // lower isotropic fraction
    lowerParameters( 0 ) = vectorOfLowerParameters[ 0 ]; 
    // lower intracellular fraction
    lowerParameters( 1 ) = vectorOfLowerParameters[ 1 ]; 
    // lower kappa
    lowerParameters( 2 ) = vectorOfLowerParameters[ 2 ]; 
    // lower orientation x
    lowerParameters( 3 ) = vectorOfLowerParameters[ 3 ]; 
    // lower orientation y
    lowerParameters( 4 ) = vectorOfLowerParameters[ 4 ]; 
    // lower orientation z
    lowerParameters( 5 ) = vectorOfLowerParameters[ 5 ]; 
    // lower parallel diffusivity
    lowerParameters( 6 ) = vectorOfLowerParameters[ 6 ]; 
    // lower isotropic diffusivity
    lowerParameters( 7 ) = vectorOfLowerParameters[ 7 ]; 
    // lower stationary fraction
    lowerParameters( 8 ) = vectorOfLowerParameters[ 8 ]; 
        
    
    gkg::Vector upperParameters( 9 );
    // upper isotropic fraction
    upperParameters( 0 ) = vectorOfUpperParameters[ 0 ]; 
    // upper intracellular fraction
    upperParameters( 1 ) = vectorOfUpperParameters[ 1 ]; 
    // upper kappa
    upperParameters( 2 ) = vectorOfUpperParameters[ 2 ]; 
    // upper orientation x
    upperParameters( 3 ) = vectorOfUpperParameters[ 3 ]; 
    // upper orientation y
    upperParameters( 4 ) = vectorOfUpperParameters[ 4 ]; 
    // upper orientation z
    upperParameters( 5 ) = vectorOfUpperParameters[ 5 ]; 
    // upper parallel diffusivity
    upperParameters( 6 ) = vectorOfUpperParameters[ 6 ]; 
    // upper isotropic diffusivity
    upperParameters( 7 ) = vectorOfUpperParameters[ 7 ]; 
    // upper stationary fraction
    upperParameters( 8 ) = vectorOfUpperParameters[ 8 ]; 
    
    gkg::Vector deltaParameters( 9 );
    // delta isotropic fraction
    deltaParameters( 0 ) = vectorOfDeltaParameters[ 0 ]; 
    // delta intracellular fraction
    deltaParameters( 1 ) = vectorOfDeltaParameters[ 1 ]; 
    // delta kappa
    deltaParameters( 2 ) = vectorOfDeltaParameters[ 2 ]; 
    // delta orientation x
    deltaParameters( 3 ) = vectorOfDeltaParameters[ 3 ]; 
    // delta orientation y
    deltaParameters( 4 ) = vectorOfDeltaParameters[ 4 ]; 
    // delta orientation z
    deltaParameters( 5 ) = vectorOfDeltaParameters[ 5 ]; 
    // delta parallel diffusivity
    deltaParameters( 6 ) = vectorOfDeltaParameters[ 6 ]; 
    // delta isotropic diffusivity
    deltaParameters( 7 ) = vectorOfDeltaParameters[ 7 ]; 
    // delta stationary fraction
    deltaParameters( 8 ) = vectorOfDeltaParameters[ 8 ]; 
    
    std::vector< bool > fixedParameters( 9 );   
     // fixed isotropic fraction
    fixedParameters[ 0 ] = ( vectorOfFixedParameters[ 0 ] > 0 ? true : false ); 
    // fixed intracellular fraction
    fixedParameters[ 1 ] = ( vectorOfFixedParameters[ 1 ] > 0 ? true : false ); 
    // fixed kappa
    fixedParameters[ 2 ] = ( vectorOfFixedParameters[ 2 ] > 0 ? true : false ); 
    // fixed orientation x
    fixedParameters[ 3 ] = ( vectorOfFixedParameters[ 3 ] > 0 ? true : false ); 
    // fixed orientation y
    fixedParameters[ 4 ] = ( vectorOfFixedParameters[ 4 ] > 0 ? true : false ); 
    // fixed orientation z
    fixedParameters[ 5 ] = ( vectorOfFixedParameters[ 5 ] > 0 ? true : false ); 
    // fixed parallel diffusivity
    fixedParameters[ 6 ] = ( vectorOfFixedParameters[ 6 ] > 0 ? true : false ); 
    // fixed isotropic diffusivity
    fixedParameters[ 7 ] = ( vectorOfFixedParameters[ 7 ] > 0 ? true : false ); 
    // fixed stationary fraction
    fixedParameters[ 8 ] = ( vectorOfFixedParameters[ 8 ] > 0 ? true : false ); 

    // creating a dedicated parameter set
    gkg::RCPointer< gkg::NoddiMCMCParameterSet > 
      noddiMCMCParameterSet( new gkg::NoddiMCMCWatsonParameterSet(
                                                           lowerParameters,
                                                           upperParameters,
                                                           fixedParameters,
                                                           deltaParameters ) );

    
    //////////////////////////////////////////////////////////////////////////
    // computing the tensor model to get the initial orientation
    //////////////////////////////////////////////////////////////////////////

    gkg::Tensor tensor( 0,
                        t2, 
                        dw,
                        gkg::Vector3d< int32_t >( 0, 0, 0 ),
                        gkg::Tensor::RobustPositiveDefinite );

    // setting the principal orientation to the first eigenvector of the 
    // diffusion tensor model
    const gkg::Vector3d< float >& principalOrientation =
                                 tensor.getEigenVector( gkg::Tensor::Maximum );


    initialParameters( 3 ) = principalOrientation.x;
    initialParameters( 4 ) = principalOrientation.y;
    initialParameters( 5 ) = principalOrientation.z;

    std::cout << "principal orientation detected using tensor model: "
              << principalOrientation
              << std::endl;


    //////////////////////////////////////////////////////////////////////////
    // performing quick evaluation of likelyhood
    // (quick search only on volume fractions)
    //////////////////////////////////////////////////////////////////////////

    // creating the NLP function
    gkg::RCPointer< gkg::NoddiNLPFunction >
      noddiNLPFunction( new gkg::NoddiNLPFunction( noddiMCMCWatsonFunction,
                                                   normalizedSignal ) );


    if ( applyQuickSearch )
    {

      std::cout << "likelihood when normalizedSignal is chosen: "
        	<< getLogLikelihood( normalizedSignal,
        			     normalizedSignal,
        			     noiseStandardDeviation / 1000.0,
        			     factory )
        	<< std::endl;

      gkg::Volume< float > minusLogLikelihoods( STEP_COUNT_P1,
                                                STEP_COUNT_P2,
                                                STEP_COUNT_P3,
                                                1 );
      int32_t p1, p2, p3;
      double optimalP1 = 0.0;
      double optimalP2 = 0.0;
      double optimalP3 = 0.0;
      double optimalMinusLogLikelihood = 1e38;

      double minusLogLikelihood = 0.0;

      for ( p3 = 0; p3 < STEP_COUNT_P3; p3++ )
      {


        std::cout << "p3=" << p3 << std::endl;
        for ( p2 = 0; p2 < STEP_COUNT_P2; p2++ )
        {

          for ( p1 = 0; p1 < STEP_COUNT_P1; p1++ )
          {

            gkg::Vector quickSearchParameters( initialParameters );
            quickSearchParameters( 0 ) =
                              lowerParameters( 0 ) +
        		      p1 * ( upperParameters( 0 ) -
        			     lowerParameters( 0 ) ) / STEP_COUNT_P1;
            quickSearchParameters( 1 ) =
                              lowerParameters( 1 ) +
        		      p2 * ( upperParameters( 1 ) -
        			     lowerParameters( 1 ) ) / STEP_COUNT_P2;
            quickSearchParameters( 2 ) =
                              lowerParameters( 2 ) +
        		      p3 * ( upperParameters( 2 ) -
        			     lowerParameters( 2 ) ) / STEP_COUNT_P3;

            minusLogLikelihood = noddiNLPFunction->getValueAt(
                                                        quickSearchParameters );
            minusLogLikelihoods( p1, p2, p3 ) = minusLogLikelihood;
            if ( minusLogLikelihood < optimalMinusLogLikelihood )
            {

               optimalP1 = quickSearchParameters( 0 );
               optimalP2 = quickSearchParameters( 1 );
               optimalP3 = quickSearchParameters( 2 );
               optimalMinusLogLikelihood = minusLogLikelihood;

            }

          }

        }

      }
      gkg::Writer::getInstance().write( fileNameBaseOutputVolume +
        				"_minusLoglikelihood.ima",
        				minusLogLikelihoods );

      // replacing initial parameters by those found using the quick search
      initialParameters( 0 ) = optimalP1;
      initialParameters( 1 ) = optimalP2;
      initialParameters( 2 ) = optimalP3;

      std::cout << "quick optimized initial parameters : " << initialParameters
        	<< std::endl;

    }



    //////////////////////////////////////////////////////////////////////////
    // preparing and running the NLP optimization
    //////////////////////////////////////////////////////////////////////////

    if ( applyNLP )
    {

      std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >
        constraints( 9 );

      constraints[ 0 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 0 ),
                                                       upperParameters( 0 ) ) );
      constraints[ 1 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 1 ),
                                                       upperParameters( 1 ) ) );
      constraints[ 2 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 2 ),
                                                       upperParameters( 2 ) ) );
      constraints[ 3 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 3 ),
                                                       upperParameters( 3 ) ) );
      constraints[ 4 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 4 ),
                                                       upperParameters( 4 ) ) );
      constraints[ 5 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 5 ),
                                                       upperParameters( 5 ) ) );
      constraints[ 6 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 6 ),
                                                       upperParameters( 6 ) ) );
      constraints[ 7 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 7 ),
                                                       upperParameters( 7 ) ) );
      constraints[ 8 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerParameters( 8 ),
                                                       upperParameters( 8 ) ) );
      std::vector< double > optimizerParameters( 11 );
      optimizerParameters[ 0 ] = 1000;
      optimizerParameters[ 1 ] = 0.001;
      optimizerParameters[ 2 ] = ( fixedParameters[ 0 ] ? 0.0 :
                                                        deltaParameters( 0 ) );
      optimizerParameters[ 3 ] = ( fixedParameters[ 1 ] ? 0.0 :
                                                        deltaParameters( 1 ) );
      optimizerParameters[ 4 ] = ( fixedParameters[ 2 ] ? 0.0 :
                                                        deltaParameters( 2 ) );
      optimizerParameters[ 5 ] = ( fixedParameters[ 3 ] ? 0.0 :
                                                        deltaParameters( 3 ) );
      optimizerParameters[ 6 ] = ( fixedParameters[ 4 ] ? 0.0 :
                                                        deltaParameters( 4 ) );
      optimizerParameters[ 7 ] = ( fixedParameters[ 5 ] ? 0.0 :
                                                        deltaParameters( 5 ) );
      optimizerParameters[ 8 ] = ( fixedParameters[ 6 ] ? 0.0 :
                                                        deltaParameters( 6 ) );
      optimizerParameters[ 9 ] = ( fixedParameters[ 7 ] ? 0.0 :
                                                        deltaParameters( 7 ) );
      optimizerParameters[ 10 ] = ( fixedParameters[ 8 ] ? 0.0 :
                                                        deltaParameters( 8 ) );

      factory->getConstrainedNonLinearProgrammingOptimumParameters(
            gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
            initialParameters,
            constraints,
            *noddiNLPFunction,
            optimizerParameters,
            verbose );
   
      std::cout << "Contrained NLP optimized parameters : " << initialParameters
                << std::endl;

    }

    //////////////////////////////////////////////////////////////////////////
    // preparing and running the MCMC optimization
    //////////////////////////////////////////////////////////////////////////
    if ( applyMCMC )
    {

      gkg::Matrix parameterEstimates;
      int32_t updateCallCount = 0;
      double acceptanceRate = 0.0;

      int32_t mcmcBurninCount =
                               ( int32_t )( vectorOfMCMCParameters[ 0 ] + 0.5 );
      int32_t mcmcSampleCount = 
                              ( int32_t )( vectorOfMCMCParameters[ 1 ] + 0.5 );
      int32_t mcmcIntervalCount = ( int32_t )( vectorOfMCMCParameters[ 2 ] +
                                                                         0.5 );
      int32_t mcmcMaximumIterationCount = ( int32_t )(
                                           vectorOfMCMCParameters[ 3 ] + 0.5 );
      gkg::MonteCarloMarkovChainEstimator 
        monteCarloMarkovChainEstimator( noddiMCMCWatsonFunction,
                                        noddiMCMCParameterSet );
        

      monteCarloMarkovChainEstimator.estimate( normalizedSignal,
 					       initialParameters,
 					       parameterEstimates,
 					       updateCallCount,
 					       acceptanceRate,
 					       mcmcBurninCount,
 					       mcmcSampleCount,
 					       mcmcIntervalCount,
 					       1,
 					       mcmcMaximumIterationCount,
 					       true );

      //////////////////////////////////////////////////////////////////////////
      // computing parameter histograms
      //////////////////////////////////////////////////////////////////////////
    
      int32_t p = 0;
      int32_t s = 0;
      for (  p = 0; p < 9; p++ )
      {

        if ( fixedParameters[ p ] == false )
        {


          std::cout << "building histogram of parameter " << p << std::endl;

          gkg::Vector samples( mcmcSampleCount );
          for ( s = 0; s < mcmcSampleCount; s++ )
          {
 
            samples( s ) = parameterEstimates( p, s );

          }

          std::vector< double > histogram( 100 );
          double lowerBoundary = 0.0;
          double upperBoundary = 0.0;

          try
          {

            gkg::HistogramAnalyzer::getInstance().getHistogram(
        					  samples,
        					  histogram,
        					  lowerBoundary,
        					  upperBoundary,
        					  deltaParameters( p ) / 1.0 );

 	    std::ofstream os( ( fileNameBaseDistribution + '-' +
 	    			gkg::StringConverter::toString( p ) +
 	    			".csv" ).c_str() );
 	    for ( s = 0; s < 100; s++ )
 	    {

 	      os << lowerBoundary +
 	    	    ( ( upperBoundary - lowerBoundary ) /
 	    	      ( double )( 100 - 1 ) ) * ( double ) s
 	    	 << "\t"
 	    	 << histogram[ s ]
 	    	 << std::endl;

 	    }
 	    os.close();

          }
          catch ( std::exception& )
          {


            std::cout << "special case for parameter " << p << ": "
                      << "value is " << samples( 0 ) << std::endl;

 	    std::ofstream os( ( fileNameBaseDistribution + '-' +
 	    			gkg::StringConverter::toString( p ) +
 	    			".csv" ).c_str() );
 	    for ( s = 0; s < 100; s++ )
 	    {

              lowerBoundary = samples( s ) - deltaParameters( p );
              upperBoundary = samples( s ) + deltaParameters( p );

 	      os << lowerBoundary +
 	    	    ( ( upperBoundary - lowerBoundary ) /
 	    	      ( double )( 100 - 1 ) ) * ( double ) s
 	    	 << "\t"
 	    	 << ( ( s == 49 ) ? mcmcSampleCount : 0.0 )
 	    	 << std::endl;

 	    }
 	    os.close();

          }

        }

      }

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

  
}
