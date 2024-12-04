#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
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


    uint32_t n = 0U;
    for ( n = 0U; n < 2U; n++ )
    {

      for ( uint32_t r = 0U; r < 50U; r++ )
      {

        std::cout << factory->getZeroBesselDerivative( n, r ) << " ";

      }

      std::cout << std::endl;

    }


    std::cout << factory->getBesselDerivative( 0, 2.0 * M_PI, 0.001 )
              << std::endl;
    std::cout << factory->getBesselDerivative( 1, 2.0 * M_PI, 0.001 )
              << std::endl;
    std::cout << factory->getBesselDerivative( 2, 2.0 * M_PI, 0.001 )
              << std::endl;




  }
  GKG_CATCH_COMMAND( result );

  return result;


}
