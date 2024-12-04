#include <gkg-processing-plugin-gsl-lapack/GSLConvexQuadraticProgrammingSolver.h>
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



gkg::GSLConvexQuadraticProgrammingSolver::GSLConvexQuadraticProgrammingSolver(
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

    this->allocateYZ();
    this->sanityChecks();
    this->allocateStatusParameters();

  }
  GKG_CATCH( "gkg::GSLConvexQuadraticProgrammingSolver::"
             "GSLConvexQuadraticProgrammingSolver( "
             "const gkg::Matrix& Q, "
             "const gkg::Vector& q, "
             "const gkg::Matrix& A, "
             "const gkg::Vector& b, "
             "const gkg::Matrix& C, "
             "const gkg::Vector& d, "
             "gkg::Vector& x )" );

}


gkg::GSLConvexQuadraticProgrammingSolver::GSLConvexQuadraticProgrammingSolver(
                                         const gkg::Matrix& Q,
                                         const gkg::Vector& q,
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

    this->allocateYZ();
    this->sanityChecks();
    this->allocateStatusParameters();

    std::cout << "end of gkg::GSLConvexQuadraticProgrammingSolver::GSLConvexQuadraticProgrammingSolver()" 
              << std::endl;

  }
  GKG_CATCH( "gkg::GSLConvexQuadraticProgrammingSolver::"
             "GSLConvexQuadraticProgrammingSolver( "
             "const gkg::Matrix& Q, "
             "const gkg::Vector& q, "
             "const gkg::Matrix& C, "
             "const gkg::Vector& d, "
             "gkg::Vector& x )" );

}


gkg::GSLConvexQuadraticProgrammingSolver::~GSLConvexQuadraticProgrammingSolver()
{

  gsl_vector_free( _s );
  gsl_matrix_free( _kkt );
  gsl_vector_free( _r);

  if ( _y )
  {

    gsl_vector_free( _y );

  }
  gsl_vector_free( _z );

}


bool gkg::GSLConvexQuadraticProgrammingSolver::solve(
                                                  int32_t maximumIterationCount,
                                                  double epsilonGap,
                                                  double epsilonResiduals,
                                                  double epsilonInfeasible,
                                                  bool verbose )
{

  try
  {

    std::cout << "solve: 1" << std::endl;

    this->settingInitialPoints();

    std::cout << "solve: 2" << std::endl;


    int32_t iteration = 1;

    if  ( verbose )
    {

      std::cout << "=== Iteration === f ==== ||gap|| "
                << "==== ||residuals||===="
                << std::endl
                << std::endl;

    }

    bool hasConverged = false;
    do
    {

      std::cout << "  do 1" << std::endl;

      this->iterate();

      std::cout << "  do 2" << std::endl;

      hasConverged = this->testConvergence( epsilonGap, epsilonResiduals );

      std::cout << "  do 3" << std::endl;

      if ( verbose )
      {

        std::cout << "\t" << iteration << "\t"
                  << this->getObjectiveFunction() << "\t"
                  << _gap << "\t"
                  << _residualNorm
                  << std::endl;

      }

      if ( hasConverged )
      {

        if ( verbose )
        {

          std::cout << std::endl
                    << "Minimum is found at : "
                    << std::flush;
          int32_t i = 0;
          for ( i = 0; i < _N; i++ )
          {

            std::cout << gsl_vector_get( _x, i ) << " ";

          }
          std::cout << std::endl;

        }

      }
      ++ iteration;
      std::cout << "  do 4" << std::endl;


    }
    while ( ( hasConverged == false ) && 
            ( iteration <= maximumIterationCount ) );

    // if the solver behaves instable, stop at this point
    bool validSolution = true;
    if ( !hasConverged )
    {

      if ( !this->testInfeasibility( epsilonInfeasible ) )
      {

        validSolution = false;

      }

    }

    return validSolution;

  }
  GKG_CATCH( "bool gkg::GSLConvexQuadraticProgrammingSolver::solve( "
             "int32_t maximumIterationCount, "
             "double epsilonGap, "
             "double epsilonResiduals, "
             "double epsilonInfeasible, "
             "bool verbose )" );


}


void gkg::GSLConvexQuadraticProgrammingSolver::allocateYZ()
{

  try
  {

    // collecting the dimension of the problem
    _N = ( int32_t )_Q->size1;

    // allocating Lagrange multiplier corresponding to Ax = b
    if ( _A )
    {

      _Me = ( int32_t )_b->size;
      _y = gsl_vector_alloc( _Me );

    }
    else
    {

      _Me = 0;
      _y = 0;

    }

    // allocating Lagrange multiplier corresponding to Cx >= d
    _Mi = ( int32_t )_d->size;
    _z = gsl_vector_alloc( _Mi );

  }
  GKG_CATCH( "void gkg::GSLConvexQuadraticProgrammingSolver::allocatingYZ()" );

}


void gkg::GSLConvexQuadraticProgrammingSolver::sanityChecks()
{

  try
  {

    if ( ( int32_t )_x->size != _N )
    {

      throw std::runtime_error( "Q and x have incompatible sizes" );

    }

    if ( ( int32_t )_Q->size2 != _N )
    {

      throw std::runtime_error( "Q is not a square matrix" );

    }

    if ( ( int32_t )_q->size != _N )
    {

      throw std::runtime_error( "q and Q have incompatible sizes" );

    }

    if ( _A )
    {

      if ( ( int32_t )_A->size2 != _N )
      {

        throw std::runtime_error( "A has a bad column count" );

      }

      if ( ( int32_t )_A->size1 != _Me )
      {

        throw std::runtime_error( "A has a bad row count" );

      }

    }

    if ( ( int32_t )_C->size2 != _N )
    {

      throw std::runtime_error( "C has a bad column count" );

    }

    if ( ( int32_t )_C->size1 != _Mi )
    {

      throw std::runtime_error( "C has a bad row count" );

    }

  }
  GKG_CATCH( "void gkg::GSLConvexQuadraticProgrammingSolver::sanityChecks()" );

}


void gkg::GSLConvexQuadraticProgrammingSolver::allocateStatusParameters()
{

  try
  {

    // allocating slack vector for the constrain: Cx + s = d
    _s = gsl_vector_alloc( _Mi );

    // allocating kkt matrix
    _kkt = gsl_matrix_calloc( _N + _Me + _Mi, _N + _Me + _Mi);

    // allocating the vector of the right-hand side in the kkt-system
    // r=(r_Q,r_A,r_C)
    _r = gsl_vector_alloc( _N + _Me + _Mi );

    // constant for the Mehrotra's heuristic
    _tau = 0.0;

    // Gondzio's parameters :
    // maximum number of corrections
    _maximumK = 0;
    // minimum threshold values for outlier compl. products
    _minimumBeta = 0.0;
    // maximum threshold values for outlier compl. products
    _maximumBeta = 0.0;
    // required increase of stepsize
    _deltaAlpha = 0.0;
    // minimum acceptable increase of stepsize
    _gamma = 0.0;
    // norm of the problem data
    _dataNorm = 0.0;

  }
  GKG_CATCH( "void gkg::GSLConvexQuadraticProgrammingSolver::"
             "allocateStatusParameters()" );

}




void gkg::GSLConvexQuadraticProgrammingSolver::settingInitialPoints()
{

  try
  {
std::cout << "step0" << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // computing x feasible initial point x = A^t * ( A A^t )^{-1} * b
    ////////////////////////////////////////////////////////////////////////////
    if ( _A )
    {

      gsl_matrix* AA_t = gsl_matrix_calloc( _Me, _Me );
      gsl_vector* tmp = gsl_vector_alloc( _Me );
      gsl_permutation* p = gsl_permutation_alloc( _Me );
      gsl_blas_dgemm( CblasNoTrans, CblasTrans, 1.0, _A, _A, 0.0, AA_t );
      // (AA^t)tmp = b
      int32_t signum = 0;
      gsl_linalg_LU_decomp( AA_t, p, &signum );
      gsl_linalg_LU_solve( AA_t, p, _b, tmp );

      gsl_blas_dgemv( CblasTrans, 1.0, _A, tmp, 0.0, _x );

      gsl_matrix_free( AA_t );
      gsl_vector_free( tmp );
      gsl_permutation_free( p );

    }
    else
    {

      gsl_vector_set_zero( _x );

    }
    displayVector( "x feasible initial point:", _x );
std::cout << "step1" << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // computing s feasible initial point s = C x - d
    ////////////////////////////////////////////////////////////////////////////
    gsl_blas_dcopy( _d, _s );
    gsl_blas_dgemv( CblasNoTrans, 1.0, _C, _x, -1.0, _s );


    displayVector( "s feasible initial point:", _s );

std::cout << "step2" << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // computing y feasible initial point y=(AA^t)^{-1}(A(Qx+q))
    ////////////////////////////////////////////////////////////////////////////
    if ( _A )
    {

      gsl_matrix* AA_t = gsl_matrix_calloc( _Me, _Me );
      gsl_vector* tmp = gsl_vector_alloc( _N );
      gsl_permutation* p = gsl_permutation_alloc( _Me );

      gsl_blas_dcopy( _q, tmp );
      gsl_blas_dsymv( CblasUpper, 1.0, _Q, _x, 1.0, tmp );
      gsl_blas_dgemv( CblasNoTrans, 1.0, _A, tmp, 0.0, _y );

      gsl_vector_free( tmp );

      tmp = gsl_vector_alloc( _Me );
      gsl_blas_dcopy( _y, tmp );
      gsl_blas_dgemm( CblasNoTrans, CblasTrans, 1.0, _A, _A, 0.0, AA_t );

      int32_t signum = 0;
      gsl_linalg_LU_decomp( AA_t, p, &signum);
      gsl_linalg_LU_solve( AA_t, p, tmp, _y );

      gsl_matrix_free( AA_t );
      gsl_vector_free( tmp );
      gsl_permutation_free( p );

    }
    else
    {

      std::cout << "no y feasible inital point computed since _A=0"
                << std::endl;

    }

std::cout << "step3" << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // computing z feasible initial point z = (0.1,0.1,...)
    ////////////////////////////////////////////////////////////////////////////
    gsl_vector_set_all( _z, 0.1 );


    ////////////////////////////////////////////////////////////////////////////
    // computing the strictly feasible initial points z and s
    ////////////////////////////////////////////////////////////////////////////
    double deltaZ = GSL_MAX_DBL( -1.5 * gsl_vector_min( _z ), 0.0 );
    double deltaS = GSL_MAX_DBL( -1.5 * gsl_vector_min( _s ), 0.0 );
    double rg = 1e-10;
    double tmp = 0.0;

    if ( ( deltaZ < rg ) && ( deltaS < rg ) )
    {

      gsl_blas_ddot( _z, _s, &tmp );

    }
    // if the initial point is not optimal
    if ( tmp >= rg )
    {

      int32_t i = 0U;
      double zs = 0.0;
      double sumZ = 0.0;
      double sumS = 0.0;
      for ( i = 0; i < _Mi; i++ )
      {

        zs += ( gsl_vector_get( _z, i ) + deltaZ ) *
              ( gsl_vector_get( _s, i ) + deltaS );

        sumZ += gsl_vector_get( _z, i );
        sumS += gsl_vector_get( _s, i );

      }

      sumZ += deltaZ * ( double )_Mi;
      sumS += deltaS * ( double )_Mi;

      deltaZ += 0.5 * zs / sumS;
      deltaS += 0.5 * zs / sumZ;

      gsl_vector_add_constant( _z, deltaZ );
      gsl_vector_add_constant( _s, deltaS );

    }

    displayVector( "s strictly feasible initial point:", _s );
    displayVector( "z strictly feasible initial point:", _z );

std::cout << "step4" << std::endl;

    // duality gap
    _gap = 0.0;
    gsl_blas_ddot( _z, _s, &_gap );
    _gap /= ( double )_Mi;

    this->buildKkt();

    _tau = 3.0;

    // for the convergence conditions  dataNorm = ||Q,A,C,q,b,d||_{infinity}
    _minimizerDataNorm = gsl_matrix_max_norm( _Q );
    if ( _A )
    {

      _minimizerDataNorm = GSL_MAX_DBL( _minimizerDataNorm,
                                        gsl_matrix_max_norm( _A ) );

    }
    _minimizerDataNorm = GSL_MAX_DBL( _minimizerDataNorm,
                                      gsl_matrix_max_norm( _C ) );
    _minimizerDataNorm = GSL_MAX_DBL( _minimizerDataNorm,
                                      gsl_vector_max_norm( _q ) );
    if ( _A )
    {

      _minimizerDataNorm = GSL_MAX_DBL( _minimizerDataNorm,
                                        gsl_vector_max_norm( _b ) );

    }
    _minimizerDataNorm = GSL_MAX_DBL( _minimizerDataNorm,
                                      gsl_vector_max_norm( _d ) );

    _dataNorm = _minimizerDataNorm;

    std::cout << "_dataNorm=" << _dataNorm << std::endl;

std::cout << "step5" << std::endl;

    // ||r||_{infinity}=||r_Q, r_A, r_C||_{infinity}
    this->computeResiduals();

    displayVector( "_r=", _r );

std::cout << "step6" << std::endl;

    _residualNorm = gsl_vector_max_norm( _r );

    _quantityOfInfeasibility = ( _residualNorm +
                                 this->computeGapInfeasiblePoints() ) /
                               _dataNorm;
    _minimumQuantityOfInfeasibility = _quantityOfInfeasibility;

    std::cout << "_minimumQuantityOfInfeasibility=" << _minimumQuantityOfInfeasibility << std::endl;

    // Gondzio's paameters
    // maximum number of corrections
    _maximumK = 0;
    // minimum threshold values for outlier compl. products
    _minimumBeta = 0.1;
    // maximum threshold values for outlier compl. products
    _maximumBeta = 10.0;
    // required increase of stepsize
    _deltaAlpha = 0.1;
    // minimum acceptable increase of stepsize
    _gamma = 0.1;
std::cout << "step7" << std::endl;

  }
  GKG_CATCH( "void gkg::GSLConvexQuadraticProgrammingSolver::"
             "settingInitialPoints()" );

}


void gkg::GSLConvexQuadraticProgrammingSolver::buildKkt()
{

  try
  {

    // KKT - Matrix
    //
    //       | Q    A^t     C^t   |
    // kkt = | A    0       0     |
    //       | C    0    -Z^{-1}S |

    gsl_matrix_view kktBlock;

    // 1st block
    kktBlock = gsl_matrix_submatrix( _kkt, 0, 0, _N, _N );
    gsl_matrix_memcpy( &kktBlock.matrix, _Q );


    // 2nd block
    if ( _A )
    {

      kktBlock = gsl_matrix_submatrix( _kkt, 0, _N, _N, _Me );
      gsl_matrix_transpose_memcpy( &kktBlock.matrix, _A );

    }


    // 3rd block
    kktBlock = gsl_matrix_submatrix( _kkt, 0, _N + _Me, _N, _Mi );
    gsl_matrix_transpose_memcpy( &kktBlock.matrix, _C );


    // 4th block
    if ( _A )
    {

      kktBlock = gsl_matrix_submatrix( _kkt, _N, 0, _Me, _N );
      gsl_matrix_memcpy( &kktBlock.matrix, _A );

    }


    // 5th block
    kktBlock = gsl_matrix_submatrix( _kkt, _N + _Me, 0, _Mi, _N );
    gsl_matrix_memcpy( &kktBlock.matrix, _C );


    // Null block
    kktBlock = gsl_matrix_submatrix( _kkt, _N, _N, _Me + _Mi, _Me + _Mi );
    gsl_matrix_set_zero( &kktBlock.matrix );

    // 6th block
    int32_t i = 0;
    for ( i = _N + _Me; i < _N + _Me + _Mi; i++ )
    {
      gsl_matrix_set( _kkt, i, i,
                      -gsl_vector_get( _s, i - ( _N + _Me ) ) /
                       gsl_vector_get( _z, i - ( _N + _Me ) ) );

    }

  }
  GKG_CATCH( "void gkg::GSLConvexQuadraticProgrammingSolver::buildKkt()" );

}


void gkg::GSLConvexQuadraticProgrammingSolver::computeResiduals()
{

  try
  {

    gsl_vector_view rBlock;

    // r_Q=Qx+q-A^ty-C^tz

    rBlock = gsl_vector_subvector( _r, 0, _N );
    gsl_blas_dcopy( _q, &rBlock.vector );
    gsl_blas_dsymv( CblasUpper, 1.0, _Q, _x, 1.0, &rBlock.vector);

    if ( _A )
    {

      gsl_blas_dgemv( CblasTrans, -1.0, _A, _y, 1.0, &rBlock.vector );

    }
    gsl_blas_dgemv( CblasTrans, -1.0, _C, _z, 1.0, &rBlock.vector );

    // r_A=Ax-b
    if ( _A )
    {

      rBlock = gsl_vector_subvector( _r, _N, _Me );
      gsl_blas_dcopy( _b, &rBlock.vector );
      gsl_blas_dgemv( CblasNoTrans, 1.0, _A, _x, -1.0, &rBlock.vector );

    }

    // r_C=Cx-s-d
    rBlock = gsl_vector_subvector( _r, _N + _Me, _Mi );
    gsl_blas_dcopy( _s, &rBlock.vector );
    gsl_blas_daxpy( 1.0, _d, &rBlock.vector );
    gsl_blas_dgemv( CblasNoTrans, 1.0, _C, _x, -1.0, &rBlock.vector );

  }
  GKG_CATCH( "void gkg::GSLConvexQuadraticProgrammingSolver::"
             "computeResiduals()" );

}


double gkg::GSLConvexQuadraticProgrammingSolver::computeGapInfeasiblePoints()
{

  try
  {

    gsl_vector* tmpV = gsl_vector_alloc( _N );
    gsl_blas_dcopy( _q, tmpV );
    gsl_blas_dsymv( CblasUpper, 1.0, _Q, _x, 1.0, tmpV );

    double gap = 0.0;
    gsl_blas_ddot( _x, tmpV, &gap );

    double tmpD = 0.0;
    if ( _A )
    {

      gsl_blas_ddot( _b, _y, &tmpD );
      gap -= tmpD;

    }

    gsl_blas_ddot( _d, _z, &tmpD );
    gap -= tmpD;

    gsl_vector_free( tmpV );

    return gap;

  }
  GKG_CATCH( "double gkg::GSLConvexQuadraticProgrammingSolver::"
             "computeGapInfeasiblePoints()" );

}


void gkg::GSLConvexQuadraticProgrammingSolver::iterate()
{

  try
  {

    gsl_permutation* p = gsl_permutation_alloc( _N + _Me + _Mi );
    gsl_vector* delta = gsl_vector_alloc( _N + _Me + _Mi );
    gsl_vector* deltaGondzio = gsl_vector_alloc( _N + _Me + _Mi );
    gsl_vector* deltaSGondzio = gsl_vector_alloc( _Mi );
    gsl_vector* deltaS = gsl_vector_alloc( _Mi );
    gsl_vector* r_zs = gsl_vector_alloc( _Mi );

    std::cout << "    iterate 1" << std::endl;


    // the right-hand side of the KKT-system: r = -( r_Q, r_A, r_C+Z^{-1}*r_zs )
    // the vectors of variables: delta = ( deltaX, -deltaY, -deltaZ )
    // delta_s for the slack variable s: deltaS = Z^{-1}( -r_zs - S*deltaZ )

    ////////////////////////////////////////////////////////////////////////////
    // predictor step
    ////////////////////////////////////////////////////////////////////////////

    // in the predictor step: r_zs = ZSe
    gsl_vector_view rBlock = gsl_vector_subvector( _r, _N + _Me , _Mi );
    gsl_blas_daxpy( 1.0, _s, &rBlock.vector );
    gsl_blas_dscal( -1.0, _r );

    std::cout << "    iterate 2" << std::endl;


    // solving the KKT-system
    // evaluating the LU-decomposition of the KKT-matrix
    int32_t signum = 0;

    displayMatrix( "_kkt:", _kkt );

    gsl_linalg_LU_decomp( _kkt, p, &signum );

    std::cout << "    iterate 3" << std::endl;

    // finding the predictor step


    gsl_linalg_LU_solve( _kkt, p, _r, delta );

    std::cout << "    iterate 4" << std::endl;


    int32_t i = 0;
    for ( i = 0; i < _Mi; i++ )
    {

      // finding deltaS for the slack variable s:
      //   deltaS = Z^{-1}(-r_zs - * S * deltaZ)
      gsl_vector_set( deltaS, i,
                      gsl_vector_get( _s, i ) *
                      ( gsl_vector_get( delta, _N + _Me + i ) /
                        gsl_vector_get( _z, i ) - 1.0 ) );

    }

    std::cout << "    iterate 5" << std::endl;

    // finding the stepsize of the predictor step
    gsl_vector_view deltaBlock = gsl_vector_subvector( delta, _N + _Me, _Mi );
    double alpha = this->getStepLength( deltaS, &deltaBlock.vector );

    std::cout << "    iterate 6" << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    //  evaluation of the centering parameter sigma
    //    sigma = (gap_aff/gap)^tau
    ////////////////////////////////////////////////////////////////////////////

    double sigma = 0.0;
    for ( i = 0; i < _Mi; i++ )
    {

      sigma += ( gsl_vector_get( _z, i ) -
                 alpha * gsl_vector_get( delta, _N + _Me + i ) ) *
               ( gsl_vector_get( _s, i ) +
                 alpha * gsl_vector_get( deltaS, i ) );
    }
    sigma /= ( _Mi * _gap );
    sigma = std::pow( sigma, _tau );

    std::cout << "    iterate 7" << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // corrector step
    ////////////////////////////////////////////////////////////////////////////

    // modifying the right-hand side of the kkt-system in order to find the
    // Mehrotra's corrector step

    rBlock = gsl_vector_subvector( _r, _N + _Me, _Mi );
    double tmpD = 0.0;
    for ( i = 0; i < _Mi; i++ )
    {

      tmpD = -sigma * _gap - gsl_vector_get( delta, _N + _Me + i ) *
             gsl_vector_get( deltaS, i );
      gsl_vector_set( r_zs, i,
                      gsl_vector_get( _z, i ) * gsl_vector_get( _s, i ) +
                      tmpD );
      gsl_vector_set( &rBlock.vector, i,
                      gsl_vector_get( &rBlock.vector, i ) - tmpD /
                      gsl_vector_get( _z, i ) );

    }
    std::cout << "    iterate 8" << std::endl;

    // finding the corrector step
    gsl_linalg_LU_solve( _kkt, p, _r, delta );


    std::cout << "    iterate 9" << std::endl;

    for ( i = 0; i < _Mi; i++ )
    {

      // deltaS = Z^{-1}*(-r_zs-S*deltaZ)
      gsl_vector_set( deltaS, i,
                      ( -gsl_vector_get( r_zs, i ) +
                        gsl_vector_get( delta, _N + _Me + i ) *
                        gsl_vector_get( _s, i ) ) / gsl_vector_get( _z, i ) );

    }
    std::cout << "    iterate 10" << std::endl;

    // evaluating the stepsize
    deltaBlock = gsl_vector_subvector( delta, _N + _Me, _Mi );
    alpha = this->getStepLength( deltaS, &deltaBlock.vector );

    std::cout << "    iterate 11" << std::endl;

    // Gondzio's centrality correction steps
    double alphaGondzio = 0.0;
    double tmpV = 0.0;
    double tmpVt = 0.0;
    i = 0;
    int32_t j = 0; 
    while ( i < _maximumK )
    {

      alphaGondzio = GSL_MIN_DBL( alpha + _deltaAlpha, 1.0 );
      rBlock = gsl_vector_subvector( _r, _N + _Me, _Mi );

      for ( j = 0; j < _Mi; j++ )
      {

        tmpV = ( gsl_vector_get( _z, j ) - alphaGondzio *
                 gsl_vector_get( delta, _N + _Me + j ) ) *
               ( gsl_vector_get( _s, j ) + alphaGondzio *
                 gsl_vector_get( deltaS, j ) );
        tmpVt = GSL_MIN_DBL( GSL_MAX_DBL( tmpV, _minimumBeta * sigma * _gap ),
                             _maximumBeta * sigma * _gap );
        gsl_vector_set( r_zs, j, gsl_vector_get( r_zs, j ) - (tmpVt - tmpV ) );
        gsl_vector_set( &rBlock.vector, j,
                        gsl_vector_get( &rBlock.vector, j ) +
                        ( tmpVt - tmpV ) / gsl_vector_get( _z, j ) );

      }

      gsl_linalg_LU_solve( _kkt, p, _r, deltaGondzio );
      for ( j = 0; j < _Mi; j++ )
      {

        gsl_vector_set( deltaSGondzio, j,
                        ( -gsl_vector_get( r_zs, j ) +
                          gsl_vector_get( deltaGondzio, _N + _Me + j ) *
                          gsl_vector_get( _s, j ) ) / gsl_vector_get( _z, j ) );

      }

      // evaluating the stepsize
      deltaBlock = gsl_vector_subvector( deltaGondzio, _N + _Me, _Mi );
      alphaGondzio = this->getStepLength( deltaS, &deltaBlock.vector );


      if ( alphaGondzio >= alpha + _gamma * _deltaAlpha )
      {

        ++ i;
        alpha = alphaGondzio;
        gsl_blas_dcopy( deltaGondzio, delta );
        gsl_blas_dcopy( deltaSGondzio, deltaS );

      }
      else
      {

        break;

      }

    }

    std::cout << "    iterate 12" << std::endl;

    // heuristic for step length
    alpha = GSL_MIN_DBL( 0.995 * alpha, 1.0 );


    // Updating:  x^k = x^k + alpha * deltaX
    deltaBlock = gsl_vector_subvector( delta, 0, _N );
    gsl_blas_daxpy( alpha, &deltaBlock.vector, _x );

    std::cout << "    iterate 13" << std::endl;

    // y^k = y^k - alpha * (-deltaY)
    if ( _y != 0 )
    {

      deltaBlock = gsl_vector_subvector( delta, _N, _Me );
      gsl_blas_daxpy( -alpha, &deltaBlock.vector, _y );

    }

    std::cout << "    iterate 14" << std::endl;

    // z^k = z^k - alpha * (-deltaZ)
    deltaBlock = gsl_vector_subvector( delta, _N + _Me, _Mi );
    gsl_blas_daxpy( -alpha, &deltaBlock.vector, _z );

    std::cout << "    iterate 15" << std::endl;

    // s^k = s^k + alpha * (deltaS)
    gsl_blas_daxpy( alpha, deltaS, _s );

    std::cout << "    iterate 16" << std::endl;

    // duality gap
    gsl_blas_ddot( _z, _s, &_gap );
    _gap /= _Mi;

    std::cout << "    iterate 17" << std::endl;

    // data for the next iteration
    this->computeResiduals();

    std::cout << "    iterate 18" << std::endl;

    _residualNorm = gsl_vector_max_norm( _r );

    this->buildKkt();
    std::cout << "    iterate 19" << std::endl;

    // for the infeasibility test
    _quantityOfInfeasibility = ( _residualNorm + 
                                 this->computeGapInfeasiblePoints() ) /
                               _dataNorm;
    std::cout << "    iterate 20" << std::endl;

    _minimumQuantityOfInfeasibility = GSL_MIN_DBL(
                                              _quantityOfInfeasibility,
                                              _minimumQuantityOfInfeasibility );

    std::cout << "    iterate 21" << std::endl;

    gsl_permutation_free( p );
    gsl_vector_free( delta );
    gsl_vector_free( deltaGondzio );
    gsl_vector_free( deltaSGondzio );
    gsl_vector_free( deltaS );
    gsl_vector_free( r_zs );

    std::cout << "    iterate 22" << std::endl;

  }
  GKG_CATCH( "void gkg::GSLConvexQuadraticProgrammingSolver::iterate()" );

}


bool gkg::GSLConvexQuadraticProgrammingSolver::testConvergence(
                                                 double epsilonGap,
                                                 double epsilonResiduals ) const
{

  try
  {

    if ( ( _gap <= epsilonGap ) &&
         ( _residualNorm <= ( epsilonResiduals * _minimizerDataNorm ) ) )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "bool gkg::GSLConvexQuadraticProgrammingSolver::testConvergence( "
             "double epsilonGap, "
             "double epsilonResiduals ) const" );

}


bool gkg::GSLConvexQuadraticProgrammingSolver::testInfeasibility(
                                                double epsilonInfeasible ) const
{

  try
  {

    if ( ( _quantityOfInfeasibility > epsilonInfeasible ) &&
         ( _quantityOfInfeasibility >
                   ( std::pow( 10.0, 4 ) * _minimumQuantityOfInfeasibility ) ) )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "bool "
             "gkg::GSLConvexQuadraticProgrammingSolver::testInfeasibility( "
             "double epsilonInfeasible ) const" );

}


double gkg::GSLConvexQuadraticProgrammingSolver::getStepLength(
                      const gsl_vector* deltaS, const gsl_vector* deltaZ ) const
{

  try
  {

    double alpha = 1.0;

    int32_t i = 0;
    for ( i = 0; i < _Mi; i++ )
    {

      if ( gsl_vector_get( deltaZ, i ) > 0.0 )
      {

        alpha = GSL_MIN_DBL( alpha, gsl_vector_get( _z, i ) /
                                    gsl_vector_get( deltaZ, i ) );

      }

      if ( gsl_vector_get( deltaS, i ) < 0.0 )
      {

        alpha = GSL_MIN_DBL( alpha, -gsl_vector_get( _s, i ) /
                                     gsl_vector_get( deltaS, i ) );

      }

    }

    return alpha;

  }
  GKG_CATCH( "double gkg::GSLConvexQuadraticProgrammingSolver::getStepLength( "
             "const gsl_vector* deltaS, const gsl_vector* deltaZ ) const" );
}


double gkg::GSLConvexQuadraticProgrammingSolver::getObjectiveFunction() const
{

  try
  {

    // the value of the objective at the point x is:
    // f = 0.5*(x^t)Qx+(q^t)x
    double f = 0.0;

    gsl_vector* tmp = gsl_vector_alloc( _N );

    gsl_blas_dcopy( _q, tmp );
    gsl_blas_dsymv( CblasUpper, 0.5, _Q, _x, 1.0, tmp );
    gsl_blas_ddot( _x, tmp, &f );
    gsl_vector_free(tmp);

    return f;

  }
  GKG_CATCH( "double gkg::GSLConvexQuadraticProgrammingSolver::"
             "getObjectiveFunction() const" );

}
