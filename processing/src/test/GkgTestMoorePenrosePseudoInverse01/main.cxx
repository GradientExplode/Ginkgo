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

    // testing GSL real SVD decomposition
    std::cout << "testing Moore-Penrose pseudo-inverse :" << std::endl;
    std::cout << "====================================" << std::endl;
    gkg::Matrix A( 4, 3 );
    A( 0, 0 ) = +2.5;    A( 0, 1 ) = -4.0;    A( 0, 2 ) = -6.0;
    A( 1, 0 ) = +6.0;    A( 1, 1 ) = -3.0;    A( 1, 2 ) = +3.0;
    A( 2, 0 ) = +2.5;    A( 2, 1 ) = +1.2;    A( 2, 2 ) = +3.0;
    A( 3, 0 ) = -0.5;    A( 3, 1 ) = +2.1;    A( 3, 2 ) = -1.0;

    std::cout << "A=" << std::endl << A << std::endl << std::endl;

    gkg::Vector X( 3 );
    X( 0 ) = 1.0;
    X( 1 ) = 2.0;
    X( 2 ) = 3.0;
    std::cout << "X=" << std::endl << X << std::endl << std::endl;

    gkg::Vector B = A.getComposition( X );
    std::cout << "B=A.X=" << std::endl << B
              << std::endl << std::endl;

    gkg::Matrix Aplus( 3, 4 );

    factory->getMoorePenrosePseudoInverse( A, Aplus );

    std::cout << "Aplus=" << std::endl << Aplus << std::endl << std::endl;

    std::cout << "X=Aplus.B=" << std::endl << Aplus.getComposition( B )
              << std::endl << std::endl;

    std::cout << "A.Aplus=" << std::endl
                << A.getComposition( Aplus )
                << std::endl << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
