#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>


#define TRIAL_COUNT  40


class MyFunction : public gkg::NelderMeadSimplexFunction
{

  public:

    MyFunction();
    ~MyFunction();

    double getValueAt( const gkg::Vector& parameters ) const;

};


MyFunction::MyFunction()
           : gkg::NelderMeadSimplexFunction( 2 )
{
}


MyFunction::~MyFunction()
{
}


double MyFunction::getValueAt( const gkg::Vector& parameters ) const
{

  return 10.0 * ( parameters( 0 ) - 1.0 ) *
                ( parameters( 0 ) - 1.0 ) +
         20.0 * ( parameters( 1 ) - 2.0 ) *
                ( parameters( 1 ) - 2.0 ) +
         30.0;

}


int main( int /*argc*/, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    gkg::PluginLoader::getInstance().load();

    // in fact, 'gsl' library is the default; so the following line is not
    // mandatory: it is only for didactic purpose
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    // setting initial parameters and delta parameters
    gkg::Vector parameters( 2 );
    parameters( 0 ) = 5.0;
    parameters( 1 ) = 7.0;

    gkg::Vector deltaParameters( 2 );
    deltaParameters( 0 ) = 1.0;
    deltaParameters( 1 ) = 1.0;

    // allocating my function
    MyFunction myFunction;


    // getting parameters that optimize my function
    // should give (1.0,2.0) optimal solution
    double testSize = 0.0; 
    factory->getNelderMeadSimplexOptimumParameters( parameters,
                                                    deltaParameters,
                                                    testSize,
                                                    myFunction,
                                                    100,
                                                    1e-2,
                                                    true );

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
