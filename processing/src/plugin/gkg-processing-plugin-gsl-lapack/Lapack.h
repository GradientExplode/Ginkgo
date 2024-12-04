#ifndef _gkg_processing_plugin_gsl_lapack_Lapack_h_
#define _gkg_processing_plugin_gsl_lapack_Lapack_h_


//  DGESVD computes the singular value decomposition (SVD) of a real M-by-N
//  matrix A, optionally computing the left and/or right singular vectors. The
//  SVD is written
//  
//       A = U * SIGMA * transpose(V)
//  
//  where SIGMA is an M-by-N matrix which is zero except for its min(m,n) diag-
//  onal elements, U is an M-by-M orthogonal matrix, and V is an N-by-N orthog-
//  onal matrix.  The diagonal elements of SIGMA are the singular values of A;
//  they are real and non-negative, and are returned in descending order.  The
//  first min(m,n) columns of U and V are the left and right singular vectors
//  of A.
//  
//  Note that the routine returns V**T, not V.
//  
//  ARGUMENTS:
//  ---------
//  JOBU    (input) CHARACTER*1
//          Specifies options for computing all or part of the matrix U:
//          = 'A':  all M columns of U are returned in array U:
//          = 'S':  the first min(m,n) columns of U (the left singular vectors)
//          are returned in the array U; = 'O':  the first min(m,n) columns of
//          U (the left singular vectors) are overwritten on the array A; =
//          'N':  no columns of U (no left singular vectors) are computed.
//  
//  JOBVT   (input) CHARACTER*1
//          Specifies options for computing all or part of the matrix V**T:
//          = 'A':  all N rows of V**T are returned in the array VT;
//          = 'S':  the first min(m,n) rows of V**T (the right singular vec-
//            tors) are returned in the array VT; = 'O':  the first min(m,n) rows
//            of V**T (the right singular vectors) are overwritten on the array
//            A; = 'N':  no rows of V**T (no right singular vectors) are com-
//            puted.
//  
//            JOBVT and JOBU cannot both be 'O'.
//  
//  M       (input) INTEGER
//          The number of rows of the input matrix A.  M >= 0.
//  
//  N       (input) INTEGER
//          The number of columns of the input matrix A.  N >= 0.
//  
//  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)
//          On entry, the M-by-N matrix A.  On exit, if JOBU = 'O',  A is
//          overwritten with the first min(m,n) columns of U (the left singular
//          vectors, stored columnwise); if JOBVT = 'O', A is overwritten with
//          the first min(m,n) rows of V**T (the right singular vectors, stored
//          rowwise); if JOBU .ne. 'O' and JOBVT .ne. 'O', the contents of A
//          are destroyed.
//  
//  LDA     (input) INTEGER
//          The leading dimension of the array A.  LDA >= max(1,M).
//  
//  S       (output) DOUBLE PRECISION array, dimension (min(M,N))
//          The singular values of A, sorted so that S(i) >= S(i+1).
//  
//  
//  
//  
//  U       (output) DOUBLE PRECISION array, dimension (LDU,UCOL)
//          (LDU,M) if JOBU = 'A' or (LDU,min(M,N)) if JOBU = 'S'.  If JOBU =
//          'A', U contains the M-by-M orthogonal matrix U; if JOBU = 'S', U
//          contains the first min(m,n) columns of U (the left singular vec-
//          tors, stored columnwise); if JOBU = 'N' or 'O', U is not refer-
//          enced.
//  
//  LDU     (input) INTEGER
//          The leading dimension of the array U.  LDU >= 1; if JOBU = 'S' or
//          'A', LDU >= M.
//  
//  VT      (output) DOUBLE PRECISION array, dimension (LDVT,N)
//          If JOBVT = 'A', VT contains the N-by-N orthogonal matrix V**T; if
//          JOBVT = 'S', VT contains the first min(m,n) rows of V**T (the right
//          singular vectors, stored rowwise); if JOBVT = 'N' or 'O', VT is not
//          referenced.
//  
//  LDVT    (input) INTEGER
//          The leading dimension of the array VT.  LDVT >= 1; if JOBVT = 'A',
//          LDVT >= N; if JOBVT = 'S', LDVT >= min(M,N).
//  
//  WORK    (workspace/output) DOUBLE PRECISION array, dimension (LWORK)
//          On exit, if INFO = 0, WORK(1) returns the optimal LWORK; if INFO >
//          0, WORK(2:MIN(M,N)) contains the unconverged superdiagonal elements
//          of an upper bidiagonal matrix B whose diagonal is in S (not neces-
//          sarily sorted). B satisfies A = U * B * VT, so it has the same
//          singular values as A, and singular vectors related by U and VT.
//  
//  LWORK   (input) INTEGER
//          The dimension of the array WORK. LWORK >= 1.  LWORK >=
//          MAX(3*MIN(M,N)+MAX(M,N),5*MIN(M,N)-4).  For good performance, LWORK
//          should generally be larger.
//  
//  INFO    (output) INTEGER
//          = 0:  successful exit.
//          < 0:  if INFO = -i, the i-th argument had an illegal value.
//          > 0:  if DBDSQR did not converge, INFO specifies how many superdi-
//          agonals of an intermediate bidiagonal form B did not converge to
//          zero. See the description of WORK above for details.

extern "C" 
void dgesvd_( char* jobu, char* jobvt,
              int32_t* m, int32_t* n,
              double* a, int32_t* lda,
              double* s,
              double* u, int32_t* ldu,
              double* vt, int32_t* ldvt,
              double* work, int32_t* lwork,
              int32_t* info );


//  ZGESVD computes the singular value decomposition (SVD) of a complex M-by-N
//  matrix A, optionally computing the left and/or right singular vectors. The
//  SVD is written
//  
//       A = U * SIGMA * conjugate-transpose(V)
//  
//  where SIGMA is an M-by-N matrix which is zero except for its min(m,n) diag-
//  onal elements, U is an M-by-M unitary matrix, and V is an N-by-N unitary
//  matrix.  The diagonal elements of SIGMA are the singular values of A; they
//  are real and non-negative, and are returned in descending order.  The first
//  min(m,n) columns of U and V are the left and right singular vectors of A.
//  
//  Note that the routine returns V**H, not V.
//  
//  ARGUMENTS:
//  ---------
//  JOBU    (input) CHARACTER*1
//          Specifies options for computing all or part of the matrix U:
//          = 'A':  all M columns of U are returned in array U:
//          = 'S':  the first min(m,n) columns of U (the left singular vectors)
//          are returned in the array U; = 'O':  the first min(m,n) columns of
//          U (the left singular vectors) are overwritten on the array A; =
//          'N':  no columns of U (no left singular vectors) are computed.
//  
//  JOBVT   (input) CHARACTER*1
//          Specifies options for computing all or part of the matrix V**H:
//          = 'A':  all N rows of V**H are returned in the array VT;
//          = 'S':  the first min(m,n) rows of V**H (the right singular vec-
//          tors) are returned in the array VT; = 'O':  the first min(m,n) rows
//          of V**H (the right singular vectors) are overwritten on the array
//          A; = 'N':  no rows of V**H (no right singular vectors) are com-
//          puted.
//  
//          JOBVT and JOBU cannot both be 'O'.
//
//  M       (input) INTEGER
//          The number of rows of the input matrix A.  M >= 0.
//  
//  N       (input) INTEGER
//          The number of columns of the input matrix A.  N >= 0.
//  
//  A       (input/output) COMPLEX*16 array, dimension (LDA,N)
//          On entry, the M-by-N matrix A.  On exit, if JOBU = 'O',  A is
//          overwritten with the first min(m,n) columns of U (the left singular
//          vectors, stored columnwise); if JOBVT = 'O', A is overwritten with
//          the first min(m,n) rows of V**H (the right singular vectors, stored
//          rowwise); if JOBU .ne. 'O' and JOBVT .ne. 'O', the contents of A
//          are destroyed.
//  
//  LDA     (input) INTEGER
//          The leading dimension of the array A.  LDA >= max(1,M).
//
//  S       (output) DOUBLE PRECISION array, dimension (min(M,N))
//          The singular values of A, sorted so that S(i) >= S(i+1).
//  
//  U       (output) COMPLEX*16 array, dimension (LDU,UCOL)
//          (LDU,M) if JOBU = 'A' or (LDU,min(M,N)) if JOBU = 'S'.  If JOBU =
//          'A', U contains the M-by-M unitary matrix U; if JOBU = 'S', U con-
//          tains the first min(m,n) columns of U (the left singular vectors,
//          stored columnwise); if JOBU = 'N' or 'O', U is not referenced.
//  
//  LDU     (input) INTEGER
//          The leading dimension of the array U.  LDU >= 1; if JOBU = 'S' or
//          'A', LDU >= M.
//  
//  VT      (output) COMPLEX*16 array, dimension (LDVT,N)
//          If JOBVT = 'A', VT contains the N-by-N unitary matrix V**H; if
//          JOBVT = 'S', VT contains the first min(m,n) rows of V**H (the right
//          singular vectors, stored rowwise); if JOBVT = 'N' or 'O', VT is not
//          referenced.
//  
//  LDVT    (input) INTEGER
//          The leading dimension of the array VT.  LDVT >= 1; if JOBVT = 'A',
//          LDVT >= N; if JOBVT = 'S', LDVT >= min(M,N).
//  
//  WORK    (workspace/output) COMPLEX*16 array, dimension (LWORK)
//          On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
//  
//  LWORK   (input) INTEGER
//          The dimension of the array WORK. LWORK >= 1.  LWORK >=
//          2*MIN(M,N)+MAX(M,N).  For good performance, LWORK should generally
//          be larger.
//  
//  RWORK   (workspace) DOUBLE PRECISION array, dimension (5*max(M,N))
//          On exit, if INFO > 0, RWORK(1:MIN(M,N)-1) contains the unconverged
//          superdiagonal elements of an upper bidiagonal matrix B whose diago-
//          nal is in S (not necessarily sorted).  B satisfies A = U * B * VT,
//          so it has the same singular values as A, and singular vectors
//          related by U and VT.
//  
//  INFO    (output) INTEGER
//          = 0:  successful exit.
//          < 0:  if INFO = -i, the i-th argument had an illegal value.
//          > 0:  if ZBDSQR did not converge, INFO specifies how many superdi-
//        agonals of an intermediate bidiagonal form B did not converge to
//        zero. See the description of RWORK above for details.

extern "C"
void zgesvd_( char* jobu, char* jobvt,
              int32_t* m, int32_t* n,
              double* a, int32_t* lda,
              double* s,
              double* u, int32_t* ldu,
              double* vt, int32_t* ldvt,
              double* cwork, int32_t* lwork,
              double* rwork,
              int32_t* info );


//  ZGGSVD computes the generalized singular value decomposition (GSVD) of the
//  M-by-N complex matrix A and P-by-N complex matrix B:
//
//        U'*A*Q = D1*( 0 R ),    V'*B*Q = D2*( 0 R )               (1)
//
//  where U, V and Q are unitary matrices, R is an upper triangular matrix, and
//  Z' means the conjugate transpose of Z.  Let K+L = the numerical effective
//  rank of the matrix (A',B')', then D1 and D2 are M-by-(K+L) and P-by-(K+L)
//  "diagonal" matrices and of the following structures, respectively:
//
//  If M-K-L >= 0,
//
//     U'*A*Q = D1*( 0 R )
//
//            = K     ( I  0 ) * (  0   R11  R12 ) K
//              L     ( 0  C )   (  0    0   R22 ) L
//              M-K-L ( 0  0 )    N-K-L  K    L
//                      K  L
//
//     V'*B*Q = D2*( 0 R )
//
//            = L     ( 0  S ) * (  0   R11  R12 ) K
//              P-L   ( 0  0 )   (  0    0   R22 ) L
//                      K  L      N-K-L  K    L
//  where
//
//    C = diag( ALPHA(K+1), ... , ALPHA(K+L) ),
//    S = diag( BETA(K+1),  ... , BETA(K+L) ), C**2 + S**2 = I.
//    The nonsingular triangular matrix R = ( R11 R12 ) is stored
//                                          (  0  R22 )
//    in A(1:K+L,N-K-L+1:N) on exit.
//
//  If M-K-L < 0,
//
//     U'*A*Q = D1*( 0 R )
//
//            = K   ( I  0    0   ) * ( 0    R11  R12  R13  ) K
//              M-K ( 0  C    0   )   ( 0     0   R22  R23  ) M-K
//                    K M-K K+L-M     ( 0     0    0   R33  ) K+L-M
//                                     N-K-L  K   M-K  K+L-M
//
//     V'*B*Q = D2*( 0 R )
//
//            = M-K   ( 0  S    0   ) * ( 0    R11  R12  R13  ) K
//              K+L-M ( 0  0    I   )   ( 0     0   R22  R23  ) M-K
//              P-L   ( 0  0    0   )   ( 0     0    0   R33  ) K+L-M
//                      K M-K K+L-M      N-K-L  K   M-K  K+L-M where
//
//    C = diag( ALPHA(K+1), ... , ALPHA(M) ),
//    S = diag( BETA(K+1),  ... , BETA(M) ), C**2 + S**2 = I.
//    R = ( R11 R12 R13 ) is a nonsingular upper triangular matrix,
//        (  0  R22 R23 )
//        (  0   0  R33 )
//    (R11 R12 R13 ) is stored in A(1:M, N-K-L+1:N), and R33 is stored
//    ( 0  R22 R23 )
//    in B(M-K+1:L,N+M-K-L+1:N) on exit.
//
//  The routine computes C, S, R, and optionally the unitary
//  transformation matrices U, V and Q.
//
//  In particular, if B is an N-by-N nonsingular matrix, then the GSVD of A and
//  B implicitly gives the SVD of the matrix A*inv(B):
//                       A*inv(B) = U*(D1*inv(D2))*V'.
//  If ( A',B')' has orthnormal columns, then the GSVD of A and B is also equal
//  to the CS decomposition of A and B. Furthermore, the GSVD can be used to
//  derive the solution of the eigenvalue problem:
//                       A'*A x = lambda* B'*B x.
//  In some literature, the GSVD of A and B is presented in the form
//                   U'*A*X = ( 0 D1 ),   V'*B*X = ( 0 D2 )          (2) where
//  U and V are orthogonal and X is nonsingular, and D1 and D2 are ``diago-
//  nal''.  It is easy to see that the GSVD form (1) can be converted to the
//  form (2) by taking the nonsingular matrix X as
//
//                        X = Q*(  I   0    )
//                              (  0 inv(R) )
//
//  ARGUMENTS:
//  ---------
//  JOBU    (input) CHARACTER*1
//          = 'U':  Unitary matrix U is computed;
//          = 'N':  U is not computed.
//
//  JOBV    (input) CHARACTER*1
//          = 'V':  Unitary matrix V is computed;
//          = 'N':  V is not computed.
//
//  JOBQ    (input) CHARACTER*1
//          = 'Q':  Unitary matrix Q is computed;
//          = 'N':  Q is not computed.
//
//  M       (input) INTEGER
//          The number of rows of the matrix A.  M >= 0.
//
//  N       (input) INTEGER
//          The number of columns of the matrices A and B.  N >= 0.
//
//  P       (input) INTEGER
//          The number of rows of the matrix B.  P >= 0.
//
//  K       (output) INTEGER
//          L       (output) INTEGER On exit, K and L specify the dimension of
//          the subblocks described in Purpose.  K + L = effective numerical
//          rank of (A',B')'.
//
//  A       (input/output) COMPLEX*16 array, dimension (LDA,N)
//          On entry, the M-by-N matrix A.  On exit, A contains the triangular
//          matrix R, or part of R.  See Purpose for details.
//
//  LDA     (input) INTEGER
//          The leading dimension of the array A. LDA >= max(1,M).
//
//  B       (input/output) COMPLEX*16 array, dimension (LDB,N)
//          On entry, the P-by-N matrix B.  On exit, B contains part of the
//          triangular matrix R if M-K-L < 0.  See Purpose for details.
//
//  LDB     (input) INTEGER
//          The leading dimension of the array B. LDB >= max(1,P).
//
//  ALPHA   (output) DOUBLE PRECISION array, dimension (N)
//          BETA    (output) DOUBLE PRECISION array, dimension (N) On exit,
//          ALPHA and BETA contain the generalized singular value pairs of A
//          and B; if M-K-L >= 0, ALPHA(1:K) = ONE,  ALPHA(K+1:K+L) = C,
//          BETA(1:K)  = ZERO, BETA(K+1:K+L)  = S; or if M-K-L < 0, ALPHA(1:K)=
//          ONE,  ALPHA(K+1:M)= C, ALPHA(M+1:K+L)= ZERO,
//          BETA(1:K) = ZERO, BETA(K+1:M) = S, BETA(M+1:K+L) = ONE.  and
//          ALPHA(K+L+1:N) = ZERO
//          BETA(K+L+1:N)  = ZERO
//
//  U       (output) COMPLEX*16 array, dimension (LDU,M)
//          If JOBU = 'U', U contains the M-by-M unitary matrix U.  If JOBU =
//          'N', U is not referenced.
//
//  LDU     (input) INTEGER
//          The leading dimension of the array U. LDU >= max(1,M).
//
//  V       (output) COMPLEX*16 array, dimension (LDV,P)
//          If JOBU = 'V', V contains the P-by-P unitary matrix V.  If JOBV =
//          'N', V is not referenced.
//
//  LDV     (input) INTEGER
//          The leading dimension of the array V. LDV >= max(1,P).
//
//  Q       (output) COMPLEX*16 array, dimension (LDQ,N)
//          If JOBU = 'Q', Q contains the N-by-N unitary matrix Q.  If JOBQ =
//          'N', Q is not referenced.
//
//  LDQ     (input) INTEGER
//          The leading dimension of the array Q. LDQ >= max(1,N).
//
//  WORK    (workspace) COMPLEX*16 array, dimension (MAX(3*N,M,P)+N)
//
//  RWORK   (workspace) DOUBLE PRECISION array, dimension (2*N)
//
//  IWORK   (workspace) INTEGER array, dimension (N)
//
//  INFO    (output)INTEGER
//          = 0:  successful exit.
//          < 0:  if INFO = -i, the i-th argument had an illegal value.
//          > 0:  if INFO = 1, the Jacobi-type procedure failed to converge.
//          For further details, see subroutine ZTGSJA.
//
//  PARAMETERS:
//  ----------
//  TOLA    DOUBLE PRECISION
//          TOLB    DOUBLE PRECISION TOLA and TOLB are the thresholds to deter-
//          mine the effective rank of (A',B')'. Generally, they are set to
//          TOLA = MAX(M,N)*norm(A)*MAZHEPS, TOLB = MAX(P,N)*norm(B)*MAZHEPS.
//          The size of TOLA and TOLB may affect the size of backward errors of
//          the decomposition.

extern "C"
int32_t zggsvd_( char* jobu, char* jobv, char* jobq,
                 int32_t* m, int32_t* n, int32_t* p, int32_t* k, int32_t* l,
                 double* a, int32_t* lda,
                 double* b, int32_t* ldb,
                 double* alpha, 
                 double* beta,
                 double* u, int32_t* ldu,
                 double* v, int32_t* ldv,
                 double* q, int32_t* ldq,
                 double* work, 
                 double* rwork, int32_t* iwork,
                 int32_t* info );

//  DGESV computes the solution to a real system of linear equations   
//     A * X = B,   
//  where A is an N-by-N matrix and X and B are N-by-NRHS matrices.   
//
//  The LU decomposition with partial pivoting and row interchanges is   
//  used to factor A as   
//     A = P * L * U,   
//  where P is a permutation matrix, L is unit lower triangular, and U is   
//  upper triangular.  The factored form of A is then used to solve the   
//  system of equations A * X = B.   
//
//  ARGUMENTS:   
//  ---------   
//
//  N       (input) INTEGER   
//          The number of linear equations, i.e., the order of the   
//          matrix A.  N >= 0.   
//
//  NRHS    (input) INTEGER   
//          The number of right hand sides, i.e., the number of columns   
//          of the matrix B.  NRHS >= 0.   
//
//  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N)   
//          On entry, the N-by-N coefficient matrix A.   
//          On exit, the factors L and U from the factorization   
//          A = P*L*U; the unit diagonal elements of L are not stored.   
//
//  LDA     (input) INTEGER   
//          The leading dimension of the array A.  LDA >= max(1,N).   
//
//  IPIV    (output) INTEGER array, dimension (N)   
//          The pivot indices that define the permutation matrix P;   
//          row i of the matrix was interchanged with row IPIV(i).   
//
//  B       (input/output) DOUBLE PRECISION array, dimension (LDB,NRHS)   
//          On entry, the N-by-NRHS matrix of right hand side matrix B.   
//          On exit, if INFO = 0, the N-by-NRHS solution matrix X.   
//
//  LDB     (input) INTEGER   
//          The leading dimension of the array B.  LDB >= max(1,N).   
//
//  INFO    (output) INTEGER   
//          = 0:  successful exit   
//          < 0:  if INFO = -i, the i-th argument had an illegal value   
//          > 0:  if INFO = i, U(i,i) is exactly zero.  The factorization   
//                has been completed, but the factor U is exactly   
//                singular, so the solution could not be computed.   

extern "C"
void dgesv_( int32_t* n, int32_t* nrhs, 
             double* a, int32_t* lda, int32_t* ipiv,
             double* b, int32_t* ldb, int32_t* info );


//   DSYTRD reduces a real symmetric matrix A to real symmetric
//   tridiagonal form T by an orthogonal similarity transformation:
//   Q**T * A * Q = T.
// 
// 
//   Arguments:
//   ==========
// 
//   [in]UPLO
//            UPLO is CHARACTER*1
//            = 'U':  Upper triangle of A is stored;
//            = 'L':  Lower triangle of A is stored.
// 
//   [in]N
//            N is INTEGER
//            The order of the matrix A.  N >= 0.
// 
//   [in,out] A
//            A is DOUBLE PRECISION array, dimension (LDA,N)
//            On entry, the symmetric matrix A.  If UPLO = 'U', the leading
//            N-by-N upper triangular part of A contains the upper
//            triangular part of the matrix A, and the strictly lower
//            triangular part of A is not referenced.  If UPLO = 'L', the
//            leading N-by-N lower triangular part of A contains the lower
//            triangular part of the matrix A, and the strictly upper
//            triangular part of A is not referenced.
//            On exit, if UPLO = 'U', the diagonal and first superdiagonal
//            of A are overwritten by the corresponding elements of the
//            tridiagonal matrix T, and the elements above the first
//            superdiagonal, with the array TAU, represent the orthogonal
//            matrix Q as a product of elementary reflectors; if UPLO
//            = 'L', the diagonal and first subdiagonal of A are over-
//            written by the corresponding elements of the tridiagonal
//            matrix T, and the elements below the first subdiagonal, with
//            the array TAU, represent the orthogonal matrix Q as a product
//            of elementary reflectors. See Further Details.
// 
//   [in] LDA
//            LDA is INTEGER
//            The leading dimension of the array A.  LDA >= max(1,N).
// 
//   [out] D
//            D is DOUBLE PRECISION array, dimension (N)
//            The diagonal elements of the tridiagonal matrix T:
//            D(i) = A(i,i).
// 
//   [out] E
//            E is DOUBLE PRECISION array, dimension (N-1)
//            The off-diagonal elements of the tridiagonal matrix T:
//            E(i) = A(i,i+1) if UPLO = 'U', E(i) = A(i+1,i) if UPLO = 'L'.
// 
//   [out] TAU
//            TAU is DOUBLE PRECISION array, dimension (N-1)
//            The scalar factors of the elementary reflectors (see Further
//            Details).
// 
//   [out] WORK
//            WORK is DOUBLE PRECISION array, dimension (MAX(1,LWORK))
//            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.
// 
//   [in] LWORK
//            LWORK is INTEGER
//            The dimension of the array WORK.  LWORK >= 1.
//            For optimum performance LWORK >= N*NB, where NB is the
//            optimal blocksize.
// 
//            If LWORK = -1, then a workspace query is assumed; the routine
//            only calculates the optimal size of the WORK array, returns
//            this value as the first entry of the WORK array, and no error
//            message related to LWORK is issued by XERBLA.
// 
//   [out] INFO
//            INFO is INTEGER
//            = 0:  successful exit
//            < 0:  if INFO = -i, the i-th argument had an illegal value


extern "C"
int32_t dsytrd_( char* UPLO,
                 int32_t* N,
                 double* A,
                 int32_t* LDA,
                 double* D,
                 double* E,
                 double* TAU,
                 double* WORK,
                 int32_t* LWORK,
                 int32_t* INFO );


//   DSTERF computes all eigenvalues of a symmetric tridiagonal matrix
//   using the Pal-Walker-Kahan variant of the QL or QR algorithm.
// 
//   Arguments:
//   ==========
// 
//   [in] N
//            N is INTEGER
//            The order of the matrix.  N >= 0.
// 
//   [in,out] D
//            D is DOUBLE PRECISION array, dimension (N)
//            On entry, the n diagonal elements of the tridiagonal matrix.
//            On exit, if INFO = 0, the eigenvalues in ascending order.
// 
//   [in,out] E
//            E is DOUBLE PRECISION array, dimension (N-1)
//            On entry, the (n-1) subdiagonal elements of the tridiagonal
//            matrix.
//            On exit, E has been destroyed.
// 
//   [out] INFO
//            INFO is INTEGER
//            = 0:  successful exit
//            < 0:  if INFO = -i, the i-th argument had an illegal value
//            > 0:  the algorithm failed to find all of the eigenvalues in
//                  a total of 30*N iterations; if INFO = i, then i
//                  elements of E have not converged to zero.

extern "C"
int32_t dsterf_( int32_t* N,
                 double* D,
                 double* E,
                 int32_t* INFO );

#endif
