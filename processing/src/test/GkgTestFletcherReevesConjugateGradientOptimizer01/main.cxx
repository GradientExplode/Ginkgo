#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/FletcherReevesConjugateGradientFunction.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>


#define TRIAL_COUNT  40


class MyFunction : public gkg::FletcherReevesConjugateGradientFunction
{

  public:

    MyFunction();
    ~MyFunction();

    double getValueAt( const gkg::Vector& parameters ) const;
    gkg::Vector getDerivativeAt( const gkg::Vector& parameters ) const;
    double getValueAndDerivativeAt( const gkg::Vector& parameters,
                                    gkg::Vector& derivative ) const;

};


MyFunction::MyFunction()
           : gkg::FletcherReevesConjugateGradientFunction( 2 )
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


gkg::Vector 
MyFunction::getDerivativeAt( const gkg::Vector& parameters ) const
{

  gkg::Vector derivative( 2 );
  derivative( 0 ) = 2.0 * 10.0 * ( parameters( 0 ) - 1.0 );
  derivative( 1 ) = 2.0 * 20.0 * ( parameters( 1 ) - 2.0 );
  return derivative;

}


double
MyFunction::getValueAndDerivativeAt( const gkg::Vector& parameters,
                                     gkg::Vector& derivative ) const
{

  derivative = getDerivativeAt( parameters );
  return getValueAt( parameters );

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


    // allocating my function
    MyFunction myFunction;


    // getting parameters that optimize my function
    // should give (1.0,2.0) optimal solution
    factory->getFletcherReevesConjugateGradientOptimumParameters( parameters,
                                                                  myFunction,
                                                                  100,
                                                                  1e-2,
                                                                  1e-3,
                                                                  1e-4,
                                                                  true );

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
