#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-io/SparseMatrixItemIOFactory.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-core-exception/Exception.h>
#include <complex>
#include <iostream>
#include <fstream>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {


    gkg::PluginLoader::getInstance().load();

    // in fact, 'gsl' library is the default; so the following line is not
    // mandatory: it is only for didactic purpose
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

    //
    // testing SparseMatrix I/O
    //

    gkg::SparseMatrix m1( 730183, 730183 );
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );
    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    int32_t i = 0;
    int32_t j = 0;
    for ( int32_t k = 0; k < 244148; k++ )
    {

      i = ( int32_t )factory->getUniformRandomNumber( randomGenerator,
                                                      0.0, 730182.0 );
      j = ( int32_t )factory->getUniformRandomNumber( randomGenerator,
                                                      0.0, 730182.0 );
      m1( i, j ) = ( double )( i + j );


    }
    std::cout << "sparse matrix created" << std::endl;

    // writing data
    std::ofstream os;
    try
    {

      os.open( "sparse_matrix.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string(
                                        "cannot open \"sparce_matrix.txt\"" ) );

    }

    gkg::ItemWriter< gkg::SparseMatrix >& sparseMatrixW =
    gkg::SparseMatrixItemIOFactory::getInstance().getWriter( true, false );
    sparseMatrixW.write( os, m1 );

    os.close();

    std::cout << "sparse matrix saved" << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
