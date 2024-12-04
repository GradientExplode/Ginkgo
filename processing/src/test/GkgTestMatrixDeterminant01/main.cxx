#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
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

    gkg::Matrix m( 3, 3 );

    m( 0, 0 ) = 2;
    m( 0, 1 ) = 1;
    m( 0, 2 ) = 0;
    m( 1, 0 ) = 0;
    m( 1, 1 ) = 2;
    m( 1, 2 ) = 0;
    m( 2, 0 ) = -3;
    m( 2, 1 ) = 0;
    m( 2, 2 ) = 2;

    std::cout << "Matrix:" << std::endl;
    std::cout << m << std::endl;

    double det = m.getDeterminant();

    std::cout << "Determinant : " << det << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
