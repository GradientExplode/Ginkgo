#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
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

    std::cout << "2 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   2.0,
                   true,
                   1000,
                   0.0001 )
               << std::endl;
    std::cout << "5 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   5.0,
                   true,
                   1000,
                   0.001 )
               << std::endl;
    std::cout << "10 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   10.0,
                   true,
                   1000,
                   0.1 )
               << std::endl;
    std::cout << "20 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   20.0,
                   true,
                   1000,
                   0.1 )
               << std::endl;
    std::cout << "30 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   30.0,
                   true,
                   1000,
                   0.1 )
               << std::endl;
    std::cout << "40 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   40.0,
                   true,
                   1000,
                   0.1 )
               << std::endl;
    std::cout << "50 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   50.0,
                   true,
                   1000,
                   1 )
               << std::endl;
    std::cout << "60 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   60.0,
                   true,
                   10000,
                   5 )
               << std::endl;
    std::cout << "70 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   70.0,
                   true,
                   100000,
                   10 )
               << std::endl;
    std::cout << "80 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   80.0,
                   true,
                   1000,
                   1 )
               << std::endl;
    std::cout << "89 degrees -> "
              << factory->getWatsonKappaParameterFromAngularDispersion(
                   89.0,
                   true,
                   1000,
                   1 )
               << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
