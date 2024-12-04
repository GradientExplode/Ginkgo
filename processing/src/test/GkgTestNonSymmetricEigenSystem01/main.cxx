#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
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

    // creating the input non symmetric matrix
    gkg::Matrix A( 4, 4 );
    A( 0, 0 ) = -1.0;
    A( 0, 1 ) = 1.0;
    A( 0, 2 ) = -1.0;
    A( 0, 3 ) = 1.0;

    A( 1, 0 ) = -8.0;
    A( 1, 1 ) = 4.0;
    A( 1, 2 ) = -2.0;
    A( 1, 3 ) = 1.0;

    A( 2, 0 ) = 27.0;
    A( 2, 1 ) = 9.0;
    A( 2, 2 ) = 3.0;
    A( 2, 3 ) = 1.0;

    A( 3, 0 ) = 64.0;
    A( 3, 1 ) = 16.0;
    A( 3, 2 ) = 4.0;
    A( 3, 3 ) = 1.0;
    
    // allocating complex vector and matrix for the eigensystem computation
    gkg::CVector D;
    gkg::CMatrix V;
    
    // computing the eigensystem
    factory->getNonSymmetricEigenSystem( A, D, V );
    
    // ordering the eigenvalues/eigenvectors in asbolute descending order
    factory->sortNonSymmetricEigenSystem(
          D, V,
          gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending );
          
    // displaying results:
    std::cout << "====================== A ================= " << std::endl;
    std::cout << A << std::endl << std::endl;
    std::cout << "====================== D ================= " << std::endl;
    std::cout << D << std::endl << std::endl;
    std::cout << "====================== V ================= " << std::endl;
    std::cout << V << std::endl << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
