#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/PowellNewUOAFunction.h>
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

class MyPowellNewUOAFunction : public gkg::PowellNewUOAFunction
{

  public:

    MyPowellNewUOAFunction( double noiseStandardDeviation );
    virtual ~MyPowellNewUOAFunction();

    double getValueAt( const double* parameters,
                       int32_t count ) const;

  protected:

    double _noiseStandardDeviation;
    gkg::NumericalAnalysisImplementationFactory* _factory;
    gkg::RCPointer< gkg::RandomGenerator > _randomGenerator;

    gkg::Vector _realXs;
    gkg::Vector _realMeasurements;



};


MyPowellNewUOAFunction::MyPowellNewUOAFunction( double noiseStandardDeviation )
                       : gkg::PowellNewUOAFunction( 5 ),
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
  GKG_CATCH( "MyPowellNewUOAFunction::MyPowellNewUOAFunction()" );

}


MyPowellNewUOAFunction::~MyPowellNewUOAFunction()
{
}


// m = 0.3 *  std::exp( -0.2 * x^2 ) + 0.7 * std::exp( -0.2 * ( x - 10 )^2 )

double MyPowellNewUOAFunction::getValueAt( const double* parameters,
                                           int32_t /* count */ ) const
{

  try
  {

    double meanSquareDifference = 0.0;

    gkg::Vector values( MEASUREMENT_COUNT );

    static double constrainedParameters[ 5 ];
    this->applyConstraints( parameters, 5, constrainedParameters );

    int32_t m = 0;
    for ( m = 0; m < MEASUREMENT_COUNT; m++ )
    {

      values( m ) = constrainedParameters[ 0 ] *
                    std::exp( constrainedParameters[ 1 ] *
                              _realXs( m ) * _realXs( m ) ) +
                    constrainedParameters[ 2 ] *
                     std::exp( constrainedParameters[ 3 ] *
                               ( _realXs( m ) + constrainedParameters[ 4 ] ) *
                               ( _realXs( m ) + constrainedParameters[ 4 ] ) );

      meanSquareDifference += ( ( values( m ) - _realMeasurements( m ) ) *
                                ( values( m ) - _realMeasurements( m ) ) );

    }

    return meanSquareDifference;

  }
  GKG_CATCH( "double MyPowellNewUOAFunction::getValueAt( "
             "const double* parameters, int32_t count ) const" );

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
    bool verbose = false;

    gkg::Application application( argc, argv,
                                  "Powell New UOA solver testing" );
    application.addSeriesOption(
              "-optimizerParameters",
              "optimizerParameters are set according to the optimizer type.\n"
              "   - optimizerParameters[ 0 ] -> initial trust region radius\n"
              "   - optimizerParameters[ 1 ] -> maximum iteration count\n"
              "   - optimizerParameters[ 2 ] -> maximum test size",
              optimizerParameters,
              3, 3 );
    application.addSingleOption( "-noiseStandardDeviation",
                                 "Noise standard deviation (default=0.01)",
                                 noiseStandardDeviation, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
 
    application.initialize();


    std::cout << "je passe par la1" << std::endl;

    MyPowellNewUOAFunction myPowellNewUOAFunction( noiseStandardDeviation );

    std::vector< gkg::RCPointer< gkg::OptimizerConstraint > > constraints( 5 );
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
    std::cout << "je passe par la2" << std::endl;

    myPowellNewUOAFunction.addConstraints( constraints );

    std::cout << "je passe par la3" << std::endl;

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

    factory->getPowellNewUOAOptimumParameters(
            initialParameters,
            myPowellNewUOAFunction,
            optimizerParameters[ 0 ],
            ( int32_t )( optimizerParameters[ 1 ] + 0.5 ),
            optimizerParameters[ 2 ],
            verbose );

    gkg::Vector expectedParameters( 5 );
    expectedParameters( 0 ) = 0.3;
    expectedParameters( 1 ) = -0.2;
    expectedParameters( 2 ) = 0.7;
    expectedParameters( 3 ) = -0.2;
    expectedParameters( 4 ) = -10.0;


    gkg::Vector constrainedParameters( 5 );
    myPowellNewUOAFunction.getConstrainedParameters( initialParameters,
                                                     constrainedParameters );

    std::cout << "optimum  parameters: " << constrainedParameters << std::endl;
    std::cout << "expected parameters: " << expectedParameters << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
