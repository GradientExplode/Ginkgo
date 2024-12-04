#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/PowellNewUOAFunction.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>


class MyFunction : public gkg::PowellNewUOAFunction
{

  public:

    MyFunction();
    ~MyFunction();

    double getValueAt( const double* parameters, int32_t count ) const;

};


MyFunction::MyFunction()
           : gkg::PowellNewUOAFunction( 15 )
{
}


MyFunction::~MyFunction()
{
}


double MyFunction::getValueAt( const double* parameters, int32_t count ) const
{

  try
  {

    // rosenbrock function ( n variables )
    double val = 0.0;

    int32_t l = 0;
    for( l = 0; l < ( count - 1 ) ; ++l )
    {

      val += 100.0 * 
             ( parameters[ l + 1 ] - parameters[ l ] * parameters[ l ] ) *
             ( parameters[ l + 1 ] - parameters[ l ] * parameters[ l ] ) +
             ( parameters[ l ] - 1.0 ) * ( parameters[ l ] - 1.0 ); 

    }

    return val;

  }
  GKG_CATCH( "double MyFunction::getValueAt( "
             "const double* parameters, int32_t count ) const" );

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

    // setting initial parameters to null
    gkg::Vector parameters( 15 );
    parameters.fill( 0.0 );

    // allocating my function
    MyFunction myFunction;

    // getting parameters that optimize my function
    /* double res = */ factory->getPowellNewUOAOptimumParameters( parameters,
                                                                  myFunction,
                                                                  1e2,
                                                                  2000,
                                                                  1e-6,
                                                                  true );

    std::cout << "optimum parameters: " << parameters << std::endl;
 
  }
  GKG_CATCH_COMMAND( result );

  return result;

}
