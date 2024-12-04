#include <gkg-processing-plugin-gsl-lapack/GSLConvexQuadraticProgrammingSolver2.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-core-exception/Exception.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <iostream>
#include <cmath>



inline void displayVector( const std::string& text, const gsl_vector* v )
{

  std::cout << text;
  for ( int32_t i = 0; i < ( int32_t )v->size; i++ )
  {

    std::cout << *gsl_vector_const_ptr( v, i ) << " ";

  }
  std::cout << std::endl;

}


inline void displayMatrix( const std::string& text, const gsl_matrix* m )
{

  std::cout << text << std::endl;
  for ( int32_t i = 0; i < ( int32_t )m->size1; i++ )
  {


    for ( int32_t j = 0; j < ( int32_t )m->size2; j++ )
    {

      std::cout << *gsl_matrix_const_ptr( m, i, j ) << " ";

    }
    std::cout << std::endl;

  }
  std::cout << std::endl;

}


static double gsl_vector_max_norm( const gsl_vector* v )
{

  try
  {

    double maximumNorm = std::fabs( gsl_vector_get( v, 0 ) );

    size_t i = 0;
    for ( i = 1; i < v->size; i++ )
    {

      maximumNorm = GSL_MAX_DBL( maximumNorm,
                                 std::fabs( gsl_vector_get( v, i ) ) );

    }
    return maximumNorm;

  }
  GKG_CATCH( "static double gsl_vector_max_norm( const gsl_vector* v )" );


}


static double gsl_matrix_max_norm( const gsl_matrix* M )
{

  try
  {

    double maximumNorm = 0.0;

    size_t i = 0;
    size_t j = 0;
    for ( i = 0; i < M->size1; i++ )
    {

      for ( j = 0; j < M->size2; j++ )
      {

        maximumNorm = GSL_MAX_DBL( maximumNorm,
                                   std::fabs( gsl_matrix_get( M, i, j ) ) );

      }

    }
    return maximumNorm;

  }
  GKG_CATCH( "static double gsl_matrix_max_norm( const gsl_matrix* M )" );

}



gkg::GSLConvexQuadraticProgrammingSolver2::GSLConvexQuadraticProgrammingSolver2(
                                         const gkg::Matrix& Q,
                                         const gkg::Vector& q,
                                         const gkg::Matrix& A,
                                         const gkg::Vector& b,
                                         const gkg::Matrix& C,
                                         const gkg::Vector& d,
                                         gkg::Vector& x )
{

  try
  {



    _Q = static_cast< gkg::GSLMatrixImplementation* >(
                                 Q.getImplementation() )->getGSLMatrixPointer();
    _q = static_cast< gkg::GSLVectorImplementation* >(
                                 q.getImplementation() )->getGSLVectorPointer();
    _A = static_cast< gkg::GSLMatrixImplementation* >(
                                 A.getImplementation() )->getGSLMatrixPointer();
    _b = static_cast< gkg::GSLVectorImplementation* >(
                                 b.getImplementation() )->getGSLVectorPointer();
    _C = static_cast< gkg::GSLMatrixImplementation* >(
                                 C.getImplementation() )->getGSLMatrixPointer();
    _d = static_cast< gkg::GSLVectorImplementation* >(
                                 d.getImplementation() )->getGSLVectorPointer();
    _x = static_cast< gkg::GSLVectorImplementation* >(
                                 x.getImplementation() )->getGSLVectorPointer();


    if ( x.getSize() != Q.getSize1() )
    {

      x.reallocate( Q.getSize1() );

    }

  }
  GKG_CATCH( "gkg::GSLConvexQuadraticProgrammingSolver2::"
             "GSLConvexQuadraticProgrammingSolver2( "
             "const gkg::Matrix& Q, "
             "const gkg::Vector& q, "
             "const gkg::Matrix& A, "
             "const gkg::Vector& b, "
             "const gkg::Matrix& C, "
             "const gkg::Vector& d, "
             "gkg::Vector& x )" );

}


gkg::GSLConvexQuadraticProgrammingSolver2::GSLConvexQuadraticProgrammingSolver2(
                                const gkg::Matrix& Q,
                                const gkg::Vector& q,
                                const gkg::Matrix& C,
                                const gkg::Vector& d,
                                gkg::Vector& x,
                                double solutionMinimumNormRegularisation )
{

  try
  {

    _Q = static_cast< gkg::GSLMatrixImplementation* >(
                                 Q.getImplementation() )->getGSLMatrixPointer();
    _q = static_cast< gkg::GSLVectorImplementation* >(
                                 q.getImplementation() )->getGSLVectorPointer();
    _A = 0;
    _b = 0;
    _C = static_cast< gkg::GSLMatrixImplementation* >(
                                 C.getImplementation() )->getGSLMatrixPointer();
    _d = static_cast< gkg::GSLVectorImplementation* >(
                                 d.getImplementation() )->getGSLVectorPointer();
    _x = static_cast< gkg::GSLVectorImplementation* >(
                                 x.getImplementation() )->getGSLVectorPointer();

    displayMatrix( "=========Q===========", _Q );
    displayVector( "=========q===========\n", _q );
    displayMatrix( "=========C===========", _C );
    displayVector( "=========d===========\n", _d );

    if ( x.getSize() != Q.getSize1() )
    {

      x.reallocate( Q.getSize1() );

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing the maximum coefficient on the diagonal of Q
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector diagonal = Q.getDiagonal();
    gkg::MaximumFilter< gkg::Vector, double > maximumFilter;
    double maximumCoefficient = 0.0;
    maximumFilter.filter( diagonal, maximumCoefficient );


    ////////////////////////////////////////////////////////////////////////////
    // adding a minimum norm constraint
    ////////////////////////////////////////////////////////////////////////////

    int32_t size = diagonal.getSize();
    int32_t s = 0;
    for ( s = 0; s < size; s++ )
    {

      Q( s, s ) += solutionMinimumNormRegularisation * maximumCoefficient );

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the Cholesky decomposition (corresponding to the lower
    // triangular part of L, the upper one remaining Q
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix L;
    this->getCholeskyDecomposition( Q, L );


    ////////////////////////////////////////////////////////////////////////////
    // computing the transpose of matrix projecting b onto preconditioned 
    // quadratic problem H^T H \ H
    ////////////////////////////////////////////////////////////////////////////



    this->prepare();

  }
  GKG_CATCH( "gkg::GSLConvexQuadraticProgrammingSolver2::"
             "GSLConvexQuadraticProgrammingSolver2( "
             "const gkg::Matrix& Q, "
             "const gkg::Vector& q, "
             "const gkg::Matrix& C, "
             "const gkg::Vector& d, "
             "gkg::Vector& x )" );

}


gkg::GSLConvexQuadraticProgrammingSolver2::
                                         ~GSLConvexQuadraticProgrammingSolver2()
{
}


bool gkg::GSLConvexQuadraticProgrammingSolver2::solve(
                                                  int32_t maximumIterationCount,
                                                  double epsilonGap,
                                                  double epsilonResiduals,
                                                  double epsilonInfeasible,
                                                  bool verbose )
{

  try
  {

    gkg::Vector


  }
  GKG_CATCH( "bool gkg::GSLConvexQuadraticProgrammingSolver2::solve( "
             "int32_t maximumIterationCount, "
             "double epsilonGap, "
             "double epsilonResiduals, "
             "double epsilonInfeasible, "
             "bool verbose )" );


}


bool gkg::GSLConvexQuadraticProgrammingSolver2::prepare()
{

  try
  {

    gsl_linalg_cholesky_decomp1( _Q );


  }
  GKG_CATCH( "bool gkg::GSLConvexQuadraticProgrammingSolver2::prepare()" );

}
