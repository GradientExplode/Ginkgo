#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-io/VectorItemIOFactory.h>
#include <gkg-processing-io/CVectorItemIOFactory.h>
#include <gkg-processing-io/MatrixItemIOFactory.h>
#include <gkg-processing-io/CMatrixItemIOFactory.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/CVector.h>
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
    // testing Vector I/O
    //

    // writing data
    std::ofstream os;

    try
    {

      os.open( "vector.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"vector.txt\"" ) );

    }

    gkg::Vector v1( 10 );
    for ( int32_t i = 0; i < 10; i++ )
    {

      v1( i ) = ( double )i;

    }
    gkg::ItemWriter< gkg::Vector >& vectorW =
    gkg::VectorItemIOFactory::getInstance().getWriter( true, false );
    vectorW.write( os, v1 );

    os.close();

    // reading data
    std::ifstream is;

    try
    {

      is.open( "vector.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"vector.txt\"" ) );

    }

    gkg::Vector v2;
    gkg::ItemReader< gkg::Vector >& vectorR =
    gkg::VectorItemIOFactory::getInstance().getReader( true, false );
    vectorR.read( is, v2 );

    is.close();

    std::cout << v2 << std::endl;


    //
    // testing CVector I/O
    //

    // writing data
    try
    {

      os.open( "cvector.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"cvector.txt\"" ) );

    }

    gkg::CVector cv1( 10 );
    for ( int32_t i = 0; i < 10; i++ )
    {

      cv1( i ) = std::complex< double >( ( double )i, ( double )-i );

    }
    gkg::ItemWriter< gkg::CVector >& cvectorW =
    gkg::CVectorItemIOFactory::getInstance().getWriter( true, false );
    cvectorW.write( os, cv1 );

    os.close();

    // reading data
    try
    {

      is.open( "cvector.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"cvector.txt\"" ) );

    }

    gkg::CVector cv2;
    gkg::ItemReader< gkg::CVector >& cvectorR =
    gkg::CVectorItemIOFactory::getInstance().getReader( true, false );
    cvectorR.read( is, cv2 );

    is.close();

    std::cout << cv2 << std::endl;

    //
    // testing Matrix I/O
    //

    // writing data
    try
    {

      os.open( "matrix.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"matrix.txt\"" ) );

    }

    gkg::Matrix m1( 5, 5 );
    for ( int32_t j = 0; j < 5; j++ )
    {

      for ( int32_t i = 0; i < 5; i++ )
      {

        m1( i, j ) = ( double )( i + j );

      }

    }
    gkg::ItemWriter< gkg::Matrix >& matrixW =
    gkg::MatrixItemIOFactory::getInstance().getWriter( true, false );
    matrixW.write( os, m1 );

    os.close();

    // reading data
    try
    {

      is.open( "matrix.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"matrix.txt\"" ) );

    }

    gkg::Matrix m2;
    gkg::ItemReader< gkg::Matrix >& matrixR =
    gkg::MatrixItemIOFactory::getInstance().getReader( true, false );
    matrixR.read( is, m2 );

    is.close();

    std::cout << m2 << std::endl;

    //
    // testing CMatrix I/O
    //

    // writing data
    try
    {

      os.open( "cmatrix.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"cmatrix.txt\"" ) );

    }

    gkg::CMatrix cm1( 5, 5 );
    for ( int32_t j = 0; j < 5; j++ )
    {

      for ( int32_t i = 0; i < 5; i++ )
      {

        cm1( i, j ) = std::complex< double >( ( double )i, ( double )j );

      }

    }
    gkg::ItemWriter< gkg::CMatrix >& cmatrixW =
    gkg::CMatrixItemIOFactory::getInstance().getWriter( true, false );
    cmatrixW.write( os, cm1 );

    os.close();

    // reading data
    try
    {

      is.open( "cmatrix.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"cmatrix.txt\"" ) );

    }

    gkg::CMatrix cm2;
    gkg::ItemReader< gkg::CMatrix >& cmatrixR =
    gkg::CMatrixItemIOFactory::getInstance().getReader( true, false );
    cmatrixR.read( is, cm2 );

    is.close();

    std::cout << cm2 << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
