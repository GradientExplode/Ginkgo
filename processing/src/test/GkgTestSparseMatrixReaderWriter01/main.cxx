#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-io/SparseMatrixItemIOFactory.h>
#include <gkg-processing-io/SparseCMatrixItemIOFactory.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/SparseCMatrix.h>
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

    // writing data
    std::ofstream os;
    try
    {

      os.open( "sparse_matrix.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string(
                                        "cannot open \"sparse_matrix.txt\"" ) );

    }

    gkg::SparseMatrix m1( 3, 3 );
    for ( int32_t j = 0; j < 3; j++ )
    {

      for ( int32_t i = 0; i < 2; i++ )
      {

        m1( i, j ) = ( double )( i + j );

      }

    }
    gkg::ItemWriter< gkg::SparseMatrix >& sparseMatrixW =
    gkg::SparseMatrixItemIOFactory::getInstance().getWriter( true, false );
    sparseMatrixW.write( os, m1 );

    os.close();

    // reading data
    std::ifstream is;
    try
    {

      is.open( "sparse_matrix.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string(
                                       "cannot open \"sparse_matrix.txt\"" ) );

    }

    gkg::SparseMatrix m2;
    gkg::ItemReader< gkg::SparseMatrix >& sparseMatrixR =
    gkg::SparseMatrixItemIOFactory::getInstance().getReader( true, false );
    sparseMatrixR.read( is, m2 );

    is.close();

    std::cout << m2 << std::endl;


    //
    // testing SparseCMatrix I/O
    //

    // writing data
    try
    {

      os.open( "sparse_cmatrix.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string(
                                      "cannot open \"sparce_cmatrix.txt\"" ) );

    }

    gkg::SparseCMatrix m3( 3, 3 );
    for ( int32_t j = 0; j < 3; j++ )
    {

      for ( int32_t i = 0; i < 2; i++ )
      {

        m3( i, j ) = std::complex< double >( ( double )( i + j ),
                                             ( double )( i + j ) );

      }

    }
    gkg::ItemWriter< gkg::SparseCMatrix >& sparseCMatrixW =
    gkg::SparseCMatrixItemIOFactory::getInstance().getWriter( true, false );
    sparseCMatrixW.write( os, m3 );

    os.close();

    // reading data
    try
    {

      is.open( "sparse_cmatrix.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string(
                                       "cannot open \"sparse_cmatrix.txt\"" ) );

    }

    gkg::SparseCMatrix m4;
    gkg::ItemReader< gkg::SparseCMatrix >& sparseCMatrixR =
    gkg::SparseCMatrixItemIOFactory::getInstance().getReader( true, false );
    sparseCMatrixR.read( is, m4 );

    is.close();

    std::cout << m4 << std::endl;


  }
  GKG_CATCH_COMMAND( result );

  return result;

}
