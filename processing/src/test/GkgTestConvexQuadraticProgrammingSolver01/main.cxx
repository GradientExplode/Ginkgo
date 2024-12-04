#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


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


    gkg::Matrix Q( 2, 2 );
    Q( 0, 0 ) = 4.0;
    Q( 0, 1 ) = -2.0;
    Q( 1, 0 ) = -2.0;
    Q( 1, 1 ) = 4.0;

    gkg::Vector q( 2 );
    q( 0 ) = 6.0;
    q( 1 ) = 0.0;

    gkg::Matrix A( 1, 2 );
    A( 0, 0 ) = 1.0;
    A( 0, 1 ) = 1.0;

    gkg::Vector b( 1 );
    b( 0 ) = 3.0;

    gkg::Matrix C( 3, 2 );
    C( 0, 0 ) = 1.0;
    C( 0, 1 ) = 0.0;
    C( 1, 0 ) = 0.0;
    C( 1, 1 ) = 1.0;
    C( 2, 0 ) = 1.0;
    C( 2, 1 ) = 1.0;

    gkg::Vector d( 3 );
    d( 0 ) = 0.0;
    d( 1 ) = 0.0;
    d( 2 ) = 2.0;


    gkg::Vector x;
    factory->getConvexQuadraticProgrammingSolution( Q, q, A, b, C, d,
                                                    x,
                                                    100,
                                                    1e-10,
                                                    1e-10,
                                                    1e-10,
                                                    true );

    std::cout << "Final solution x : " << x << std::endl;
    std::cout << "Expected solution x : 1.0 2.0" << std::endl;
    std::cout << "Expected optimal objective function value : 12.0"
              << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
