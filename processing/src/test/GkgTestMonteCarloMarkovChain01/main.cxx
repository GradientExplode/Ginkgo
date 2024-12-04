#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainEstimator.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


#define MEASUREMENT_COUNT 500
#define NOISE_SIGMA 0.01



////////////////////////////////////////////////////////////////////////////////
// class MyMCMCFunction
////////////////////////////////////////////////////////////////////////////////

class MyMCMCFunction : public gkg::MonteCarloMarkovChainFunction
{

  public:

    MyMCMCFunction();
    virtual ~MyMCMCFunction();

    void getValuesAt( const gkg::Vector& parameters,
                      gkg::Vector& values ) const;
    double getLikelihoodRatio( const gkg::Vector& realMeasurements,
                               const gkg::Vector& currentMeasurements,
                               const gkg::Vector& newMeasurements ) const;

    const gkg::Vector& getRealXs() const;
    const gkg::Vector& getRealMeasurements() const;

  protected:

    gkg::NumericalAnalysisImplementationFactory* _factory;
    gkg::RCPointer< gkg::RandomGenerator > _randomGenerator;

    gkg::Vector _realXs;
    gkg::Vector _realMeasurements;

};


MyMCMCFunction::MyMCMCFunction()
               : gkg::MonteCarloMarkovChainFunction()
{

  try
  {

    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    _randomGenerator.reset(
                       new gkg::RandomGenerator( gkg::RandomGenerator::Taus ) );

    _realXs.reallocate( MEASUREMENT_COUNT );
    _realMeasurements.reallocate( MEASUREMENT_COUNT );

    int32_t m = 0;
    for ( m = 0; m < MEASUREMENT_COUNT; m++ )
    {

      _realXs( m ) = -10.0 + 30.0 * ( double )m /
                                    ( double )( MEASUREMENT_COUNT - 1 );
      _realMeasurements( m ) = 0.3 *  std::exp( -0.2 * _realXs( m ) *
                                                       _realXs( m ) ) +
                               0.7 * std::exp( -0.2 * ( _realXs( m ) - 10 ) *
                                                      ( _realXs( m ) - 10 ) ) +
                               _factory->getGaussianRandomNumber( 
                                                              *_randomGenerator,
                                                              0.0,
                                                              NOISE_SIGMA );

    }

  }
  GKG_CATCH( "MyMCMCFunction::MyMCMCFunction()" );

}


MyMCMCFunction::~MyMCMCFunction()
{
}


// m = 0.3 *  std::exp( -0.2 * x^2 ) + 0.7 * std::exp( -0.2 * ( x - 10 )^2 )

void MyMCMCFunction::getValuesAt( const gkg::Vector& parameters,
                                  gkg::Vector& values ) const
{

  try
  {

    int32_t m = 0;
    for ( m = 0; m < MEASUREMENT_COUNT; m++ )
    {

      values( m ) = parameters( 0 ) *  std::exp( parameters( 1 ) *
                                       _realXs( m ) * _realXs( m ) ) +
                    parameters( 2 ) * std::exp( parameters( 3 ) *
                                           ( _realXs( m ) +  parameters( 4 ) ) *
                                           ( _realXs( m ) + parameters( 4 ) ) );

    }

  }
  GKG_CATCH( "void MyMCMCFunction::getValuesAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& values ) const" );

}


double MyMCMCFunction::getLikelihoodRatio(
                                      const gkg::Vector& realMeasurements,
                                      const gkg::Vector& currentMeasurements,
                                      const gkg::Vector& newMeasurements ) const
{

  try
  {

    double logLikelihood1 = 0.0;
    double logLikelihood2 = 0.0;

    int32_t m = 0;
    for ( m = 0; m < MEASUREMENT_COUNT; m++ )
    {

      logLikelihood1 += -std::log( NOISE_SIGMA * std::sqrt( 2 * M_PI ) ) -
                        ( ( currentMeasurements( m ) -
                            realMeasurements( m ) ) *
                          ( currentMeasurements( m ) -
                            realMeasurements( m ) ) ) / 
                        ( 2.0 * NOISE_SIGMA * NOISE_SIGMA );
                        
      logLikelihood2 += -std::log( NOISE_SIGMA * std::sqrt( 2 * M_PI ) ) -
                        ( ( newMeasurements( m ) -
                            realMeasurements( m ) ) *
                          ( newMeasurements( m ) -
                            realMeasurements( m ) ) ) / 
                        ( 2.0 * NOISE_SIGMA * NOISE_SIGMA );



    }

    return std::exp( logLikelihood2 - logLikelihood1 );

  }
  GKG_CATCH( "double MyMCMCFunction::getLikelihoodRatio( "
             "const gkg::Vector& realMeasurements, "
             "const gkg::Vector& currentMeasurements, "
             "const gkg::Vector& newMeasurements ) const" );

}


const gkg::Vector& MyMCMCFunction::getRealXs() const
{

  try
  {

    return _realXs;

  }
  GKG_CATCH( "const gkg::Vector& MyMCMCFunction::getRealXs() const" );

}


const gkg::Vector& MyMCMCFunction::getRealMeasurements() const
{

  try
  {

    return _realMeasurements;

  }
  GKG_CATCH( "const gkg::Vector& MyMCMCFunction::getRealMeasurements() const" );

}


////////////////////////////////////////////////////////////////////////////////
// class MyMCMCParameterSet
////////////////////////////////////////////////////////////////////////////////

class MyMCMCParameterSet : public gkg::MonteCarloMarkovChainParameterSet
{

  public:

    MyMCMCParameterSet( const gkg::Vector& lowerBounds,
                        const gkg::Vector& upperBounds,
                        const std::vector< bool >& fixed,
                        const gkg::Vector& steps );
    virtual ~MyMCMCParameterSet();

    bool updateParameters();

  protected:

    gkg::NumericalAnalysisImplementationFactory* _factory;
    gkg::RCPointer< gkg::RandomGenerator > _randomGenerator;

};


MyMCMCParameterSet::MyMCMCParameterSet( const gkg::Vector& lowerBounds,
                                        const gkg::Vector& upperBounds,
                                        const std::vector< bool >& fixed,
                                        const gkg::Vector& steps )
                    : gkg::MonteCarloMarkovChainParameterSet( 5,
                                                              lowerBounds,
                                                              upperBounds,
                                                              fixed,
                                                              steps )
{

  try
  {

    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    _randomGenerator.reset(
                       new gkg::RandomGenerator( gkg::RandomGenerator::Taus ) );

  }
  GKG_CATCH( "MyMCMCParameterSet::MyMCMCParameterSet( "
             "const gkg::Vector& lowerBounds, "
             "const gkg::Vector& upperBounds, "
             "const std::vector< bool >& fixed, "
             "const gkg::Vector& steps )" );

}


MyMCMCParameterSet::~MyMCMCParameterSet()
{
}


bool MyMCMCParameterSet::updateParameters()
{

  try
  {
 
    int32_t p = 0;
    for ( p = 0; p < this->_parameterCount; p++ )
    {

      if ( this->_fixed[ p ] == false )
      { 

        this->_newParameters( p ) = this->_currentParameters( p ) +
                                    _steps( p ) *
                                    _factory->getGaussianRandomNumber(
                                                              *_randomGenerator,
                                                              0.0,
                                                              1.0 );
        if ( ( this->_newParameters( p ) < this->_lowerBounds( p ) ) ||
             ( this->_newParameters( p ) > this->_upperBounds( p ) ) )
        {

          return false;

        }

      }

    }

    return true;

  }
  GKG_CATCH( "bool MyMCMCParameterSet::updateParameters()" );

}



////////////////////////////////////////////////////////////////////////////////
// main()
////////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    int32_t burninCount = 0;
    int32_t sampleCount = 0;
    int32_t intervalCount = 0;
    int32_t runCount = 0;
    int32_t maximumIterationCount = 0;
    std::string outputName = "";

    gkg::Application application( argc, argv,
                                  "MCMC estimator testing" );
    application.addSingleOption( "-b",
                                 "Burn-in count",
                                 burninCount );
    application.addSingleOption( "-s",
                                 "Sample count",
                                 sampleCount );
    application.addSingleOption( "-i",
                                 "Interval count",
                                 intervalCount );
    application.addSingleOption( "-r",
                                 "Run count",
                                 runCount );
    application.addSingleOption( "-m",
                                 "Maximum iteration count",
                                 maximumIterationCount );
    application.addSingleOption( "-o",
                                 "Output text file",
                                 outputName );

    application.initialize();


    gkg::Vector lowerBounds( 5 );
    lowerBounds( 0 ) = 0.0;
    lowerBounds( 1 ) = -1.0;
    lowerBounds( 2 ) = 0.0;
    lowerBounds( 3 ) = -1.0;
    lowerBounds( 4 ) = -20.0;


    gkg::Vector upperBounds( 5 );
    upperBounds( 0 ) = 1.0;
    upperBounds( 1 ) = 0.0;
    upperBounds( 2 ) = 1.0;
    upperBounds( 3 ) = 0.0;
    upperBounds( 4 ) = 0.0;

    std::vector< bool > fixed( 5 );
    fixed[ 0 ] = false;
    fixed[ 1 ] = false;
    fixed[ 2 ] = false;
    fixed[ 3 ] = false;
    fixed[ 4 ] = false;

    gkg::Vector steps( 5 );
    steps( 0 ) = 0.1;
    steps( 1 ) = 0.1;
    steps( 2 ) = 0.1;
    steps( 3 ) = 0.1;
    steps( 4 ) = +5.0;

    MyMCMCParameterSet* myMCMCParameterSet = new MyMCMCParameterSet( 
                                                                    lowerBounds,
                                                                    upperBounds,
                                                                    fixed,
                                                                    steps );
    gkg::RCPointer< gkg::MonteCarloMarkovChainParameterSet >
      monteCarloMarkovChainParameterSet( myMCMCParameterSet  );

    MyMCMCFunction* myMCMCFunction = new MyMCMCFunction;
    gkg::RCPointer< gkg::MonteCarloMarkovChainFunction >
      monteCarloMarkovChainFunction( myMCMCFunction );


    gkg::MonteCarloMarkovChainEstimator
      monteCarloMarkovChainEstimator( monteCarloMarkovChainFunction,
                                      monteCarloMarkovChainParameterSet );
    gkg::Matrix parameterEstimates;

    int32_t updateCallCount = 0;
    double acceptanceRate = 0.0;


    gkg::Vector initialParameters( 5 );
    initialParameters( 0 ) = 0.0;
    initialParameters( 1 ) = 0.0;
    initialParameters( 2 ) = 0.0;
    initialParameters( 3 ) = 0.0;
    initialParameters( 4 ) = 0.0;

    monteCarloMarkovChainEstimator.estimate(
                                          myMCMCFunction->getRealMeasurements(),
                                          initialParameters,
                                          parameterEstimates,
                                          updateCallCount,
                                          acceptanceRate,
                                          burninCount,
                                          sampleCount,
                                          intervalCount,
                                          runCount,
                                          maximumIterationCount,
                                          true );


    std::cout << "updateCallCount : " << updateCallCount << std::endl;
    std::cout << "acceptanceRate : " << acceptanceRate << std::endl;

    int32_t p = 0;
    int32_t s = 0;
    for (  p = 0; p < 5; p++ )
    {

      if ( fixed[ p ] == false )
      {

        gkg::Vector samples( sampleCount );
        for ( s = 0; s < sampleCount; s++ )
        {
 
          samples( s ) = parameterEstimates( p, s );

        }

        std::vector< double > histogram( 100 );
        double lowerBoundary = 0.0;
        double upperBoundary = 0.0;

        gkg::HistogramAnalyzer::getInstance().getHistogram(
                                                           samples,
                                                           histogram,
                                                           lowerBoundary,
                                                           upperBoundary,
                                                           steps( p ) / 100.0 );
 
        std::ofstream os( ( outputName + '-' +
                            gkg::StringConverter::toString( p ) +
                            ".csv" ).c_str() );
        for ( s = 0; s < 100; s++ )
        {

          os << lowerBoundary +
                ( upperBoundary - lowerBoundary ) /
                ( double )( 100 - 1 ) * ( double ) s
             << "\t"
             << histogram[ s ]
             << std::endl;

        }
        os.close();

      }

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
