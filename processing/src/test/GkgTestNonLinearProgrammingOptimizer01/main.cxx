#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>



#define MEASUREMENT_COUNT 500


////////////////////////////////////////////////////////////////////////////////
// class MyMCMCFunction
////////////////////////////////////////////////////////////////////////////////

class MyNLPFunction : public gkg::NonLinearProgrammingFunction
{

  public:

    MyNLPFunction( double noiseStandardDeviation );
    virtual ~MyNLPFunction();

    double getValueAt( const gkg::Vector& parameters ) const;
    
    //void getDerivativeAt( const Vector& parameters,
    //                      Vector& derivative ) const;
    //double getValueAndDerivativeAt( const Vector& parameters,
    //                                Vector& derivative ) const;

  protected:

    double _noiseStandardDeviation;
    gkg::NumericalAnalysisImplementationFactory* _factory;
    gkg::RCPointer< gkg::RandomGenerator > _randomGenerator;

    gkg::Vector _realXs;
    gkg::Vector _realMeasurements;

};


MyNLPFunction::MyNLPFunction( double noiseStandardDeviation )
              : gkg::NonLinearProgrammingFunction( 5, true ),
                _noiseStandardDeviation( noiseStandardDeviation )
{

  try
  {

    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    _randomGenerator.reset(
                       new gkg::RandomGenerator( gkg::RandomGenerator::Taus ) );

    _realXs.reallocate( MEASUREMENT_COUNT );
    _realMeasurements.reallocate( MEASUREMENT_COUNT );

    std::ofstream os( "function.txt" );

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
                                                      _noiseStandardDeviation );
      os << _realXs( m ) << " " << _realMeasurements( m ) << std::endl;

    }

    os.close();

  }
  GKG_CATCH( "MyNLPFunction::MyNLPFunction()" );

}


MyNLPFunction::~MyNLPFunction()
{
}


// m = 0.3 *  std::exp( -0.2 * x^2 ) + 0.7 * std::exp( -0.2 * ( x - 10 )^2 )

double MyNLPFunction::getValueAt( const gkg::Vector& parameters ) const
{

  try
  {

    double meanSquareDifference = 0.0;

    gkg::Vector values( MEASUREMENT_COUNT );

    int32_t m = 0;
    for ( m = 0; m < MEASUREMENT_COUNT; m++ )
    {

      values( m ) = parameters( 0 ) *  std::exp( parameters( 1 ) *
                                       _realXs( m ) * _realXs( m ) ) +
                    parameters( 2 ) * std::exp( parameters( 3 ) *
                                           ( _realXs( m ) +  parameters( 4 ) ) *
                                           ( _realXs( m ) + parameters( 4 ) ) );

      meanSquareDifference += ( ( values( m ) - _realMeasurements( m ) ) *
                                ( values( m ) - _realMeasurements( m ) ) );

    }

    return meanSquareDifference;

  }
  GKG_CATCH( "double MyNLPFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// main()
////////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    std::vector< double > optimizerParameters;
    double noiseStandardDeviation = 0.01;
    bool unconstrained = false;
    bool verbose = false;

    gkg::Application application( argc, argv,
                                  "NLP solver testing\n"
                                   "Use 1000 0.001 0.2 0.2 0.2 0.2 5.0 "
                                   "in this case" );
    application.addSeriesOption(
              "-optimizerParameters",
              "optimizerParameters are set according to the optimizer type.\n"
              " for NelderMeadSimplex\n"
              "     NelderMeadSimplex2\n"
              "     NelderMeadSimplex2Rand:\n"
              "   - optimizerParameters[ 0 ] -> maximum iteration count\n"
              "   - optimizerParameters[ 1 ] -> maximum simplex size\n"
              "   - optimizerParameters[ 2 .. <parameterCount+2> ] -> "
              "variations of parameters\n"
              "\n"
              " for FletcherReevesConjugateGradient\n"
              "     PolakRibiereConjugateGradient\n"
              "     BroydenFletcherGoldfarbShanno\n"
              "     SteepestDescent\n"
              "     BroydenFletcherGoldfarbShanno2:\n"
              "   - optimizerParameters[ 0 ] -> maximum iteration count\n"
              "   - optimizerParameters[ 2 ] -> step size of first trial step\n"
              "   - optimizerParameters[ 2 ] -> line linimization tolerance\n"
              "   - optimizerParameters[ 3 ] -> precision\n",
              optimizerParameters,
              2 );
    application.addSingleOption( "-noiseStandardDeviation",
                                 "Noise standard deviation (default=0.01)",
                                 noiseStandardDeviation, 
                                 true );
    application.addSingleOption( "-unconstrained",
                                 "Unconstrained NLP (default=false)",
                                 unconstrained, 
                                 true );
     application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
 
    application.initialize();


    std::vector< gkg::RCPointer< gkg::OptimizerConstraint > > constraints( 5 );

    if ( unconstrained )
    {

      constraints[ 0 ].reset( new gkg::NoneOptimizerConstraint );
      constraints[ 1 ].reset( new gkg::NoneOptimizerConstraint );
      constraints[ 2 ].reset( new gkg::NoneOptimizerConstraint );
      constraints[ 3 ].reset( new gkg::NoneOptimizerConstraint );
      constraints[ 4 ].reset( new gkg::NoneOptimizerConstraint );

    }
    else
    {
    
      constraints[ 0 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                                   0.0, 1.0 ) );
      constraints[ 1 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                                  -1.0, 0.0 ) );
      constraints[ 2 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                                   0.0, 1.0 ) );
      constraints[ 3 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                                  -1.0, 0.0 ) );
      constraints[ 4 ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                                 -20.0, 0.0 ) );

    }

    MyNLPFunction myNLPFunction( noiseStandardDeviation );

    // m = 0.3 *  std::exp( -0.2 * x^2 ) + 0.7 * std::exp( -0.2 * ( x - 10 )^2 )
    gkg::Vector initialParameters( 5 );
    initialParameters( 0 ) = 0.0;
    initialParameters( 1 ) = 0.0;
    initialParameters( 2 ) = 0.0;
    initialParameters( 3 ) = 0.0;
    initialParameters( 4 ) = 0.0;

    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    factory->getConstrainedNonLinearProgrammingOptimumParameters(
            gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
            initialParameters,
            constraints,
            myNLPFunction,
            optimizerParameters,
            verbose );

    gkg::Vector expectedParameters( 5 );
    expectedParameters( 0 ) = 0.3;
    expectedParameters( 1 ) = -0.2;
    expectedParameters( 2 ) = 0.7;
    expectedParameters( 3 ) = -0.2;
    expectedParameters( 4 ) = -10.0;

    std::cout << "optimum  parameters: " << initialParameters << std::endl;
    std::cout << "expected parameters: " << expectedParameters << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
