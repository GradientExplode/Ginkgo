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

    // testing real matrix
    {

      std::cout << "testing real matrix:" << std::endl;
      std::cout << "====================" << std::endl;
      gkg::Matrix matrix( 10, 10 );
      matrix.setIdentity();

      std::cout << "matrix=" << std::endl << matrix << std::endl;
      std::cout << std::endl;

      gkg::Matrix matrix2( matrix );
      matrix2( 3, 0 ) = 5.0;
      std::cout << "matrix2=" << std::endl << matrix2 << std::endl;
      std::cout << std::endl;

      matrix2.transpose();
      std::cout << "matrix2^t=" << std::endl << matrix2 << std::endl;
      std::cout << std::endl;

      gkg::Matrix matrix3( 4, 3 );
      matrix3( 0, 0 ) = 1; matrix3( 0, 1 ) = 2; matrix3( 0, 2 ) = 3; 
      matrix3( 1, 0 ) = 4; matrix3( 1, 1 ) = 5; matrix3( 1, 2 ) = 6; 
      matrix3( 2, 0 ) = 7; matrix3( 2, 1 ) = 8; matrix3( 2, 2 ) = 9; 
      matrix3( 3, 0 ) = 10; matrix3( 3, 1 ) = 11; matrix3( 3, 2 ) = 12; 

      std::cout << "matrix3=" << std::endl << matrix3 << std::endl;
      std::cout << std::endl;

      std::cout << "matrix3^t=" << std::endl
                << matrix3.getTransposition() << std::endl;
      std::cout << std::endl;

    }

    // testing real vectors
    {

      std::cout << "testing real vector:" << std::endl;
      std::cout << "====================" << std::endl;
      gkg::Vector vector( 8 );
      vector.setBasis( 3 );
      std::cout << "vector=" << std::endl << vector << std::endl;
      std::cout << std::endl;

    }

    // testing complex matrix
    {

      std::cout << "testing complex matrix:" << std::endl;
      std::cout << "=======================" << std::endl;
      gkg::CMatrix cmatrix( 10, 10 );
      cmatrix.setIdentity();

      std::cout << "cmatrix=" << std::endl << cmatrix << std::endl;
      std::cout << std::endl;

    }

    // testing complex vectors
    {

      std::cout << "testing complex vector:" << std::endl;
      std::cout << "=======================" << std::endl;
      gkg::CVector cvector( 8 );
      cvector.setBasis( 3 );
      std::cout << "cvector=" << std::endl << cvector << std::endl;
      std::cout << std::endl;

    }

    // testing GSL real SVD decomposition
    {

      std::cout << "testing GSL real SVD deomposition:" << std::endl;
      std::cout << "==================================" << std::endl;
      gkg::Matrix A( 3, 3 );
      A( 0, 0 ) = +2.5;    A( 0, 1 ) = -4.0;    A( 0, 2 ) = -6.0;
      A( 1, 0 ) = +6.0;    A( 1, 1 ) = -3.0;    A( 1, 2 ) = +3.0;
      A( 2, 0 ) = +2.5;    A( 2, 1 ) = +1.2;    A( 2, 2 ) = +3.0;

      std::cout << "A=" << std::endl << A << std::endl << std::endl;

      gkg::Matrix V( 3, 3 );
      gkg::Vector S( 3 );

      factory->getSingularValueDecomposition( A, V, S );

      std::cout << "U=" << std::endl << A << std::endl << std::endl;
      std::cout << "S=" << std::endl << S << std::endl << std::endl;
      std::cout << "V=" << std::endl << V << std::endl << std::endl;

      std::cout << "U.S.V^t=" << std::endl
                << A.getComposition(
                     S.getDiagonal().getComposition( V.getTransposition() ) )
                << std::endl << std::endl;

    }

    // testing Lapack real SVD decomposition
    {

      std::cout << "testing Lapack real SVD decomposition:" << std::endl;
      std::cout << "======================================" << std::endl;
      gkg::Matrix A( 3, 3 );
      A( 0, 0 ) = +2.5;    A( 0, 1 ) = -4.0;    A( 0, 2 ) = -6.0;
      A( 1, 0 ) = +6.0;    A( 1, 1 ) = -3.0;    A( 1, 2 ) = +3.0;
      A( 2, 0 ) = +2.5;    A( 2, 1 ) = +1.2;    A( 2, 2 ) = +3.0;

      std::cout << "A=" << std::endl << A << std::endl << std::endl;

      gkg::Matrix U( 3, 3 );
      gkg::Vector S( 3 );
      gkg::Matrix V( 3, 3 );

      factory->getSingularValueDecomposition2( A, U, S, V );

      std::cout << "U=" << std::endl << U << std::endl << std::endl;
      std::cout << "S=" << std::endl << S << std::endl << std::endl;
      std::cout << "V=" << std::endl << V << std::endl << std::endl;

      std::cout << "U.S.V^t=" << std::endl
                << U.getComposition(
                     S.getDiagonal().getComposition( V.getTransposition() ) )
                << std::endl << std::endl;

    }

    // testing GSL real SVD solver
    {

      std::cout << "testing GSL real SVD solver:" << std::endl;
      std::cout << "============================" << std::endl;
      gkg::Matrix A( 4, 4 );
      A( 0, 0 ) = 0.18;  A( 0, 1 ) = 0.60;  A( 0, 2 ) = 0.57; A( 0, 3 ) = 0.96;
      A( 1, 0 ) = 0.41;  A( 1, 1 ) = 0.24;  A( 1, 2 ) = 0.99; A( 1, 3 ) = 0.58;
      A( 2, 0 ) = 0.14;  A( 2, 1 ) = 0.30;  A( 2, 2 ) = 0.97; A( 2, 3 ) = 0.66;
      A( 3, 0 ) = 0.51;  A( 3, 1 ) = 0.13;  A( 3, 2 ) = 0.19; A( 3, 3 ) = 0.85;

      std::cout << "A=" << std::endl << A << std::endl << std::endl;

      gkg::Matrix U( 4, 4 );
      gkg::Matrix V( 4, 4 );
      gkg::Vector S( 4 );

      factory->getSingularValueDecomposition2( A, U, S, V );

      std::cout << "U=" << std::endl << U << std::endl << std::endl;
      std::cout << "S=" << std::endl << S << std::endl << std::endl;
      std::cout << "V=" << std::endl << V << std::endl << std::endl;

      std::cout << "U.S.V^t=" << std::endl
                << U.getComposition(
                     S.getDiagonal().getComposition( V.getTransposition() ) )
                << std::endl << std::endl;

      gkg::Vector b( 4 );
      b( 0 ) = 1.0;
      b( 1 ) = 2.0;
      b( 2 ) = 3.0;
      b( 3 ) = 4.0;

      gkg::Vector x( 4 );
      factory->getSingularValueSolution( U, S, V, b, x );

      std::cout << "x=A^-1.b=" << std::endl
                << x 
                << std::endl << std::endl;

      std::cout << "A.x="
                << A.getComposition( x )
                << std::endl << std::endl;

    }

    // testing real SVD Nth order fit
    {

      std::cout << "testing real SVD Nth order fit:" << std::endl;
      std::cout << "===============================" << std::endl;

      gkg::Vector x( 10 );
      gkg::Vector y( 10 );

      x( 0 ) = -5;
      x( 1 ) = -3;
      x( 2 ) = -2;
      x( 3 ) = 0;
      x( 4 ) = -9;
      x( 5 ) = 5;
      x( 6 ) = 3;
      x( 7 ) = 12;
      x( 8 ) = 7;
      x( 9 ) = 2;

      for ( int32_t i = 0; i < 10; i++ )
      {

        y( i ) = 1 + 2 * x( i ) - 3 * x( i ) * x( i );

      }

      std::cout << "x=" << std::endl << x << std::endl << std::endl;
      std::cout << "y=" << std::endl << y << std::endl << std::endl;

      gkg::Vector a( 3 );
      factory->getSingularValueNthOrderFit( x, y, a );

      std::cout << "a=" << std::endl << a << std::endl << std::endl;

    }

    // testing Lapack complex ESVD decomposition
    {

      std::cout << "testing Lapack complex ESVD decomposition:" << std::endl;
      std::cout << "==========================================" << std::endl;
      gkg::CMatrix A( 3, 3 );
      A( 0, 0 ) = +2.5;    A( 0, 1 ) = -4.0;    A( 0, 2 ) = -6.0;
      A( 1, 0 ) = +6.0;    A( 1, 1 ) = -3.0;    A( 1, 2 ) = +3.0;
      A( 2, 0 ) = +2.5;    A( 2, 1 ) = +1.2;    A( 2, 2 ) = +3.0;

      std::cout << "A=" << std::endl << A << std::endl << std::endl;

      gkg::CMatrix U( 3, 3 );
      gkg::Vector S( 3 );
      gkg::CMatrix V( 3, 3 );

      factory->getESingularValueDecomposition( A, U, S, V );

      std::cout << "U=" << std::endl << U << std::endl << std::endl;
      std::cout << "S=" << std::endl << S << std::endl << std::endl;
      std::cout << "V=" << std::endl << V << std::endl << std::endl;

      std::cout << "U.S.V^t=" << std::endl
                << U.getComposition(
                     gkg::CMatrix( S.getDiagonal() ).getComposition( 
                                                        V.getTransposition() ) )
                << std::endl << std::endl;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
