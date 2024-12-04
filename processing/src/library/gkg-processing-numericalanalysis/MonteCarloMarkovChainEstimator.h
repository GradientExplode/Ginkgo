#ifndef _gkg_processing_numericalanalysis_MonteCarloMarkovChainEstimator_h_
#define _gkg_processing_numericalanalysis_MonteCarloMarkovChainEstimator_h_


#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainFunction.h>
#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainParameterSet.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class NumericalAnalysisImplementationFactory;


class MonteCarloMarkovChainEstimator
{

  public:

    MonteCarloMarkovChainEstimator(
      RCPointer< MonteCarloMarkovChainFunction > monteCarloMarkovChainFunction,
      RCPointer< MonteCarloMarkovChainParameterSet > 
                                            monteCarloMarkovChainParameterSet );
    virtual ~MonteCarloMarkovChainEstimator();

    //
    // measurements: the actual measurements to be fitted with the MCMC function
    // initialParameters: default parameter set at the begining of the MCMC
    // burninCount: number of burnin steps to initialize the MCMC algorithm
    // sampleCount: final number of samples collected (eg second size of the 
    //              output matrix)
    // interval: number of steps between each sample to preserve statistical
    //           independence of the estimates
    // runCount: number of times the whole procedure has to be run
    //
    void estimate( const Vector& realMeasurements,
                   const Vector& initialParameters,
                   Matrix& parameterEstimates,
                   int32_t& updateCallCount,
                   double& acceptanceRate,
                   int32_t burninCount = 10000,
                   int32_t sampleCount = 500,
                   int32_t intervalCount = 1000,
                   int32_t runCount = 1,
                   int32_t maximumIterationCount = 1500,
                   bool verbose = false ) const;
                   

  protected:

    void updateParameters( const Vector& realMeasurements,
                           Vector& currentMeasurements,
                           int32_t& acceptanceCount ) const;

    RCPointer< MonteCarloMarkovChainFunction > _monteCarloMarkovChainFunction;
    RCPointer< MonteCarloMarkovChainParameterSet >
                                             _monteCarloMarkovChainParameterSet;
    NumericalAnalysisImplementationFactory* 
                                        _numericalAnalysisImplementationFactory;
    RCPointer< RandomGenerator > _randomGenerator;

};


}


#endif
