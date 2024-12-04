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
      factory __attribute__((unused)) = 
        gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    // creating the input non symmetric matrix
    gkg::CVector A( 3 );
    std::complex<double> mycomplex (3.0, 4.0);
    A( 0 ) = mycomplex;
    A( 1 ) = 0.6;
    A( 2 ) = 0.0;

    gkg::Vector B;
    B = A.getRealPart();
    //gkg::CVector B = A ;

    // Printing A before inverse computation
    std::cout << "====================== A ================= " << std::endl;
    std::cout << A << std::endl << std::endl;

    std::cout << "====================== B ================= " << std::endl;
    std::cout << B << std::endl << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
