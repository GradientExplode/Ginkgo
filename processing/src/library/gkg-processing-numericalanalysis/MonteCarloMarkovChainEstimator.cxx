#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainEstimator.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>


gkg::MonteCarloMarkovChainEstimator::MonteCarloMarkovChainEstimator(
              gkg::RCPointer< gkg::MonteCarloMarkovChainFunction >
                                                 monteCarloMarkovChainFunction,
              gkg::RCPointer< gkg::MonteCarloMarkovChainParameterSet > 
                                            monteCarloMarkovChainParameterSet )
                                    : _monteCarloMarkovChainFunction(
                                                monteCarloMarkovChainFunction ),
                                      _monteCarloMarkovChainParameterSet(
                                             monteCarloMarkovChainParameterSet )
{

  try
  {

    _numericalAnalysisImplementationFactory = 
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();
    _randomGenerator.reset(
                       new gkg::RandomGenerator( gkg::RandomGenerator::Taus ) );

  }
  GKG_CATCH( "gkg::MonteCarloMarkovChainEstimator::"
             "MonteCarloMarkovChainEstimator( "
             "gkg::RCPointer< gkg::MonteCarloMarkovChainFunction > "
             "monteCarloMarkovChainFunction )" );

}


gkg::MonteCarloMarkovChainEstimator::~MonteCarloMarkovChainEstimator()
{
}


void gkg::MonteCarloMarkovChainEstimator::estimate(
                                     const gkg::Vector& realMeasurements,
                                     const gkg::Vector& initialParameters,
                                     gkg::Matrix& parameterEstimates,
                                     int32_t& updateCallCount,
                                     double& acceptanceRate,
                                     int32_t burninCount,
                                     int32_t sampleCount,
                                     int32_t intervalCount,
                                     int32_t runCount,
                                     int32_t maximumIterationCount,
                                     bool verbose ) const
{

  try
  {

    // sanity check(s)
    int32_t parameterCount =
                        _monteCarloMarkovChainParameterSet->getParameterCount();

    _monteCarloMarkovChainParameterSet->setCurrentParameters(
                                                            initialParameters );


    // allocating vector for current signal
    gkg::Vector currentMeasurements( realMeasurements.getSize() );
    _monteCarloMarkovChainFunction->getValuesAt( initialParameters,
                                                 currentMeasurements );

    // reallocating the output matrix of samples to the adequate size
    parameterEstimates.reallocate( parameterCount, sampleCount * runCount );
    parameterEstimates.fill( 0.0 );

    // looping over MCMC runs
    int32_t r = 0;
    updateCallCount = 0;
    int32_t acceptanceCount = 0;
    for ( r = 0; r < runCount; r++ )
    {

      // initializing MCMC through burnin steps
      int32_t b = 0;
      for ( b = 0; b < burninCount; b++ )
      {

        if ( verbose )
        {

          std::cout << "run=" << r << "  burnin=" <<  b << std::endl;  

        }
        this->updateParameters( realMeasurements,
                                currentMeasurements,
                                acceptanceCount );
        ++ updateCallCount;

      }

      // sampling at regular intervals
      int32_t i = 0;
      while ( i < std::min( sampleCount, maximumIterationCount ) )
      {

        // do not sample during the intervals
        int32_t j = 0;
        for ( j = 0; j < intervalCount; j++ )
        {

          if ( verbose )
          {

            std::cout << "run=" << r
                      << "  sample=" << i
                      << "  interval=" << j << std::endl;

          }

          this->updateParameters( realMeasurements,
                                  currentMeasurements,
                                  acceptanceCount );
          ++ updateCallCount;

        }

        // storing the actual sampling
        parameterEstimates.setColumn( sampleCount * r + i, 
                                      _monteCarloMarkovChainParameterSet->
                                                       getCurrentParameters() );

        ++ i;

      }

    }

    if ( updateCallCount > 0 )
    {

      acceptanceRate = ( double )acceptanceCount / ( double )updateCallCount;

    }
    else
    {

      acceptanceRate = 0.0;

    }

  }
  GKG_CATCH( "void gkg::MonteCarloMarkovChainEstimator::estimate( "
             "const gkg::Vector& realMeasurements, "
             "const gkg::Vector& initialParameters, "
             "gkg::Matrix& parameterEstimates, "
             "int32_t& updateCallCount, "
             "double& acceptanceRate, "
             "int32_t burninCount, "
             "int32_t sampleCount, "
             "int32_t intervalCount, "
             "int32_t runCount, "
             "int32_t maximumIterationCount ) const" );

}


void gkg::MonteCarloMarkovChainEstimator::updateParameters(
                                   const gkg::Vector& realMeasurements,
                                   gkg::Vector& currentMeasurements,
                                   int32_t& acceptanceCount ) const
{

  try
  {

    bool isBetweenBounds =
                         _monteCarloMarkovChainParameterSet->updateParameters();

    if ( isBetweenBounds )
    {

      gkg::Vector newMeasurements( realMeasurements.getSize() );

      _monteCarloMarkovChainFunction->getValuesAt( 
         _monteCarloMarkovChainParameterSet->getNewParameters(),
         newMeasurements );
          
      double likelihoodRatio = _monteCarloMarkovChainFunction->
                                        getLikelihoodRatio( realMeasurements,
                                                            currentMeasurements,
                                                            newMeasurements );
      double uniformRandomNumber = _numericalAnalysisImplementationFactory->
                         getUniformRandomNumber( *_randomGenerator, 0.0, 1.0 );

      if ( likelihoodRatio > uniformRandomNumber )
      {

        _monteCarloMarkovChainParameterSet->acceptNewParameters();
        currentMeasurements = newMeasurements;
        ++ acceptanceCount;

      }


    }

    

  }
  GKG_CATCH( "void gkg::MonteCarloMarkovChainEstimator::updateParameters( "
             "const gkg::Vector& realMeasurements, "
             "gkg::Vector& currentMeasurements, "
             "int32_t& acceptanceCount ) const" );


}


