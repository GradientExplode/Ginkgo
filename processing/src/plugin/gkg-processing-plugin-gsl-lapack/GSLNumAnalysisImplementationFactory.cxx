#include <gkg-processing-plugin-gsl-lapack/GSLNumAnalysisImplementationFactory.h>
#include <gkg-processing-plugin-gsl-lapack/GSLMatrixImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLVectorImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLCMatrixImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLCVectorImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLFunctionInterpolatorImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLRandomGeneratorImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLLMFunctionImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLNLPFunctionImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLNMSFunctionImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLFRCGFunctionImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLSAConfigurationImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLConvexQuadraticProgrammingSolver.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>
#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-processing-numericalanalysis/FletcherReevesConjugateGradientFunction.h>
#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-processing-numericalanalysis/SimulatedAnnealingConfiguration.h>
#include <gkg-processing-numericalanalysis/IntegrableFunction.h>
#include <gkg-processing-algobase/VarianceFilter_i.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/SumOfSquareFilter_i.h>
#include <gkg-processing-plugin-gsl-lapack/Lapack.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <gsl/gsl_sf.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_sf_legendre.h>
#include <gsl/gsl_sf_erf.h>
#include <gsl/gsl_sf_dawson.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_deriv.h>
#include <gsl/gsl_siman.h>
#include <gsl/gsl_poly.h>
#include <gsl/gsl_version.h>
#include <gsl/gsl_statistics.h>
#include <nlopt.h>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <sys/time.h>
#include <iomanip>

#include <fstream>
#include <cstdlib>


//
// GSLNumericalAnalysisImplementationFactory methods
//

ForceSingletonInstanciation( gkg::GSLNumericalAnalysisImplementationFactory,
                             gsl );

gkg::GSLNumericalAnalysisImplementationFactory::
                                     GSLNumericalAnalysisImplementationFactory()
                                 : gkg::NumericalAnalysisImplementationFactory()
{

  try
  {

    gkg::NumericalAnalysisSelector::getInstance().registerFactory(
                 "gsl",
                 &gkg::GSLNumericalAnalysisImplementationFactory::getAccessor );

  }
  GKG_CATCH( "gkg::GSLNumericalAnalysisImplementationFactory::"
             "GSLNumericalAnalysisImplementationFactory()" );

}


gkg::GSLNumericalAnalysisImplementationFactory::
                                    ~GSLNumericalAnalysisImplementationFactory()
{
}


//////////////////// real linear algebra implementation(s) /////////////////////


gkg::MatrixImplementation* 
gkg::GSLNumericalAnalysisImplementationFactory::createMatrixImplementation(
                                                           gkg::Matrix* matrix,
                                                           int32_t size1,
                                                           int32_t size2 ) const
{

  try
  {

    return new gkg::GSLMatrixImplementation( matrix,
                                             size1,
                                             size2 );

  }
  GKG_CATCH( "gkg::MatrixImplementation* "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "createMatrixImplementation( "
             "gkg::Matrix* matrix, "
             "int size1, "
             "int size2 ) const" );

}


gkg::VectorImplementation* 
gkg::GSLNumericalAnalysisImplementationFactory::createVectorImplementation(
                                                            gkg::Vector* vector,
                                                            int32_t size ) const
{

  try
  {

    return new gkg::GSLVectorImplementation( vector,
                                             size );

  }
  GKG_CATCH( "gkg::VectorImplementation* "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "createVectorImplementation( "
             "gkg::Vector* vector, "
             "int size ) const" );

}


void 
gkg::GSLNumericalAnalysisImplementationFactory::getSingularValueDecomposition(
                                                          gkg::Matrix& A,
                                                          gkg::Matrix& V,
                                                          gkg::Vector& S ) const
{

  try
  {

    gsl_matrix* a = static_cast< gkg::GSLMatrixImplementation* >(
                                 A.getImplementation() )->getGSLMatrixPointer();
    gsl_matrix* v = static_cast< gkg::GSLMatrixImplementation* >(
                                 V.getImplementation() )->getGSLMatrixPointer();
    gsl_vector* s = static_cast< gkg::GSLVectorImplementation* >(
                                 S.getImplementation() )->getGSLVectorPointer();
    gsl_vector* work = gsl_vector_alloc( A.getSize2() );

    int32_t gsl_errno = gsl_linalg_SV_decomp( a, v, s, work );

    gsl_vector_free( work );

    if ( gsl_errno )
    {

      throw std::runtime_error( gsl_strerror( gsl_errno ) );

    }

  }
  GKG_CATCH( "int gkg::GSLNumericalAnalysisImplementationFactory::"
             "getSingularValueDecomposition( "
             "gkg::Matrix& A, "
             "gkg::Matrix& V, "
             "gkg::Vector& S ) const" );

}


void 
gkg::GSLNumericalAnalysisImplementationFactory::getSingularValueDecomposition2(
                                                          const gkg::Matrix& A,
                                                          gkg::Matrix& U,
                                                          gkg::Vector& S,
                                                          gkg::Matrix& V ) const
{

  try
  {

    gkg::Matrix B( A );
    B.transpose();

    gsl_matrix* b = static_cast< gkg::GSLMatrixImplementation* >(
                                 B.getImplementation() )->getGSLMatrixPointer();
    gsl_matrix* u = static_cast< gkg::GSLMatrixImplementation* >(
                                 U.getImplementation() )->getGSLMatrixPointer();
    gsl_vector* s = static_cast< gkg::GSLVectorImplementation* >(
                                 S.getImplementation() )->getGSLVectorPointer();
    gsl_matrix* v = static_cast< gkg::GSLMatrixImplementation* >(
                                 V.getImplementation() )->getGSLMatrixPointer();

    char jobU = 'A';
    char jobVt = 'A';
    int32_t m = A.getSize1();
    int32_t n = A.getSize2();

    int32_t lda = m;
    int32_t ldu = m;

    int32_t ldvt = n;

    int32_t lwork = std::max( 3 * std::min( m, n ) + std::max( m, n ),
                              5 * std::min( m, n ) );

    int32_t info;

    gsl_vector* work = gsl_vector_alloc( lwork );

    dgesvd_( &jobU, &jobVt,
             &m, &n,
             b->data, &lda,
             s->data,
             u->data, &ldu,
             v->data, &ldvt,
             work->data, &lwork,
             &info );

    gsl_vector_free( work );

    U.transpose();

  }
  GKG_CATCH( "int gkg::GSLNumericalAnalysisImplementationFactory::"
             "getSingularValueDecomposition( "
             "gkg::Matrix& A, "
             "gkg::Matrix& U, "
             "gkg::Vector& S, "
             "gkg::Matrix& V ) const" );

}


void 
gkg::GSLNumericalAnalysisImplementationFactory::getSingularValueSolution(
                                                          const gkg::Matrix& U,
                                                          const gkg::Vector& S,
                                                          const gkg::Matrix& V,
                                                          const gkg::Vector& b,
                                                          gkg::Vector& x ) const
{

  try
  {

    gsl_matrix* u = static_cast< gkg::GSLMatrixImplementation* >(
                                 U.getImplementation() )->getGSLMatrixPointer();
    gsl_vector* s = static_cast< gkg::GSLVectorImplementation* >(
                                 S.getImplementation() )->getGSLVectorPointer();
    gsl_matrix* v = static_cast< gkg::GSLMatrixImplementation* >(
                                 V.getImplementation() )->getGSLMatrixPointer();
    gsl_vector* gsl_b = static_cast< gkg::GSLVectorImplementation* >(
                                 b.getImplementation() )->getGSLVectorPointer();
    gsl_vector* gsl_x = static_cast< gkg::GSLVectorImplementation* >(
                                 x.getImplementation() )->getGSLVectorPointer();


    int32_t gsl_errno = gsl_linalg_SV_solve( u, v, s, gsl_b, gsl_x );

    if ( gsl_errno )
    {

      throw std::runtime_error( gsl_strerror( gsl_errno ) );

    }

  }
  GKG_CATCH( "int gkg::GSLNumericalAnalysisImplementationFactory::"
             "getSingularValueSolution( "
             "const gkg::Matrix& U, "
             "const gkg::Vector& S, "
             "const gkg::Matrix& V, "
             "const gkg::Vector& b, "
             "gkg::Vector& x ) const" );

}


void gkg::GSLNumericalAnalysisImplementationFactory::getEigenSystem(
                                                          const gkg::Matrix& A,
                                                          gkg::Vector& D,
                                                          gkg::Matrix& V ) const
{

  try
  {

    gsl_matrix* a = static_cast< gkg::GSLMatrixImplementation* >(
                                 A.getImplementation() )->getGSLMatrixPointer();
    gsl_vector* d = static_cast< gkg::GSLVectorImplementation* >(
                                 D.getImplementation() )->getGSLVectorPointer();
    gsl_matrix* v = static_cast< gkg::GSLMatrixImplementation* >(
                                 V.getImplementation() )->getGSLMatrixPointer();

    gsl_eigen_symmv_workspace* w = gsl_eigen_symmv_alloc( D.getSize() );

    gsl_eigen_symmv( a, d, v, w );

    gsl_eigen_symmv_free( w );

  }
  GKG_CATCH( "void gkg::GSLNumericalAnalysisImplementationFactory::"
             "getEigenSystem( "
             "const gkg::Matrix& A, "
             "gkg::Vector& D, "
             "gkg::Matrix& V ) const" );

}


void gkg::GSLNumericalAnalysisImplementationFactory::sortEigenSystem(
                gkg::Vector& D,
                gkg::Matrix& V,
                gkg::NumericalAnalysisImplementationFactory::EigenSystemSortType
                  eigenSystemSortType ) const
{

  try
  {

    // getting access to the GSL vector structure
    gsl_vector* d = static_cast< gkg::GSLVectorImplementation* >(
                                 D.getImplementation() )->getGSLVectorPointer();

    // reallocating the matrix for the eigenvectors
    gsl_matrix* v = static_cast< gkg::GSLMatrixImplementation* >(
                                 V.getImplementation() )->getGSLMatrixPointer();

    gsl_eigen_sort_t type = GSL_EIGEN_SORT_VAL_ASC;
    switch ( eigenSystemSortType )
    {

      case gkg::NumericalAnalysisImplementationFactory::Ascending:

        type = GSL_EIGEN_SORT_VAL_ASC;
        break;

      case gkg::NumericalAnalysisImplementationFactory::Descending:

        type = GSL_EIGEN_SORT_VAL_DESC;
        break;

      case gkg::NumericalAnalysisImplementationFactory::AbsoluteAscending:

        type = GSL_EIGEN_SORT_ABS_ASC;
        break;

      case gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending:

        type = GSL_EIGEN_SORT_ABS_DESC;
        break;

    }

    gsl_eigen_symmv_sort( d, v, type );


  }
  GKG_CATCH( "void gkg::GSLNumericalAnalysisImplementationFactory::"
             "sortEigenSystem( "
             "gkg::Vector& D, "
             "gkg::Matrix& V, "
             "gkg::NumericalAnalysisImplementationFactory::EigenSystemSortType "
             "  eigenSystemSortType ) const" );

}


void 
gkg::GSLNumericalAnalysisImplementationFactory::getNonSymmetricEigenSystem(
                                     const gkg::Matrix& A,
                                     gkg::CVector& D,
                                     gkg::CMatrix& V ) const
{

  try
  {
  
    // collecting matrix size(s)
    int32_t size = A.getSize1();
    
    // if not a square matrix, launch an exeception
    if ( A.getSize2() != size )
    {
    
      throw std::runtime_error( "not a square matrix" );
      
    }

    // making a copy of matrix A
    gkg::Matrix aCopy( A );
    
    // getting access to the GSL matrix structure
    gsl_matrix* a = static_cast< gkg::GSLMatrixImplementation* >(
                                 aCopy.getImplementation() )->
                                                          getGSLMatrixPointer();

    // reallocating the complex vector for the eigenvalues
    D.reallocate( size );
    
    // getting access to the GSL complex vector structure
    gsl_vector_complex* d = static_cast< gkg::GSLCVectorImplementation* >(
                                 D.getImplementation() )->getGSLCVectorPointer();
                                 
    // reallocating the complex matrix for the eigenvectors
    V.reallocate( size, size );
    
    // getting access to the GSL complex matrix structure
    gsl_matrix_complex* v = static_cast< gkg::GSLCMatrixImplementation* >(
                                 V.getImplementation() )->getGSLCMatrixPointer();

    // allocating a workspace for computation
    gsl_eigen_nonsymmv_workspace* w = gsl_eigen_nonsymmv_alloc( size );

    // computing the non-symmetric eigensystem
    gsl_eigen_nonsymmv( a, d, v, w );
    
    // free the workspace
    gsl_eigen_nonsymmv_free( w );
  
  }
  GKG_CATCH( "void "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getNonSymmetricEigenSystem( "
             "const gkg::Matrix& A, "
             "gkg::CVector& D, "
             "gkg::CMatrix& V ) const" );

}


void 
gkg::GSLNumericalAnalysisImplementationFactory::sortNonSymmetricEigenSystem(
          gkg::CVector& D,
          gkg::CMatrix& V,
          gkg::GSLNumericalAnalysisImplementationFactory::EigenSystemSortType 
            eigenSystemSortType ) const
{

  try
  {

    // getting access to the GSL complex vector structure
    gsl_vector_complex* d = static_cast< gkg::GSLCVectorImplementation* >(
                                 D.getImplementation() )->getGSLCVectorPointer();
                                 
    // reallocating the complex matrix for the eigenvectors
    gsl_matrix_complex* v = static_cast< gkg::GSLCMatrixImplementation* >(
                                 V.getImplementation() )->getGSLCMatrixPointer();

    gsl_eigen_sort_t type = GSL_EIGEN_SORT_VAL_ASC;
    switch ( eigenSystemSortType )
    {

      case gkg::NumericalAnalysisImplementationFactory::Ascending:

        type = GSL_EIGEN_SORT_VAL_ASC;
        break;

      case gkg::NumericalAnalysisImplementationFactory::Descending:

        type = GSL_EIGEN_SORT_VAL_DESC;
        break;

      case gkg::NumericalAnalysisImplementationFactory::AbsoluteAscending:

        type = GSL_EIGEN_SORT_ABS_ASC;
        break;

      case gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending:

        type = GSL_EIGEN_SORT_ABS_DESC;
        break;

    }

    gsl_eigen_nonsymmv_sort( d, v, type );

  }
  GKG_CATCH( "void "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "sortNonSymmetricEigenSystem( "
             "gkg::CVector& D, "
             "gkg::CMatrix& V, "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "EigenSystemSortType eigenSystemSortType ) const" );

}


void gkg::GSLNumericalAnalysisImplementationFactory::getEigenValues(
                const gkg::Matrix& A,
                gkg::Vector& D,
                gkg::NumericalAnalysisImplementationFactory::EigenSystemSortType
                  eigenSystemSortType ) const
{

  try
  {


    if ( ( eigenSystemSortType !=
            gkg::NumericalAnalysisImplementationFactory::Ascending ) &&
         ( eigenSystemSortType !=
            gkg::NumericalAnalysisImplementationFactory::Descending ) )
    {

      throw std::runtime_error( "eigensystem sort type must be only "
                                "Ascending or Descending" );

    }
         

    gsl_matrix* a = static_cast< gkg::GSLMatrixImplementation* >(
                                 A.getImplementation() )->getGSLMatrixPointer();
    gsl_vector* d = static_cast< gkg::GSLVectorImplementation* >(
                                 D.getImplementation() )->getGSLVectorPointer();



    // reducing the real symmetric matrix A to a real symmetric tridiagonal 
    // form T by an orthogonal similarity transformation:  Q**T * A * Q = T

    char uplo = 'U';
    int32_t n = A.getSize1();
    int32_t lda = n;

    double* e = new double[ n - 1 ];
    double* tau = new double[ n - 1 ];

    double* work = new double[ 1 ];
    int32_t lwork = -1;
    int32_t info = 0;

    /* int32_t result = */ dsytrd_( &uplo,
                                    &n,
                                    a->data,
                                    &lda,
                                    d->data,
                                    e,
                                    tau,
                                    work,
                                    &lwork,
                                    &info );

    lwork = ( int32_t )( work[ 0 ] + 0.5 );
    delete [] work;
    work = new double[ lwork ];

    /* result = */ dsytrd_( &uplo,
                            &n,
                            a->data,
                            &lda,
                            d->data,
                            e,
                            tau,
                            work,
                            &lwork,
                            &info );

    if ( info != 0 )
    {

      throw std::runtime_error( std::string( "argument " ) +
                                gkg::StringConverter::toString( -info ) +
                                " has an illegal value" );

    }


    delete [] work;
    delete [] tau;

    // computing the eigenvalues of the symmetric tridiagonal matrix
    // using the Pal-Walker-Kahan variant of the QL or QR algorithm.

    /* result = */ dsterf_( &n,
                            d->data,
                            e,
                            &info );

    if ( info != 0 )
    {

      if ( info < 0 )
      {

        throw std::runtime_error( std::string( "argument " ) +
                                  gkg::StringConverter::toString( -info ) +
                                  " has an illegal value" );

      }
      else
      {

        throw std::runtime_error( "failed to find all the eigenvalues" );

      }

    }


    if ( eigenSystemSortType ==
         gkg::NumericalAnalysisImplementationFactory::Descending )
    {

      int32_t e = 0;
      int32_t halfN = n / 2;
      for ( e = 0; e < halfN; e++ )
      {

        std::swap( ( d->data )[ e ], ( d->data )[ n - 1 - e ] );

      }

    }

  }
  GKG_CATCH( "void gkg::GSLNumericalAnalysisImplementationFactory::"
             "getEigenValues( "
             "const gkg::Matrix& A, "
             "gkg::Vector& D, "
             "gkg::NumericalAnalysisImplementationFactory::EigenSystemSortType "
             "eigenSystemSortType ) cons" );

}



void 
gkg::GSLNumericalAnalysisImplementationFactory::getHouseHolderSolution(
                                                          const gkg::Matrix& A,
                                                          const gkg::Vector& B,
                                                          gkg::Vector& X ) const
{

  try
  {

    gsl_matrix* a = static_cast< gkg::GSLMatrixImplementation* >(
                                 A.getImplementation() )->getGSLMatrixPointer();
    gsl_vector* b = static_cast< gkg::GSLVectorImplementation* >(
                                 B.getImplementation() )->getGSLVectorPointer();
    gsl_vector* x = static_cast< gkg::GSLVectorImplementation* >(
                                 X.getImplementation() )->getGSLVectorPointer();

    int32_t gsl_errno = gsl_linalg_HH_solve( a, b, x );
    if ( gsl_errno )
    {

      throw std::runtime_error( gsl_strerror( gsl_errno ) );

    }

  }
  GKG_CATCH( "void "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getHouseHolderSolution( "
             "const gkg::Matrix& A, "
             "const gkg::Vector& B, "
             "gkg::Vector& X ) const" );

}


void 
gkg::GSLNumericalAnalysisImplementationFactory::getLUSolution(
                                                          const gkg::Matrix& A,
                                                          const gkg::Vector& B,
                                                          gkg::Vector& X ) const
{

  try
  {

    gsl_matrix* a = static_cast< gkg::GSLMatrixImplementation* >(
                                 A.getImplementation() )->getGSLMatrixPointer();
    gsl_vector* b = static_cast< gkg::GSLVectorImplementation* >(
                                 B.getImplementation() )->getGSLVectorPointer();
    gsl_vector* x = static_cast< gkg::GSLVectorImplementation* >(
                                 X.getImplementation() )->getGSLVectorPointer();

    int32_t n = A.getSize1();
    int32_t lda = n;
    int32_t ldb = n;
    int32_t nrhs = 1;
    int32_t info;

    int32_t* ipiv = new int32_t[ n ];


    if ( !ipiv )
    {

      throw std::runtime_error( "Not enough memory for LU solver" );

    }

    std::memcpy( x->data, b->data, n * sizeof( double ) );

    dgesv_( &n, &nrhs, a->data, &lda, ipiv, x->data, &ldb, &info );

    delete [] ipiv;

  }
  GKG_CATCH( "void "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getLUSolution( "
             "const gkg::Matrix& A, "
             "const gkg::Vector& B, "
             "gkg::Vector& X ) const" );

}


void 
gkg::GSLNumericalAnalysisImplementationFactory::getCholeskyDecomposition(
                                                          const gkg::Matrix& A,
                                                          gkg::Matrix& L ) const
{

  try
  {

    int32_t n = A.getSize1();

    if ( A.getSize2() != n )
    {

      throw std::runtime_error( "input matrix is not a square one" );

    }

    L = A;

    gsl_matrix* l = static_cast< gkg::GSLMatrixImplementation* >(
                                 L.getImplementation() )->getGSLMatrixPointer();

    if ( gsl_linalg_cholesky_decomp1( l ) == GSL_EDOM )
    {

      throw std::runtime_error( "input matrix is not positive definite" );

    }

  }
  GKG_CATCH( "void "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getCholeskyDecomposition( "
             "const gkg::Matrix& A, "
             "gkg::Vector& L ) const" );

}


void gkg::GSLNumericalAnalysisImplementationFactory::getInverse(
                                                      const gkg::Matrix& A,
                                                      gkg::Matrix& Aplus ) const
{

  try
  {

    int32_t size = A.getSize1();
    if ( A.getSize2() != size )
    {

      throw std::runtime_error( "the matrix is not a square matrix" );

    }

    // reallocating Aplus with the good size
    Aplus.reallocate( size, size );

    // making a copy of matrix A
    gkg::Matrix aCopy( A );

    // getting access to the GSL matrix structure(s)
    gsl_matrix* a = static_cast< gkg::GSLMatrixImplementation* >(
                                 aCopy.getImplementation() )->
                                                          getGSLMatrixPointer();
      
    gsl_matrix* aPlus = static_cast< gkg::GSLMatrixImplementation* >(
                                 Aplus.getImplementation() )->
                                                          getGSLMatrixPointer();

    // allocating a permutation structure
    gsl_permutation* p = gsl_permutation_alloc( size );

    // computing the LU decomposition of A
    int32_t s = 0;
    gsl_linalg_LU_decomp( a, p, &s );

    // computing the inverse of the LU decomposition
    gsl_linalg_LU_invert( a, p, aPlus );

    // free permutation
    gsl_permutation_free( p );


  }
  GKG_CATCH( "void gkg::GSLNumericalAnalysisImplementationFactory::getInverse( "
             "const gkg::Matrix& A, "
             "gkg::Matrix& Aplus ) const" );

}


/////////////////// complex linear algebra implementation(s) ///////////////////

gkg::CMatrixImplementation* 
gkg::GSLNumericalAnalysisImplementationFactory::createCMatrixImplementation(
                                                          gkg::CMatrix* cmatrix,
                                                          int32_t size1,
                                                          int32_t size2 ) const
{

  try
  {

    return new gkg::GSLCMatrixImplementation( cmatrix,
                                              size1,
                                              size2 );

  }
  GKG_CATCH( "gkg::CMatrixImplementation* "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "createCMatrixImplementation( "
             "gkg::CMatrix* cmatrix, "
             "int size1, "
             "int size2 ) const" );

}


gkg::CVectorImplementation* 
gkg::GSLNumericalAnalysisImplementationFactory::createCVectorImplementation(
                                                          gkg::CVector* cvector,
                                                          int32_t size ) const
{

  try
  {

    return new gkg::GSLCVectorImplementation( cvector,
                                              size );

  }
  GKG_CATCH( "gkg::CVectorImplementation* "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "createCVectorImplementation( "
             "gkg::CVector* cvector, "
             "int size ) const" );

}


void 
gkg::GSLNumericalAnalysisImplementationFactory::getESingularValueDecomposition(
                                                         const gkg::CMatrix& A,
                                                         gkg::CMatrix& U,
                                                         gkg::Vector& S,
                                                         gkg::CMatrix& V ) const
{

  try
  {

    gkg::CMatrix B( A );
    B.transpose();

    gsl_matrix_complex* b = static_cast< gkg::GSLCMatrixImplementation* >(
                                B.getImplementation() )->getGSLCMatrixPointer();
    gsl_matrix_complex* u = static_cast< gkg::GSLCMatrixImplementation* >(
                                U.getImplementation() )->getGSLCMatrixPointer();
    gsl_vector* s = static_cast< gkg::GSLVectorImplementation* >(
                                S.getImplementation() )->getGSLVectorPointer();
    gsl_matrix_complex* v = static_cast< gkg::GSLCMatrixImplementation* >(
                                V.getImplementation() )->getGSLCMatrixPointer();

    char jobU = 'A';
    char jobVt = 'A';
    int32_t m = A.getSize1();
    int32_t n = A.getSize2();

    int32_t lda = m;
    int32_t ldu = m;

    int32_t ldvt = n;

    int32_t lwork = 2 * std::min( m, n ) + std::max( m, n );

    int32_t info;

    gsl_vector_complex* cwork = gsl_vector_complex_alloc( lwork );
    gsl_vector* rwork = gsl_vector_alloc( std::max( 1, 5 * std::min( m, n ) ) );

    zgesvd_( &jobU, &jobVt,
             &m, &n,
             b->data, &lda,
             s->data,
             u->data, &ldu,
             v->data, &ldvt,
             cwork->data, &lwork,
             rwork->data,
             &info );

    gsl_vector_complex_free( cwork );
    gsl_vector_free( rwork );

    U.transpose();

    // need for V conjugate operation because when dealing with complex
    // matrices, transpose is hermitian transpose
    V.conjugate();

  }
  GKG_CATCH( "int gkg::GSLNumericalAnalysisImplementationFactory::"
             "getESingularValueDecomposition( "
             "gkg::CMatrix& A, "
             "gkg::Vector& S, "
             "gkg::CMatrix& V ) const" );

}


/*
int32_t 
gkg::GSLNumericalAnalysisImplementationFactory::
                                       getGeneralizedSingularValueDecomposition(
                                                         const gkg::CMatrix& A,
                                                         gkg::CMatrix& U,
                                                         gkg::CVector& S,
                                                         gkg::CMatrix& V ) const
{

  try
  {

    gsl_matrix_complex* a = static_cast< gkg::GSLCMatrixImplementation* >(
                                A.getImplementation() )->getGSLCMatrixPointer();
    gsl_matrix_complex* u = static_cast< gkg::GSLCMatrixImplementation* >(
                                U.getImplementation() )->getGSLCMatrixPointer();
    gsl_vector_complex* s = static_cast< gkg::GSLCVectorImplementation* >(
                                S.getImplementation() )->getGSLCVectorPointer();
    gsl_matrix_complex* v = static_cast< gkg::GSLCMatrixImplementation* >(
                                V.getImplementation() )->getGSLCMatrixPointer();

    char jobU = 'A';
    char jobVt = 'A';
    int32_t m = A.getSize1();
    int32_t n = A.getSize2();

    int32_t lda = m;
    int32_t ldu = m;

    int32_t ldvt = n;

    int32_t lwork = 2 * std::min( m, n ) + std::max( m, n );

    int32_t info;

    gsl_matrix_complex* b = gsl_matrix_complex_alloc( m, n );
    gsl_matrix_complex_memcpy( b, a );
    gsl_matrix_complex_transpose( b );
    gsl_vector_complex* cwork = gsl_vector_complex_alloc( lwork );
    gsl_vector* rwork = gsl_vector_alloc( std::max( 1, 5 * std::min( m, n ) ) );

    zgesvd_( &jobU, &jobVt,
             &m, &n,
             b->data, &lda,
             s->data,
             u->data, &ldu,
             v->data, &ldvt,
             cwork->data, &lwork,
             rwork->data,
             &info );

    gsl_matrix_complex_transpose( u );
    gsl_vector_complex_free( cwork );
    gsl_vector_free( rwork );
    gsl_matrix_complex_free( b );

    return 1;

  }
  GKG_CATCH( "int gkg::GSLNumericalAnalysisImplementationFactory::"
             "getESingularValueDecomposition( "
             "gkg::CMatrix& A, "
             "gkg::CVector& S, "
             "gkg::CMatrix& V ) const" );

}
*/


void gkg::GSLNumericalAnalysisImplementationFactory::getEigenSystem(
                                                         const gkg::CMatrix& A,
                                                         gkg::Vector& D,
                                                         gkg::CMatrix& V ) const
{

  try
  {

    gsl_matrix_complex* a = static_cast< gkg::GSLCMatrixImplementation* >(
                                A.getImplementation() )->getGSLCMatrixPointer();
    gsl_vector* d = static_cast< gkg::GSLVectorImplementation* >(
                                 D.getImplementation() )->getGSLVectorPointer();
    gsl_matrix_complex* v = static_cast< gkg::GSLCMatrixImplementation* >(
                                V.getImplementation() )->getGSLCMatrixPointer();

    gsl_eigen_hermv_workspace* w = gsl_eigen_hermv_alloc( D.getSize() );

    gsl_eigen_hermv( a, d, v, w );

    gsl_eigen_hermv_free( w );

  }
  GKG_CATCH( "void gkg::GSLNumericalAnalysisImplementationFactory::"
             "getEigenSystem( "
             "const gkg::Matrix& A, "
             "gkg::Vector& D, "
             "gkg::Matrix& V ) const" );

}


void gkg::GSLNumericalAnalysisImplementationFactory::sortEigenSystem(
                gkg::Vector& D,
                gkg::CMatrix& V,
                gkg::NumericalAnalysisImplementationFactory::EigenSystemSortType
                  eigenSystemSortType ) const
{

  try
  {

    gsl_vector* d = static_cast< gkg::GSLVectorImplementation* >(
                                 D.getImplementation() )->getGSLVectorPointer();
    gsl_matrix_complex* v = static_cast< gkg::GSLCMatrixImplementation* >(
                                V.getImplementation() )->getGSLCMatrixPointer();

    gsl_eigen_sort_t type = GSL_EIGEN_SORT_VAL_ASC;
    switch ( eigenSystemSortType )
    {

      case gkg::NumericalAnalysisImplementationFactory::Ascending:

        type = GSL_EIGEN_SORT_VAL_ASC;
        break;

      case gkg::NumericalAnalysisImplementationFactory::Descending:

        type = GSL_EIGEN_SORT_VAL_DESC;
        break;

      case gkg::NumericalAnalysisImplementationFactory::AbsoluteAscending:

        type = GSL_EIGEN_SORT_ABS_ASC;
        break;

      case gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending:

        type = GSL_EIGEN_SORT_ABS_DESC;
        break;

    }

    gsl_eigen_hermv_sort( d, v, type );


  }
  GKG_CATCH( "void gkg::GSLNumericalAnalysisImplementationFactory::"
             "sortEigenSystem( "
             "gkg::Vector& D, "
             "gkg::CMatrix& V, "
             "gkg::NumericalAnalysisImplementationFactory::EigenSystemSortType "
             "  eigenSystemSortType ) const" );

}


/////////////////////////// random number generation ///////////////////////////


gkg::RandomGeneratorImplementation* 
gkg::GSLNumericalAnalysisImplementationFactory::
                                            createRandomGeneratorImplementation(
                                         gkg::RandomGenerator* randomGenerator,
                                         gkg::RandomGenerator::Type type ) const
{

  try
  {

    return new gkg::GSLRandomGeneratorImplementation( randomGenerator,
                                                      type );

  }
  GKG_CATCH( "gkg::RandomGeneratorImplementation* "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "createRandomGeneratorImplementation( "
             "gkg::RandomGenerator* randomGenerator, "
             "gkg::RandomGenerator::Type type ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGaussianPdf(
                                                            double x,
                                                            double mean,
                                                            double sigma ) const
{

  try
  {


    return ::gsl_ran_gaussian_pdf( x - mean, sigma );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGaussianPdf("
             "double x, "
             "double mean, "
             "double sigma ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGaussianCdfP(
                                                            double x,
                                                            double mean,
                                                            double sigma ) const
{

  try
  {


    return ::gsl_cdf_gaussian_P( x - mean, sigma );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGaussianCdfP("
             "double x, "
             "double mean, "
             "double sigma ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGaussianCdfQ(
                                                            double x,
                                                            double mean,
                                                            double sigma ) const
{

  try
  {


    return ::gsl_cdf_gaussian_Q( x - mean, sigma );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGaussianCdfQ("
             "double x, "
             "double mean, "
             "double sigma ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGaussianCdfInverseP(
                                                            double P,
                                                            double mean,
                                                            double sigma ) const
{

  try
  {


    return ::gsl_cdf_gaussian_Pinv( P, sigma ) + mean;

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGaussianCdfInverseP("
             "double P, "
             "double mean, "
             "double sigma ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGaussianCdfInverseQ(
                                                            double Q,
                                                            double mean,
                                                            double sigma ) const
{

  try
  {


    return ::gsl_cdf_gaussian_Qinv( Q, sigma ) + mean;

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGaussianCdfInverseQ("
             "double Q, "
             "double mean, "
             "double sigma ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGaussianRandomNumber(
                                    const gkg::RandomGenerator& randomGenerator,
                                    double mean,
                                    double sigma ) const
{

  try
  {

    gsl_rng* r = static_cast< gkg::GSLRandomGeneratorImplementation* >(
          randomGenerator.getImplementation() )->getGSLRandomGeneratorPointer();

    return ( mean + ::gsl_ran_gaussian( r, sigma ) );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGaussianRandomNumber("
             "const gkg::RandomGenerator& randomGenerator, "
             "double mean, "
             "double sigma ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getUniformPdf(
                                                                double x,
                                                                double a,
                                                                double b ) const
{

  try
  {


    return ::gsl_ran_flat_pdf( x, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getUniformPdf("
             "double x, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getUniformCdfP(
                                                                double x,
                                                                double a,
                                                                double b ) const
{

  try
  {


    return ::gsl_cdf_flat_P( x, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getUniformCdfP("
             "double x, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getUniformCdfQ(
                                                                double x,
                                                                double a,
                                                                double b ) const
{

  try
  {


    return ::gsl_cdf_flat_Q( x, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getUniformCdfQ("
             "double x, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getUniformCdfInverseP(
                                                                double P,
                                                                double a,
                                                                double b ) const
{

  try
  {


    return ::gsl_cdf_flat_Pinv( P, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getUniformCdfInverseP("
             "double P, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getUniformCdfInverseQ(
                                                                double Q,
                                                                double a,
                                                                double b ) const
{

  try
  {


    return ::gsl_cdf_flat_Qinv( Q, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getUniformCdfInverseQ("
             "double Q, "
             "double a, "
             "double b ) const" );

}

uint32_t gkg::GSLNumericalAnalysisImplementationFactory
           ::getUniformRandomUInt32(
                                    const gkg::RandomGenerator& randomGenerator,
                                    uint32_t maximum ) const
{

  try
  {

    gsl_rng* r = static_cast< gkg::GSLRandomGeneratorImplementation* >(
          randomGenerator.getImplementation() )->getGSLRandomGeneratorPointer();

    return ::gsl_rng_uniform_int( r, maximum );

  }
  GKG_CATCH( "uint32_t" 
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getUniformRandomUInt32("
             "const gkg::RandomGenerator& randomGenerator,"
             "uint32_t maximum ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getUniformRandomNumber(
                                    const gkg::RandomGenerator& randomGenerator,
                                    double a,
                                    double b ) const
{

  try
  {

    gsl_rng* r = static_cast< gkg::GSLRandomGeneratorImplementation* >(
          randomGenerator.getImplementation() )->getGSLRandomGeneratorPointer();

    return ::gsl_ran_flat( r, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getUniformRandomNumber("
             "const gkg::RandomGenerator& randomGenerator, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGammaPdf(
                                                                double x,
                                                                double a,
                                                                double b ) const
{

  try
  {


    return ::gsl_ran_gamma_pdf( x, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGammaPdf("
             "double x, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGammaCdfP(
                                                                double x,
                                                                double a,
                                                                double b ) const
{

  try
  {


    return ::gsl_cdf_gamma_P( x, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGammaCdfP("
             "double x, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGammaCdfQ(
                                                                double x,
                                                                double a,
                                                                double b ) const
{

  try
  {


    return ::gsl_cdf_gamma_Q( x, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGammaCdfQ("
             "double x, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGammaCdfInverseP(
                                                                double P,
                                                                double a,
                                                                double b ) const
{

  try
  {


    return ::gsl_cdf_gamma_Pinv( P, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGammaCdfInverseP("
             "double P, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGammaCdfInverseQ(
                                                                double Q,
                                                                double a,
                                                                double b ) const
{

  try
  {


    return ::gsl_cdf_gamma_Qinv( Q, a, b );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGammaCdfInverseP("
             "double Q, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getGammaRandomNumber(
                                    const gkg::RandomGenerator& randomGenerator,
                                    double a,
                                    double b ) const
{

  try
  {

    gsl_rng* r = static_cast< gkg::GSLRandomGeneratorImplementation* >(
          randomGenerator.getImplementation() )->getGSLRandomGeneratorPointer();

    return ::gsl_ran_gamma( r, a, b );


  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGammaRandomNumber( "
             "const gkg::RandomGenerator& randomGenerator, "
             "double a, "
             "double b ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getTDistPdf(
                                                               double x,
                                                               double nu ) const
{

  try
  {


    return ::gsl_ran_tdist_pdf( x, nu );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getTDistPdf("
             "double x, "
             "double nu ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getTDistCdfP(
                                                               double x,
                                                               double nu ) const
{

  try
  {


    return ::gsl_cdf_tdist_P( x, nu );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getTDistCdfP("
             "double x, "
             "double nu ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getTDistCdfQ(
                                                               double x,
                                                               double nu ) const
{

  try
  {


    return ::gsl_cdf_tdist_Q( x, nu );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getTDistCdfQ("
             "double x, "
             "double nu ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getTDistCdfInverseP(
                                                               double P,
                                                               double nu ) const
{

  try
  {


    return ::gsl_cdf_tdist_Pinv( P, nu );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getTDistCdfInverseP("
             "double P, "
             "double nu ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getTDistCdfInverseQ(
                                                               double Q,
                                                               double nu ) const
{

  try
  {


    return ::gsl_cdf_tdist_Qinv( Q, nu );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getTDistCdfInverseQ("
             "double Q, "
             "double nu ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getTDistRandomNumber(
                                    const gkg::RandomGenerator& randomGenerator,
                                    double nu ) const
{

  try
  {

    gsl_rng* r = static_cast< gkg::GSLRandomGeneratorImplementation* >(
          randomGenerator.getImplementation() )->getGSLRandomGeneratorPointer();

    return ::gsl_ran_tdist( r, nu );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getTDistRandomNumber("
             "const gkg::RandomGenerator& randomGenerator, "
             "double nu ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getFDistPdf(
                                                              double x,
                                                              double nu1,
                                                              double nu2 ) const
{

  try
  {


    return ::gsl_ran_fdist_pdf( x, nu1, nu2 );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getFDistPdf("
             "double x, "
             "double nu1, "
             "double nu2 ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getFDistCdfP(
                                                              double x,
                                                              double nu1,
                                                              double nu2 ) const
{

  try
  {


    return ::gsl_cdf_fdist_P( x, nu1, nu2 );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getFDistCdfP("
             "double x, "
             "double nu1, "
             "double nu2 ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getFDistCdfQ(
                                                              double x,
                                                              double nu1,
                                                              double nu2 ) const
{

  try
  {


    return ::gsl_cdf_fdist_Q( x, nu1, nu2 );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getFDistCdfQ("
             "double x, "
             "double nu1, "
             "double nu2 ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getFDistRandomNumber(
                                    const gkg::RandomGenerator& randomGenerator,
                                    double nu1,
                                    double nu2 ) const
{

  try
  {

    gsl_rng* r = static_cast< gkg::GSLRandomGeneratorImplementation* >(
          randomGenerator.getImplementation() )->getGSLRandomGeneratorPointer();

    return ::gsl_ran_fdist( r, nu1, nu2 );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getFDistRandomNumber("
             "const gkg::RandomGenerator& randomGenerator, "
             "double nu1, "
             "double nu2 ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getChi2CdfP(
                                                               double x,
                                                               double nu ) const
{

  try
  {


    return ::gsl_cdf_chisq_P( x, nu );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getChi2CdfP("
             "double x, "
             "double nu ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getChi2CdfQ(
                                                               double x,
                                                               double nu ) const
{

  try
  {


    return ::gsl_cdf_chisq_Q( x, nu );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getChi2CdfQ("
             "double x, "
             "double nu ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getChi2CdfInverseP(
                                                               double P,
                                                               double nu ) const
{

  try
  {


    return ::gsl_cdf_chisq_Pinv( P, nu );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getChi2CdfInverseP("
             "double P, "
             "double nu ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getChi2CdfInverseQ(
                                                               double Q,
                                                               double nu ) const
{

  try
  {


    return ::gsl_cdf_chisq_Qinv( Q, nu );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getChi2CdfInverseQ("
             "double Q, "
             "double nu ) const" );

}


/////////////////////////////// multilinear fit ////////////////////////////////

void gkg::GSLNumericalAnalysisImplementationFactory::getMultiLinearFit(
                                                        const gkg::Matrix& X,
                                                        const gkg::Vector& y,
                                                        gkg::Vector& c,
                                                        gkg::Matrix& covariance,
                                                        double& chisq ) const
{

  try
  {

    gsl_matrix* gsl_X = static_cast< gkg::GSLMatrixImplementation* >(
                                 X.getImplementation() )->getGSLMatrixPointer();
    gsl_vector* gsl_y = static_cast< gkg::GSLVectorImplementation* >(
                                 y.getImplementation() )->getGSLVectorPointer();
    gsl_vector* gsl_c = static_cast< gkg::GSLVectorImplementation* >(
                                 c.getImplementation() )->getGSLVectorPointer();
    gsl_matrix* gsl_covariance = static_cast< gkg::GSLMatrixImplementation* >(
                        covariance.getImplementation() )->getGSLMatrixPointer();

    gsl_multifit_linear_workspace*
      work = gsl_multifit_linear_alloc( ( size_t )y.getSize(),
                                        ( size_t )c.getSize() );

    int32_t gsl_errno = gsl_multifit_linear( gsl_X,
                                             gsl_y,
                                             gsl_c,
                                             gsl_covariance,
                                             &chisq,
                                             work );

    gsl_multifit_linear_free( work );

    if ( gsl_errno )
    {

      throw std::runtime_error( gsl_strerror( gsl_errno ) );

    }

  }
  GKG_CATCH( "void gkg::GSLNumericalAnalysisImplementationFactory::"
             "getMultiLinearFit( "
             "const gkg::Matrix& X, "
             "const gkg::Vector& y, "
             "gkg::Vector& c, "
             "gkg::Matrix& covariance, "
             "double& chisq ) const" );

}


void gkg::GSLNumericalAnalysisImplementationFactory::getWeightedMultiLinearFit(
                                                        const gkg::Matrix& X,
                                                        const gkg::Vector& w,
                                                        const gkg::Vector& y,
                                                        gkg::Vector& c,
                                                        gkg::Matrix& covariance,
                                                        double& chisq ) const
{

  try
  {

    gsl_matrix* gsl_X = static_cast< gkg::GSLMatrixImplementation* >(
                                 X.getImplementation() )->getGSLMatrixPointer();
    gsl_vector* gsl_w = static_cast< gkg::GSLVectorImplementation* >(
                                 w.getImplementation() )->getGSLVectorPointer();
    gsl_vector* gsl_y = static_cast< gkg::GSLVectorImplementation* >(
                                 y.getImplementation() )->getGSLVectorPointer();
    gsl_vector* gsl_c = static_cast< gkg::GSLVectorImplementation* >(
                                 c.getImplementation() )->getGSLVectorPointer();
    gsl_matrix* gsl_covariance = static_cast< gkg::GSLMatrixImplementation* >(
                        covariance.getImplementation() )->getGSLMatrixPointer();

    gsl_multifit_linear_workspace*
      work = gsl_multifit_linear_alloc( ( size_t )y.getSize(),
                                        ( size_t )c.getSize() );

    int32_t gsl_errno = gsl_multifit_wlinear( gsl_X,
                                              gsl_w,
                                              gsl_y,
                                              gsl_c,
                                              gsl_covariance,
                                              &chisq,
                                              work );

    gsl_multifit_linear_free( work );

    if ( gsl_errno )
    {

      throw std::runtime_error( gsl_strerror( gsl_errno ) );

    }

  }
  GKG_CATCH( "void gkg::GSLNumericalAnalysisImplementationFactory::"
             "getWeightedMultiLinearFit( "
             "const gkg::Matrix& X, "
             "const gkg::Vector& w, "
             "const gkg::Vector& y, "
             "gkg::Vector& c, "
             "gkg::Matrix& covariance, "
             "double& chisq ) const" );

}


/////////////////////////// complex trigonometry ///////////////////////////////
std::complex< double > gkg::GSLNumericalAnalysisImplementationFactory::arccos(
                                         const std::complex< double >& z ) const
{

  try
  {

    gsl_complex gsl_zin, gsl_zout;

    gsl_zin.dat[ 0 ] = z.real();
    gsl_zin.dat[ 1 ] = z.imag();

    gsl_zout = gsl_complex_arccos( gsl_zin );

    return std::complex< double >( GSL_REAL( gsl_zout ),
                                   GSL_IMAG( gsl_zout ) );

  }
  GKG_CATCH( "std::complex< double > "
             "gkg::GSLNumericalAnalysisImplementationFactory::arccos( "
             "const std::complex< double >& z ) const" );

}


std::complex< double > gkg::GSLNumericalAnalysisImplementationFactory::arcsin(
                                         const std::complex< double >& z ) const
{

  try
  {

    gsl_complex gsl_zin, gsl_zout;

    gsl_zin.dat[ 0 ] = z.real();
    gsl_zin.dat[ 1 ] = z.imag();

    gsl_zout = gsl_complex_arcsin( gsl_zin );

    return std::complex< double >( GSL_REAL( gsl_zout ),
                                   GSL_IMAG( gsl_zout ) );

  }
  GKG_CATCH( "std::complex< double > "
             "gkg::GSLNumericalAnalysisImplementationFactory::arcsin( "
             "const std::complex< double >& z ) const" );

}


std::complex< double > gkg::GSLNumericalAnalysisImplementationFactory::arctan(
                                         const std::complex< double >& z ) const
{

  try
  {

    gsl_complex gsl_zin, gsl_zout;

    gsl_zin.dat[ 0 ] = z.real();
    gsl_zin.dat[ 1 ] = z.imag();

    gsl_zout = gsl_complex_arctan( gsl_zin );

    return std::complex< double >( GSL_REAL( gsl_zout ),
                                   GSL_IMAG( gsl_zout ) );

  }
  GKG_CATCH( "std::complex< double > "
             "gkg::GSLNumericalAnalysisImplementationFactory::arctan( "
             "const std::complex< double >& z ) const" );

}


////////////////////////////// Gamma function //////////////////////////////////
double gkg::GSLNumericalAnalysisImplementationFactory::getGammaFunction(
                                                         const double& x ) const
{

  try
  {

    return gsl_sf_gamma( x );

  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGammaFunction( const double& x ) const" );

}


float gkg::GSLNumericalAnalysisImplementationFactory::getGammaFunction(
                                                          const float& x ) const
{

  try
  {

    return ( float )gsl_sf_gamma( ( double )x );

  }
  GKG_CATCH( "float gkg::GSLNumericalAnalysisImplementationFactory::"
             "getGammaFunction( const float& x ) const" );

}


//////////////////////////// Bessel's functions ////////////////////////////////
double 
gkg::GSLNumericalAnalysisImplementationFactory::getRegularBesselFunctionJ0(
                                                         const double& x ) const
{

  try
  {

    return gsl_sf_bessel_J0( x );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getRegularBesselFunctionJ0( "
             "const double& x ) const" );

}


double 
gkg::GSLNumericalAnalysisImplementationFactory::getRegularBesselFunctionJ1(
                                                         const double& x ) const
{

  try
  {

    return gsl_sf_bessel_J1( x );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getRegularBesselFunctionJ1( "
             "const double& x ) const" );

}


double 
gkg::GSLNumericalAnalysisImplementationFactory::getRegularBesselFunctionI0(
                                                         const double& x ) const
{

  try
  {
  
    double I0 = 0.0;
    
    // to prevent GSL overflow on large values
    if ( x < ( GSL_LOG_DBL_MAX - 10.0 ) )
    {
    
      I0 = gsl_sf_bessel_I0( x );
    
    }
    else
    {
    
      I0 = std::exp( x ) / std::sqrt( 2.0 * M_PI * x );
    
    }

    return I0;

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getRegularBesselFunctionI0( "
             "const double& x ) const" );

}


double 
gkg::GSLNumericalAnalysisImplementationFactory::getRegularBesselFunctionI1(
                                                         const double& x ) const
{

  try
  {
  
    double I1 = 0.0;
    
    // to prevent GSL overflow on large values
    if ( x < ( GSL_LOG_DBL_MAX - 1.0 ) )
    {
    
      I1 = gsl_sf_bessel_I1( x );
    
    }
    else
    {
    
      I1 = std::exp( x ) / std::sqrt( 2.0 * M_PI * x );
    
    }

    return I1;

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getRegularBesselFunctionI1( "
             "const double& x ) const" );

}


double 
gkg::GSLNumericalAnalysisImplementationFactory::getRegularBesselFunctionIn(
                                       const int32_t& n, const double& x ) const
{

  try
  {
  
    double In = 0.0;
    
    // to prevent GSL overflow on large values
    if ( x < ( GSL_LOG_DBL_MAX - 1.0 ) )
    {
    
      In = gsl_sf_bessel_In( n, x );
    
    }
    else
    {
    
      In = std::exp( x ) / std::sqrt( 2.0 * M_PI * x );
    
    }

    return In;

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getRegularBesselFunctionI1( "
             "const int& n, const double& n ) const" );

}


//  regular Modified Bessel Functions - Factional Order BesselJ_l(x)                                                        //
double 
gkg::GSLNumericalAnalysisImplementationFactory::
             getRegularBesselFunctionFractionalOrder( double l, double x ) const
{

  try
  {

    return gsl_sf_bessel_Jnu( l, x );

  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "RegularBesselFunctionFractionalOrder( double l, "
	     "double x ) const" );

}


//  hypergeometric function - 1F1(a,b,x)  //
double
gkg::GSLNumericalAnalysisImplementationFactory::
            getHypergeometricFunction1F1( double a, double b, double x ) const
{

  try
  {

    return gsl_sf_hyperg_1F1( a, b, x );

  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "HypergeometricFunction1F1( double a, "
             "double b, double x ) const" );

}


//  zero of regular Bessel Functions

double
gkg::GSLNumericalAnalysisImplementationFactory::
                              getZeroBesselFunctionJ0( const uint32_t& k ) const
{
  
  try
  {
    
    return gsl_sf_bessel_zero_J0( k );
    
  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getZeroBesselFunctionJ0( const uint32_t& k ) const" );
  
}

double
gkg::GSLNumericalAnalysisImplementationFactory::
                              getZeroBesselFunctionJ1( const uint32_t& k ) const
{
  
  try
  {
    
    return gsl_sf_bessel_zero_J1( k );
    
  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getZeroBesselFunctionJ1( const uint32_t& k ) const" );
  
}

double
gkg::GSLNumericalAnalysisImplementationFactory::
                             getZeroBesselFunctionJn( const double& n,
                                                      const uint32_t& k ) const
{
  
  try
  {
    
    if ( n >= 0 )
    {
      
      return gsl_sf_bessel_zero_Jnu( n, k );
    
    }
    else
    {
      
      return 0;
      
    }
    
  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getZeroBesselFunctionJn( "
             "const double& n, "
             "const uint32_t& k ) const" );
  
}


struct BesselParameters 
{

  BesselParameters( double theN ) { n = theN; }
  double n;

};

static double besselDerivativeFunction( double x, void* p ) 
{

  try
  {

     BesselParameters* besselParameters = static_cast< BesselParameters* >( p );

     return  gsl_sf_bessel_Jnu( besselParameters->n, x );

  }
  GKG_CATCH( "static double besselDerivativeFunction( double x, void* p )" );

}

double 
gkg::GSLNumericalAnalysisImplementationFactory::getBesselDerivative( 
                                                           double n,
                                                           double x,
                                                           double h,
                                                           double* error ) const
                     
{
  
  try
  {

    BesselParameters besselParameters( n );
    gsl_function gslFunction;
    gslFunction.function = &besselDerivativeFunction;
    gslFunction.params = &besselParameters; 
   
    double result = 0.0;
    double internalError = 0.0;
    double* errorPointer = ( error ? error : &internalError );
    
    gsl_deriv_central( &gslFunction,
                       x,
                       h,
                       &result,
                       errorPointer );
   
    return result;
    
  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getBesselDerivative( "
             "double n, "
             "double x, "
             "double h, "
             "double* error ) const" );
  
}


////////////////////// non-linear least-squares fitting ////////////////////////
int32_t 
gkg::GSLNumericalAnalysisImplementationFactory::getLevenbergMarquardtFit(
                                            const gkg::Matrix& x,
                                            const gkg::Vector& y,
                                            const gkg::Vector& w,
                                            gkg::LevenbergMarquardtFunction& f,
                                            gkg::Matrix& covariance,
                                            double& chiSquarePerDegreeOfFreedom,
                                            int32_t maximumIterationCount,
                                            double absoluteError,
                                            double relativeError ) const
{

  try
  {

    gsl_vector* gsl_p = static_cast< gkg::GSLVectorImplementation* >(
                 f.getParameters().getImplementation() )->getGSLVectorPointer();


    int32_t trialCount = y.getSize();
    int32_t parameterCount = f.getParameters().getSize();

    // sanity checks
    if ( x.getSize1() != trialCount )
    {

      throw std::runtime_error( "incompatible x and y size(s)" );

    }
    if ( ( covariance.getSize1() != parameterCount ) || 
         ( covariance.getSize2() != parameterCount ) )
    {

      covariance.reallocate( parameterCount, parameterCount );

    }
    gsl_matrix* gsl_covariance = static_cast< gkg::GSLMatrixImplementation* >(
                        covariance.getImplementation() )->getGSLMatrixPointer();

    // initializing Levenberg-Marquardt solver
    const gsl_multifit_fdfsolver_type*
      solverType = gsl_multifit_fdfsolver_lmsder;
    gsl_multifit_fdfsolver*
      solver = gsl_multifit_fdfsolver_alloc( solverType,
                                             ( size_t )trialCount,
                                             ( size_t )parameterCount );

    gkg::GSLLMFunctionImplementation
       gslLMFunctionImplementation( &x, &y, &w, &f );

    gsl_multifit_fdfsolver_set( solver,
                                gslLMFunctionImplementation.getFdfPointer(),
                                gsl_p );

    // iterating until convergence
    int32_t iteration = 0;
    int32_t status;
    do
    {

      ++ iteration;
      status = gsl_multifit_fdfsolver_iterate( solver );

      if ( status )
      {

        break;

      }

      status = gsl_multifit_test_delta( solver->dx,
                                        solver->x,
                                        absoluteError,
                                        relativeError );


    }
    while ( ( status == GSL_CONTINUE ) &&
            ( iteration < maximumIterationCount ) );

    // processing covariance matrix
#if GSL_MAJOR_VERSION > 1

    gsl_matrix* J = gsl_matrix_alloc( ( size_t )trialCount,
                                      ( size_t )parameterCount );
    gsl_multifit_fdfsolver_jac( solver, J );
    gsl_multifit_covar( J, 0.0, gsl_covariance );
    gsl_matrix_free( J );

#else

    gsl_multifit_covar( solver->J, 0.0, gsl_covariance );

#endif

    // processing chi-square per degree of freedom
    chiSquarePerDegreeOfFreedom = std::pow( gsl_blas_dnrm2( solver->f ), 2.0 ) /
                                  ( trialCount - parameterCount );

    // copying back results to parameter vector
    gsl_vector_memcpy( gsl_p, solver->x );

    gsl_multifit_fdfsolver_free( solver );

    return iteration;

  }
  GKG_CATCH( "void "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getLevenbergMarquardtFit( "
             "const gkg::Matrix& x, "
             "const gkg::Vector& y, "
             "gkg::LevenbergMarquardtFunction& f, "
             "gkg::Matrix& covariance, "
             "double& chiSquarePerDegreeOfFreedom, "
             "int maximumIterationCount, "
             "double absoluteError, "
             "double relativeError ) const" );

}


/////////////////////// Nelder-Mead Simplex optimizer //////////////////////////
int32_t 
gkg::GSLNumericalAnalysisImplementationFactory::
                                          getNelderMeadSimplexOptimumParameters(
                                           gkg::Vector& parameters,
                                           gkg::Vector& deltaParameters,
                                           double& testSize,
                                           gkg::NelderMeadSimplexFunction& f,
                                           int32_t maximumIterationCount,
                                           double maximumTestSize,
                                           bool verbose  ) const
{

  try
  {

    gsl_vector* gsl_parameters = static_cast< gkg::GSLVectorImplementation* >(
                   parameters.getImplementation() )->getGSLVectorPointer();
    gsl_vector* gsl_deltaParameters =
                                 static_cast< gkg::GSLVectorImplementation* >(
                   deltaParameters.getImplementation() )->getGSLVectorPointer();


    // sanity checks
    int32_t parameterCount = parameters.getSize();
    if ( deltaParameters.getSize() != parameterCount )
    {

      throw std::runtime_error(
                         "incompatible parameter and delta parameter size(s)" );

    }

    // initializing Nelder-Mead optimizer type
    const gsl_multimin_fminimizer_type*
    //  optimizerType = gsl_multimin_fminimizer_nmsimplex;
    // only available from GSL 1.2
      optimizerType = gsl_multimin_fminimizer_nmsimplex2;
   // optimizerType = gsl_multimin_fminimizer_nmsimplex2rand;
    gsl_multimin_fminimizer*
      solver = gsl_multimin_fminimizer_alloc( optimizerType,
                                             ( size_t )parameterCount );

    gkg::GSLNMSFunctionImplementation
       gslNMSFunctionImplementation( &f );

    gsl_multimin_fminimizer_set(
                      solver,
                      gslNMSFunctionImplementation.getMultiminFunctionPointer(),
                      gsl_parameters,
                      gsl_deltaParameters );


    // iterating until convergence
    int32_t p = 0, iteration = 0;
    int32_t status;
    do
    {

      ++ iteration;
      status = gsl_multimin_fminimizer_iterate( solver );

      if ( status )
      {

        break;

      }

      testSize = gsl_multimin_fminimizer_size( solver );
      status = gsl_multimin_test_size( testSize,
                                       maximumTestSize );

      if ( verbose )
      {

        std::cout << "iteration " << iteration << " : "
                  << "parameters = ( ";
        for ( p = 0; p < parameterCount - 1; p++ )
        {

          std::cout << gsl_vector_get( solver->x, p ) << ", ";

        }
        std::cout << gsl_vector_get( solver->x, p ) << ") ";
        std::cout << "size = " << testSize << std::endl;

      }

    }
    while ( ( status == GSL_CONTINUE ) &&
            ( iteration < maximumIterationCount ) );

    // copying back results to parameter vector
    gsl_vector_memcpy( gsl_parameters, solver->x );

    // deleting the allocated solver
    gsl_multimin_fminimizer_free( solver );

    return iteration;

  }
  GKG_CATCH( "int32_t "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getNelderMeadSimplexOptimumParameters( "
             "gkg::Vector& parameters, "
             "gkg::Vector& deltaParameters, "
             "double& testSize, "
             "gkg::NelderMeadSimplexFunction& f, "
             "int32_t maximumIterationCount, "
             "double maximumTestSize, "
             "bool verbose  ) const" );

}


////////////////// Fletcher-Reeves Conjugate Gradient optimizer ////////////////
int32_t gkg::GSLNumericalAnalysisImplementationFactory::
                            getFletcherReevesConjugateGradientOptimumParameters( 
                                gkg::Vector& parameters,
                                gkg::FletcherReevesConjugateGradientFunction& f,
                                int32_t maximumIterationCount,
                                double stepSize,
                                double maximumTestGradient,
                                double maximumTolerance,
                                bool verbose ) const
{

  try
  {

    gsl_vector* gsl_parameters = static_cast< gkg::GSLVectorImplementation* >(
                   parameters.getImplementation() )->getGSLVectorPointer();

    // initializing Fletcher-Reeves Conjugate Gradient optimizer type
    int32_t parameterCount = parameters.getSize();
    const gsl_multimin_fdfminimizer_type*
      optimizerType = gsl_multimin_fdfminimizer_conjugate_fr;
    gsl_multimin_fdfminimizer*
      solver = gsl_multimin_fdfminimizer_alloc( optimizerType,
                                                ( size_t )parameterCount );

    gkg::GSLFRCGFunctionImplementation gslFRCGFunctionImplementation( &f );

    gsl_multimin_fdfminimizer_set(
                      solver,
                      gslFRCGFunctionImplementation.getFdfPointer(),
                      gsl_parameters,
                      stepSize,
                      maximumTolerance );

    // iterating until convergence
    int32_t p = 0, iteration = 0;
    int32_t status;
    do
    {

      ++ iteration;
      status = gsl_multimin_fdfminimizer_iterate( solver );

      if ( status )
      {

        break;

      }

      status = gsl_multimin_test_gradient( solver->gradient,
                                           maximumTestGradient );

      if ( verbose )
      {

        std::cout << "iteration " << iteration << " : "
                  << "parameters = ( ";
        for ( p = 0; p < parameterCount - 1; p++ )
        {

          std::cout << gsl_vector_get( solver->x, p ) << ", ";

        }
        std::cout << gsl_vector_get( solver->x, p ) << ") ";
        std::cout << "f = " << solver->f << std::endl;

      }

    }
    while ( ( status == GSL_CONTINUE ) &&
            ( iteration < maximumIterationCount ) );

    // copying back results to parameter vector
    gsl_vector_memcpy( gsl_parameters, solver->x );

    // deleting the allocated solver
    gsl_multimin_fdfminimizer_free( solver );

    return iteration;


  }
  GKG_CATCH( "int32_t gkg::GSLNumericalAnalysisImplementationFactory::"
             "getFletcherReevesConjugateGradientOptimumParameters( "
             "gkg::Vector& parameters, "
             "gkg::FletcherReevesConjugateGradientFunction& f, "
             "int32_t maximumIterationCount, "
             "double stepSize, "
             "double maximumTestGradient, "
             "double maximumTolerance, "
             "bool verbose ) const" );

}


/////////////////////// Non-Linear Programming optimizer ///////////////////////
int32_t gkg::GSLNumericalAnalysisImplementationFactory::
                           getConstrainedNonLinearProgrammingOptimumParameters( 
 gkg::GSLNumericalAnalysisImplementationFactory::NLPOptimizerType optimizerType,
 gkg::Vector& parameters,
 const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >& constraints,
 gkg::NonLinearProgrammingFunction& f,
 const std::vector< double >& optimizerParameters,
 bool verbose ) const
{

  try
  {

    //std::cout << "parameters=" << parameters << std::endl;

    //gsl_vector* gsl_parameters = static_cast< gkg::GSLVectorImplementation* >(
    //               parameters.getImplementation() )->getGSLVectorPointer();

    // collecting parameter count
    int32_t parameterCount = parameters.getSize();
    int32_t p = 0;

    // initializing optimizer type (either f or fdf according to type of
    // optimizer wo/derivative or w/derivative
    const gsl_multimin_fminimizer_type* optimizerWithoutDerivativeType = 0;
    const gsl_multimin_fdfminimizer_type* optimizerWithDerivativeType = 0;

    switch ( optimizerType )
    {

      // without derivative

      case gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex:

        optimizerWithoutDerivativeType = gsl_multimin_fminimizer_nmsimplex;
        break;

      case gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2:

        optimizerWithoutDerivativeType = gsl_multimin_fminimizer_nmsimplex2;
        break;

      case gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2Rand:

        optimizerWithoutDerivativeType = gsl_multimin_fminimizer_nmsimplex2rand;
        break;

      // with derivative

      case gkg::NumericalAnalysisImplementationFactory::
                                                FletcherReevesConjugateGradient:

        optimizerWithDerivativeType = gsl_multimin_fdfminimizer_conjugate_fr;
        break;

      case gkg::NumericalAnalysisImplementationFactory::
                                                  PolakRibiereConjugateGradient:

        optimizerWithDerivativeType = gsl_multimin_fdfminimizer_conjugate_pr;
        break;

      case gkg::NumericalAnalysisImplementationFactory::
                                                  BroydenFletcherGoldfarbShanno:

        optimizerWithDerivativeType = gsl_multimin_fdfminimizer_vector_bfgs;
        break;

      case gkg::NumericalAnalysisImplementationFactory::SteepestDescent:

        optimizerWithDerivativeType =
                                     gsl_multimin_fdfminimizer_steepest_descent;
        break;

      case gkg::NumericalAnalysisImplementationFactory::
                                                 BroydenFletcherGoldfarbShanno2:

        optimizerWithDerivativeType = gsl_multimin_fdfminimizer_vector_bfgs2;
        break;

    }

    int32_t maximumIterationCount = 0;
    double stepSizeOfTheFirstTrialStep = 0.0;
    double lineMinimizationTolerance = 0.0;
    double precision = 0.0;
    double maximumSizeOfTheSimplex = 0.0;
    gkg::Vector deltaParameters;

    if ( ( optimizerType == 
           gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex
         ) ||
         ( optimizerType == 
           gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2
         ) ||
         ( optimizerType == 
           gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2Rand
         ) )
    {

      if ( optimizerParameters.size() != 2U + ( uint32_t )parameterCount )
      {

        throw std::runtime_error( "size of 'optimizerParameters' must be "
                                  "equal to the number of parameters + 2" ); 
               

      }
      maximumIterationCount = ( int32_t )( optimizerParameters[ 0 ] + 0.5 );
      maximumSizeOfTheSimplex = optimizerParameters[ 1 ];

      deltaParameters.reallocate( parameterCount );
      for ( p = 0; p < parameterCount; p++ )
      {

        deltaParameters( p ) = optimizerParameters[ p + 2 ];

      }

      if ( verbose )
      {


        std::cout << "maximum iteration count: "
                  << maximumIterationCount
                  << std::endl; 
        std::cout << "maximum simplex size: "
                  << maximumSizeOfTheSimplex
                  << std::endl;
        std::cout << "delta parameters:";
        for ( p = 0; p < parameterCount; p++ )
        {

          std::cout << deltaParameters( p ) << " ";

        }
        std::cout << std::endl;

      }

    }
    else
    {

      if ( optimizerParameters.size() != 4U )
      {

        throw std::runtime_error( "size of 'optimizerParameters' must be "
                                  "equal to 4" ); 

      }
      maximumIterationCount = ( int32_t )( optimizerParameters[ 0 ] + 0.5 );
      stepSizeOfTheFirstTrialStep = optimizerParameters[ 1 ];
      lineMinimizationTolerance = optimizerParameters[ 2 ];
      precision = optimizerParameters[ 3 ];

      if ( verbose )
      {

        std::cout << "maximum iteration count: "
                  << maximumIterationCount
                  << std::endl; 
        std::cout << "step size of first trial step: "
                  << stepSizeOfTheFirstTrialStep
                  << std::endl; 
        std::cout << "line minimization tolerance: "
                  << lineMinimizationTolerance
                  << std::endl; 
        std::cout << "precision: "
                  << precision
                  << std::endl; 

      }

    }

    // initializing constrained parameter vector to manage constraints
    gkg::Vector constrainedParameters( parameterCount );
    for ( p = 0; p < parameterCount; p++ )
    {

      constrainedParameters( p ) = constraints[ p ]->transform( 
                                                              parameters( p ) );

    }
    gsl_vector* gsl_constrained_parameters =
      static_cast< gkg::GSLVectorImplementation* >(
             constrainedParameters.getImplementation() )->getGSLVectorPointer();

    // displaying initial values and boundaries before/after transformation
    if ( verbose )
    {
 
      std::cout << "initial values and boundaries before/after transformation:"
                << std::endl;
      for ( p = 0; p < parameterCount; p++ )
      {

        std::cout << "parameter " << p << " = "
                  << constraints[ p ]->displayTransformed(  parameters( p ) )
                  << std::endl;

      }

    }

    // allocating optimizer wo/derivative or w/derivative
    gsl_multimin_fminimizer* solverWithoutDerivativeType = 0;
    gsl_multimin_fdfminimizer* solverWithDerivativeType = 0;

    int32_t iteration = 0;

    if ( ( optimizerType == 
           gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex
         ) ||
         ( optimizerType == 
           gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2
         ) ||
         ( optimizerType == 
           gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2Rand
         ) )
    {

      solverWithoutDerivativeType = gsl_multimin_fminimizer_alloc(
                                                 optimizerWithoutDerivativeType,
                                                 ( size_t )parameterCount );


      gkg::GSLNLPFunctionImplementation 
        gslNLPFunctionImplementation( &f, constraints );

      gsl_vector* gsl_delta_parameters =
        static_cast< gkg::GSLVectorImplementation* >(
             deltaParameters.getImplementation() )->getGSLVectorPointer();
      

      gsl_multimin_fminimizer_set(
                      solverWithoutDerivativeType,
                      gslNLPFunctionImplementation.getFPointer(),
                      gsl_constrained_parameters,
                      gsl_delta_parameters );

      // iterating until convergence
      iteration = 0;
      double testSize = 0.0;
      int32_t status;
      do
      {

        ++ iteration;
        status = gsl_multimin_fminimizer_iterate( solverWithoutDerivativeType );

        if ( status )
        {

          break;

        }
        testSize = gsl_multimin_fminimizer_size( solverWithoutDerivativeType );
        status = gsl_multimin_test_size( testSize,
                                         maximumSizeOfTheSimplex );

        if ( verbose )
        {

          std::cout << "iteration " << iteration << " : "
                    << "parameters = ( ";
          double parameter = 0.0;
          for ( p = 0; p < parameterCount - 1; p++ )
          {

            parameter = gsl_vector_get( solverWithoutDerivativeType->x, p );
            std::cout << parameter << "->"
                      << constraints[ p ]->inverseTransform( parameter )
                      << ", ";

          }
          parameter = gsl_vector_get( solverWithoutDerivativeType->x, p );
          std::cout << parameter << "->"
                    << constraints[ p ]->inverseTransform( parameter )
                    << ") ";

          std::cout << "size = " << testSize << std::endl;

        }

      }
      while ( ( status == GSL_CONTINUE ) &&
              ( iteration < maximumIterationCount ) );

      // copying back results to constrained parameter vector
      gsl_vector_memcpy( gsl_constrained_parameters,
                         solverWithoutDerivativeType->x );

      // deleting the allocated solver
      gsl_multimin_fminimizer_free( solverWithoutDerivativeType );

    }
    else
    {

      solverWithDerivativeType = gsl_multimin_fdfminimizer_alloc(
                                                 optimizerWithDerivativeType,
                                                 ( size_t )parameterCount );

      gkg::GSLNLPFunctionImplementation
        gslNLPFunctionImplementation( &f, constraints );

      gsl_multimin_fdfminimizer_set(
                      solverWithDerivativeType,
                      gslNLPFunctionImplementation.getFdfPointer(),
                      gsl_constrained_parameters,
                      stepSizeOfTheFirstTrialStep,
                      lineMinimizationTolerance );

      // iterating until convergence
      iteration = 0;
      int32_t status;
      do
      {

        ++ iteration;
        status = gsl_multimin_fdfminimizer_iterate( solverWithDerivativeType );
        if ( status )
        {

          break;

        }

        status = gsl_multimin_test_gradient( solverWithDerivativeType->gradient,
                                             precision );
        if ( verbose )
        {

          std::cout << "iteration " << iteration << " : "
                    << "parameters = ( ";
          double parameter = 0.0;
          for ( p = 0; p < parameterCount - 1; p++ )
          {

            parameter = gsl_vector_get( solverWithDerivativeType->x, p );
            std::cout << parameter << "->"
                      << constraints[ p ]->inverseTransform( parameter )
                      << ", ";

          }
          parameter = gsl_vector_get( solverWithoutDerivativeType->x, p );
          std::cout << parameter << "->"
                    << constraints[ p ]->inverseTransform( parameter )
                    << ") ";

          std::cout << "f = " << solverWithDerivativeType->f << std::endl;

        }

      }
      while ( ( status == GSL_CONTINUE ) &&
              ( iteration < maximumIterationCount ) );

      // copying back results to constrained parameter vector
      gsl_vector_memcpy( gsl_constrained_parameters,
                         solverWithDerivativeType->x );

      // deleting the allocated solver
      gsl_multimin_fdfminimizer_free( solverWithDerivativeType );

    }

    // converting back constrained parameters to original parameter space
    for ( p = 0; p < parameterCount; p++ )
    {

      parameters( p ) = constraints[ p ]->inverseTransform( 
                                                   constrainedParameters( p ) );

    }

    return iteration;

  }
  GKG_CATCH( "int32_t gkg::GSLNumericalAnalysisImplementationFactory::"
             "getConstrainedNonLinearProgrammingOptimumParameters( " 
             "gkg::GSLNumericalAnalysisImplementationFactory::NLPOptimizerType "
             "optimizerType, "
             "gkg::Vector& parameters, "
             "const std::vector< gkg::RCPointer< gkg::OptimizerContraint > >& "
             "constraints, "
             "gkg::NonLinearProgrammingFunction& f, "
             "const std::vector< double >& optimizerParameters, "
             "bool verbose ) const" );

}


//
// generic wrapper from gkg::NonLinearProgrammingFunction objects to
// NLopt functions

static double gkg_to_nlopt_function_wrapper( uint32_t n,
                                             const double* parameters,
                                             double* gradient,
                                             void* data )
{

  try
  {

    gkg::NonLinearProgrammingFunction*
      nonLinearProgrammingFunction =
                      ( gkg::NonLinearProgrammingFunction* )data;

    gkg::Vector gkgParameters( n );
    int32_t i = 0;
    for ( i = 0; i < ( int32_t )n; i++ )
    {

      gkgParameters( i ) = parameters[ i ];

    }

    if ( nonLinearProgrammingFunction->hasDerivative() && gradient )
    {


      gkg::Vector gkgGradient( n );

      nonLinearProgrammingFunction->getDerivativeAt( gkgParameters,
                                                     gkgGradient );
     
      for ( i = 0; i < ( int32_t )n; i++ )
      {

        gradient[ i ] = gkgGradient( i );

      }

    }

    return nonLinearProgrammingFunction->getValueAt( gkgParameters );

  }
  GKG_CATCH( "static double gkg_to_nlopt_function_wrapper( "
             "uint32_t n, "
             "const double* parameters, "
             "double* gradient, "
             "void* data )" );

}


void gkg::GSLNumericalAnalysisImplementationFactory::
                           getConstrainedNonLinearProgrammingOptimumParameters2(
gkg::GSLNumericalAnalysisImplementationFactory::NLPOptimizerType2 optimizerType,
gkg::Vector& parameters,
const std::vector< gkg::RCPointer< gkg::NonLinearProgrammingFunction > >&
                                                          inequalityConstraints,
const std::vector< gkg::RCPointer< gkg::NonLinearProgrammingFunction > >&
                                                          equalityConstraints,
gkg::NonLinearProgrammingFunction& f,
const std::vector< double >& parameterLowerBounds,
const std::vector< double >& parameterUpperBounds,
const std::vector< double >& inequalityTolerances,
const std::vector< double >& equalityTolerances,
double relativeTolerance,
int32_t* achievedIterationCount,
double* reachedMinimumObjectiveValue,
bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // creating the NLP optimizer
    ////////////////////////////////////////////////////////////////////////////

    nlopt_algorithm nloptAlgorithm = NLOPT_LN_NEWUOA;

    if ( optimizerType == gkg::GSLNumericalAnalysisImplementationFactory::
                          LocalWoDerivative_COBYLA )
    {

      nloptAlgorithm = NLOPT_LN_COBYLA;
      if ( verbose )
      {

        std::cout << "using the LocalWoDerivative_COBYLA NLP algorithm"
                  << std::endl;

      }

    }
    else if ( optimizerType == gkg::GSLNumericalAnalysisImplementationFactory::
                          LocalWoDerivative_NEWUOA )
    {

      nloptAlgorithm = NLOPT_LN_NEWUOA;
      if ( verbose )
      {

        std::cout << "using the LocalWoDerivative_NEWUOA NLP algorithm"
                  << std::endl;

      }

    }
    else if ( optimizerType == gkg::GSLNumericalAnalysisImplementationFactory::
                          LocalWoDerivative_NEWUOA_BOUND )
    {

      nloptAlgorithm = NLOPT_LN_NEWUOA_BOUND;
      if ( verbose )
      {

        std::cout << "using the LocalWoDerivative_NEWUOA_BOUND NLP algorithm"
                  << std::endl;

      }

    }
    else if ( optimizerType == gkg::GSLNumericalAnalysisImplementationFactory::
                          LocalWithDerivative_MMA )
    {

      nloptAlgorithm = NLOPT_LD_MMA;
      if ( verbose )
      {

        std::cout << "using the LocalWithDerivative_MMA NLP algorithm"
                  << std::endl;

      }

    }

    int32_t parameterCount = f.getParameterCount();

    if ( verbose )
    {

      std::cout << parameterCount << " parameters to be optimized" << std::endl;

    }
    nlopt_opt nloptOptimizer = nlopt_create( nloptAlgorithm,
                                             ( uint32_t )parameterCount );

    ////////////////////////////////////////////////////////////////////////////
    // setting lower and upper bounds for parameter(s)
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double > localParameterLowerBounds( parameterCount, -HUGE_VAL);
    std::vector< double > localParameterUpperBounds( parameterCount, +HUGE_VAL);


    if ( parameterLowerBounds.size() == ( uint32_t )parameterCount )
    {

      localParameterLowerBounds = parameterLowerBounds;

    }
    else if ( !parameterLowerBounds.empty() )
    {

      throw std::runtime_error( "inconsistent lower bound vector size" );

    }

    if ( parameterUpperBounds.size() == ( uint32_t )parameterCount )
    {

      localParameterUpperBounds = parameterUpperBounds;

    }
    else if ( !parameterUpperBounds.empty() )
    {

      throw std::runtime_error( "inconsistent upper bound vector size" );

    }

    if ( verbose )
    {

      std::cout << "lower bound(s) : " << std::flush;
      std::vector< double >::const_iterator
        lb = localParameterLowerBounds.begin(),
        lbe = localParameterLowerBounds.end();
      while ( lb != lbe )
      {

        std::cout << *lb << " ";
        ++ lb;

      }
      std::cout << std::endl;

      std::cout << "upper bound(s) : " << std::flush;
      std::vector< double >::const_iterator
        ub = localParameterUpperBounds.begin(),
        ube = localParameterUpperBounds.end();
      while ( ub != ube )
      {

        std::cout << *ub << " ";
        ++ ub;

      }
      std::cout << std::endl;

    }
    

    nlopt_set_lower_bounds( nloptOptimizer, 
                            &localParameterLowerBounds[ 0 ] );
    nlopt_set_upper_bounds( nloptOptimizer, 
                            &localParameterUpperBounds[ 0 ] );
                            

    ////////////////////////////////////////////////////////////////////////////
    // setting objective function
    ////////////////////////////////////////////////////////////////////////////

    nlopt_set_min_objective( nloptOptimizer,
                             gkg_to_nlopt_function_wrapper,
                             &f );


    ////////////////////////////////////////////////////////////////////////////
    // adding inequality contraints
    ////////////////////////////////////////////////////////////////////////////


    // sanity check
    std::vector< double > localInequalityTolerances;

    std::cout << "inequalityConstraints.size()=" << inequalityConstraints.size()
              << std::endl;
    std::cout << "inequalityTolerances.size()=" << inequalityTolerances.size()
              << std::endl;
    if ( !inequalityConstraints.empty() )
    {

      localInequalityTolerances = std::vector< double >(
                                                   inequalityConstraints.size(),
                                                   1e-8 );
      if ( inequalityTolerances.size() == localInequalityTolerances.size() )
      {

        localInequalityTolerances = inequalityTolerances;

      }
      else if ( !inequalityTolerances.empty() )
      {

        throw std::runtime_error( "bad inequality tolerance vector size" );

      }

      if ( verbose )
      {

        std::cout << inequalityConstraints.size()
                  << " inequality constraints used"
                  << std::endl;
        std::cout << "inequality tolerance(s) : " << std::flush;
        std::vector< double >::const_iterator
          it = localInequalityTolerances.begin(),
          ite = localInequalityTolerances.end();
        while ( it != ite )
        {

          std::cout << *it << " ";
          ++ it;

        }
        std::cout << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

        std::cout << "no equality constraints used"
                  << std::endl;

      }

    }


    // creating the inequality constraints
    std::vector< gkg::RCPointer<
                           gkg::NonLinearProgrammingFunction > >::const_iterator
      ic = inequalityConstraints.begin(),
      ice = inequalityConstraints.end();
    std::vector< double >::const_iterator
      it = inequalityTolerances.begin();
    while ( ic != ice )
    {

      nlopt_add_inequality_constraint( nloptOptimizer,
                                       gkg_to_nlopt_function_wrapper,
                                       &**ic,
                                       *it );
      ++ it;
      ++ ic;

    }


    ////////////////////////////////////////////////////////////////////////////
    // adding equality contraints
    ////////////////////////////////////////////////////////////////////////////

    // sanity check
    std::vector< double > localEqualityTolerances;
    if ( !inequalityConstraints.empty() )
    {

      localEqualityTolerances = std::vector< double >(
                                                     equalityConstraints.size(),
                                                     1e-8 );
      if ( equalityTolerances.size() == localEqualityTolerances.size() )
      {

        localEqualityTolerances = equalityTolerances;

      }
      else if ( !equalityTolerances.empty() )
      {

        throw std::runtime_error( "bad equality tolerance vector size" );

      }

      if ( verbose )
      {

        std::cout << equalityConstraints.size()
                  << " equality constraints used"
                  << std::endl;
        std::cout << "equality tolerance(s) : " << std::flush;
        std::vector< double >::const_iterator
          et = localInequalityTolerances.begin(),
          ete = localInequalityTolerances.end();
        while ( et != ete )
        {

          std::cout << *et << " ";
          ++ et;

        }
        std::cout << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

        std::cout << "no equality constraints used"
                  << std::endl;

      }

    }

    // creating the equality constraints
    std::vector< gkg::RCPointer<
                           gkg::NonLinearProgrammingFunction > >::const_iterator
      ec = equalityConstraints.begin(),
      ece = equalityConstraints.end();
    std::vector< double >::const_iterator
      et = equalityTolerances.begin();
    while ( ec != ece )
    {

      nlopt_add_equality_constraint( nloptOptimizer,
                                     gkg_to_nlopt_function_wrapper,
                                     &**ec,
                                     *et );
      ++ et;
      ++ ec;

    }


    ////////////////////////////////////////////////////////////////////////////
    // setting relative tolerance
    ////////////////////////////////////////////////////////////////////////////

    nlopt_set_xtol_rel( nloptOptimizer, relativeTolerance );

    if ( verbose )
    {

      std::cout << "relative tolerance : " << relativeTolerance << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // optimizing
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "optimizing : " << std::flush;

    }
    double minimumObjectiveValue = 0.0;
    nlopt_result result = nlopt_optimize( nloptOptimizer,
                                          &parameters( 0 ),
                                          &minimumObjectiveValue );

    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "optimal parameters : " << parameters << std::endl;
      std::cout << "reached minimum objective function : "
                << minimumObjectiveValue << std::endl;
      std::cout << "achieved iteration count : "
                << nlopt_get_numevals( nloptOptimizer )
                << std::endl;

    }

    

    if ( reachedMinimumObjectiveValue )
    {

      *reachedMinimumObjectiveValue = minimumObjectiveValue;

    }

    if ( achievedIterationCount )
    {

      *achievedIterationCount = nlopt_get_numevals( nloptOptimizer );

    }


    ////////////////////////////////////////////////////////////////////////////
    // erasing optimizer
    ////////////////////////////////////////////////////////////////////////////

    nlopt_destroy( nloptOptimizer );


    ////////////////////////////////////////////////////////////////////////////
    // managing errors
    ////////////////////////////////////////////////////////////////////////////

    if ( result == -1 )
    {

      throw std::runtime_error( "NLOpt failure" );

    }
    else if ( result == -2 )
    {

      throw std::runtime_error( "NLOpt invalid arguments" );

    }
    else if ( result == -3 )
    {

      throw std::runtime_error( "NLOpt out of memory" );

    }
    else if ( result == -4 )
    {

      throw std::runtime_error( "NLOpt roundoff limited" );

    }
    else if ( result == -5 )
    {

      throw std::runtime_error( "NLOpt forced stop" );

    }

  }
  GKG_CATCH( "void gkg::GSLNumericalAnalysisImplementationFactory::"
             "getConstrainedNonLinearProgrammingOptimumParameters2( "
             "gkg::GSLNumericalAnalysisImplementationFactory::NLPOptimizerType2"
             " optimizerType, "
             "gkg::Vector& parameters, "
             "const std::vector< gkg::RCPointer< "
             "gkg::NonLinearProgrammingFunction > >& inequalityConstraints, "
             "const std::vector< gkg::RCPointer< "
             "gkg::NonLinearProgrammingFunction > >& equalityConstraints, "
             "gkg::NonLinearProgrammingFunction& f, "
             "const std::vector< double >& parameterLowerBounds, "
             "const std::vector< double >& parameterUpperBounds, "
             "const std::vector< double >& inequalityTolerances, "
             "const std::vector< double >& equalityTolerances," 
             "double relativeTolerance, "
             "int32_t* achievedIterationCount, "
             "double* reachedMinimumObjectiveValue, "
             "bool verbose ) const" );

}


////////////////////// convex quadratic programming solver /////////////////////
bool 
gkg::GSLNumericalAnalysisImplementationFactory::
                                          getConvexQuadraticProgrammingSolution(
                                                  const gkg::Matrix& Q,
                                                  const gkg::Vector& q,
                                                  const gkg::Matrix& A,
                                                  const gkg::Vector& b,
                                                  const gkg::Matrix& C,
                                                  const gkg::Vector& d,
                                                  gkg::Vector& x,
                                                  int32_t maximumIterationCount,
                                                  double epsilonGap,
                                                  double epsilonResiduals,
                                                  double epsilonInfeasible,
                                                  bool verbose ) const
{

  try
  {

    gkg::GSLConvexQuadraticProgrammingSolver
      convexQuadraticProgrammingSolver( Q, q, A, b, C, d, x );

    return convexQuadraticProgrammingSolver.solve( maximumIterationCount,
                                                   epsilonGap,
                                                   epsilonResiduals,
                                                   epsilonInfeasible,
                                                   verbose );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getConvexQuadraticProgrammingSolution( "
             "const gkg::Matrix& Q, "
             "const gkg::Vector& q, "
             "const gkg::Matrix& A, "
             "const gkg::Vector& b, "
             "const gkg::Matrix& C, "
             "const gkg::Vector& d, "
             "gkg::Vector& x, "
             "int32_t maximumIterationCount, "
             "double epsilonGap, "
             "double epsilonResiduals, "
             "double epsilonInfeasible, "
             "bool verbose ) const" );

}


bool 
gkg::GSLNumericalAnalysisImplementationFactory::
                                          getConvexQuadraticProgrammingSolution(
                                                  const gkg::Matrix& Q,
                                                  const gkg::Vector& q,
                                                  const gkg::Matrix& C,
                                                  const gkg::Vector& d,
                                                  gkg::Vector& x,
                                                  int32_t maximumIterationCount,
                                                  double epsilonGap,
                                                  double epsilonResiduals,
                                                  double epsilonInfeasible,
                                                  bool verbose ) const
{

  try
  {

    gkg::GSLConvexQuadraticProgrammingSolver
      convexQuadraticProgrammingSolver( Q, q, C, d, x );

    return convexQuadraticProgrammingSolver.solve( maximumIterationCount,
                                                   epsilonGap,
                                                   epsilonResiduals,
                                                   epsilonInfeasible,
                                                   verbose );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getConvexQuadraticProgrammingSolution( "
             "const gkg::Matrix& Q, "
             "const gkg::Vector& q, "
             "const gkg::Matrix& C, "
             "const gkg::Vector& d, "
             "gkg::Vector& x, "
             "int32_t maximumIterationCount, "
             "double epsilonGap, "
             "double epsilonResiduals, "
             "double epsilonInfeasible, "
             "bool verbose ) const" );

}


///////////////////////// Simulated Annealing optimizer ////////////////////////
int32_t gkg::GSLNumericalAnalysisImplementationFactory::
                                      getSimulatedAnnealingOptimumConfiguration(
                gkg::SimulatedAnnealingConfiguration& configuration,
                int32_t trialCountPerStep,
                int32_t iterationCountPerTemperature,
                double maximumStepSize,
                const gkg::SimulatedAnnealingConfiguration::BoltzmannParameters&
                                                            boltzmannParameters,
                const gkg::RandomGenerator& randomGenerator,
                bool verbose ) const
{

  try
  {

    gsl_rng* r = static_cast< gkg::GSLRandomGeneratorImplementation* >(
          randomGenerator.getImplementation() )->getGSLRandomGeneratorPointer();

    gkg::GSLSAConfigurationImplementation
      gslSAConfigurationImplementation( &configuration, verbose );

    gsl_siman_params_t simanParams;
    simanParams.n_tries = trialCountPerStep;
    simanParams.iters_fixed_T = iterationCountPerTemperature;
    simanParams.step_size = maximumStepSize;
    simanParams.k = boltzmannParameters.k;
    simanParams.t_initial = boltzmannParameters.initialTemperature;
    simanParams.mu_t = boltzmannParameters.mu;
    simanParams.t_min = boltzmannParameters.minimumTemperature;


    gsl_siman_solve(
                r,
                &configuration,
                gslSAConfigurationImplementation.getSimanEFuncPointer(),
                gslSAConfigurationImplementation.getSimanStepPointer(),
                gslSAConfigurationImplementation.getSimanMetricPointer(),
                gslSAConfigurationImplementation.getSimanPrintPointer(),
                gslSAConfigurationImplementation.getSimanCopyPointer(),
                gslSAConfigurationImplementation.getSimanCopyConstructPointer(),
                gslSAConfigurationImplementation.getSimanDestroyPointer(),
                0,
                simanParams );

    return 1;

  }
  GKG_CATCH( "int32_t gkg::GSLNumericalAnalysisImplementationFactory::"
             "getSimulatedAnnealingOptimumConfiguration( "
             "gkg::SimulatedAnnealingConfiguration& configuration, "
             "int32_t trialCountPerStep, "
             "int32_t iterationCountPerTemperature, "
             "double maximumStepSize, "
             "const gkg::SimulatedAnnealingParameters::BoltzmannParameters& "
             "boltzmannParameters, "
             "const gkg::RandomGenerator& randomGenerator, "
             "bool verbose ) const" );

}


/////////////////////////// Legendre polynomial(s) /////////////////////////////
double gkg::GSLNumericalAnalysisImplementationFactory::getLegendrePolynomial(
                                                     int32_t l, double x ) const
{

  try
  {

    return gsl_sf_legendre_Pl( l, x );

  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getLegendrePolynomial( int32_t l, double x ) const" );

}

double 
gkg::GSLNumericalAnalysisImplementationFactory::getLegendrePolynomialSpherical(
                                          int32_t l, int32_t m, double x ) const
{

  try
  {

    return gsl_sf_legendre_sphPlm( l, m, x );

  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getLegendrePolynomialSpherical( "
             "int32_t l, int32_t m, double x ) const" );

}


/////////////////////////// spherical harmonic(s) //////////////////////////////
std::complex< double > 
gkg::GSLNumericalAnalysisImplementationFactory::getSphericalHarmonic( 
                                                              int32_t l,
                                                              int32_t m,
                                                              double theta,
                                                              double phi ) const
{

  try
  {

    std::complex< double >
      value = std::polar( 1.0, std::abs( m ) * phi ) * 
              gsl_sf_legendre_sphPlm( l, std::abs( m ), std::cos( theta ) );

    if ( m < 0 )
    {

      value = std::conj( value );
      if ( m % 2 )
      {

        value *= -1.0;

      }

    } 
    return value;

  }
  GKG_CATCH( "std::complex< double > "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getSphericalHarmonic( "
             "int32_t l, "
             "int32_t m, "
             "double theta, "
             "double phi ) const" );

}

double
gkg::GSLNumericalAnalysisImplementationFactory::
getSharpeningDeconvolutionTransformCoefficient( int32_t l,
						double e1,
						double e2) const
{
  
  try
  {
    
    //eigen values from diffusion kernel (e1, e2, e2), where e1 >> e2
    double ratio = e2/e1;  
    
    double sharp = 0;
    double integral = 0.0;

    //
    // 	  Trapezoidal integration
    // 	  1/2 [ f(x0) + 2f(x1) + ... + 2f(x{n-1}) + f(xn) ] delta
    //    
    double delta = 0.002;  
    double z;
    for(z = -1; z <= 1; z+=delta) 
    {
      
      if(z == -1 || z == 1) 
      {
	
	sharp += getLegendrePolynomial(l,z)*std::sqrt(1/(1- (1 - ratio)*z*z));
	integral += std::sqrt(1/(1 - (1 - ratio)*z*z));
      
      }
      else 
      {
	
	sharp += 2*getLegendrePolynomial(l,z)*std::sqrt(1/(1- (1 - ratio)*z*z));
	integral += 2*std::sqrt(1/(1- (1 - ratio)*z*z));
      
      }
    
    }  
    
    integral = integral/2*delta;				
    sharp = sharp/2*delta;
    sharp = sharp/integral;
					
    return 1/sharp;

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getSharpeningDeconvolutionTransformCoefficient( "
	     "int32_t l,"
	     "double e1,"
	     "double e2) const" );
}

// DiracDeltaFunction on the sphere parametrised by SH Y_l^m is given by
// Y_l^0 (theta,phi) if m == 0 and 0 otherwise
double 
gkg::GSLNumericalAnalysisImplementationFactory::getDiracCoefficient(
							      int32_t j,
							      double theta,
							      double phi ) const
{
  try
  {
    
    int32_t l, m;
    getSymmetricalSphericalHarmonicLM( j, l, m );
    
    if( m == 0 )
    {

      return getSymmetricalSphericalHarmonic(j, theta, phi);

    }
    else
    {
      
      return 0.0;
      
    }

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getDiracCoefficient("
	     "int32_t j,"
	     "double theta,"
	     "double phi ) const" );
}

double 
gkg::GSLNumericalAnalysisImplementationFactory::getOddFactorial(
                                                                double n ) const
{
  try
  {
    
    if ( n < 0 )
    {

      return 1.0;

    }

    if ( !( ( int32_t )n % 2 ) )
    {
      
      std::runtime_error( "odd factorial must be called with an odd number" );

    }
    return  gsl_sf_doublefact( (int32_t)n );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
	     "getOddFactorial( double n ) const" );
}


double 
gkg::GSLNumericalAnalysisImplementationFactory::getEvenFactorial(
                                                                double n ) const
{
  try
  {

    if ( n < 0 )
    {

      return 1.0;

    }
 
    if ( ( int32_t )n % 2 )
    {
      
      std::runtime_error( "even factorial must be called with an even number" );

    }
    return gsl_sf_doublefact( (int32_t)n );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
	     "getEvenFactorial( double n ) const" );
}


//  Pochhammer symbol (a)_x //
double
gkg::GSLNumericalAnalysisImplementationFactory::
                                       getPochhammer( double a, double x ) const
{

  try
  {

    return gsl_sf_poch( a, x );

  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getPochhammer( double a, double x ) const" );

}


double
gkg::GSLNumericalAnalysisImplementationFactory::getSymmetricalSphericalHarmonic(
                                                              int32_t j,
                                                              double theta,
                                                              double phi ) const
{

  try
  {

    int32_t l, m;
    getSymmetricalSphericalHarmonicLM( j, l, m );

    double value;
    if ( ( m >= -l ) && ( m < 0 ) )
    {

      value = std::sqrt( 2.0 ) *
              std::real( getSphericalHarmonic( l, std::abs(m), theta, phi ) ); 

    }
    else if ( m == 0 )
    {

      value = std::real( getSphericalHarmonic( l, 0, theta, phi ) );

    }
    else
    {

      value = std::sqrt( 2.0 ) *
              std::imag( getSphericalHarmonic( l, m, theta, phi ) ); 
      if ( ( m & 2 ) == 0 )
      {

        value *= -1;

      }

    }
    return value;

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getSymmetricalSphericalHarmonic( "
             "int32_t j, "
             "double theta, "
             "double phi ) const" );

}


void gkg::GSLNumericalAnalysisImplementationFactory::
                                              getSymmetricalSphericalHarmonicLM( 
                                                              int32_t j,
                                                              int32_t& l,
                                                              int32_t& m ) const
{

  try
  {

    l = 0;
    while ( std::abs( j - ( l * l + l ) / 2 ) > l )
    {

      l += 2;

    }
    m = j - ( l * l + l ) / 2;

  }
  GKG_CATCH( "void "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getSymmetricalSphericalHarmonicLM( "
             "int32_t j, "
             "int32_t l, "
             "int32_t m ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getErf( double x ) const
{

  try
  {

    return gsl_sf_erf( x );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::getErf( "
             "double x ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getErfC( double x ) const
{

  try
  {

    return gsl_sf_erfc( x );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::getErfC( "
             "double x ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getErfZ( double x ) const
{

  try
  {

    return gsl_sf_erf_Z( x );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::getErfZ( "
             "double x ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getErfQ( double x ) const
{

  try
  {

    return gsl_sf_erf_Q( x );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::getErfQ( "
             "double x ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getDawsonIntegral(
                                                                double x ) const
{

  try
  {

    return gsl_sf_dawson( x );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getDawsonIntegral( "
             "double x ) const" );

}


/////////////////////////// numerical integration //////////////////////////////


static double integralFunction( double x, void* params )
{

  const gkg::IntegrableFunction*
   integrableFunction = static_cast< const gkg::IntegrableFunction* >( params );

  return integrableFunction->getValue( x );

}


double gkg::GSLNumericalAnalysisImplementationFactory::getQAGSIntegral(
                              const gkg::IntegrableFunction& integrableFunction,
                              double a,
                              double b,
                              double epsilonAbsolute,
                              double epsilonRelative,
                              double* error,
                              int32_t subIntervalCount ) const
{

  try
  {

    gsl_set_error_handler_off();

    gsl_integration_workspace*
      workSpace = gsl_integration_workspace_alloc( subIntervalCount );

    gsl_function gslFunction;
    gslFunction.function = &integralFunction;
    gslFunction.params = ( void* )&integrableFunction;

    double result = 0.0;
    double internalError = 0.0;
    double* errorPointer = ( error ? error : &internalError );

    int32_t status = gsl_integration_qags( &gslFunction,
                                           a,
                                           b,
                                           epsilonAbsolute,
                                           epsilonRelative,
                                           subIntervalCount,
                                           workSpace,
                                           &result,
                                           errorPointer );

    if ( status == GSL_EDIVERGE )
    {

      gsl_integration_qags( &gslFunction,
                            a,
                            b,
                            epsilonAbsolute * 1e-4,
                            epsilonRelative * 1e-4,
                            subIntervalCount,
                            workSpace,
                            &result,
                            errorPointer );

    }

    gsl_integration_workspace_free( workSpace );

    return result;

  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getQAGSIntegral( "
             "const gkg::IntegrableFunction& integrableFunction, "
             "double a, "
             "double b, "
             "double epsilonAbsolute, "
             "double epsilonRelative, "
             "double* error, "
             "int32_t subIntervalCount ) const" );

}


/////////////////////////// function interpolation /////////////////////////////


gkg::FunctionInterpolatorImplementation* 
gkg::GSLNumericalAnalysisImplementationFactory::
                                       createFunctionInterpolatorImplementation(
                                gkg::FunctionInterpolator* functionInterpolator,
                                gkg::FunctionInterpolator::Type type,
                                const std::vector< double >& xs,
                                const std::vector< double >& ys ) const
{

  try
  {

    return new gkg::GSLFunctionInterpolatorImplementation( functionInterpolator,
                                                           type,
                                                           xs,
                                                           ys );

  }
  GKG_CATCH( "gkg::FunctionInterpolatorImplementation* "
             "gkg::GSLNumericalAnalysisImplementationFactory:: "
             "createFunctionInterpolatorImplementation( "
             "gkg::FunctionInterpolator* functionInterpolator, "
             "gkg::FunctionInterpolator::Type type, "
             "const std::vector< double >& xs, "
             "const std::vector< double >& ys ) const" );

}


gkg::FunctionInterpolatorImplementation* 
gkg::GSLNumericalAnalysisImplementationFactory::
                                       createFunctionInterpolatorImplementation(
                                gkg::FunctionInterpolator* functionInterpolator,
                                gkg::FunctionInterpolator::Type type,
                                const gkg::Vector& xs,
                                const gkg::Vector& ys ) const
{

  try
  {

    return new gkg::GSLFunctionInterpolatorImplementation( functionInterpolator,
                                                           type,
                                                           xs,
                                                           ys );

  }
  GKG_CATCH( "gkg::FunctionInterpolatorImplementation* "
             "gkg::GSLNumericalAnalysisImplementationFactory:: "
             "createFunctionInterpolatorImplementation( "
             "gkg::FunctionInterpolator* functionInterpolator, "
             "gkg::FunctionInterpolator::Type type, "
             "const gkg::Vector& xs, "
             "const gkg::Vector& ys ) const" );

}


void gkg::GSLNumericalAnalysisImplementationFactory::
                     getPolynomialRoots( const gkg::Vector& polynomCoefficients,
                                         gkg::CVector& roots ) const
{

  try
  {

    int32_t polynomCoefficientCount = polynomCoefficients.getSize();

    gsl_vector* gsl_polynom_coefficients =
      static_cast< gkg::GSLVectorImplementation* >(
               polynomCoefficients.getImplementation() )->getGSLVectorPointer();

    gsl_poly_complex_workspace* 
      workspace = gsl_poly_complex_workspace_alloc( polynomCoefficientCount );

    roots.reallocate( polynomCoefficientCount - 1 );

    gsl_vector_complex* gsl_roots =
      static_cast< gkg::GSLCVectorImplementation* >(
                            roots.getImplementation() )->getGSLCVectorPointer();

    gsl_poly_complex_solve( gsl_polynom_coefficients->data,
                            polynomCoefficientCount,
                            workspace,
                            gsl_roots->data );

    gsl_poly_complex_workspace_free( workspace );

  }
  GKG_CATCH( "void gkg::GSLNumericalAnalysisImplementationFactory::"
             "getPolynomialRoots( const gkg::Vector& polynomCoefficients, "
             "gkg::CVector& roots ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::
                                 getExpectationMaximizationGaussianMixtureModel(
                     const gkg::Matrix& inputData,
                     int32_t distributionCount,
                     std::vector< gkg::Vector >& distributionMeans,
                     std::vector< gkg::Matrix >& distributionStandardDeviations,
                     std::vector< double >& distributionScales,
                     double tolerance,
                     int32_t maximumIterationCount,
                     bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting input data information
    ////////////////////////////////////////////////////////////////////////////

    int32_t sampleCount = inputData.getSize1();
    int32_t featureCount = inputData.getSize2();


    ////////////////////////////////////////////////////////////////////////////
    // allocating a random generator
    ////////////////////////////////////////////////////////////////////////////

    gsl_rng* randomGenerator = ::gsl_rng_alloc( gsl_rng_ranlxd2 );

    FILE* urandom = fopen( "/dev/urandom", "r" );
    uint32_t seed = fgetc( urandom ) |
                    ( fgetc( urandom ) <<  8 ) |
                    ( fgetc( urandom ) << 16 ) |
                    ( fgetc( urandom ) << 24 );
    fclose( urandom );
    ::gsl_rng_set( randomGenerator, ( uint32_t )time( NULL ) ^ seed );


    ////////////////////////////////////////////////////////////////////////////
    // allocating output(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( ( int32_t )distributionMeans.size() != distributionCount )
    {

      distributionMeans.resize( distributionCount );

    }
    int32_t d = 0;
    for ( d = 0; d < distributionCount; d++ )
    {

      if ( distributionMeans[ d ].getSize() != featureCount )
      {

        distributionMeans[ d ].reallocate( featureCount );

      }
      distributionMeans[ d ].setZero();

    }


    if ( ( int32_t )distributionStandardDeviations.size() != distributionCount )
    {

      distributionStandardDeviations.resize( distributionCount );

    }
    for ( d = 0; d < distributionCount; d++ )
    {

      if ( ( distributionStandardDeviations[ d ].getSize1() != featureCount ) ||
           ( distributionStandardDeviations[ d ].getSize2() != featureCount ) )
      {

        distributionStandardDeviations[ d ].reallocate( featureCount,
                                                        featureCount );

      }
      distributionStandardDeviations[ d ].setZero();

    }

    if ( ( int32_t )distributionScales.size() != distributionCount )
    {

      distributionScales = std::vector< double >( distributionCount, 0.0f );

    }

    std::vector< gsl_vector* > 
      gsl_distributionMeans( distributionCount );
    std::vector< gsl_matrix* >
      gsl_distributionStandardDeviations( distributionCount );
    for ( d = 0; d < distributionCount; d++ )
    {

      gsl_distributionMeans[ d ] =
        static_cast< gkg::GSLVectorImplementation* >(
            distributionMeans[ d ].getImplementation() )->getGSLVectorPointer();
      gsl_distributionStandardDeviations[ d ] =
        static_cast< gkg::GSLMatrixImplementation* >(
                 distributionStandardDeviations[ d ].getImplementation() )
                                                        ->getGSLMatrixPointer();

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating temporary work space(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector workSpace1( featureCount );
    gkg::Vector workSpace2( featureCount );
    gkg::Vector workSpace3( distributionCount );
    gkg::Vector xDiff( sampleCount );

    gsl_vector* gsl_workSpace1 =
      static_cast< gkg::GSLVectorImplementation* >(
                        workSpace1.getImplementation() )->getGSLVectorPointer();
    gsl_vector* gsl_workSpace2 =
      static_cast< gkg::GSLVectorImplementation* >(
                        workSpace2.getImplementation() )->getGSLVectorPointer();
    gsl_vector* gsl_workSpace3 =
      static_cast< gkg::GSLVectorImplementation* >(
                        workSpace3.getImplementation() )->getGSLVectorPointer();


    workSpace1.setZero();
    workSpace2.setZero();
    workSpace3.setZero();
    xDiff.setZero();


    std::vector< gkg::Vector > 
      previousDistributionMeans( distributionCount,
                                 gkg::Vector( featureCount ) );
    for ( d = 0; d < distributionCount; d++ )
    {

      previousDistributionMeans[ d ].setZero();

    }

    ////////////////////////////////////////////////////////////////////////////
    // initializing distribution mean(s)
    ////////////////////////////////////////////////////////////////////////////

    distributionMeans[ 0 ] = inputData.getRow( ::gsl_rng_uniform_int(
                                                                randomGenerator,
                                                                sampleCount ) );
    distributionStandardDeviations[ 0 ].setIdentity();
    distributionScales[ 0 ] = 1.0 / ( double )distributionCount;

    int32_t s = 0;
    int32_t dPrime = 0;
    double tmp2 = 0.0f;
    double sum = 0.0f;
    for ( d = 1; d < distributionCount; d++ )
    {

      sum = 0.0;

      for ( s = 0; s < sampleCount; s++ )
      {

        double& xDiffValue = xDiff( s );
        xDiffValue = INFINITY;

        workSpace1 = inputData.getRow( s );

        for ( dPrime = 0; dPrime < d; dPrime++ )
        {

          workSpace2 = workSpace1 - distributionMeans[ dPrime ];
          tmp2 = gsl_stats_tss_m( gsl_workSpace2->data,
                                  gsl_workSpace2->stride,
                                  gsl_workSpace2->size,
                                  0 );
          if ( tmp2 < xDiffValue )
          {

            xDiffValue = tmp2;

          }

        }
        sum += xDiffValue;

      }


      xDiff /= sum;

      double randomValue = ::gsl_rng_uniform( randomGenerator );

      sum = 0.0;
      for ( s = 0; s < sampleCount - 1; s++ )
      {

        sum += xDiff( s );
        if ( randomValue <= sum )
        {

          break;

        }

      }
      distributionMeans[ d ] = inputData.getRow( s );
      distributionStandardDeviations[ d ].setIdentity();
      distributionScales[ d ] = 1.0 / ( double )distributionCount;

    }


    ////////////////////////////////////////////////////////////////////////////
    // initializing the indicator matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix indicatorMatrix( sampleCount, distributionCount );
    indicatorMatrix.setZero();
    for ( s = 0; s < sampleCount; s++ )
    {

      indicatorMatrix( s,
                       ::gsl_rng_uniform_int( randomGenerator,
                                              distributionCount ) ) = 1.0;

    }


    ////////////////////////////////////////////////////////////////////////////
    // launching the main Expectation-Maximization loop
    ////////////////////////////////////////////////////////////////////////////

    int32_t iteration = 0;
    double difference = INFINITY;

    gkg::Vector distributionMeanDifference( featureCount );
    gsl_vector* gsl_distributionMeanDifference =
        static_cast< gkg::GSLVectorImplementation* >(
        distributionMeanDifference.getImplementation() )->getGSLVectorPointer();

    for ( iteration = 0; iteration < maximumIterationCount; iteration++ )
    {

      //////////////////////////////////////////////////////////////////////////
      // verbosity
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {


        /*if ( iteration != 0 )
        {

          std::cout << gkg::Eraser( 48 );

        }*/
        std::cout << std::endl;
        //std::string stringDifference = gkg::StringConverter::toString(
        //                                               difference, 'g', 20, 3 );
                std::cout << " [ " << std::setw( 6 ) << iteration + 1 
                  << " / " << std::setw( 6 ) << maximumIterationCount
                  << " ] -> " << difference << std::flush;

      }

      //////////////////////////////////////////////////////////////////////////
      // storing actual mean(s) into previous mean(s)
      //////////////////////////////////////////////////////////////////////////

      for ( d = 0; d < distributionCount; d++ )
      {

        previousDistributionMeans[ d ] = distributionMeans[ d ];

      }


      //////////////////////////////////////////////////////////////////////////
      // performing the Expectation step
      //////////////////////////////////////////////////////////////////////////

      for ( s = 0; s < sampleCount; s++ )
      {

        workSpace1 = inputData.getRow( s );
        for ( d = 0; d < distributionCount; d++ )
        {

          double* ptr = ::gsl_vector_ptr( gsl_workSpace3, d );
          ::gsl_ran_multivariate_gaussian_log_pdf(
                                        gsl_workSpace1,
                                        gsl_distributionMeans[ d ],
                                        gsl_distributionStandardDeviations[ d ],
                                        ptr,
                                        gsl_workSpace2 );

          *ptr += std::log( distributionScales[ d ] + DBL_MIN );

        }
        double maximum = ::gsl_vector_max( gsl_workSpace3 );
        double res = 0.0;
        for ( d = 0; d < distributionCount; d++ )
        {

          res += std::exp( ::gsl_vector_get( gsl_workSpace3, d ) - maximum );

        }
        double logSumExp = std::log( res + DBL_MIN ) + maximum;

        ::gsl_vector_add_constant( gsl_workSpace3,
                                   -logSumExp );

        for ( d = 0; d < distributionCount; d++ )
        {

          ::gsl_vector_set( gsl_workSpace3, d,
                            std::exp( ::gsl_vector_get( gsl_workSpace3,
                                                        d ) ) );

        }

        indicatorMatrix.setRow( s, workSpace3 );

      }


      //////////////////////////////////////////////////////////////////////////
      // performing the Maximization step
      //////////////////////////////////////////////////////////////////////////

      double denominator = 0.0;
      double tmp = 0.0;
      for ( d = 0; d < distributionCount; d++ )
      {

        distributionMeans[ d ].setZero();
        distributionStandardDeviations[ d ].setZero();

        denominator = 0.0;
        for ( s = 0; s < sampleCount; s++ )
        {

          tmp = indicatorMatrix( s, d );
          denominator += tmp;

          workSpace1 = inputData.getRow( s );
          workSpace1 *= tmp;
          distributionMeans[ d ] = distributionMeans[ d ] + workSpace1;

        }
        distributionScales[ d ] = denominator / ( double )sampleCount;
        denominator = 1.0 / denominator;

        distributionMeans[ d ] *= denominator;

        for ( s = 0; s < sampleCount; s++ )
        {

          tmp = indicatorMatrix( s, d );
          workSpace1 = inputData.getRow( s );
          workSpace1 = workSpace1 - distributionMeans[ d ];
          ::gsl_blas_dger( tmp,
                           gsl_workSpace1,
                           gsl_workSpace1, 
                           gsl_distributionStandardDeviations[ d ] );

        }
        ::gsl_matrix_scale( gsl_distributionStandardDeviations[ d ],
                            denominator );
        // the multivariate Gaussian Mixture Model requires a Cholesky decomp.
        //std::cout << distributionStandardDeviations[ d ] << std::endl;
        for ( int32_t f = 0; f < featureCount; f++ )
        {

          if ( distributionStandardDeviations[ d ]( f, f ) <= 0.0 )
          {

            distributionStandardDeviations[ d ]( f, f ) = DBL_MIN;

          }

        }
        ::gsl_linalg_cholesky_decomp( gsl_distributionStandardDeviations[ d ] );

      }

      //////////////////////////////////////////////////////////////////////////
      // checking the convergence
      //////////////////////////////////////////////////////////////////////////

      difference = 0.0;
      for ( d = 0; d < distributionCount; d++ )
      {

        distributionMeanDifference = previousDistributionMeans[ d ] -
                                     distributionMeans[ d ];

        difference += gsl_stats_tss_m( gsl_distributionMeanDifference->data,
                                       gsl_distributionMeanDifference->stride,
                                       gsl_distributionMeanDifference->size,
                                       0 );

      }

      if ( difference < tolerance )
      {

        break;

      }

    }

    if ( verbose )
    {

      //std::cout << gkg::Eraser( 48 );
      std::cout << std::endl;

    }

    return difference;

  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getExpectationMaximizationGaussianMixtureModel( "
             "const gkg::Matrix& inputData, "
             "int32_t distributionCount, "
             "std::vector< gkg::Vector >& distributionMeans, "
             "std::vector< gkg::Matrix >& distributionStandardDeviations, "
             "std::vector< double >& distributionScales, "
             "double tolerance, "
             "int32_t maximumIterationCount, "
             "bool verbose ) const" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::
                        getExpectationMaximizationGaussianMixtureModelAndLabels(
                     const gkg::Matrix& inputData,
                     int32_t distributionCount,
                     std::vector< gkg::Vector >& distributionMeans,
                     std::vector< gkg::Matrix >& distributionStandardDeviations,
                     std::vector< double >& distributionScales,
                     std::vector< int32_t >& labels,
                     gkg::Matrix& probabilities,
                     double& bic,
                     double tolerance,
                     int32_t maximumIterationCount,
                     bool /* verbose */ ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting input data information
    ////////////////////////////////////////////////////////////////////////////

    int32_t sampleCount = inputData.getSize1();
    int32_t featureCount = inputData.getSize2();


    ////////////////////////////////////////////////////////////////////////////
    // writing sklearn script
    ////////////////////////////////////////////////////////////////////////////

    std::ofstream os( "/tmp/gmm.py" );

    os << "#!/usr/bin/python" << std::endl;
    os << std::endl;
    os << "import numpy" << std::endl;
    os << "from sklearn import mixture" << std::endl;
    os << std::endl;
    os << "from scipy import linalg" << std::endl;
    os << "import matplotlib" << std::endl;
    os << "import itertools" << std::endl;
    os << std::endl;
    os << std::endl;

    os << "X = numpy.array( [ ";

    int32_t s = 0;
    int32_t f = 0;
    for ( s = 0; s < sampleCount; s++ )
    {

      os << "[ ";
      for ( f = 0; f < featureCount; f++ )
      {

        os << inputData( s, f );
        if ( f != featureCount - 1 )
        {

          os << ", ";

        }

      }
      if ( s != sampleCount - 1 )
      {

        os << "], ";

      }
      else
      {

        os << "] ";

      }

    }
    os << "], dtype = float )" << std::endl;
    os << std::endl;
    os << "gmm = mixture.GaussianMixture( n_components = "
       << distributionCount << ", " << std::endl
       << "                               tol = "
       << tolerance << ", " << std::endl
       << "                               max_iter = "
       << maximumIterationCount << ", " << std::endl
       << "                               covariance_type = 'full' )"
       << std::endl;
    os << "gmm.fit( X )" << std::endl;
    os << "bic = numpy.array( [ gmm.bic( X ) ], dtype = float )" << std::endl;
    os << std::endl;
    os << "means = gmm.means_" << std::endl;
    os << "covariances = gmm.covariances_" << std::endl;
    os << "scales = gmm.weights_" << std::endl;
    os << std::endl;
    os << "print()" << std::endl;
    os << "print( 'means :' )" << std::endl;
    os << "print( means )" << std::endl;
    os << "print()" << std::endl;
    os << "print( 'covariances :' )" << std::endl;
    os << "print( covariances )" << std::endl;
    os << std::endl;
    os << "file = open( '/tmp/means.bin', 'wb' )" << std::endl;
    os << "means.tofile( file )" << std::endl;
    os << "file.close()" << std::endl;
    os << std::endl;
    os << "file = open( '/tmp/covariances.bin', 'wb' )" << std::endl;
    os << "covariances.tofile( file )" << std::endl;
    os << "file.close()" << std::endl;
    os << std::endl;
    os << "file = open( '/tmp/scales.bin', 'wb' )" << std::endl;
    os << "scales.tofile( file )" << std::endl;
    os << "file.close()" << std::endl;
    os << std::endl;
    os << "labels = numpy.array( [ gmm.predict( X ) ], dtype = float )"
       << std::endl;
    os << "probabilities = numpy.array( [ gmm.predict_proba( X ) ], "
          "dtype = float )" << std::endl;
    os << std::endl;
    os << "file = open( '/tmp/labels.bin', 'wb' )" << std::endl;
    os << "labels.tofile( file )" << std::endl;
    os << "file.close()" << std::endl;
    os << std::endl;
    os << "file = open( '/tmp/probabilities.bin', 'wb' )" << std::endl;
    os << "probabilities.tofile( file )" << std::endl;
    os << "file.close()" << std::endl;
    os << std::endl;
    os << "file = open( '/tmp/bic.bin', 'wb' )" << std::endl;
    os << "bic.tofile( file )" << std::endl;
    os << "file.close()" << std::endl;
    os << std::endl;

    os.close();

    ////////////////////////////////////////////////////////////////////////////
    // executing python script
    ////////////////////////////////////////////////////////////////////////////

    if ( system( "python /tmp/gmm.py" ) < 0 )
    {

      throw std::runtime_error( "failed to execute python script" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating output(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( ( int32_t )distributionMeans.size() != distributionCount )
    {

      distributionMeans.resize( distributionCount );

    }
    int32_t d = 0;
    for ( d = 0; d < distributionCount; d++ )
    {

      if ( distributionMeans[ d ].getSize() != featureCount )
      {

        distributionMeans[ d ].reallocate( featureCount );

      }
      distributionMeans[ d ].setZero();

    }


    if ( ( int32_t )distributionStandardDeviations.size() != distributionCount )
    {

      distributionStandardDeviations.resize( distributionCount );

    }
    for ( d = 0; d < distributionCount; d++ )
    {

      if ( ( distributionStandardDeviations[ d ].getSize1() != featureCount ) ||
           ( distributionStandardDeviations[ d ].getSize2() != featureCount ) )
      {

        distributionStandardDeviations[ d ].reallocate( featureCount,
                                                        featureCount );

      }
      distributionStandardDeviations[ d ].setZero();

    }

    if ( ( int32_t )distributionScales.size() != distributionCount )
    {

      distributionScales = std::vector< double >( distributionCount, 0.0f );

    }


    if ( ( int32_t )labels.size() != sampleCount )
    {

      labels.resize( sampleCount );

    }

    if ( ( probabilities.getSize1() != sampleCount ) ||
         ( probabilities.getSize2() != distributionCount ) )
    {

      probabilities.reallocate( sampleCount, distributionCount );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading and filling distribution mean(s)
    ////////////////////////////////////////////////////////////////////////////

    std::ifstream is( "/tmp/means.bin" );

    for ( d = 0; d < distributionCount; d++ )
    {

      for ( f = 0; f < featureCount; f++ )
      {

        is.read( ( char* )&distributionMeans[ d ]( f ), sizeof( double ) );

      }

    }    

    is.close();


    ////////////////////////////////////////////////////////////////////////////
    // reading and filling distribution covariance(s)
    ////////////////////////////////////////////////////////////////////////////

    is.open( "/tmp/covariances.bin" );

    int32_t f1 = 0;
    int32_t f2 = 0;
    for ( d = 0; d < distributionCount; d++ )
    {

      for ( f1 = 0; f1 < featureCount; f1++ )
      {

        for ( f2 = 0; f2 < featureCount; f2++ )
        {

          is.read( ( char* )&distributionStandardDeviations[ d ]( f1, f2 ),
                   sizeof( double ) );

        }

      }

    }    

    is.close();


    ////////////////////////////////////////////////////////////////////////////
    // reading and filling distribution scale(s)
    ////////////////////////////////////////////////////////////////////////////

    is.open( "/tmp/scales.bin" );

    for ( d = 0; d < distributionCount; d++ )
    {

      is.read( ( char* )&distributionScales[ d ], sizeof( double ) );

    }    

    is.close();


    ////////////////////////////////////////////////////////////////////////////
    // reading and filling label(s)
    ////////////////////////////////////////////////////////////////////////////

    is.open( "/tmp/labels.bin" );

    double tmp = 0.0;
    for ( s = 0; s < sampleCount; s++ )
    {

      
      is.read( ( char* )&tmp, sizeof( double ) );
      labels[ s ] = ( int32_t )( tmp + 0.5 );

    }    

    is.close();


    ////////////////////////////////////////////////////////////////////////////
    // reading and filling probabilitie(s)
    ////////////////////////////////////////////////////////////////////////////

    is.open( "/tmp/probabilities.bin" );

    for ( s = 0; s < sampleCount; s++ )
    {

      for ( d = 0; d < distributionCount; d++ )
      {

        is.read( ( char* )&probabilities( s, d ), sizeof( double ) );

      }

    }    

    is.close();


    ////////////////////////////////////////////////////////////////////////////
    // reading and filling probabilitie(s)
    ////////////////////////////////////////////////////////////////////////////

    is.open( "/tmp/bic.bin" );
    is.read( ( char* )&bic, sizeof( double ) );
    is.close();

    return 0;

  }
  GKG_CATCH( "double gkg::GSLNumericalAnalysisImplementationFactory::"
             "getExpectationMaximizationGaussianMixtureModelAndLabels( "
             "const gkg::Matrix& inputData, int32_t distributionCount, "
             "std::vector< gkg::Vector >& distributionMeans, "
             "std::vector< gkg::Matrix >& distributionStandardDeviations, "
             "std::vector< double >& distributionScales, "
             "std::vector< int32_t >& labels, gkg::Matrix& probabilities, "
             "double& bic, double tolerance, int32_t maximumIterationCount, "
             "bool /* verbose */ ) const" );

}


/*

int32_t gkg::GSLNumericalAnalysisImplementationFactory::getKMeansClustering(
                                 const gkg::Matrix& inputData,
                                 int32_t clusterCount,
                                 int32_t maximumIterationCount,
                                 double tolerance = 1e-4,
                                 gkg::Matrix& centroids,
                                 std::vector< int32_t >& labels ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // allocating a random generator
    ////////////////////////////////////////////////////////////////////////////

    gsl_rng* randomGenerator = ::gsl_rng_alloc( gsl_rng_ranlxd2 );

    FILE* urandom = fopen( "/dev/urandom", "r" );
    uint32_t seed = fgetc( urandom ) |
                    ( fgetc( urandom ) <<  8 ) |
                    ( fgetc( urandom ) << 16 ) |
                    ( fgetc( urandom ) << 24 );
    fclose( urandom );
    ::gsl_rng_set( randomGenerator, ( uint32_t )time( NULL ) ^ seed );


    ////////////////////////////////////////////////////////////////////////////
    // collecting input data information
    ////////////////////////////////////////////////////////////////////////////

    int32_t sampleCount = inputData.getSize1();
    int32_t featureCount = inputData.getSize2();


    ////////////////////////////////////////////////////////////////////////////
    // computing a variance independent of the dataset
    ////////////////////////////////////////////////////////////////////////////

    gkg::AverageFilter< gkg::Vector, double > averageFilter;
    gkg::VarianceFilter< gkg::Vector, double > varianceFilter;
    gkg::Vector featureMeans( featureCount );
    gkg::Vector featureVariances( featureCount );
    int32_t f = 0;
    for ( f = 0; f < featureCount; f++ )
    {

      averageFilter.filter( inputData.getColumn( f ), featureMeans( f ) );
      varianceFilter.filter( inputData.getColumn( f ), featureVariances( f ) );

    }

    double actualTolerance = 0.0;
    averageFilter.filter( variances, actualTolerance );

    actualTolerance *= tolerance;

 
    ////////////////////////////////////////////////////////////////////////////
    // activating distance precomputing only if used memory not too large
    ////////////////////////////////////////////////////////////////////////////

    bool precomputeDistances = true;
    if ( clusterCount * sampleCount < 12000000 )
    {

      precomputeDistances = false;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating a modified input matrix after removing feature mean(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix inputDataMinusMeans( sampleCout, featureCount );
    int32_t s = 0;
    for ( f = 0; f < featureCount; f++ )
    {

      const double& featureMean = featureMeans( f );
      for ( s = 0; s < sampleCount; s++ )
      {

        inputDataMinusMeans( s, f ) = inputData( s, f ) - featureMean;

      }

    }



    ////////////////////////////////////////////////////////////////////////////
    // computing modified input data squared norms
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector inputDataMinusMeanSquareNorms( sampleCout );
    gkg::SumOfSquareFilter< gkg::Vector, double > sumOfSquareFilter;
    for ( s = 0; s < sampleCount; s++ )
    {

      sumOfSquareFilter.filter( inputDataMinusMeans.getRow( s ),
                                inputDataMinusMeanSquareNorms( s ) );

    }


    ////////////////////////////////////////////////////////////////////////////
    // initializing centroid(s) modified input data squared norms
    ////////////////////////////////////////////////////////////////////////////

    centroids.reallocate( clusterCount, featureCount );

    int32_t localTrialCount = 2 + ( int32_t )std::log( ( double )clusterCount );


    // picking first centroid randomly
    int32_t centroidIndex = ::gsl_rng_uniform_int( randomGenerator,
                                                   sampleCount );
    centroids.setRow( 0, inputDataMinusMeans.getRow( centroidIndex ) );

    // allocating vectors to store distance from centroid candidate to
    // other samples
    gkg::Vector squareEuclideanDistanceToCentroidCandidate( sampleCount );
    gkg::Vector newSquareEuclideanDistanceToCentroidCandidate( sampleCount );
    gkg::Vector bestSquareEuclideanDistanceToCentroidCandidate( sampleCount );
    
    // computing the distances and potential for the first centroid
    for ( s = 0; s < sampleCount; s++ )
    {

      squareEuclideanDistanceToCentroidCandidate( s ) =
                                inputDataMinusMeanSquareNorms( centroidIndex ) +
                                inputDataMinusMeanSquareNorms( s ) - 2.0 *
                                inputDataMinusMeans.getRow( centroidIndex ).dot(
                                              inputDataMinusMeans.getRow( s ) );

    }
    double currentPotential =
                    squareEuclideanDistanceToCentroidCandidate.getSumOfValues();


    // creating other centroids
    gkg::Vector randomValues( localTrialCount );
    std::vector< int32_t > centroidCandidateIndices( localTrialCount );
    int32_t c = 0;
    int r = 0;
    double newPotential = 0.0;
    double bestPotential = INFINITY;
    int32_t bestCentroidCandidateIndex = 0;

    for ( c = 1; c < clusterCount; c++ )
    {

      for ( t = 0; t < trialCount; t++ )
      {

        randomValues( t ) = ::gsl_rng_uniform( randomGenerator ) *
                            currentPotential;

      }
      centroidCandidateIndices = np.searchsorted(
                                       stable_cumsum( closest_dist_sq ),
                                       randomValues )

      gkg::Matrix squareEuclideanDistanceToCentroidCandidates( localTrialCount,
                                                               sampleCount );
      for ( t = 0; t < trialCount; t++ )
      {

        for ( s = 0; s < sampleCount; s++ )
        {

          squareEuclideanDistanceToCentroidCandidates( t, s ) =
                                inputDataMinusMeanSquareNorms( t ) +
                                inputDataMinusMeanSquareNorms( s ) - 2.0 *
                                inputDataMinusMeans.getRow( t ).dot(
                                              inputDataMinusMeans.getRow( s ) );

        }

      }

      for ( t = 0; t < trialCount; t++ )
      {

        for ( s = 0; s < sampleCount; s++ )
        {

          newSquareEuclideanDistanceToCentroidCandidate( s ) =
            std::min( squareEuclideanDistanceToCentroidCandidate( s ),
                      squareEuclideanDistanceToCentroidCandidates( t, s ) );

        }
        newPotential = 
                 newSquareEuclideanDistanceToCentroidCandidate.getSumOfValues();
        if ( newPotential < bestPotential )
        {

          bestCentroidCandidateIndex = centroidCandidateIndices[ t ];
          bestPotential = newPotential;
          bestSquareEuclideanDistanceToCentroidCandidate =
                                  newSquareEuclideanDistanceToCentroidCandidate;

        }

      }

      centroids.setRow( c, inputDataMinusMeans.getRow( 
                                                 bestCentroidCandidateIndex ) );
      squareEuclideanDistanceToCentroidCandidate =
                                 bestSquareEuclideanDistanceToCentroidCandidate;   
      currentPotential = bestPotential;

    }


    ////////////////////////////////////////////////////////////////////////////
    // running the k-means algorithm
    ////////////////////////////////////////////////////////////////////////////



    int32_t actualIterationCount = 0;
    int32_t actualClusterCount = 0;


    inputs: inputDataMinusMeans, centroids, cluterCount,
            maximumIteration, actualTolerance
    outputs: centroids, labels
    

    gkg::Matrix copyOfCentroids = centroids;
    gkg::Vector halfCentroidNorms( clusterCount );
    for ( c = 0; c < clusterCount; c++ )
    {

      halfCentroidNorms( s ) = copyOfCentroid.getRow( s ).getNorm() / 2.0;

    }

    gkg::Matrix lowerBounds( sampleCount, clusterCount );
    lowerBounds.fill( 0.0 );
   
    labels.resize( sampleCount );

    gkg::Vector uperBounds( sampleCount );

    ////////////////////////////////////////////////////////////////////////////
    // adding the mean that was originally removed
    ////////////////////////////////////////////////////////////////////////////


    for ( f = 0; f < featureCount; f++ )
    {

      const double& featureMean = featureMeans( f );
      for ( c = 0; c < clusterCount; c++ )
      {

        centroids( c, f ) += featureMean;

      }

    }

  }
  GKG_CATCH( "" );

}


double gkg::GSLNumericalAnalysisImplementationFactory::
                                getExpectationMaximizationGaussianMixtureModel2(
                     const gkg::Matrix& inputData,
                     int32_t distributionCount,
                     std::vector< gkg::Vector >& distributionMeans,
                     std::vector< gkg::Matrix >& distributionStandardDeviations,
                     std::vector< double >& distributionScales,
                     double tolerance,
                     int32_t maximumIterationCount,
                     bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting input data information
    ////////////////////////////////////////////////////////////////////////////

    int32_t sampleCount = inputData.getSize1();
    int32_t featureCount = inputData.getSize2();

    double maximumLowerBound = -INFINITY;
    bool hasConverged = false;


    ////////////////////////////////////////////////////////////////////////////
    // initializing Gaussian mixture parameters
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix resp( sampleCount, distributionCount );

    

  }
  GKG_CATCH( "" );

}
*/

double 
gkg::GSLNumericalAnalysisImplementationFactory::getMultivariateGaussianPdf(
                                          const gkg::Vector& values,
                                          const gkg::Vector& means,
                                          const gkg::Matrix& standardDeviations,
                                          double scale ) const
{

  try
  {

    int32_t componentCount = values.getSize();

    if ( ( means.getSize() != componentCount ) ||
         ( standardDeviations.getSize1() != componentCount ) ||
         ( standardDeviations.getSize2() != componentCount ) )
    {

      throw std::runtime_error(
               "inconsistent size(s) of values, means and standardDeviations" );

    }
         
    gsl_vector* v = static_cast< gkg::GSLVectorImplementation* >(
                            values.getImplementation() )->getGSLVectorPointer();
    gsl_vector* m = static_cast< gkg::GSLVectorImplementation* >(
                            means.getImplementation() )->getGSLVectorPointer();

    gsl_matrix* s = static_cast< gkg::GSLMatrixImplementation* >(
                standardDeviations.getImplementation() )->getGSLMatrixPointer();

    gsl_vector* work = gsl_vector_alloc( componentCount );
    
    double probability;

    gsl_ran_multivariate_gaussian_pdf( v, m, s, &probability, work );

    return scale * probability;

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getMultivariateGaussianPdf( "
             "const gkg::Vector& values, "
             "const gkg::Vector& means, "
             "const gkg::Matrix& standardDeviations, "
             "double scale ) const" );

}



double 
gkg::GSLNumericalAnalysisImplementationFactory::getMultivariateGaussianLogPdf(
                                          const gkg::Vector& values,
                                          const gkg::Vector& means,
                                          const gkg::Matrix& standardDeviations,
                                          double scale ) const
{

  try
  {

    int32_t componentCount = values.getSize();

    if ( ( means.getSize() != componentCount ) ||
         ( standardDeviations.getSize1() != componentCount ) ||
         ( standardDeviations.getSize2() != componentCount ) )
    {

      throw std::runtime_error(
               "inconsistent size(s) of values, means and standardDeviations" );

    }
         
    gsl_vector* v = static_cast< gkg::GSLVectorImplementation* >(
                            values.getImplementation() )->getGSLVectorPointer();
    gsl_vector* m = static_cast< gkg::GSLVectorImplementation* >(
                            means.getImplementation() )->getGSLVectorPointer();

    gsl_matrix* s = static_cast< gkg::GSLMatrixImplementation* >(
                standardDeviations.getImplementation() )->getGSLMatrixPointer();

    gsl_vector* work = gsl_vector_alloc( componentCount );
    
    double probability;

    gsl_ran_multivariate_gaussian_log_pdf( v, m, s, &probability, work );

    return probability + std::log( scale );

  }
  GKG_CATCH( "double "
             "gkg::GSLNumericalAnalysisImplementationFactory::"
             "getMultivariateGaussianLogPdf( "
             "const gkg::Vector& values, "
             "const gkg::Vector& means, "
             "const gkg::Matrix& standardDeviations, "
             "double scale ) const" );

}


