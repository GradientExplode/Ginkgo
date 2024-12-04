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

    // testing gaussian random number
    {

      std::cout << "testing gaussian random number:" << std::endl;
      std::cout << "===============================" << std::endl;
      gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

      for ( int32_t i = 0; i < 10; i++ )
      {

        std::cout << factory->getGaussianRandomNumber( randomGenerator,
                                                       0.0, 1.0 )
                  << std::endl;

      }

    }


    // testing uniform random number
    {

      std::cout << "testing uniform random number:" << std::endl;
      std::cout << "==============================" << std::endl;
      gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

      for ( int32_t i = 0; i < 10; i++ )
      {

        std::cout << factory->getUniformRandomNumber( randomGenerator,
                                                       0.0, 100.0 )
                  << std::endl;

      }

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
