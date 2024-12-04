#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-algobase/RandomIndexVector.h>
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

    std::vector< int32_t > data( 20 );

    // first shot
    gkg::getRandomIndexVector( data );

    std::vector< int32_t >::const_iterator d = data.begin(), de = data.end();
    while ( d != de )
    {

      std::cout << *d << ' ';
      ++ d;

    }
    std::cout << std::endl;

    // second shot
    gkg::getRandomIndexVector( data );

    d = data.begin();
    while ( d != de )
    {

      std::cout << *d << ' ';
      ++ d;

    }
    std::cout << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
