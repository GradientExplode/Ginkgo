#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
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

    // testing eigen system real decomposition
    {

      std::cout << "testing eigen system real decomposition:" << std::endl;
      std::cout << "=======================================" << std::endl;
      gkg::Matrix A( 4, 4 );

      A( 0, 0 ) = 1.0;
      A( 0, 1 ) = 1.0 / 2.0;
      A( 0, 2 ) = 1.0 / 3.0;
      A( 0, 3 ) = 1.0 / 4.0;
      A( 1, 0 ) = 1.0 / 2.0;
      A( 1, 1 ) = 1.0 / 3.0;
      A( 1, 2 ) = 1.0 / 4.0;
      A( 1, 3 ) = 1.0 / 5.0;
      A( 2, 0 ) = 1.0 / 3.0;
      A( 2, 1 ) = 1.0 / 4.0;
      A( 2, 2 ) = 1.0 / 5.0;
      A( 2, 3 ) = 1.0 / 6.0;
      A( 3, 0 ) = 1.0 / 4.0;
      A( 3, 1 ) = 1.0 / 5.0;
      A( 3, 2 ) = 1.0 / 6.0;
      A( 3, 3 ) = 1.0 / 7.0;

      std::cout << "A=" << std::endl << A << std::endl;
      std::cout << std::endl;

      gkg::Vector D( 4 );
      gkg::Matrix V( 4, 4 );

      factory->getEigenSystem( A, D, V );
      factory->sortEigenSystem( D, V,
              gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending );

      std::cout << "D=" << std::endl << D << std::endl << std::endl;
      std::cout << "V=" << std::endl << V << std::endl << std::endl;

    }

    // testing SVD real decomposition
    {

      std::cout << "testing SVD real decomposition:" << std::endl;
      std::cout << "==============================" << std::endl;
      gkg::Matrix A( 4, 4 );

      A( 0, 0 ) = 1.0;
      A( 0, 1 ) = 1.0 / 2.0;
      A( 0, 2 ) = 1.0 / 3.0;
      A( 0, 3 ) = 1.0 / 4.0;
      A( 1, 0 ) = 1.0 / 2.0;
      A( 1, 1 ) = 1.0 / 3.0;
      A( 1, 2 ) = 1.0 / 4.0;
      A( 1, 3 ) = 1.0 / 5.0;
      A( 2, 0 ) = 1.0 / 3.0;
      A( 2, 1 ) = 1.0 / 4.0;
      A( 2, 2 ) = 1.0 / 5.0;
      A( 2, 3 ) = 1.0 / 6.0;
      A( 3, 0 ) = 1.0 / 4.0;
      A( 3, 1 ) = 1.0 / 5.0;
      A( 3, 2 ) = 1.0 / 6.0;
      A( 3, 3 ) = 1.0 / 7.0;

      std::cout << "A=" << std::endl << A << std::endl;
      std::cout << std::endl;

      gkg::Vector D( 4 );
      gkg::Matrix V( 4, 4 );

      factory->getSingularValueDecomposition( A, V, D );

      std::cout << "A=" << std::endl << A << std::endl << std::endl;
      std::cout << "D=" << std::endl << D << std::endl << std::endl;
      std::cout << "V=" << std::endl << V << std::endl << std::endl;

    }

    // testing eigen system complex decomposition
    {

      std::cout << "testing eigen system complex decomposition:" << std::endl;
      std::cout << "==========================================" << std::endl;
      gkg::CMatrix A( 4, 4 );

      A( 0, 0 ) = 1.0;
      A( 0, 1 ) = 1.0 / 2.0;
      A( 0, 2 ) = 1.0 / 3.0;
      A( 0, 3 ) = 1.0 / 4.0;
      A( 1, 0 ) = 1.0 / 2.0;
      A( 1, 1 ) = 1.0 / 3.0;
      A( 1, 2 ) = 1.0 / 4.0;
      A( 1, 3 ) = 1.0 / 5.0;
      A( 2, 0 ) = 1.0 / 3.0;
      A( 2, 1 ) = 1.0 / 4.0;
      A( 2, 2 ) = 1.0 / 5.0;
      A( 2, 3 ) = 1.0 / 6.0;
      A( 3, 0 ) = 1.0 / 4.0;
      A( 3, 1 ) = 1.0 / 5.0;
      A( 3, 2 ) = 1.0 / 6.0;
      A( 3, 3 ) = 1.0 / 7.0;

      std::cout << "A=" << std::endl << A << std::endl;
      std::cout << std::endl;

      gkg::Vector D( 4 );
      gkg::CMatrix V( 4, 4 );

      factory->getEigenSystem( A, D, V );
      factory->sortEigenSystem( D, V,
              gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending );

      std::cout << "D=" << std::endl << D << std::endl << std::endl;
      std::cout << "V=" << std::endl << V << std::endl << std::endl;

    }


  }
  GKG_CATCH_COMMAND( result );

  return result;


}
