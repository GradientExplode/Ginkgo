#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-io/SparseMatrixItemIOFactory.h>
#include <gkg-processing-io/SparseCMatrixItemIOFactory.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/SparseCMatrix.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Writer_i.h>
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


    // reading data
    std::ifstream is;
    try
    {

      is.open(
        "/home/cpoupon/Data/Estournet/training_data_sparse_matrix_noise1.mat" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open sparse matrix." ) );

    }

    gkg::SparseMatrix m;
    gkg::ItemReader< gkg::SparseMatrix >& sparseMatrixR =
    gkg::SparseMatrixItemIOFactory::getInstance().getReader( true, false );
    sparseMatrixR.read( is, m );

    is.close();

    std::cout << "sparse matrix read" << std::endl;

    // splitting into 4 ADC/FA/LP/LT matrices
    gkg::SparseMatrix adc( 63, 343 );
    gkg::SparseMatrix fa( 63, 343 );
    gkg::SparseMatrix lambdaParallel( 63, 343 );
    gkg::SparseMatrix lambdaTransverse( 63, 343 );

    gkg::Volume< float > adcVolume( 343, 63 );
    gkg::Volume< float > faVolume( 343, 63 );
    gkg::Volume< float > lambdaParallelVolume( 343, 63 );
    gkg::Volume< float > lambdaTransverseVolume( 343, 63 );

    gkg::Vector3d< double > resolution( 1.0, 5.36, 1.0 );
    adcVolume.setResolution( resolution );
    faVolume.setResolution( resolution );
    lambdaParallelVolume.setResolution( resolution );
    lambdaTransverseVolume.setResolution( resolution );

    for ( int32_t j = 0; j < 343; j++ )
    {

      for ( int32_t i = 0; i < 63; i++ )
      {

        adc( i, j ) = m( i * 1000, j * 4 + 0 );
        fa( i, j ) = m( i * 1000, j * 4 + 1 );
        lambdaParallel( i, j ) = m( i * 1000, j * 4 + 2 );
        lambdaTransverse( i, j ) = m( i * 1000, j * 4 + 3 );

        adcVolume( j, i ) = ( float )adc( i, j );
        faVolume( j, i ) = ( float )fa( i, j );
        lambdaParallelVolume( j, i ) = ( float )lambdaParallel( i, j );
        lambdaTransverseVolume( j, i ) = ( float )lambdaTransverse( i, j );

      }

    }

    std::cout << "volumes & sparse matrices created" << std::endl;


    // writing volumes
    gkg::Writer::getInstance().write( "adc.ima", adcVolume );
    gkg::Writer::getInstance().write( "fa.ima", faVolume );
    gkg::Writer::getInstance().write( "lambda_parallel.ima",
                                      lambdaParallelVolume );
    gkg::Writer::getInstance().write( "lambda_transverse.ima",
                                      lambdaTransverseVolume );

    std::cout << "volumes written" << std::endl;

    // writing splitted matrices
    std::ofstream os;
    try
    {

      os.open( "adc.mat" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string(
                                        "cannot open \"adc.mat\"" ) );

    }

    gkg::ItemWriter< gkg::SparseMatrix >& sparseMatrixW =
    gkg::SparseMatrixItemIOFactory::getInstance().getWriter( true, false );
    sparseMatrixW.write( os, adc );

    os.close();
    std::cout << "adc sparse matrix written" << std::endl;

    try
    {

      os.open( "fa.mat" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string(
                                        "cannot open \"fa.mat\"" ) );

    }

    sparseMatrixW.write( os, fa );

    os.close();
    std::cout << "fa sparse matrix written" << std::endl;

    try
    {

      os.open( "lambda_parallel.mat" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string(
                                      "cannot open \"lambda_parallel.mat\"" ) );

    }

    sparseMatrixW.write( os, lambdaParallel );

    os.close();
    std::cout << "lambda_parallel sparse matrix written" << std::endl;

    try
    {

      os.open( "lambda_transverse.mat" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string(
                                    "cannot open \"lambda_transverse.mat\"" ) );

    }

    sparseMatrixW.write( os, lambdaTransverse );

    os.close();
    std::cout << "lambda_transverse sparse matrix written" << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
