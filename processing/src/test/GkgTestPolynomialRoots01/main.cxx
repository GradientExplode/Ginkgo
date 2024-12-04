#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/CVector.h>
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


    gkg::Vector polynomCoefficients( 6 );
    polynomCoefficients( 0 ) = -1.0;
    polynomCoefficients( 1 ) = 0.0;
    polynomCoefficients( 2 ) = 0.0;
    polynomCoefficients( 3 ) = 0.0;
    polynomCoefficients( 4 ) = 0.0;
    polynomCoefficients( 5 ) = +1.0;

    gkg::CVector roots;

    factory->getPolynomialRoots( polynomCoefficients,
                                 roots );

    std::cout << "roots=" << roots << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
