#ifndef _gkg_processing_numericalanalysis_PowellNewUOA_i_h_
#define _gkg_processing_numericalanalysis_PowellNewUOA_i_h_


inline double gkg::NumericalAnalysisImplementationFactory::newuoa(
                                          int32_t n,
                                          int32_t npt,
                                          double* x,
                                          double rhobeg,
                                          double rhoend,
                                          int32_t* ret_nf,
                                          int32_t maxfun,
                                          double* w,
                                          const gkg::PowellNewUOAFunction& func,
                                          bool verbose ) const
{

  try
  {

    // This subroutine seeks the least value of a function of many
    // variables, by a trust region method that forms quadratic models
    // by interpolation. There can be some freedom in the interpolation
    // conditions, which is taken up by minimizing the Frobenius norm of
    // the change to the second derivative of the quadratic model,
    // beginning with a zero matrix. The arguments of the subroutine are
    //  as follows.

    // N must be set to the number of variables and must be at least
    // two. NPT is the number of interpolation conditions. Its value
    // must be in the interval [N+2,(N+1)(N+2)/2]. Initial values of the
    // variables must be set in X(1),X(2),...,X(N). They will be changed
    // to the values that give the least calculated F. RHOBEG and RHOEND
    // must be set to the initial and final values of a trust region
    // radius, so both must be positive with RHOEND<=RHOBEG. Typically
    // RHOBEG should be about one tenth of the greatest expected change
    // to a variable, and RHOEND should indicate the accuracy that is
    // required in the final values of the variables. MAXFUN must be set
    // to an upper bound on the number of calls of CALFUN.  The array W
    // will be used for working space. Its length must be at least
    // (NPT+13)*(NPT+N)+3*N*(N+3)/2.

    // SUBROUTINE CALFUN (N,X,F) must be provided by the user. It must
    // set F to the value of the objective function for the variables
    // X(1),X(2),...,X(N). Partition the working space array, so that
    // different parts of it can be treated separately by the subroutine
    // that performs the main calculation.

    int32_t id, np, iw, igq, ihq, ixb, ifv, ipq, ivl, ixn, ixo, ixp, ndim, nptm,
            ibmat, izmat;

    // parameter adjustments
    --w;
    --x;
    // gkg::NewUoaFunctiontion Body
    np = n + 1;
    nptm = npt - np;
    if ( npt < n + 2 || npt > ( n + 2 ) * np / 2 )
    {

      throw std::runtime_error( "NPT is not in the required interval" );
      return 1;

    }
    ndim = npt + n;
    ixb = 1;
    ixo = ixb + n;
    ixn = ixo + n;
    ixp = ixn + n;
    ifv = ixp + n * npt;
    igq = ifv + npt;
    ihq = igq + n;
    ipq = ihq + n * np / 2;
    ibmat = ipq + npt;
    izmat = ibmat + ndim * n;
    id = izmat + npt * nptm;
    ivl = id + n;
    iw = ivl + ndim;

    // The above settings provide a partition of W for subroutine
    // NEWUOB. The partition requires the first NPT*(NPT+N)+5*N*(N+3)/2
    // elements of W plus the space that is needed by the last array of
    // NEWUOB.
    return newuob( n,
                   npt,
                   &x[ 1 ], 
                   rhobeg,
                   rhoend,
                   ret_nf,
                   maxfun,
                   &w[ ixb ],
                   &w[ ixo ],
                   &w[ ixn ],
                   &w[ ixp ],
                   &w[ ifv ],
                   &w[ igq ],
                   &w[ ihq ],
                   &w[ ipq ],
                   &w[ ibmat ],
                   &w[ izmat ],
                   &ndim,
                   &w[ id ],
                   &w[ ivl ],
                   &w[ iw ],
                   func,
                   verbose );
  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::newuoa( "
             "int32_t n, "
             "int32_t npt, "
             "double* x, "
             "double rhobeg, "
             "double rhoend, "
             "int32_t* ret_nf, "
             "int32_t maxfun, "
             "double* w, "
             "const gkg::PowellNewUOAFunction& func, "
             "bool verbose ) const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::newuob(
                                          int32_t n,
                                          int32_t npt,
                                          double *x,
                                          double rhobeg,
                                           double rhoend,
                                          int32_t *ret_nf,
                                          int32_t maxfun,
                                          double *xbase,
                                          double *xopt,
                                          double *xnew,
                                          double *xpt,
                                          double *fval,
                                          double *gq,
                                          double *hq,
                                          double *pq,
                                          double *bmat,
                                          double *zmat,
                                          int32_t *ndim,
                                          double *d__,
                                          double *vlag,
                                          double *w,
                                          const gkg::PowellNewUOAFunction& func,
                                          bool verbose ) const
{

  try
  {

    // XBASE will hold a shift of origin that contributions from rounding errors
    // to values of the model and Lagrange functions.
    // XOPT will be set to the displacement from XBASE of the vector of
    // variables that provides the least calculated F so far.
    // XNEW will be set to the displacement from XBASE of the vector of
    // variables for the current calculation of F.
    // XPT will contain the interpolation point coordinates relative to XBASE.
    // FVAL will hold the values of F at the interpolation points.
    // GQ will hold the gradient of the quadratic model at XBASE.
    // HQ will hold the explicit second derivatives of the quadratic model.
    // PQ will contain the parameters of the implicit second derivatives
    // of the quadratic model.
    // BMAT will hold the last N columns of H.
    // ZMAT will hold the factorization of the leading NPT by NPT submatrix of 
    // H, this factorization being ZMAT times Diag(DZ) times ZMAT^T, where the 
    // elements of DZ are plus or minus 1.0, as specified by IDZ.
    // NDIM is the first dimension of BMAT and has the value NPT+N.
    // D is reserved for trial steps from XOPT.
    // VLAG will contain the values of the Lagrange functions at a new
    // point X.  They are part of a product that requires VLAG to be of
    // length NDIM.
    // The array W will be used for working space. Its length must be at
    // least 10*NDIM = 10*(NPT+N). Set some constants.

    int32_t xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
            zmat_offset, i__1, i__2, i__3 = 0, i__ = 0, j, k, ih, nf, nh,
            ip = 0, jp = 0, np, nfm = 0, idz = 0, ipt, jpt, nfmm = 0, knew = 0,
            kopt = 0, nptm, ksave = 0, nfsav, itemp = 0, ktemp = 0, itest,
            nftest;
    double d__1 = 0.0, d__2 = 0.0, d__3 = 0.0, f, dx = 0.0, dsq = 0.0, rho,
           sum = 0.0, fbeg, diff = 0.0, beta = 0.0, gisq = 0.0, temp = 0.0,
           suma = 0.0, sumb = 0.0, fopt, bsum = 0.0, gqsq = 0.0, xipt, xjpt,
           sumz = 0.0, diffa, diffb, diffc, hdiag = 0.0, alpha, delta = 0.0,
           recip, reciq, fsave = 0.0, dnorm, ratio, dstep, vquad = 0.0,
           tempq = 0.0, rhosq, detrat = 0.0, crvmin = 0.0, distsq = 0.0, xoptsq;

    // parameter adjustments
    diffc = 0;
    ratio = 0;
    dnorm = 0;
    nfsav = 0;
    diffa = 0;
    diffb = 0;
    xoptsq = 0;
    f = 0;
    rho = fbeg = fopt = xjpt = xipt = 0.0;
    itest = ipt = jpt = 0;
    alpha = dstep = 0.0;
    zmat_dim1 = npt;
    zmat_offset = 1 + zmat_dim1;
    zmat -= zmat_offset;
    xpt_dim1 = npt;
    xpt_offset = 1 + xpt_dim1;
    xpt -= xpt_offset;
    --x;
    --xbase;
    --xopt;
    --xnew;
    --fval;
    --gq;
    --hq;
    --pq;
    bmat_dim1 = *ndim;
    bmat_offset = 1 + bmat_dim1;
    bmat -= bmat_offset;
    --d__;
    --vlag;
    --w;

    // gkg::PowellNewUOAFunction Body
    np = n + 1;
    nh = n * np / 2;
    nptm = npt - np;
    nftest = ( maxfun > 1 ) ? maxfun : 1;

    // Set the initial elements of XPT, BMAT, HQ, PQ and ZMAT to 0.
    i__1 = n;
    for ( j = 1; j <= i__1; ++j )
    {

      xbase[ j ] = x[ j ];
      i__2 = npt;
      for ( k = 1; k <= i__2; ++k )
      {

        xpt[ k + j * xpt_dim1 ] = 0;

      }
      i__2 = *ndim;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        bmat[ i__ + j * bmat_dim1 ] = 0;

      }

    }
    i__2 = nh;
    for ( ih = 1; ih <= i__2; ++ih )
    {

      hq[ ih ] = 0;

    }
    i__2 = npt;
    for ( k = 1; k <= i__2; ++k )
    {

      pq[ k ] = 0;
      i__1 = nptm;
      for ( j = 1; j <= i__1; ++j )
      {

        zmat[ k + j * zmat_dim1 ] = 0;

      }

    }

    // Begin the initialization procedure. NF becomes 1.0 more than the
    // number of function values so far. The coordinates of the
    // displacement of the next initial interpolation point from XBASE
    // are set in XPT(NF,.).
    rhosq = rhobeg * rhobeg;
    recip = 1.0 / rhosq;
    reciq = sqrt( 0.5 ) / rhosq;
    nf = 0;

    L50L70( n, npt, x, rhobeg, rhoend, ret_nf,
            maxfun, xbase, xopt, xnew, xpt,
            fval, gq, hq, pq, bmat, zmat,
            ndim, d__, vlag, w, func, verbose,
            xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1,
            zmat_offset,
            i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
            jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
            nftest,
            d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
            temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
            diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio,
            dstep, vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );

    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::newuob( "
             "int32_t n, "
             "int32_t npt, "
             "double *x, "
             "double rhobeg, "
             "double rhoend, "
             "int32_t *ret_nf, "
             "int32_t maxfun, "
             "double *xbase, "
             "double *xopt, "
             "double *xnew, "
             "double *xpt, "
             "double *fval, "
             "double *gq, "
             "double *hq, "
             "double *pq, "
             "double *bmat, "
             "double *zmat, "
             "int32_t *ndim, "
             "double *d__, "
             "double *vlag, "
             "double *w, "
             "const gkg::PowellNewUOAFunction& func, "
             "bool verbose ) const" );

}


inline int32_t gkg::NumericalAnalysisImplementationFactory::biglag( 
                                  int32_t n,
                                  int32_t npt,
                                  double* xopt,
                                  double* xpt,
                                  double* bmat,
                                  double* zmat,
                                  int32_t* idz,
                                  int32_t* ndim,
                                  int32_t* knew,
                                  double* delta,
                                  double* d__,
                                  double* alpha,
                                  double* hcol,
                                  double* gc,
                                  double* gd,
                                  double* s,
                                  double* w,
                                  const PowellNewUOAFunction& /* func */ ) const
{

  try
  {

    // N is the number of variables. NPT is the number of interpolation
    // equations. XOPT is the best interpolation point so far. XPT
    // contains the coordinates of the current interpolation
    // points. BMAT provides the last N columns of H.  ZMAT and IDZ give
    // a factorization of the first NPT by NPT submatrix of H. NDIM is
    // the first dimension of BMAT and has the value NPT+N.  KNEW is the
    // index of the interpolation point that is going to be moved. DEBLLTA
    // is the current trust region bound. D will be set to the step from
    // XOPT to the new point. ABLLPHA will be set to the KNEW-th diagonal
    // element of the H matrix. HCOBLL, GC, GD, S and W will be used for
    // working space. 
    // The step D is calculated in a way that attempts to maximize the
    // modulus of BLLFUNC(XOPT+D), subject to the bound ||D|| .BLLE. DEBLLTA,
    // where BLLFUNC is the KNEW-th BLLagrange function. 

    int32_t xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1,
            zmat_offset, i__1, i__2, i__, j, k, iu, nptm, iterc, isave;
    double sp, ss, cf1, cf2, cf3, cf4, cf5, dhd, cth, tau, sth, sum, temp, step,
           angle, scale, denom, delsq, tempa, tempb, twopi, taubeg, tauold,
           taumax, d__1, dd, gg;

    // Parameter adjustments
    tempa = tempb = 0.0;
    zmat_dim1 = npt;
    zmat_offset = 1 + zmat_dim1;
    zmat -= zmat_offset;
    xpt_dim1 = npt;
    xpt_offset = 1 + xpt_dim1;
    xpt -= xpt_offset;
    --xopt;
    bmat_dim1 = *ndim;
    bmat_offset = 1 + bmat_dim1;
    bmat -= bmat_offset;
    --d__;
    --hcol;
    --gc;
    --gd;
    --s;
    --w;

    twopi = 2.0 * M_PI;
    delsq = *delta * *delta;
    nptm = npt - n - 1;
    // set the first NPT components of HCOBLL to the leading elements of
    // the KNEW-th column of H. 
    iterc = 0;
    i__1 = npt;
    for ( k = 1; k <= i__1; ++k )
    {

      hcol[ k ] = 0;

    }
    i__1 = nptm;
    for ( j = 1; j <= i__1; ++j )
    {

      temp = zmat[ *knew + j * zmat_dim1 ];
      if ( j < *idz )
      {

        temp = -temp;

      }
      i__2 = npt;
      for ( k = 1; k <= i__2; ++k )
      {

        hcol[ k ] += temp * zmat[ k + j * zmat_dim1 ];

      }

    }
    *alpha = hcol[ *knew ];
    // Set the unscaled initial direction D. Form the gradient of BLLFUNC
    // atXOPT, and multiply D by the second derivative matrix of
    // BLLFUNC. 
    dd = 0;
    i__2 = n;
    for ( i__ = 1; i__ <= i__2; ++i__ )
    {

      d__[ i__ ] = xpt[ *knew + i__ * xpt_dim1 ] - xopt[ i__ ];
      gc[ i__ ] = bmat[ *knew + i__ * bmat_dim1 ];
      gd[ i__ ] = 0;
      // Computing 2nd power
      d__1 = d__[ i__ ];
      dd += d__1 * d__1;

    }
    i__2 = npt;
    for ( k = 1; k <= i__2; ++k )
    {

      temp = 0;
      sum = 0;
      i__1 = n;
      for ( j = 1; j <= i__1; ++j )
      {

        temp += xpt[ k + j * xpt_dim1 ] * xopt[ j ];
        sum += xpt[ k + j * xpt_dim1 ] * d__[ j ];

      }
      temp = hcol[ k ] * temp;
      sum = hcol[ k ] * sum;
      i__1 = n;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        gc[ i__ ] += temp * xpt[ k + i__ * xpt_dim1 ];
        gd[ i__ ] += sum * xpt[ k + i__ * xpt_dim1 ];

      }

    }
    // scale D and GD, with a sign change if required. Set S to another
    // vector in the initial two dimensional subspace.
    gg = sp = dhd = 0;
    i__1 = n;
    for ( i__ = 1; i__ <= i__1; ++i__ )
    {

      // computing 2nd power
      d__1 = gc[ i__ ];
      gg += d__1 * d__1;
      sp += d__[ i__ ] * gc[ i__ ];
      dhd += d__[ i__ ] * gd[ i__ ];

    }
    scale = *delta / sqrt( dd );
    if ( sp * dhd < 0 )
    {

      scale = -scale;

    }
    temp = 0;
    if ( sp * sp > dd * .99 * gg )
    {
  
      temp = 1.0;

    }
    tau = scale * ( fabs( sp ) + 0.5 * scale * fabs( dhd ) );
    if ( gg * delsq < tau * .01 * tau )
    {

      temp = 1.0;

    }
    i__1 = n;
    for ( i__ = 1; i__ <= i__1; ++i__ )
    {

      d__[ i__ ] = scale * d__[ i__ ];
      gd[ i__ ] = scale * gd[ i__ ];
      s[ i__ ] = gc[ i__ ] + temp * gd[ i__ ];

    }
    // Begin the iteration by overwriting S with a vector that has the
    // required length and direction, except that termination occurs if
    // the given D and S are nearly parallel.
    for ( iterc = 0; iterc != n; ++iterc )
    {

      dd = sp = ss = 0;
      i__1 = n;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        // computing 2nd power
        d__1 = d__[ i__ ];
        dd += d__1 * d__1;
        sp += d__[ i__ ] * s[ i__ ];
        // computing 2nd power 
        d__1 = s[ i__ ];
        ss += d__1 * d__1;

      }
      temp = dd * ss - sp * sp;
      if ( temp <= dd * 1e-8 * ss )
      {

        return 0;

      }
      denom = sqrt( temp );
      i__1 = n;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        s[ i__ ] = ( dd * s[ i__ ] - sp * d__[ i__ ] ) / denom;
        w[ i__ ] = 0;

      }
      // calculate the coefficients of the objective function on the
      // circle, beginning with the multiplication of S by the second
      // derivative matrix.
      i__1 = npt;
      for ( k = 1; k <= i__1; ++k )
      {

        sum = 0;
        i__2 = n;
        for ( j = 1; j <= i__2; ++j )
        {

          sum += xpt[ k + j * xpt_dim1 ] * s[ j ];

        }
        sum = hcol[ k ] * sum;
        i__2 = n;
        for ( i__ = 1; i__ <= i__2; ++i__ )
        {

          w[ i__ ] += sum * xpt[ k + i__ * xpt_dim1 ];

        }

      }
      cf1 = cf2 = cf3 = cf4 = cf5 = 0;
      i__2 = n;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        cf1 += s[ i__ ] * w[ i__ ];
        cf2 += d__[ i__ ] * gc[ i__ ];
        cf3 += s[ i__ ] * gc[ i__ ];
        cf4 += d__[ i__ ] * gd[ i__ ];
        cf5 += s[ i__ ] * gd[ i__ ];

      }
      cf1 = 0.5 * cf1;
      cf4 = 0.5 * cf4 - cf1;
      // Seek the value of the angle that maximizes the modulus of TAU.
      taubeg = cf1 + cf2 + cf4;
      taumax = tauold = taubeg;
      isave = 0;
      iu = 49;
      temp = twopi / (double) ( iu + 1 );
      i__2 = iu;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        angle = (double) i__ * temp;
        cth = cos( angle );
        sth = sin( angle );
        tau = cf1 + ( cf2 + cf4 * cth ) * cth + ( cf3 + cf5 * cth ) * sth;
        if ( fabs( tau ) > fabs( taumax ) )
        {

          taumax = tau;
          isave = i__;
          tempa = tauold;

        }
        else if ( i__ == isave + 1 )
        {
  
          tempb = tau;

        }
        tauold = tau;

      }
      if ( isave == 0 )
      {
  
        tempa = tau;

      }
      if ( isave == iu )
      {

        tempb = taubeg;

      }
      step = 0;
      if ( tempa != tempb )
      {

        tempa -= taumax;
        tempb -= taumax;
        step = 0.5 * ( tempa - tempb ) / ( tempa + tempb );

      }
      angle = temp * ( (double) isave + step );
      // calculate the new D and GD. Then test for convergence.
      cth = cos( angle );
      sth = sin( angle );
      tau = cf1 + ( cf2 + cf4 * cth ) * cth + ( cf3 + cf5 * cth ) * sth;
      i__2 = n;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        d__[ i__ ] = cth * d__[ i__ ] + sth * s[ i__ ];
        gd[ i__ ] = cth * gd[ i__ ] + sth * w[ i__ ];
        s[ i__ ] = gc[ i__ ] + gd[ i__ ];

      }
      if ( fabs( tau ) <= fabs( taubeg ) * 1.1 )
      {

        return 0;

      }

    }

    return 0;

  }
  GKG_CATCH( "inline "
             "int32_t gkg::NumericalAnalysisImplementationFactory::biglag(" 
             "int32_t n, "
             "int32_t npt, "
             "double* xopt, "
             "double* xpt, "
             "double* bmat, "
             "double* zmat, "
             "int32_t* idz, "
             "int32_t* ndim, "
             "int32_t* knew, "
             "double* delta, "
             "double* d__, "
             "double* alpha, "
             "double* hcol, "
             "double* gc, "
             "double* gd, "
             "double* s, "
             "double* w, "
             "const PowellNewUOAFunction& func ) const" );

}


inline int32_t gkg::NumericalAnalysisImplementationFactory::bigden( 
                                                            int32_t n, 
                                                            int32_t npt,
                                                            double* xopt,
                                                            double* xpt,
                                                            double* bmat,
                                                            double* zmat,
                                                            int32_t* idz,
                                                            int32_t* ndim,
                                                            int32_t* kopt,
                                                            int32_t* knew,
                                                            double* d__,
                                                            double* w,
                                                            double* vlag,
                                                            double* beta,
                                                            double* s,
                                                            double* wvec,
                                                            double* prod ) const
{

  try
  {

    int32_t xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1,
        zmat_offset,
        wvec_dim1, wvec_offset, prod_dim1, prod_offset, i__1, i__2, i__, j, k,
        isave, iterc, jc, ip, iu, nw, ksav, nptm;
    double dd, d__1, ds, ss, den[ 9 ], par[ 9 ], tau, sum, diff, temp, step,
        alpha, angle, denex[ 9 ], tempa, tempb, tempc, ssden, dtest, xoptd,
        twopi, xopts, denold, denmax, densav, dstemp, sumold, sstemp, xoptsq;

    // Parameter adjustments
    zmat_dim1 = npt;
    zmat_offset = 1 + zmat_dim1;
    zmat -= zmat_offset;
    xpt_dim1 = npt;
    xpt_offset = 1 + xpt_dim1;
    xpt -= xpt_offset;
    --xopt;
    prod_dim1 = *ndim;
    prod_offset = 1 + prod_dim1;
    prod -= prod_offset;
    wvec_dim1 = *ndim;
    wvec_offset = 1 + wvec_dim1;
    wvec -= wvec_offset;
    bmat_dim1 = *ndim;
    bmat_offset = 1 + bmat_dim1;
    bmat -= bmat_offset;
    --d__;
    --w;
    --vlag;
    --s;

    twopi = atan( 1.0 ) * 8.;
    nptm = npt - n - 1;
    // Store the first NPT elements of the KNEW-th column of H in W(N+1)
    // to W(N+NPT).
    i__1 = npt;
    for ( k = 1; k <= i__1; ++k )
    {

      w[ n + k ] = 0;

    }
    i__1 = nptm;
    for ( j = 1; j <= i__1; ++j )
    {

      temp = zmat[ *knew + j * zmat_dim1 ];
      if ( j < *idz )
      {

        temp = -temp;

      }
      i__2 = npt;
      for ( k = 1; k <= i__2; ++k )
      {

        w[ n + k ] += temp * zmat[ k + j * zmat_dim1 ];

      }

    }
    alpha = w[ n + *knew ];
    // the initial search direction D is taken from the last call of
    // BIGBDLAG, and the initial S is set below, usually to the direction
    // from X_OPT to X_KNEW, but a different direction to an
    // interpolation point may be chosen, in order to prevent S from
    // being nearly parallel to D. 
    dd = ds = ss = xoptsq = 0;
    i__2 = n;
    for ( i__ = 1; i__ <= i__2; ++i__ )
    {

      // Computing 2nd power
      d__1 = d__[ i__ ];
      dd += d__1 * d__1;
      s[ i__ ] = xpt[ *knew + i__ * xpt_dim1 ] - xopt[ i__ ];
      ds += d__[ i__ ] * s[ i__ ];
      // Computing 2nd power
      d__1 = s[ i__ ];
      ss += d__1 * d__1;
      // Computing 2nd power
      d__1 = xopt[ i__ ];
      xoptsq += d__1 * d__1;

    }
    if ( ds * ds > dd * .99 * ss )
    {

      ksav = *knew;
      dtest = ds * ds / ss;
      i__2 = npt;
      for ( k = 1; k <= i__2; ++k )
      {

        if ( k != *kopt )
        {

          dstemp = 0;
          sstemp = 0;
          i__1 = n;
          for ( i__ = 1; i__ <= i__1; ++i__ )
          {

            diff = xpt[ k + i__ * xpt_dim1 ] - xopt[ i__ ];
            dstemp += d__[ i__ ] * diff;
            sstemp += diff * diff;

          }
          if ( dstemp * dstemp / sstemp < dtest )
          {

            ksav = k;
            dtest = dstemp * dstemp / sstemp;
            ds = dstemp;
            ss = sstemp;

          }

        }

      }
      i__2 = n;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        s[ i__ ] = xpt[ ksav + i__ * xpt_dim1 ] - xopt[ i__ ];

      }

    }
    ssden = dd * ss - ds * ds;
    iterc = 0;
    densav = 0;
    // begin the iteration by overwriting S with a vector that has the
    // required length and direction.
 
    do
    {
 
      ++iterc;
      temp = 1.0 / sqrt( ssden );
      xoptd = xopts = 0;
      i__2 = n;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        s[ i__ ] = temp * ( dd * s[ i__ ] - ds * d__[ i__ ] );
        xoptd += xopt[ i__ ] * d__[ i__ ];
        xopts += xopt[ i__ ] * s[ i__ ];

      }
      // set the coefficients of the first 2.0 terms of BETA.
      tempa = 0.5 * xoptd * xoptd;
      tempb = 0.5 * xopts * xopts;
      den[ 0 ] = dd * ( xoptsq + 0.5 * dd ) + tempa + tempb;
      den[ 1 ] = 2.0 * xoptd * dd;
      den[ 2 ] = 2.0 * xopts * dd;
      den[ 3 ] = tempa - tempb;
      den[ 4 ] = xoptd * xopts;
      for ( i__ = 6; i__ <= 9; ++i__ )
      {

        den[ i__ - 1 ] = 0;

      }
      // put the coefficients of Wcheck in WVEC.
      i__2 = npt;
      for ( k = 1; k <= i__2; ++k )
      {

        tempa = tempb = tempc = 0;
        i__1 = n;
        for ( i__ = 1; i__ <= i__1; ++i__ )
        {

          tempa += xpt[ k + i__ * xpt_dim1 ] * d__[ i__ ];
          tempb += xpt[ k + i__ * xpt_dim1 ] * s[ i__ ];
          tempc += xpt[ k + i__ * xpt_dim1 ] * xopt[ i__ ];

        }
        wvec[ k + wvec_dim1 ] = 0.25 * ( tempa * tempa + tempb * tempb );
        wvec[ k + ( wvec_dim1 << 1 ) ] = tempa * tempc;
        wvec[ k + wvec_dim1 * 3 ] = tempb * tempc;
        wvec[ k + ( wvec_dim1 << 2 ) ] = 0.25 * ( tempa * tempa - 
                                                  tempb * tempb );
        wvec[ k + wvec_dim1 * 5 ] = 0.5 * tempa * tempb;

      }
      i__2 = n;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        ip = i__ + npt;
        wvec[ ip + wvec_dim1 ] = 0;
        wvec[ ip + ( wvec_dim1 << 1 ) ] = d__[ i__ ];
        wvec[ ip + wvec_dim1 * 3 ] = s[ i__ ];
        wvec[ ip + ( wvec_dim1 << 2 ) ] = 0;
        wvec[ ip + wvec_dim1 * 5 ] = 0;

      }
      // put the coefficents of THETA*Wcheck in PROD.
      for ( jc = 1; jc <= 5; ++jc )
      {

        nw = npt;
        if ( jc == 2 || jc == 3 )
        {
 
          nw = *ndim;

        }
        i__2 = npt;
        for ( k = 1; k <= i__2; ++k )
        {

          prod[ k + jc * prod_dim1 ] = 0;

        }
        i__2 = nptm;
        for ( j = 1; j <= i__2; ++j )
        {

          sum = 0;
          i__1 = npt;
          for ( k = 1; k <= i__1; ++k )
          {

            sum += zmat[ k + j * zmat_dim1 ] * wvec[ k + jc * wvec_dim1 ];

          }
          if ( j < *idz )
          {
 
            sum = -sum;

          }
          i__1 = npt;
          for ( k = 1; k <= i__1; ++k )
          {

            prod[ k + jc * prod_dim1 ] += sum * zmat[ k + j * zmat_dim1 ];

          }

        }
        if ( nw == *ndim )
        {

          i__1 = npt;
          for ( k = 1; k <= i__1; ++k )
          {

            sum = 0;
            i__2 = n;
            for ( j = 1; j <= i__2; ++j )
            {

              sum += bmat[ k + j * bmat_dim1 ] * 
                     wvec[ npt + j + jc * wvec_dim1 ];

            }
            prod[ k + jc * prod_dim1 ] += sum;

          }

        }
        i__1 = n;
        for ( j = 1; j <= i__1; ++j )
        {

          sum = 0;
          i__2 = nw;
          for ( i__ = 1; i__ <= i__2; ++i__ )
          {

            sum += bmat[ i__ + j * bmat_dim1 ] * wvec[ i__ + jc * wvec_dim1 ];

          }
          prod[ npt + j + jc * prod_dim1 ] = sum;

        }

      }
      // include in DEN the part of BETA that depends on THETA.
      i__1 = *ndim;
      for ( k = 1; k <= i__1; ++k )
      {

        sum = 0;
        for ( i__ = 1; i__ <= 5; ++i__ )
        {

          par[ i__ - 1 ] = 0.5 * prod[ k + i__ * prod_dim1 ] * wvec[ k + i__
              * wvec_dim1 ];
          sum += par[ i__ - 1 ];
        }
        den[ 0 ] = den[ 0 ] - par[ 0 ] - sum;
        tempa = prod[ k + prod_dim1 ] * wvec[ k + ( wvec_dim1 << 1 ) ] + prod[ k
            + ( prod_dim1 << 1 ) ] * wvec[ k + wvec_dim1 ];
        tempb = prod[ k + ( prod_dim1 << 1 ) ] * wvec[ k + ( wvec_dim1 << 2 ) ]
            + prod[ k + ( prod_dim1 << 2 ) ] * wvec[ k + ( wvec_dim1 << 1 ) ];
        tempc = prod[ k + prod_dim1 * 3 ] * wvec[ k + wvec_dim1 * 5 ] + prod[ k
            + prod_dim1 * 5 ] * wvec[ k + wvec_dim1 * 3 ];
        den[ 1 ] = den[ 1 ] - tempa - 0.5 * ( tempb + tempc );
        den[ 5 ] -= 0.5 * ( tempb - tempc );
        tempa = prod[ k + prod_dim1 ] * wvec[ k + wvec_dim1 * 3 ] + prod[ k
            + prod_dim1 * 3 ] * wvec[ k + wvec_dim1 ];
        tempb = prod[ k + ( prod_dim1 << 1 ) ] * wvec[ k + wvec_dim1 * 5 ]
            + prod[ k + prod_dim1 * 5 ] * wvec[ k + ( wvec_dim1 << 1 ) ];
        tempc = prod[ k + prod_dim1 * 3 ] * wvec[ k + ( wvec_dim1 << 2 ) ]
            + prod[ k + ( prod_dim1 << 2 ) ] * wvec[ k + wvec_dim1 * 3 ];
        den[ 2 ] = den[ 2 ] - tempa - 0.5 * ( tempb - tempc );
        den[ 6 ] -= 0.5 * ( tempb + tempc );
        tempa = prod[ k + prod_dim1 ] * wvec[ k + ( wvec_dim1 << 2 ) ] + prod[ k
            + ( prod_dim1 << 2 ) ] * wvec[ k + wvec_dim1 ];
        den[ 3 ] = den[ 3 ] - tempa - par[ 1 ] + par[ 2 ];
        tempa = prod[ k + prod_dim1 ] * wvec[ k + wvec_dim1 * 5 ] + prod[ k
            + prod_dim1 * 5 ] * wvec[ k + wvec_dim1 ];
        tempb = prod[ k + ( prod_dim1 << 1 ) ] * wvec[ k + wvec_dim1 * 3 ]
            + prod[ k + prod_dim1 * 3 ] * wvec[ k + ( wvec_dim1 << 1 ) ];
        den[ 4 ] = den[ 4 ] - tempa - 0.5 * tempb;
        den[ 7 ] = den[ 7 ] - par[ 3 ] + par[ 4 ];
        tempa = prod[ k + ( prod_dim1 << 2 ) ] * wvec[ k + wvec_dim1 * 5 ]
            + prod[ k + prod_dim1 * 5 ] * wvec[ k + ( wvec_dim1 << 2 ) ];
        den[ 8 ] -= 0.5 * tempa;
      }
      // extend DEN so that it holds all the coefficients of DENOM.
      sum = 0;
      for ( i__ = 1; i__ <= 5; ++i__ )
      {

        // computing 2nd power
        d__1 = prod[ *knew + i__ * prod_dim1 ];
        par[ i__ - 1 ] = 0.5 * ( d__1 * d__1 );
        sum += par[ i__ - 1 ];

      }
      denex[ 0 ] = alpha * den[ 0 ] + par[ 0 ] + sum;
      tempa = 2.0 * prod[ *knew + prod_dim1 ] * 
                    prod[ *knew + ( prod_dim1 << 1 )];
      tempb = prod[ *knew + ( prod_dim1 << 1 ) ] * prod[ *knew
          + ( prod_dim1 << 2 ) ];
      tempc = prod[ *knew + prod_dim1 * 3 ] * prod[ *knew + prod_dim1 * 5 ];
      denex[ 1 ] = alpha * den[ 1 ] + tempa + tempb + tempc;
      denex[ 5 ] = alpha * den[ 5 ] + tempb - tempc;
      tempa = 2.0 * prod[ *knew + prod_dim1 ] * prod[ *knew + prod_dim1 * 3 ];
      tempb = prod[ *knew + ( prod_dim1 << 1 ) ] * 
              prod[ *knew + prod_dim1 * 5 ];
      tempc = prod[ *knew + prod_dim1 * 3 ] * 
              prod[ *knew + ( prod_dim1 << 2 ) ];
      denex[ 2 ] = alpha * den[ 2 ] + tempa + tempb - tempc;
      denex[ 6 ] = alpha * den[ 6 ] + tempb + tempc;
      tempa = 2.0 * prod[ *knew + prod_dim1 ] * 
                    prod[ *knew + ( prod_dim1 << 2 )];
      denex[ 3 ] = alpha * den[ 3 ] + tempa + par[ 1 ] - par[ 2 ];
      tempa = 2.0 * prod[ *knew + prod_dim1 ] * prod[ *knew + prod_dim1 * 5 ];
      denex[ 4 ] = alpha * den[ 4 ] + tempa + prod[ *knew + ( prod_dim1 << 1 ) ]
          * prod[ *knew + prod_dim1 * 3 ];
      denex[ 7 ] = alpha * den[ 7 ] + par[ 3 ] - par[ 4 ];
      denex[ 8 ] = alpha * den[ 8 ] + prod[ *knew + ( prod_dim1 << 2 ) ]
          * prod[ *knew + prod_dim1 * 5 ];
      // seek the value of the angle that maximizes the modulus of DENOM.
      sum = denex[ 0 ] + denex[ 1 ] + denex[ 3 ] + denex[ 5 ] + denex[ 7 ];
      denold = denmax = sum;
      isave = 0;
      iu = 49;
      temp = twopi / (double) ( iu + 1 );
      par[ 0 ] = 1.0;
      i__1 = iu;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        angle = (double) i__ * temp;
        par[ 1 ] = cos( angle );
        par[ 2 ] = sin( angle );
        for ( j = 4; j <= 8; j += 2 )
        {

          par[ j - 1 ] = par[ 1 ] * par[ j - 3 ] - par[ 2 ] * par[ j - 2 ];
          par[ j ] = par[ 1 ] * par[ j - 2 ] + par[ 2 ] * par[ j - 3 ];

        }
        sumold = sum;
        sum = 0;
        for ( j = 1; j <= 9; ++j )
        {

          sum += denex[ j - 1 ] * par[ j - 1 ];

        } 
        if ( fabs( sum ) > fabs( denmax ) )
        {

          denmax = sum;
          isave = i__;
          tempa = sumold;

        }
        else if ( i__ == isave + 1 )
        {

          tempb = sum;

        }

      }
      if ( isave == 0 )
      {

        tempa = sum;

      }
      if ( isave == iu )
      {

        tempb = denold;

      }
      step = 0;
      if ( tempa != tempb )
      {

        tempa -= denmax;
        tempb -= denmax;
        step = 0.5 * ( tempa - tempb ) / ( tempa + tempb );

      }
      angle = temp * ( (double) isave + step );
      // calculate the new parameters of the denominator, the new VBDLAG
      // vector and the new D. Then test for convergence.
      par[ 1 ] = cos( angle );
      par[ 2 ] = sin( angle );
      for ( j = 4; j <= 8; j += 2 )
      {

        par[ j - 1 ] = par[ 1 ] * par[ j - 3 ] - par[ 2 ] * par[ j - 2 ];
        par[ j ] = par[ 1 ] * par[ j - 2 ] + par[ 2 ] * par[ j - 3 ];

      }
      *beta = 0;
      denmax = 0;
      for ( j = 1; j <= 9; ++j )
      {

        *beta += den[ j - 1 ] * par[ j - 1 ];
        denmax += denex[ j - 1 ] * par[ j - 1 ];

      }
      i__1 = *ndim;
      for ( k = 1; k <= i__1; ++k )
      {

        vlag[ k ] = 0;
        for ( j = 1; j <= 5; ++j )
        {

          vlag[ k ] += prod[ k + j * prod_dim1 ] * par[ j - 1 ];

        }

      }
      tau = vlag[ *knew ];
      dd = tempa = tempb = 0;
      i__1 = n;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        d__[ i__ ] = par[ 1 ] * d__[ i__ ] + par[ 2 ] * s[ i__ ];
        w[ i__ ] = xopt[ i__ ] + d__[ i__ ];
        // computing 2nd power
        d__1 = d__[ i__ ];
        dd += d__1 * d__1;
        tempa += d__[ i__ ] * w[ i__ ];
        tempb += w[ i__ ] * w[ i__ ];

      }
      if ( iterc >= n )
      {

        // set the vector W before the RETURN from the subroutine.
        i__2 = *ndim;
        for ( k = 1; k <= i__2; ++k )
        {

          w[ k ] = 0;
          for ( j = 1; j <= 5; ++j )
          {

            w[ k ] += wvec[ k + j * wvec_dim1 ] * par[ j - 1 ];
 
          }

        }
        vlag[ *kopt ] += 1.0;
        return 0;

      }
      if ( iterc > 1 )
      { 

        densav = fmax( densav, denold );

      }
      if ( fabs( denmax ) <= fabs( densav ) * 1.1 )
      {

        // set the vector W before the RETURN from the subroutine.
        i__2 = *ndim;
        for ( k = 1; k <= i__2; ++k )
        {

          w[ k ] = 0;
          for ( j = 1; j <= 5; ++j )
          {

            w[ k ] += wvec[ k + j * wvec_dim1 ] * par[ j - 1 ];
 
          }

        }
        vlag[ *kopt ] += 1.0;
        return 0;

      }

      densav = denmax;
      // set S to 0.5 the gradient of the denominator with respect to
      // D. Then branch for the next iteration.
      i__1 = n;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        temp = tempa * xopt[ i__ ] + tempb * d__[ i__ ] - vlag[ npt + i__ ];
        s[ i__ ] = tau * bmat[ *knew + i__ * bmat_dim1 ] + alpha * temp;

      }
      i__1 = npt;
      for ( k = 1; k <= i__1; ++k )
      {

        sum = 0;
        i__2 = n;
        for ( j = 1; j <= i__2; ++j )
        {

          sum += xpt[ k + j * xpt_dim1 ] * w[ j ];

        }
        temp = ( tau * w[ n + k ] - alpha * vlag[ k ] ) * sum;
        i__2 = n;
        for ( i__ = 1; i__ <= i__2; ++i__ )
        {

          s[ i__ ] += temp * xpt[ k + i__ * xpt_dim1 ];

        }

      }
      ss = 0;
      ds = 0;
      i__2 = n;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        // computing 2nd power
        d__1 = s[ i__ ];
        ss += d__1 * d__1;
        ds += d__[ i__ ] * s[ i__ ];

      }
      ssden = dd * ss - ds * ds;

    } while ( ssden >= dd * 1e-8 * ss );

    return 0;

  }
  GKG_CATCH( "inline "
             "int32_t gkg::NumericalAnalysisImplementationFactory::bigden( " 
             "int32_t n, "
             "int32_t npt, "
             "double* xopt, "
             "double* xpt, "
             "double* bmat, "
             "double* zmat, "
             "int32_t* idz, "
             "int32_t* ndim, "
             "int32_t* kopt, "
             "int32_t* knew, "
             "double* d__, "
             "double* w, "
             "double* vlag, "
             "double* beta, "
             "double* s, "
             "double* wvec, "
             "double* prod ) const" );

}


inline int32_t gkg::NumericalAnalysisImplementationFactory::trl(
                                                       int32_t n,
                                                       int32_t npt,
                                                       double* /* xopt */,
                                                       double* xpt,
                                                       double* gq,
                                                       double* hq,
                                                       double* pq,
                                                       double* /* delta */,
                                                       double* step,
                                                       double* d__,
                                                       double* g,
                                                       double* hd,
                                                       double* hs,
                                                       double* crvmin,
                                                       int32_t xpt_dim1,
                                                       int32_t /* xpt_offset */,
                                                       int32_t i__1,
                                                       int32_t i__2,
                                                       int32_t i__,
                                                       int32_t j,
                                                       int32_t k,
                                                       int32_t ih,
                                                       int32_t iu,
                                                       int32_t iterc,
                                                       int32_t isave,
                                                       int32_t itersw,
                                                       int32_t itermax,
                                                       double d__1,
                                                       double d__2,
                                                       double dd,
                                                       double cf,
                                                       double dg,
                                                       double gg,
                                                       double ds,
                                                       double sg,
                                                       double ss,
                                                       double dhd,
                                                       double dhs,
                                                       double cth,
                                                       double sgk,
                                                       double shs,
                                                       double sth,
                                                       double qadd,
                                                       double qbeg,
                                                       double qred,
                                                       double qmin,
                                                       double temp,
                                                       double qsav,
                                                       double qnew,
                                                       double ggbeg,
                                                       double alpha,
                                                       double angle,
                                                       double reduc,
                                                       double ggsav,
                                                       double delsq,
                                                       double tempa,
                                                       double tempb,
                                                       double bstep,
                                                       double ratio,
                                                       double twopi,
                                                       double angtest ) const
{

  try
  {

    bool loop = true;
    bool bool1, bool2, bool3;

    while ( loop )
    {

      bool1 = true;
      bool2 = true;
      bool3 = true;
 
      i__1 = n;

      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        hd[ i__ ] = 0;

      }

      i__1 = npt;

      for ( k = 1; k <= i__1; ++k )
      {
 
       temp = 0;
        i__2 = n;
        for ( j = 1; j <= i__2; ++j )
        {

          temp += xpt[ k + j * xpt_dim1 ] * d__[ j ];

        }
        temp *= pq[ k ];
        i__2 = n;
        for ( i__ = 1; i__ <= i__2; ++i__ )
        {

          hd[ i__ ] += temp * xpt[ k + i__ * xpt_dim1 ];

        }

      }
      ih = 0;
      i__2 = n;
      for ( j = 1; j <= i__2; ++j )
      {

        i__1 = j;
        for ( i__ = 1; i__ <= i__1; ++i__ )
        {

          ++ih;
          if ( i__ < j )
          {

            hd[ j ] += hq[ ih ] * d__[ i__ ];

          }
          hd[ i__ ] += hq[ ih ] * d__[ j ];

        }

      }
      if ( iterc == 0 )
      {
 
        // prepare for the first line search.
        qred = dd = 0;
        i__1 = n;
        for ( i__ = 1; i__ <= i__1; ++i__ )
        {

          step[ i__ ] = 0;
          hs[ i__ ] = 0;
          g[ i__ ] = gq[ i__ ] + hd[ i__ ];
          d__[ i__ ] = -g[ i__ ];
          // Computing 2nd power
          d__1 = d__[ i__ ];
          dd += d__1 * d__1;
 
        }

        *crvmin = 0;
        if ( dd == 0 )
        {

          return 0;

        }

        ds = ss = 0;
        gg = dd;
        ggbeg = gg;
        // calculate the step to the trust region boundary and the product
        // HD.

        ++iterc;
        temp = delsq - ss;
        bstep = temp / ( ds + sqrt( ds * ds + dd * temp ) );

        bool1 = false;

      }

      if( bool1 )
      {

        if ( iterc <= itersw )
        {

          dhd = 0;
          i__1 = n;
          for ( j = 1; j <= i__1; ++j )
          {

            dhd += d__[ j ] * hd[ j ];

          }

          // update CRVMIN and set the step-length ATRLPHA.
          alpha = bstep;
          if ( dhd > 0 )
          {

            temp = dhd / dd;
            if ( iterc == 1 )
            {

              *crvmin = temp;

            }
            *crvmin = fmin( *crvmin, temp );
            // computing MIN
            d__1 = alpha, d__2 = gg / dhd;
            alpha = fmin( d__1, d__2 );
          }
          qadd = alpha * ( gg - 0.5 * alpha * dhd );
          qred += qadd;
          // Update STEP and HS.
          ggsav = gg;
          gg = 0;
          i__1 = n;
          for ( i__ = 1; i__ <= i__1; ++i__ )
          {

            step[ i__ ] += alpha * d__[ i__ ];
            hs[ i__ ] += alpha * hd[ i__ ];
            // Computing 2nd power
            d__1 = g[ i__ ] + hs[ i__ ];
            gg += d__1 * d__1;

          }
          // begin another conjugate direction iteration if required.
          if ( alpha < bstep )
          {

            if ( qadd <= qred * .01 || gg <= ggbeg * 1e-4 || iterc == itermax )
            {

              return 0;

            }
            temp = gg / ggsav;
            dd = ds = ss = 0;
            i__1 = n;
            for ( i__ = 1; i__ <= i__1; ++i__ )
            {

              d__[ i__ ] = temp * d__[ i__ ] - g[ i__ ] - hs[ i__ ];
              // computing 2nd power
              d__1 = d__[ i__ ];
              dd += d__1 * d__1;
              ds += d__[ i__ ] * step[ i__ ];
              // computing 2nd power
              d__1 = step[ i__ ];
              ss += d__1 * d__1;

            }
            if ( ds <= 0 )
            {

              return 0;

            }

          }

          if( alpha < bstep && ss < delsq )
          {

            ++iterc;
            temp = delsq - ss;
            bstep = temp / ( ds + sqrt( ds * ds + dd * temp ) );

            bool2 = false;

          }

          if ( bool2 )
          {

            *crvmin = 0;
            itersw = iterc;
            // test whether an alternative iteration is required.
 
 
            if ( gg <= ggbeg * 1e-4 )
            {

              return 0;
 
            }
 
            sg = 0;
            shs = 0;
            i__1 = n;
            for ( i__ = 1; i__ <= i__1; ++i__ )
            {

              sg += step[ i__ ] * g[ i__ ];
              shs += step[ i__ ] * hs[ i__ ];

            }
            sgk = sg + shs;
            angtest = sgk / sqrt( gg * delsq );
            if ( angtest <= -.99 )
            {

              return 0;

            }
            // begin the alternative iteration by calculating D and HD and some
            // scalar products.
            ++iterc;
            temp = sqrt( delsq * gg - sgk * sgk );
            tempa = delsq / temp;
            tempb = sgk / temp;
            i__1 = n;
            for ( i__ = 1; i__ <= i__1; ++i__ )
            {

              d__[ i__ ] = tempa * ( g[ i__ ] + hs[ i__ ] ) - 
                           tempb * step[ i__ ];

            }

          }

           bool3 = false;

          }

          if ( bool3 )
          {

            dg = dhd = dhs = 0;
            i__1 = n;
            for ( i__ = 1; i__ <= i__1; ++i__ )
            {

              dg += d__[ i__ ] * g[ i__ ];
              dhd += hd[ i__ ] * d__[ i__ ];
              dhs += hd[ i__ ] * step[ i__ ];

            }
            // seek the value of the angle that minimizes Q.
            cf = 0.5 * ( shs - dhd );
            qbeg = sg + cf;
            qsav = qmin = qbeg;
            isave = 0;
            iu = 49;
            temp = twopi / (double) ( iu + 1 );
            i__1 = iu;
            for ( i__ = 1; i__ <= i__1; ++i__ )
            {

              angle = (double) i__ * temp;
              cth = cos( angle );
              sth = sin( angle );
              qnew = ( sg + cf * cth ) * cth + ( dg + dhs * cth ) * sth;
              if ( qnew < qmin )
              {

                qmin = qnew;
                isave = i__;
                tempa = qsav;

              }
              else if ( i__ == isave + 1 )
              {

                  tempb = qnew;

              }
              qsav = qnew;

            }
            if ( (double) isave == 0 )
            {

                tempa = qnew;

            }
            if ( isave == iu )
            {

                tempb = qbeg;

            }
            angle = 0;
            if ( tempa != tempb )
            {

              tempa -= qmin;
              tempb -= qmin;
              angle = 0.5 * ( tempa - tempb ) / ( tempa + tempb );

            }

            angle = temp * ( (double) isave + angle );
            // calculate the new STEP and HS. Then test for convergence.
            cth = cos( angle );
            sth = sin( angle );
            reduc = qbeg - ( sg + cf * cth ) * cth - ( dg + dhs * cth ) * sth;
            gg = 0;
            i__1 = n;
            for ( i__ = 1; i__ <= i__1; ++i__ )
            {

              step[ i__ ] = cth * step[ i__ ] + sth * d__[ i__ ];
              hs[ i__ ] = cth * hs[ i__ ] + sth * hd[ i__ ];
              // Computing 2nd power
              d__1 = g[ i__ ] + hs[ i__ ];
              gg += d__1 * d__1;

            }

            qred += reduc;
            ratio = reduc / qred;
 
            if ( iterc < itermax && ratio > .01 )
            {

              if ( gg <= ggbeg * 1e-4 )
              {

                return 0;
 
              }
 
              sg = 0;
              shs = 0;
              i__1 = n;
              for ( i__ = 1; i__ <= i__1; ++i__ )
              {

                sg += step[ i__ ] * g[ i__ ];
                shs += step[ i__ ] * hs[ i__ ];

              }
              sgk = sg + shs;
              angtest = sgk / sqrt( gg * delsq );
              if ( angtest <= -.99 )
              {

                return 0;

              }
              // begin the alternative iteration by calculating D and HD and 
              // some scalar products.
              ++iterc;
              temp = sqrt( delsq * gg - sgk * sgk );
              tempa = delsq / temp;
              tempb = sgk / temp;
              i__1 = n;
              for ( i__ = 1; i__ <= i__1; ++i__ )
              {

                d__[ i__ ] = tempa * ( g[ i__ ] + hs[ i__ ] ) - 
                                                              tempb*step[ i__ ];

              }

            }
            else
            {

              loop = false;

            }

          }

       }

    }

    return 0;

  }
  GKG_CATCH( "inline "
             "int32_t gkg::NumericalAnalysisImplementationFactory::trl( "
             "int32_t n, "
             "int32_t npt, "
             "double* xopt, "
             "double* xpt, "
             "double* gq, "
             "double* hq, "
             "double* pq, "
             "double* delta, "
             "double* step, "
             "double* d__, "
             "double* g, "
             "double* hd, "
             "double* hs, "
             "double* crvmin, "
             "int32_t xpt_dim1, "
             "int32_t xpt_offset, "
             "int32_t i__1, "
             "int32_t i__2, "
             "int32_t i__, "
             "int32_t j, "
             "int32_t k, "
             "int32_t ih, "
             "int32_t iu, "
             "int32_t iterc, "
             "int32_t isave, "
             "int32_t itersw, "
             "int32_t itermax, "
             "double d__1, "
             "double d__2, "
             "double dd, "
             "double cf, "
             "double dg, "
             "double gg, "
             "double ds, "
             "double sg, "
             "double ss, "
             "double dhd, "
             "double dhs, "
             "double cth, "
             "double sgk, "
             "double shs, "
             "double sth, "
             "double qadd, "
             "double qbeg, "
             "double qred, "
             "double qmin, "
             "double temp, "
             "double qsav, "
             "double qnew, "
             "double ggbeg, "
             "double alpha, "
             "double angle, "
             "double reduc, "
             "double ggsav, "
             "double delsq, "
             "double tempa, "
             "double tempb, "
             "double bstep, "
             "double ratio, "
             "double twopi, "
             "double angtest ) const" );

}


inline int32_t gkg::NumericalAnalysisImplementationFactory::trsapp( 
                                                          int32_t n,
                                                          int32_t npt,
                                                          double* xopt,
                                                          double* xpt,
                                                          double* gq,
                                                          double* hq,
                                                          double* pq,
                                                          double* delta,
                                                          double* step,
                                                          double* d__,
                                                          double* g,
                                                          double* hd,
                                                          double* hs,
                                                          double* crvmin ) const
{

  try
  {

    int32_t xpt_dim1, xpt_offset, i__1, i__2 = 0, i__, j = 0, k = 0, ih = 0,
            iu = 0, iterc, isave = 0, itersw, itermax;
    double d__1 = 0.0, d__2 = 0.0, dd, cf = 0.0, dg = 0.0, gg, ds = 0.0,
           sg, ss = 0.0, dhd = 0.0, dhs = 0.0, cth = 0.0, sgk = 0.0, shs,
           sth = 0.0, qadd = 0.0, qbeg = 0.0, qred, qmin = 0.0, temp = 0.0,
           qsav = 0.0, qnew = 0.0, ggbeg, alpha = 0.0, angle = 0.0, reduc = 0.0,
           ggsav = 0.0, delsq, tempa, tempb, bstep, ratio = 0.0, twopi,
           angtest = 0.0;

    // parameter adjustments
    tempa = tempb = shs = sg = bstep = ggbeg = gg = qred = dd = 0.0;
    xpt_dim1 = npt;
    xpt_offset = 1 + xpt_dim1;
    xpt -= xpt_offset;
    --xopt;
    --gq;
    --hq;
    --pq;
    --step;
    --d__;
    --g;
    --hd;
    --hs;

    twopi = 2.0 * M_PI;
    delsq = *delta * *delta;
    iterc = 0;
    itermax = n;
    itersw = itermax;
    i__1 = n;
    for ( i__ = 1; i__ <= i__1; ++i__ )
    {
 
      d__[ i__ ] = xopt[ i__ ];
 
    }
 

    trl( n, npt, xopt, xpt, gq, hq, pq, delta, step, d__, g,
         hd, hs, crvmin, xpt_dim1, xpt_offset, i__1, i__2, i__, j, k, ih, iu,
         iterc, isave, itersw, itermax, d__1, d__2, dd, cf, dg, gg, ds, sg, ss,
         dhd, dhs, cth, sgk, shs, sth, qadd, qbeg, qred, qmin, temp, qsav, qnew,
         ggbeg, alpha, angle, reduc, ggsav, delsq, tempa, tempb, bstep, ratio,
         twopi, angtest );

    return 0;

  }
  GKG_CATCH( "inline "
             "int32_t gkg::NumericalAnalysisImplementationFactory::trsapp(" 
             "int32_t n, "
             "int32_t npt, "
             "double* xopt, "
             "double* xpt, "
             "double* gq, "
             "double* hq, "
             "double* pq, "
             "double* delta, "
             "double* step, "
             "double* d__, "
             "double* g, "
             "double* hd, "
             "double* hs, "
             "double* crvmin ) const" );

} 


inline int32_t gkg::NumericalAnalysisImplementationFactory::update(
                                                             int32_t n,
                                                             int32_t npt,
                                                             double* bmat,
                                                             double* zmat,
                                                             int32_t* idz,
                                                             int32_t* ndim,
                                                             double* vlag,
                                                             double* beta,
                                                             int32_t* knew,
                                                             double* w ) const
{

  try
  {

    int32_t bmat_dim1, bmat_offset, zmat_dim1, zmat_offset, i__1, i__2, i__, j,
            ja, jb, jl, jp, nptm, iflag;
    double d__1, d__2, tau, temp, scala, scalb, alpha, 
           denom, tempa, tempb, tausq;

    // parameter adjustments
    tempb = 0.0;
    zmat_dim1 = npt;
    zmat_offset = 1 + zmat_dim1;
    zmat -= zmat_offset;
    bmat_dim1 = *ndim;
    bmat_offset = 1 + bmat_dim1;
    bmat -= bmat_offset;
    --vlag;
    --w;

    nptm = npt - n - 1;
    // apply the rotations that put zeros in the KNEW-th row of ZMAT.
    jl = 1;
    i__1 = nptm;
    for ( j = 2; j <= i__1; ++j )
    {

      if ( j == *idz )
      {

        jl = *idz;

      }
      else if ( zmat[ *knew + j * zmat_dim1 ] != 0 )
      {

        // computing 2nd power
        d__1 = zmat[ *knew + jl * zmat_dim1 ];
        // computing 2nd power
        d__2 = zmat[ *knew + j * zmat_dim1 ];
        temp = sqrt( d__1 * d__1 + d__2 * d__2 );
        tempa = zmat[ *knew + jl * zmat_dim1 ] / temp;
        tempb = zmat[ *knew + j * zmat_dim1 ] / temp;
        i__2 = npt;
        for ( i__ = 1; i__ <= i__2; ++i__ )
        {

          temp = tempa * zmat[ i__ + jl * zmat_dim1 ] + tempb * zmat[ i__ + j
              * zmat_dim1 ];
          zmat[ i__ + j * zmat_dim1 ] = tempa * zmat[ i__ + j * zmat_dim1 ]
              - tempb * zmat[ i__ + jl * zmat_dim1 ];
          zmat[ i__ + jl * zmat_dim1 ] = temp;

        }
        zmat[ *knew + j * zmat_dim1 ] = 0;

      }

    }
    // put the first NPT components of the KNEW-th column of HLAG into
    // W, and calculate the parameters of the updating formula.
    tempa = zmat[ *knew + zmat_dim1 ];
    if ( *idz >= 2 )
    {

      tempa = -tempa;

    }
    if ( jl > 1 )
    {

      tempb = zmat[ *knew + jl * zmat_dim1 ];

    }
    i__1 = npt;
    for ( i__ = 1; i__ <= i__1; ++i__ )
    {

      w[ i__ ] = tempa * zmat[ i__ + zmat_dim1 ];
      if ( jl > 1 )
      {

        w[ i__ ] += tempb * zmat[ i__ + jl * zmat_dim1 ];

      }

    }
    alpha = w[ *knew ];
    tau = vlag[ *knew ];
    tausq = tau * tau;
    denom = alpha * *beta + tausq;
    vlag[ *knew ] -= 1.0;
    // complete the updating of ZMAT when there is only 1.0 nonzero
    // element in the KNEW-th row of the new matrix ZMAT, but, if IFLAG
    // is set to 1.0, then the first column of ZMAT will be exchanged
    // with another 1.0 later.
    iflag = 0;
    if ( jl == 1 )
    {

      temp = sqrt( ( fabs( denom ) ) );
      tempb = tempa / temp;
      tempa = tau / temp;
      i__1 = npt;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        zmat[ i__ + zmat_dim1 ] = tempa * zmat[ i__ + zmat_dim1 ] - tempb
            * vlag[ i__ ];

      }
      if ( *idz == 1 && temp < 0 )
      {

        *idz = 2;

      }
      if ( *idz >= 2 && temp >= 0 )
      {

        iflag = 1;

      }

    }
    else
    {

      // complete the updating of ZMAT in the alternative case.
      ja = 1;
      if ( *beta >= 0 )
      {

        ja = jl;

      }
      jb = jl + 1 - ja;
      temp = zmat[ *knew + jb * zmat_dim1 ] / denom;
      tempa = temp * *beta;
      tempb = temp * tau;
      temp = zmat[ *knew + ja * zmat_dim1 ];
      scala = 1.0 / sqrt( fabs( *beta ) * temp * temp + tausq );
      scalb = scala * sqrt( ( fabs( denom ) ) );
      i__1 = npt;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        zmat[ i__ + ja * zmat_dim1 ] = scala * ( tau * zmat[ i__ + ja
            * zmat_dim1 ] - temp * vlag[ i__ ] );
        zmat[ i__ + jb * zmat_dim1 ] = scalb * ( zmat[ i__ + jb * zmat_dim1 ]
            - tempa * w[ i__ ] - tempb * vlag[ i__ ] );

      }
      if ( denom <= 0 )
      {

        if ( *beta < 0 )
        {

          ++( *idz );

        }
        if ( *beta >= 0 )
        {

          iflag = 1;

        }

      }

    }
    // IDZ is reduced in the following case, and usually the first
    // column of ZMAT is exchanged with a later 1.0.
    if ( iflag == 1 )
    {

      --( *idz );
      i__1 = npt;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        temp = zmat[ i__ + zmat_dim1 ];
        zmat[ i__ + zmat_dim1 ] = zmat[ i__ + *idz * zmat_dim1 ];
        zmat[ i__ + *idz * zmat_dim1 ] = temp;

      }

    }
    // finally, update the matrix BMAT.
    i__1 = n;
    for ( j = 1; j <= i__1; ++j )
    {

      jp = npt + j;
      w[ jp ] = bmat[ *knew + j * bmat_dim1 ];
      tempa = ( alpha * vlag[ jp ] - tau * w[ jp ] ) / denom;
      tempb = ( -( *beta ) * w[ jp ] - tau * vlag[ jp ] ) / denom;
      i__2 = jp;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        bmat[ i__ + j * bmat_dim1 ] = bmat[ i__ + j * bmat_dim1 ] + tempa
            * vlag[ i__ ] + tempb * w[ i__ ];
        if ( i__ > npt )
        {

          bmat[ jp + ( i__ - npt ) * bmat_dim1 ] = bmat[ i__ + j * bmat_dim1 ];

        }

      }

    }
    return 0;

  }
  GKG_CATCH( "inline "
             "int32_t gkg::NumericalAnalysisImplementationFactory::update( "
             "int32_t n, "
             "int32_t npt, "
             "double* bmat, "
             "double* zmat, "
             "int32_t* idz, "
             "int32_t* ndim, "
             "double* vlag, "
             "double* beta, "
             "int32_t* knew, "
             "double* w ) const" );

} 


inline double gkg::NumericalAnalysisImplementationFactory::L50L70(
       int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, 
       int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, 
       double *xnew, double *xpt, double *fval, double *gq, double *hq, 
       double *pq, double *bmat, double *zmat, int32_t *ndim, double *d__, 
       double *vlag, double *w, 
       const gkg::PowellNewUOAFunction &func, bool verbose,
       int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, 
       int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, 
       int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, int32_t j, 
       int32_t k, int32_t ih, int32_t nf, int32_t nh, int32_t ip, int32_t jp, 
       int32_t np, int32_t nfm, int32_t idz, int32_t ipt, int32_t jpt, 
       int32_t nfmm, int32_t knew, int32_t kopt , int32_t nptm, int32_t ksave, 
       int32_t nfsav, int32_t itemp, int32_t ktemp, int32_t itest, 
       int32_t nftest, double d__1, double d__2, double d__3, double f, 
       double dx, double dsq, double rho, double sum, double fbeg, double diff, 
       double beta, double gisq, double temp, double suma, double sumb, 
       double fopt, double bsum, double gqsq, double xipt, double xjpt, 
       double sumz, double diffa, double diffb, double diffc, double hdiag, 
       double alpha, double delta, double recip, double reciq, double fsave, 
       double dnorm, double ratio, double dstep, double vquad, double tempq, 
       double rhosq, double detrat, double crvmin, double distsq, 
       double xoptsq ) const
{

  try
  {

    nfm = nf;
    nfmm = nf - n;
    ++ nf;
    if ( nfm <= n << 1 )
    {
      if ( ( nfm >= 1 ) && ( nfm <= n ) )
      {

        xpt[ nf + nfm * xpt_dim1 ] = rhobeg;

      }
      else if ( nfm > n )
      {

        xpt[ nf + nfmm * xpt_dim1 ] = -( rhobeg );

      }

    }
    else
    {

      itemp = ( nfmm - 1 ) / n;
      jpt = nfm - itemp * n - n;
      ipt = jpt + itemp;
      if ( ipt > n )
      {

        itemp = jpt;
        jpt = ipt - n;
        ipt = itemp;

      }
      xipt = rhobeg;
      if ( fval[ ipt + np ] < fval[ ipt + 1 ] )
      {

        xipt = -xipt;

      }
      xjpt = rhobeg;
      if ( fval[ jpt + np ] < fval[ jpt + 1 ] )
      {

        xjpt = -xjpt;

      }
      xpt[ nf + ipt * xpt_dim1 ] = xipt;
      xpt[ nf + jpt * xpt_dim1 ] = xjpt;

    }

    // calculate the next value of F, label 70 being reached immediately
    // after this calculation. The least function value so far and its
    // index are required.
    i__1 = n;
    for ( j = 1; j <= i__1; ++j )
    {

      x[ j ] = xpt[ nf + j * xpt_dim1 ] + xbase[ j ];
 
    }

    L310L410( n, npt, x, rhobeg, rhoend, ret_nf, maxfun, xbase, xopt, xnew, xpt,
              fval, gq, hq, pq, bmat, zmat, ndim, d__, vlag, w, func, verbose,
              xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1,
              zmat_offset, i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np,
              nfm, idz, ipt, jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp,
              ktemp, itest, nftest, d__1, d__2, d__3, f, dx, dsq, rho, sum, 
              fbeg, diff, beta , gisq, temp, suma, sumb, fopt, bsum, gqsq, xipt,
              xjpt, sumz, diffa, diffb, diffc, hdiag, alpha, delta, recip, 
              reciq, fsave, dnorm, ratio, dstep, vquad, tempq, rhosq, detrat,
              crvmin, distsq, xoptsq );

    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L50L70( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::L70L90(
       int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, 
       int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, 
       double *xnew, double *xpt, double *fval, double *gq, double *hq, 
       double *pq, double *bmat, double *zmat, int32_t *ndim, double *d__, 
       double *vlag, double *w, 
       const gkg::PowellNewUOAFunction& func, bool verbose,
       int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, 
       int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, 
       int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, int32_t j, 
       int32_t k, int32_t ih, int32_t nf, int32_t nh, int32_t ip, int32_t jp, 
       int32_t np, int32_t nfm, int32_t idz, int32_t ipt, int32_t jpt, 
       int32_t nfmm, int32_t knew, int32_t kopt , int32_t nptm, int32_t ksave, 
       int32_t nfsav, int32_t itemp, int32_t ktemp, int32_t itest, 
       int32_t nftest, double d__1, double d__2, double d__3, double f, 
       double dx, double dsq, double rho, double sum, double fbeg, double diff, 
       double beta, double gisq, double temp, double suma, double sumb, 
       double fopt, double bsum, double gqsq, double xipt, double xjpt, 
       double sumz, double diffa, double diffb, double diffc, double hdiag, 
       double alpha, double delta, double recip, double reciq, double fsave, 
       double dnorm, double ratio, double dstep, double vquad, double tempq, 
       double rhosq, double detrat, double crvmin, double distsq, 
       double xoptsq ) const
{

  try
  {

    fval[ nf ] = f;
    if ( nf == 1 )
    {

      fbeg = fopt = f;
      kopt = 1;

    }
    else if ( f < fopt )
    {

      fopt = f;
      kopt = nf;

    }
    // set the non0 initial elements of BMAT and the quadratic model
    //  in the cases when NF is at most 2*N+1. 
    if ( nfm <= n << 1 )
    {

      if ( nfm >= 1 && nfm <= n )
      {

        gq[ nfm ] = ( f - fbeg ) / rhobeg;
        if ( npt < nf + n )
        {

          bmat[ nfm * bmat_dim1 + 1 ] = -1.0 / rhobeg;
          bmat[ nf + nfm * bmat_dim1 ] = 1.0 / rhobeg;
          bmat[ npt + nfm + nfm * bmat_dim1 ] = -.5 * rhosq;

        }

      }
      else if ( nfm > n )
      {
   
        bmat[ nf - n + nfmm * bmat_dim1 ] = .5 / rhobeg;
        bmat[ nf + nfmm * bmat_dim1 ] = -.5 / rhobeg;
        zmat[ nfmm * zmat_dim1 + 1 ] = -reciq - reciq;
        zmat[ nf - n + nfmm * zmat_dim1 ] = reciq;
        zmat[ nf + nfmm * zmat_dim1 ] = reciq;
        ih = nfmm * ( nfmm + 1 ) / 2;
        temp = ( fbeg - f ) / rhobeg;
        hq[ ih ] = ( gq[ nfmm ] - temp ) / rhobeg;
        gq[ nfmm ] = .5 * ( gq[ nfmm ] + temp );

      }
      // set the off-diagonal second derivatives of the Lagrange
      // functions and the initial quadratic model.
    }
    else
    {

      ih = ipt * ( ipt - 1 ) / 2 + jpt;
      if ( xipt < 0 )
        ipt += n;
      if ( xjpt < 0 )
        jpt += n;
      zmat[ nfmm * zmat_dim1 + 1 ] = recip;
      zmat[ nf + nfmm * zmat_dim1 ] = recip;
      zmat[ ipt + 1 + nfmm * zmat_dim1 ] = -recip;
      zmat[ jpt + 1 + nfmm * zmat_dim1 ] = -recip;
      hq[ ih ] = ( fbeg - fval[ ipt + 1 ] - fval[ jpt + 1 ] + f ) / ( xipt
          * xjpt );

    }
    if ( nf < npt )
    {

      L50L70( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );
           return f;

    }

    // begin the iterative procedure, because the initial model is
    // complete. 
    rho = rhobeg;
    delta = rho;
    idz = 1;
    diffa = 0;
    diffb = 0;
    itest = 0;
    xoptsq = 0;
    i__1 = n;
    for ( i__ = 1; i__ <= i__1; ++i__ )
    {

      xopt[ i__ ] = xpt[ kopt + i__ * xpt_dim1 ];
      // computing 2nd power
      d__1 = xopt[ i__ ];
      xoptsq += d__1 * d__1;

    }

    L90L100( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );

    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L70L90( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::L90L100(
       int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, 
       int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, 
       double *xnew, double *xpt, double *fval, double *gq, double *hq, 
       double *pq, double *bmat, double *zmat, int32_t *ndim, double *d__, 
       double *vlag, double *w, 
       const gkg::PowellNewUOAFunction& func, bool verbose,
       int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, 
       int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, 
       int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, int32_t j, 
       int32_t k, int32_t ih, int32_t nf, int32_t nh, int32_t ip, int32_t jp, 
       int32_t np, int32_t nfm, int32_t idz, int32_t ipt, int32_t jpt, 
       int32_t nfmm, int32_t knew, int32_t kopt , int32_t nptm, int32_t ksave, 
       int32_t nfsav, int32_t itemp, int32_t ktemp, int32_t itest, 
       int32_t nftest, double d__1, double d__2, double d__3, double f, 
       double dx, double dsq, double rho, double sum, double fbeg, double diff, 
       double beta, double gisq, double temp, double suma, double sumb, 
       double fopt, double bsum, double gqsq, double xipt, double xjpt, 
       double sumz, double diffa, double diffb, double diffc, double hdiag, 
       double alpha, double delta, double recip, double reciq, double fsave, 
       double dnorm, double ratio, double dstep, double vquad, double tempq, 
       double rhosq, double detrat, double crvmin, double distsq, 
       double xoptsq ) const
{

  try
  {

    nfsav = nf;
    L100L120( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );

    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L90L100( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::L100L120(
       int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, 
       int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, 
       double *xnew, double *xpt, double *fval, double *gq, double *hq, 
       double *pq, double *bmat, double *zmat, int32_t *ndim, double *d__, 
       double *vlag, double *w, 
       const gkg::PowellNewUOAFunction& func, bool verbose,
       int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, 
       int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, 
       int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, int32_t j, 
       int32_t k, int32_t ih, int32_t nf, int32_t nh, int32_t ip, int32_t jp, 
       int32_t np, int32_t nfm, int32_t idz, int32_t ipt, int32_t jpt, 
       int32_t nfmm, int32_t knew, int32_t kopt , int32_t nptm, int32_t ksave, 
       int32_t nfsav, int32_t itemp, int32_t ktemp, int32_t itest, 
       int32_t nftest, double d__1, double d__2, double d__3, double f, 
       double dx, double dsq, double rho, double sum, double fbeg, double diff, 
       double beta, double gisq, double temp, double suma, double sumb, 
       double fopt, double bsum, double gqsq, double xipt, double xjpt, 
       double sumz, double diffa, double diffb, double diffc, double hdiag, 
       double alpha, double delta, double recip, double reciq, double fsave, 
       double dnorm, double ratio, double dstep, double vquad, double tempq, 
       double rhosq, double detrat, double crvmin, double distsq, 
       double xoptsq ) const
{

  try
  {

    knew = 0;
    trsapp( n, npt, &xopt[ 1 ], &xpt[ xpt_offset ], &gq[ 1 ], &hq[ 1 ],
            &pq[ 1 ], &delta, &d__[ 1 ], &w[ 1 ], &w[ np ], &w[ np + n ],
            &w[ np + ( n << 1 ) ], &crvmin );
    dsq = 0;
    i__1 = n;
    for ( i__ = 1; i__ <= i__1; ++i__ )
    {

      // computing 2nd power
      d__1 = d__[ i__ ];
      dsq += d__1 * d__1;

    }
    // computing MIN 
    d__1 = delta, d__2 = sqrt( dsq );
    dnorm = fmin( d__1, d__2 );
    if ( dnorm < .5 * rho )
    {

      knew = -1;
      delta = 0.1 * delta;
      ratio = -1.;
      if ( delta <= rho * 1.5 )
      {

        delta = rho;

      }
      if ( nf <= nfsav + 2 )
      {

        L460L490( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );
           return f;

      }

      temp = crvmin * .125 * rho * rho;
      // computing MAX
      d__1 = fmax( diffa, diffb );
      if ( temp <= fmax( d__1, diffc ) )
      {

        L460L490( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );
           return f;

      }

      L490L530( n, npt, x, rhobeg, rhoend, ret_nf,
               maxfun, xbase, xopt, xnew, xpt,
               fval, gq, hq, pq, bmat, zmat,
               ndim, d__, vlag, w, func, verbose,
               xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
               zmat_offset, i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np,
               nfm, idz, ipt,
               jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
               nftest,
               d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
               temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, 
               diffb, diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, 
               ratio, dstep,vquad, tempq, rhosq, detrat, crvmin, distsq, 
               xoptsq );

      return f;

    }

    L120L290( n, npt, x, rhobeg, rhoend, ret_nf,
             maxfun, xbase, xopt, xnew, xpt,
             fval, gq, hq, pq, bmat, zmat,
             ndim, d__, vlag, w, func, verbose,
             xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
             zmat_offset,
             i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
             jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
             nftest,
             d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
             temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
             diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, 
             dstep, vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );

    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L100L120( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::L120L290(
       int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, 
       int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, 
       double *xnew, double *xpt, double *fval, double *gq, double *hq, 
       double *pq, double *bmat, double *zmat, int32_t *ndim, double *d__, 
       double *vlag, double *w, 
       const gkg::PowellNewUOAFunction& func, bool verbose,
       int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, 
       int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, 
       int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, int32_t j, 
       int32_t k, int32_t ih, int32_t nf, int32_t nh, int32_t ip, int32_t jp, 
       int32_t np, int32_t nfm, int32_t idz, int32_t ipt, int32_t jpt, 
       int32_t nfmm, int32_t knew, int32_t kopt , int32_t nptm, int32_t ksave, 
       int32_t nfsav, int32_t itemp, int32_t ktemp, int32_t itest, 
       int32_t nftest, double d__1, double d__2, double d__3, double f, 
       double dx, double dsq, double rho, double sum, double fbeg, double diff, 
       double beta, double gisq, double temp, double suma, double sumb, 
       double fopt, double bsum, double gqsq, double xipt, double xjpt, 
       double sumz, double diffa, double diffb, double diffc, double hdiag, 
       double alpha, double delta, double recip, double reciq, double fsave, 
       double dnorm, double ratio, double dstep, double vquad, double tempq, 
       double rhosq, double detrat, double crvmin, double distsq, 
       double xoptsq ) const
{

  try
  {

    if ( dsq <= xoptsq * .001 )
    {

      tempq = xoptsq * .25;
      i__1 = npt;
      for ( k = 1; k <= i__1; ++k )
      {

        sum = 0;
        i__2 = n;
        for ( i__ = 1; i__ <= i__2; ++i__ )
        {

          sum += xpt[ k + i__ * xpt_dim1 ] * xopt[ i__ ];

        }
        temp = pq[ k ] * sum;
        sum -= .5 * xoptsq;
        w[ npt + k ] = sum;
        i__2 = n;
        for ( i__ = 1; i__ <= i__2; ++i__ )
        {

          gq[ i__ ] += temp * xpt[ k + i__ * xpt_dim1 ];
          xpt[ k + i__ * xpt_dim1 ] -= .5 * xopt[ i__ ];
          vlag[ i__ ] = bmat[ k + i__ * bmat_dim1 ];
          w[ i__ ] = sum * xpt[ k + i__ * xpt_dim1 ] + tempq * xopt[ i__ ];
          ip = npt + i__;
          i__3 = i__;
          for ( j = 1; j <= i__3; ++j )
          {

            bmat[ ip + j * bmat_dim1 ] = bmat[ ip + j * bmat_dim1 ]
                + vlag[ i__ ] * w[ j ] + w[ i__ ] * vlag[ j ];

          }

        }

      }
      // then the revisions of BMAT that depend on ZMAT are
      // calculated.
      i__3 = nptm;
      for ( k = 1; k <= i__3; ++k )
      {

        sumz = 0;
        i__2 = npt;
        for ( i__ = 1; i__ <= i__2; ++i__ )
        {

          sumz += zmat[ i__ + k * zmat_dim1 ];
          w[ i__ ] = w[ npt + i__ ] * zmat[ i__ + k * zmat_dim1 ];

        }
        i__2 = n;
        for ( j = 1; j <= i__2; ++j )
        {

          sum = tempq * sumz * xopt[ j ];
          i__1 = npt;
          for ( i__ = 1; i__ <= i__1; ++i__ )
          {

            sum += w[ i__ ] * xpt[ i__ + j * xpt_dim1 ];

          }
          vlag[ j ] = sum;
          if ( k < idz )
          {

            sum = -sum;

          }
          i__1 = npt;
          for ( i__ = 1; i__ <= i__1; ++i__ )
          {

            bmat[ i__ + j * bmat_dim1 ] += sum * zmat[ i__ + k * zmat_dim1 ];

          }

        }
        i__1 = n;
        for ( i__ = 1; i__ <= i__1; ++i__ )
        {

          ip = i__ + npt;
          temp = vlag[ i__ ];
          if ( k < idz )
          {

            temp = -temp;

          }
          i__2 = i__;
          for ( j = 1; j <= i__2; ++j )
          {

            bmat[ ip + j * bmat_dim1 ] += temp * vlag[ j ];

          }

        }

      }
      // the following instructions complete the shift of XBASE,
      // including the changes to the parameters of the quadratic
      // model. 
      ih = 0;
      i__2 = n;
      for ( j = 1; j <= i__2; ++j )
      {

        w[ j ] = 0;
        i__1 = npt;
        for ( k = 1; k <= i__1; ++k )
        {

          w[ j ] += pq[ k ] * xpt[ k + j * xpt_dim1 ];
          xpt[ k + j * xpt_dim1 ] -= .5 * xopt[ j ];

        }
        i__1 = j;
        for ( i__ = 1; i__ <= i__1; ++i__ )
        {

          ++ih;
          if ( i__ < j )
          {

            gq[ j ] += hq[ ih ] * xopt[ i__ ];

          }
          gq[ i__ ] += hq[ ih ] * xopt[ j ];
          hq[ ih ] = hq[ ih ] + w[ i__ ] * xopt[ j ] + xopt[ i__ ] * w[ j ];
          bmat[ npt + i__ + j * bmat_dim1 ] = bmat[ npt + j + i__ * bmat_dim1 ];

        }

      }
      i__1 = n;
      for ( j = 1; j <= i__1; ++j )
      {

        xbase[ j ] += xopt[ j ];
        xopt[ j ] = 0;

      }
      xoptsq = 0;
    }
    // pick the model step if KNEW is positive. A different choice of D
    // may be made later, if the choice of D by BIGLAG causes
    // substantial cancellation in DENOM. 
    if ( knew > 0 )
    {

      biglag( n, npt, &xopt[ 1 ], &xpt[ xpt_offset ], &bmat[ bmat_offset ],
              &zmat[ zmat_offset ], &idz, ndim, &knew, &dstep, &d__[ 1 ],
              &alpha, &vlag[ 1 ], &vlag[ npt + 1 ], &w[ 1 ], &w[ np ], &w[ np
                   + n ], func );

    }
    // calculate VLAG and BETA for the current choice of D. The first
    // NPT components of W_check will be held in W. 
    i__1 = npt;
    for ( k = 1; k <= i__1; ++k )
    {

      suma = 0;
      sumb = 0;
      sum = 0;
      i__2 = n;
      for ( j = 1; j <= i__2; ++j )
      {

        suma += xpt[ k + j * xpt_dim1 ] * d__[ j ];
        sumb += xpt[ k + j * xpt_dim1 ] * xopt[ j ];
        sum += bmat[ k + j * bmat_dim1 ] * d__[ j ];

      }
      w[ k ] = suma * ( .5 * suma + sumb );
      vlag[ k ] = sum;

    }
    beta = 0;
    i__1 = nptm;
    for ( k = 1; k <= i__1; ++k )
    {

      sum = 0;
      i__2 = npt;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        sum += zmat[ i__ + k * zmat_dim1 ] * w[ i__ ];

      }
      if ( k < idz )
      {

        beta += sum * sum;
        sum = -sum;

      }
      else
      {

        beta -= sum * sum;

      }
      i__2 = npt;

      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        vlag[ i__ ] += sum * zmat[ i__ + k * zmat_dim1 ];

      }

    }
    bsum = 0;
    dx = 0;
    i__2 = n;
    for ( j = 1; j <= i__2; ++j )
    {

      sum = 0;
      i__1 = npt;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        sum += w[ i__ ] * bmat[ i__ + j * bmat_dim1 ];

      }
      bsum += sum * d__[ j ];
      jp = npt + j;
      i__1 = n;
      for ( k = 1; k <= i__1; ++k )
      {

        sum += bmat[ jp + k * bmat_dim1 ] * d__[ k ];

      }
      vlag[ jp ] = sum;
      bsum += sum * d__[ j ];
      dx += d__[ j ] * xopt[ j ];
    }
    beta = dx * dx + dsq * ( xoptsq + dx + dx + .5 * dsq ) + beta - bsum;
    vlag[ kopt ] += 1.0;
    // if KNEW is positive and if the cancellation in DENOM is
    // unacceptable, then BIGDEN calculates an alternative model step,
    // XNEW being used for working space. 
    if ( knew > 0 )
    {

      // Computing 2nd power 
      d__1 = vlag[ knew ];
      temp = 1.0 + alpha * beta / ( d__1 * d__1 );
      if ( fabs( temp ) <= .8 )
      {

        bigden( n, npt, &xopt[ 1 ], &xpt[ xpt_offset ], &bmat[ bmat_offset ],
                &zmat[ zmat_offset ], &idz, ndim, &kopt, &knew, &d__[ 1 ],
                &w[ 1 ], &vlag[ 1 ], &beta, &xnew[ 1 ], &w[ *ndim + 1 ],
                &w[ *ndim * 6 + 1 ] );

      }

    }

    L290L310( n, npt, x, rhobeg, rhoend, ret_nf,
              maxfun, xbase, xopt, xnew, xpt,
              fval, gq, hq, pq, bmat, zmat,
              ndim, d__, vlag, w, func, verbose,
              xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
              zmat_offset, i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np,
              nfm, idz, ipt, jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, 
              ktemp, itest, nftest,
              d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
              temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, 
              diffb, diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, 
              ratio, dstep, vquad, tempq, rhosq, detrat, crvmin, distsq, 
              xoptsq );

    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L120L290( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::L290L310(
       int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, 
       int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, 
       double *xnew, double *xpt, double *fval, double *gq, double *hq, 
       double *pq, double *bmat, double *zmat, int32_t *ndim, double *d__, 
       double *vlag, double *w, 
       const gkg::PowellNewUOAFunction& func, bool verbose,
       int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, 
       int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, 
       int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, int32_t j, 
       int32_t k, int32_t ih, int32_t nf, int32_t nh, int32_t ip, int32_t jp, 
       int32_t np, int32_t nfm, int32_t idz, int32_t ipt, int32_t jpt, 
       int32_t nfmm, int32_t knew, int32_t kopt , int32_t nptm, int32_t ksave, 
       int32_t nfsav, int32_t itemp, int32_t ktemp, int32_t itest, 
       int32_t nftest, double d__1, double d__2, double d__3, double f, 
       double dx, double dsq, double rho, double sum, double fbeg, double diff, 
       double beta, double gisq, double temp, double suma, double sumb, 
       double fopt, double bsum, double gqsq, double xipt, double xjpt, 
       double sumz, double diffa, double diffb, double diffc, double hdiag, 
       double alpha, double delta, double recip, double reciq, double fsave, 
       double dnorm, double ratio, double dstep, double vquad, double tempq, 
       double rhosq, double detrat, double crvmin, double distsq, 
       double xoptsq ) const
{

  try
  {

    i__2 = n;
    for ( i__ = 1; i__ <= i__2; ++i__ )
    {

      xnew[ i__ ] = xopt[ i__ ] + d__[ i__ ];
      x[ i__ ] = xbase[ i__ ] + xnew[ i__ ];

    }
    ++nf;
 
    if ( verbose )
    {
 
      std::cout << "iteration " << nf << " : " << std::flush;
 
    }
 
    L310L410( n, npt, x, rhobeg, rhoend, ret_nf,
             maxfun, xbase, xopt, xnew, xpt,
             fval, gq, hq, pq, bmat, zmat,
             ndim, d__, vlag, w, func, verbose,
             xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
             zmat_offset, i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, 
             nfm, idz, ipt, jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, 
             ktemp, itest, nftest, 
             d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
             temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
             diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, 
             dstep, vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );
 
    return f;
 
  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L290L310( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::L310L410(
       int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, 
       int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, 
       double *xnew, double *xpt, double *fval, double *gq, double *hq, 
       double *pq, double *bmat, double *zmat, int32_t *ndim, double *d__, 
       double *vlag, double *w, 
       const gkg::PowellNewUOAFunction& func, bool verbose,
       int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, 
       int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, 
       int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, int32_t j, 
       int32_t k, int32_t ih, int32_t nf, int32_t nh, int32_t ip, int32_t jp, 
       int32_t np, int32_t nfm, int32_t idz, int32_t ipt, int32_t jpt, 
       int32_t nfmm, int32_t knew, int32_t kopt , int32_t nptm, int32_t ksave, 
       int32_t nfsav, int32_t itemp, int32_t ktemp, int32_t itest, 
       int32_t nftest, double d__1, double d__2, double d__3, double f, 
       double dx, double dsq, double rho, double sum, double fbeg, double diff, 
       double beta, double gisq, double temp, double suma, double sumb, 
       double fopt, double bsum, double gqsq, double xipt, double xjpt, 
       double sumz, double diffa, double diffb, double diffc, double hdiag, 
       double alpha, double delta, double recip, double reciq, double fsave, 
       double dnorm, double ratio, double dstep, double vquad, double tempq, 
       double rhosq, double detrat, double crvmin, double distsq, 
       double xoptsq ) const
{

  try
  {

    if ( nf > nftest )
    {

      --nf;
      L530end(
           n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );
           return f;

    }

    f = func.getValueAt( &x[ 1 ], n );
    if ( verbose )
    {

      std::cout << "parameters = ( ";
      int32_t p = 0;
      for ( p = 0; p < n - 1; p++ )
      {

        std::cout << x[ p + 1 ] << ", ";

      }
      std::cout << x[ p + 1 ] << ") ";
      std::cout << "size = " << f << std::endl;

    }
    if ( nf <= npt )
    {
 
      L70L90(
           n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );
           return f;

    }
   
    if ( knew == -1 )
    {

      L530end(
           n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );
           return f;

    }

    // Use the quadratic model to predict the change in F due to the
    // step D, and set DIFF to the error of this prediction
    vquad = ih = 0;
    i__2 = n;
    for ( j = 1; j <= i__2; ++j )
    {

      vquad += d__[ j ] * gq[ j ];
      i__1 = j;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        ++ih;
        temp = d__[ i__ ] * xnew[ j ] + d__[ j ] * xopt[ i__ ];
        if ( i__ == j )
        {

          temp = .5 * temp;

        }
        vquad += temp * hq[ ih ];

      }

    }
    i__1 = npt;
    for ( k = 1; k <= i__1; ++k )
    {

      vquad += pq[ k ] * w[ k ];

    }
    diff = f - fopt - vquad;
    diffc = diffb;
    diffb = diffa;
    diffa = fabs( diff );
    if ( dnorm > rho )
    {

      nfsav = nf;

    }
    // Update FOPT and XOPT if the new F is the least value of the
    // objective function so far. The branch when KNEW is positive
    // occurs if D is not a trust region step.
    fsave = fopt;
    if ( f < fopt )
    {
      fopt = f;
      xoptsq = 0;
      i__1 = n;
      for ( i__ = 1; i__ <= i__1; ++i__ )
      {

        xopt[ i__ ] = xnew[ i__ ];

        // computing 2nd power
        d__1 = xopt[ i__ ];
        xoptsq += d__1 * d__1;

      }

    }
    ksave = knew;
    if ( knew > 0 )
    {

      L410L460(
           n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );

      return f;

    }
    
    // pick the next value of DELTA after a trust region step.
    if ( vquad >= 0 )
    {

      throw std::runtime_error( "a trust region step has failed to reduce Q" );
      L530end(
           n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );

      return f;

    }
    ratio = ( f - fsave ) / vquad;
    if ( ratio <= 0.1 )
    {

      delta = .5 * dnorm;

    }
    else if ( ratio <= .7 )
    {

      // computing MAX
      d__1 = .5 * delta;
      delta = fmax( d__1, dnorm );

    }
    else
    {

      // computing MAX
      d__1 = .5 * delta, d__2 = dnorm + dnorm;
      delta = fmax( d__1, d__2 );

    }
    if ( delta <= rho * 1.5 )
    {

      delta = rho;

    }
    // set KNEW to the index of the next interpolation point to be deleted.
    // computing MAX
    d__2 = 0.1 * delta;
    // computing 2nd power
    d__1 = fmax( d__2, rho );
    rhosq = d__1 * d__1;
    ktemp = 0;
    detrat = 0;
    if ( f >= fsave )
    {

      ktemp = kopt;
      detrat = 1.0;

    }
    i__1 = npt;
    for ( k = 1; k <= i__1; ++k )
    {

      hdiag = 0;
      i__2 = nptm;
      for ( j = 1; j <= i__2; ++j )
      {

        temp = 1.0;
        if ( j < idz )
        {

          temp = -1.0;

        }
        // computing 2nd power
        d__1 = zmat[ k + j * zmat_dim1 ];
        hdiag += temp * ( d__1 * d__1 );

      }
      // computing 2nd power
      d__2 = vlag[ k ];
      temp = ( d__1 = beta * hdiag + d__2 * d__2, fabs( d__1 ) );
      distsq = 0;
      i__2 = n;
      for ( j = 1; j <= i__2; ++j )
      {

        // computing 2nd power
        d__1 = xpt[ k + j * xpt_dim1 ] - xopt[ j ];
        distsq += d__1 * d__1;

      }
      if ( distsq > rhosq )
      {

        // computing 3rd power
        d__1 = distsq / rhosq;
        temp *= d__1 * ( d__1 * d__1 );

      }
      if ( temp > detrat && k != ktemp )
      {

        detrat = temp;
        knew = k;

      }

    }
    if ( knew == 0 )
    {

      L460L490(
           n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );
           return f;

    }

    L410L460(
           n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );

    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L310L410( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::L410L460(
       int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, 
       int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, 
       double *xnew, double *xpt, double *fval, double *gq, double *hq, 
       double *pq, double *bmat, double *zmat, int32_t *ndim, double *d__, 
       double *vlag, double *w, 
       const gkg::PowellNewUOAFunction& func, bool verbose,
       int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, 
       int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, 
       int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, int32_t j, 
       int32_t k, int32_t ih, int32_t nf, int32_t nh, int32_t ip, int32_t jp, 
       int32_t np, int32_t nfm, int32_t idz, int32_t ipt, int32_t jpt, 
       int32_t nfmm, int32_t knew, int32_t kopt , int32_t nptm, int32_t ksave, 
       int32_t nfsav, int32_t itemp, int32_t ktemp, int32_t itest, 
       int32_t nftest, double d__1, double d__2, double d__3, double f, 
       double dx, double dsq, double rho, double sum, double fbeg, double diff, 
       double beta, double gisq, double temp, double suma, double sumb, 
       double fopt, double bsum, double gqsq, double xipt, double xjpt, 
       double sumz, double diffa, double diffb, double diffc, double hdiag, 
       double alpha, double delta, double recip, double reciq, double fsave, 
       double dnorm, double ratio, double dstep, double vquad, double tempq, 
       double rhosq, double detrat, double crvmin, double distsq, 
       double xoptsq ) const
{

  try
  {

    update( n, npt, &bmat[ bmat_offset ], &zmat[ zmat_offset ], &idz,
            ndim, &vlag[ 1 ], &beta, &knew, &w[ 1 ] );
    fval[ knew ] = f;
    ih = 0;
    i__1 = n;
    for ( i__ = 1; i__ <= i__1; ++i__ )
    {

      temp = pq[ knew ] * xpt[ knew + i__ * xpt_dim1 ];
      i__2 = i__;
      for ( j = 1; j <= i__2; ++j )
      {

        ++ih;
        hq[ ih ] += temp * xpt[ knew + j * xpt_dim1 ];

      }

    }
    pq[ knew ] = 0;
    // update the other second derivative parameters, and then the
    // gradient vector of the model. Also include the new interpolation
    // point. 
    i__2 = nptm;
    for ( j = 1; j <= i__2; ++j )
    {

      temp = diff * zmat[ knew + j * zmat_dim1 ];
      if ( j < idz )
      {

        temp = -temp;

      }
      i__1 = npt;
      for ( k = 1; k <= i__1; ++k )
      {

        pq[ k ] += temp * zmat[ k + j * zmat_dim1 ];

      }

    }
    gqsq = 0;
    i__1 = n;
    for ( i__ = 1; i__ <= i__1; ++i__ )
    {

      gq[ i__ ] += diff * bmat[ knew + i__ * bmat_dim1 ];
      // computing 2nd power 
      d__1 = gq[ i__ ];
      gqsq += d__1 * d__1;
      xpt[ knew + i__ * xpt_dim1 ] = xnew[ i__ ];

    }
    // if a trust region step makes a small change to the objective
    // function, then calculate the gradient of the least Frobenius norm
    // interpolant at XBASE, and store it in W, using VLAG for a vector
    // of right hand sides. 
    if ( ksave == 0 && delta == rho )
    {

      if ( fabs( ratio ) > .01 )
      {

        itest = 0;

      }
      else
      {

        i__1 = npt;
        for ( k = 1; k <= i__1; ++k )
        {

          vlag[ k ] = fval[ k ] - fval[ kopt ];

        }
        gisq = 0;
        i__1 = n;
        for ( i__ = 1; i__ <= i__1; ++i__ )
        {

          sum = 0;
          i__2 = npt;
          for ( k = 1; k <= i__2; ++k )
          {

            sum += bmat[ k + i__ * bmat_dim1 ] * vlag[ k ];

          }
          gisq += sum * sum;
          w[ i__ ] = sum;

        }
        // test whether to replace the new quadratic model by the
        // least Frobenius norm interpolant, making the replacement
        // if the test is satisfied. 
        ++itest;
        if ( gqsq < gisq * 100. )
        {

          itest = 0;

        }
        if ( itest >= 3 )
        {

          i__1 = n;
          for ( i__ = 1; i__ <= i__1; ++i__ )
          {

            gq[ i__ ] = w[ i__ ];
           
          }
          i__1 = nh;
          for ( ih = 1; ih <= i__1; ++ih )
          {

            hq[ ih ] = 0;

          }
          i__1 = nptm;
          for ( j = 1; j <= i__1; ++j )
          {

            w[ j ] = 0;
            i__2 = npt;
            for ( k = 1; k <= i__2; ++k )
            {

              w[ j ] += vlag[ k ] * zmat[ k + j * zmat_dim1 ];

            }
            if ( j < idz )
            {

              w[ j ] = -w[ j ];

            }

          }
          i__1 = npt;
          for ( k = 1; k <= i__1; ++k )
          {

            pq[ k ] = 0;
            i__2 = nptm;
            for ( j = 1; j <= i__2; ++j )
            {

              pq[ k ] += zmat[ k + j * zmat_dim1 ] * w[ j ];

            }

          }

          itest = 0;

        }

      }

    }
    if ( f < fsave )
    {

      kopt = knew;

    }
    // if a trust region step has provided a sufficient decrease in F,
    // then branch for another trust region calculation. The case
    // KSAVE>0 occurs when the new function value was calculated by a
    // model step. 
    if ( f <= fsave + 0.1 * vquad )
    {

      L100L120( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );
           return f;

    }

    if ( ksave > 0 )
    {

      L100L120( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );
           return f;

    }
    // alternatively, find out if the interpolation points are close
    // enough to the best point so far. 
    knew = 0;

   L460L490( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq );

    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L410L460( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::L460L490(
       int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, 
       int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, 
       double *xnew, double *xpt, double *fval, double *gq, double *hq, 
       double *pq, double *bmat, double *zmat, int32_t *ndim, double *d__, 
       double *vlag, double *w, 
       const gkg::PowellNewUOAFunction& func, bool verbose,
       int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, 
       int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, 
       int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, int32_t j, 
       int32_t k, int32_t ih, int32_t nf, int32_t nh, int32_t ip, int32_t jp, 
       int32_t np, int32_t nfm, int32_t idz, int32_t ipt, int32_t jpt, 
       int32_t nfmm, int32_t knew, int32_t kopt , int32_t nptm, int32_t ksave, 
       int32_t nfsav, int32_t itemp, int32_t ktemp, int32_t itest, 
       int32_t nftest, double d__1, double d__2, double d__3, double f, 
       double dx, double dsq, double rho, double sum, double fbeg, double diff, 
       double beta, double gisq, double temp, double suma, double sumb, 
       double fopt, double bsum, double gqsq, double xipt, double xjpt, 
       double sumz, double diffa, double diffb, double diffc, double hdiag, 
       double alpha, double delta, double recip, double reciq, double fsave, 
       double dnorm, double ratio, double dstep, double vquad, double tempq, 
       double rhosq, double detrat, double crvmin, double distsq, 
       double xoptsq ) const
{

  try
  {

    distsq = delta * 4. * delta;
    i__2 = npt;
    for ( k = 1; k <= i__2; ++k )
    {

      sum = 0;
      i__1 = n;
      for ( j = 1; j <= i__1; ++j )
      {

        // computing 2nd power 
        d__1 = xpt[ k + j * xpt_dim1 ] - xopt[ j ];
        sum += d__1 * d__1;
      }
      if ( sum > distsq )
      {

        knew = k;
        distsq = sum;

      }

    }
    // if KNEW is positive, then set DSTEP, and branch back for the next
    // iteration, which will generate a "model step".
    if ( knew > 0 )
    {

      // computing MAX and MIN
      d__2 = 0.1 * sqrt( distsq ), d__3 = .5 * delta;
      d__1 = fmin( d__2, d__3 );
      dstep = fmax( d__1, rho );
      dsq = dstep * dstep;
      L120L290( n, npt, x, rhobeg, rhoend, ret_nf,
                maxfun, xbase, xopt, xnew, xpt,
                fval, gq, hq, pq, bmat, zmat,
                ndim, d__, vlag, w, func, verbose,
                xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
                zmat_offset,
                i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, 
                ipt, jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, 
                itest, nftest,
                d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
                temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, 
                diffb, diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, 
                ratio, dstep, vquad, tempq, rhosq, detrat, crvmin, distsq, 
                xoptsq );

      return f;

    }
    if ( ratio > 0 )
    {

      L100L120( n, npt, x, rhobeg, rhoend, ret_nf,
                maxfun, xbase, xopt, xnew, xpt,
                fval, gq, hq, pq, bmat, zmat,
                ndim, d__, vlag, w, func, verbose,
                xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
                zmat_offset,
                i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, 
                ipt, jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, 
                itest, nftest,
                d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
                temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, 
                diffb, diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, 
                ratio, dstep, vquad, tempq, rhosq, detrat, crvmin, distsq, 
                xoptsq );

      return f;

    }
    if ( fmax( delta, dnorm ) > rho )
    {

      L100L120( n, npt, x, rhobeg, rhoend, ret_nf,
                maxfun, xbase, xopt, xnew, xpt,
                fval, gq, hq, pq, bmat, zmat,
                ndim, d__, vlag, w, func, verbose,
                xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, 
                zmat_offset,
                i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, 
                ipt, jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, 
                itest, nftest,
                d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
                temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, 
                diffb, diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, 
                ratio, dstep, vquad, tempq, rhosq, detrat, crvmin, distsq, 
                xoptsq );

      return f;

    }

    L490L530(n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq  );

    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L460L490( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::L490L530(
       int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, 
       int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, 
       double *xnew, double *xpt, double *fval, double *gq, double *hq, 
       double *pq, double *bmat, double *zmat, int32_t *ndim, double *d__, 
       double *vlag, double *w, 
       const gkg::PowellNewUOAFunction& func, bool verbose,
       int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, 
       int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, 
       int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, int32_t j, 
       int32_t k, int32_t ih, int32_t nf, int32_t nh, int32_t ip, int32_t jp, 
       int32_t np, int32_t nfm, int32_t idz, int32_t ipt, int32_t jpt, 
       int32_t nfmm, int32_t knew, int32_t kopt , int32_t nptm, int32_t ksave, 
       int32_t nfsav, int32_t itemp, int32_t ktemp, int32_t itest, 
       int32_t nftest, double d__1, double d__2, double d__3, double f, 
       double dx, double dsq, double rho, double sum, double fbeg, double diff, 
       double beta, double gisq, double temp, double suma, double sumb, 
       double fopt, double bsum, double gqsq, double xipt, double xjpt, 
       double sumz, double diffa, double diffb, double diffc, double hdiag, 
       double alpha, double delta, double recip, double reciq, double fsave, 
       double dnorm, double ratio, double dstep, double vquad, double tempq, 
       double rhosq, double detrat, double crvmin, double distsq, 
       double xoptsq ) const
{

  try
  {

    if ( rho > rhoend )
    {

      delta = .5 * rho;
      ratio = rho / rhoend;
      if ( ratio <= 16. )
      {

        rho = rhoend;

      }
      else if ( ratio <= 250. )
      {

        rho = sqrt( ratio ) * rhoend;

      }
      else
      {

        rho = 0.1 * rho;

      }
      delta = fmax( delta, rho );
      L90L100( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq  );

      return f;

    }
    // return from the calculation, after another Newton-Raphson step,
    // if it is too short to have been tried before. 
    if ( knew == -1 )
    {

      L290L310( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq  );

      return f;

    }

    L530end( n, npt, x, rhobeg, rhoend, ret_nf,
           maxfun, xbase, xopt, xnew, xpt,
           fval, gq, hq, pq, bmat, zmat,
           ndim, d__, vlag, w, func, verbose,
           xpt_dim1, xpt_offset, bmat_dim1, bmat_offset, zmat_dim1, zmat_offset,
           i__1, i__2, i__3, i__, j, k, ih, nf, nh, ip, jp, np, nfm, idz, ipt,
           jpt, nfmm, knew, kopt, nptm, ksave, nfsav, itemp, ktemp, itest,
           nftest,
           d__1, d__2, d__3, f, dx, dsq, rho, sum, fbeg, diff, beta, gisq,
           temp, suma, sumb, fopt, bsum, gqsq, xipt, xjpt, sumz, diffa, diffb,
           diffc, hdiag, alpha, delta, recip, reciq, fsave, dnorm, ratio, dstep,
           vquad, tempq, rhosq, detrat, crvmin, distsq, xoptsq  );

    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L490L530( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


inline double gkg::NumericalAnalysisImplementationFactory::L530end(
       int32_t n, int32_t /* npt */, double *x, 
       double /* rhobeg */, double /* rhoend */, 
       int32_t *ret_nf, int32_t /* maxfun */, double* xbase, double *xopt, 
       double* /* xnew */, double* /* xpt */, double* /* fval */,
       double* /* gq */, double* /* hq */, 
       double* /* pq */, double* /* bmat */, double* /* zmat */,
       int32_t* /* ndim */, double* /* d__ */, 
       double* /* vlag */, double* /* w */, 
       const gkg::PowellNewUOAFunction& /* func */, bool /* verbose */,
       int32_t /* xpt_dim1 */, int32_t /* xpt_offset */,
       int32_t /* bmat_dim1 */, 
       int32_t /* bmat_offset */, int32_t /* zmat_dim1 */,
       int32_t /* zmat_offset */, 
       int32_t /* i__1 */, int32_t i__2, int32_t /* i__3 */,
       int32_t i__, int32_t /* j */, 
       int32_t /* k */, int32_t /* ih */, int32_t nf, int32_t /* nh */,
       int32_t /* ip */, int32_t /* jp */, 
       int32_t /* np */, int32_t /* nfm */, int32_t /* idz */,
       int32_t /* ipt */, int32_t /* jpt */, 
       int32_t /* nfmm */, int32_t /* knew */, int32_t /* kopt */,
       int32_t /* nptm */, int32_t /* ksave */, 
       int32_t /* nfsav */, int32_t /* itemp */, int32_t /* ktemp */,
       int32_t /* itest */, 
       int32_t /* nftest */, double /* d__1 */, double /* d__2 */,
       double /* d__3 */, double f, 
       double /* dx */, double /* dsq */, double /* rho */, double /* sum */,
       double /* fbeg */, double /* diff */, 
       double /* beta */, double /* gisq */, double /* temp */,
       double /* suma */, double /* sumb */, 
       double fopt, double /* bsum */, double /* gqsq */, double /* xipt */,
       double /* xjpt */, 
       double /* sumz */, double /* diffa */, double /* diffb */,
       double /* diffc */, double /* hdiag */, 
       double /* alpha */, double /* delta */, double /* recip */,
       double /* reciq */, double /* fsave */, 
       double /* dnorm */, double /* ratio */, double /* dstep */,
       double /* vquad */, double /* tempq */, 
       double /* rhosq */, double /* detrat */, double /* crvmin */,
       double /* distsq */, double /* xoptsq */ ) const
{

  try
  {

    if ( fopt <= f )
    {

      i__2 = n;
      for ( i__ = 1; i__ <= i__2; ++i__ )
      {

        x[ i__ ] = xbase[ i__ ] + xopt[ i__ ];

      }
      f = fopt;

    }

    *ret_nf = nf;
    return f;

  }
  GKG_CATCH( "inline "
             "double gkg::NumericalAnalysisImplementationFactory::L530end( "
             "int32_t n, int32_t npt, double *x, double rhobeg, double rhoend, "
             "int32_t *ret_nf, int32_t maxfun, double *xbase, double *xopt, "
             "double *xnew, double *xpt, double *fval, double *gq, double *hq, "
             "double *pq, double *bmat, double *zmat, int32_t *ndim, "
             "double *d__,  double *vlag, double *w, "
             "const gkg::PowellNewUOAFunction &func, bool verbose, "
             "int32_t xpt_dim1, int32_t xpt_offset, int32_t bmat_dim1, "
             "int32_t bmat_offset, int32_t zmat_dim1, int32_t zmat_offset, "
             "int32_t i__1, int32_t i__2, int32_t i__3, int32_t i__, "
             "int32_t j, int32_t k, int32_t ih, int32_t nf, int32_t nh, "
             "int32_t ip, int32_t jp, int32_t np, int32_t nfm, int32_t idz, "
             "int32_t ipt, int32_t jpt, int32_t nfmm, int32_t knew, "
             "int32_t kopt , int32_t nptm, int32_t ksave, int32_t nfsav, "
             "int32_t itemp, int32_t ktemp, int32_t itest, int32_t nftest, "
             "double d__1, double d__2, double d__3, double f, double dx, "
             "double dsq, double rho, double sum, double fbeg, double diff, "
             "double beta, double gisq, double temp, double suma, double sumb, "
             "double fopt, double bsum, double gqsq, double xipt, double xjpt, "
             "double sumz, double diffa, double diffb, double diffc, "
             "double hdiag, double alpha, double delta, double recip, "
             "double reciq, double fsave, double dnorm, double ratio, "
             "double dstep, double vquad, double tempq, double rhosq, "
             "double detrat, double crvmin, double distsq, double xoptsq ) "
             "const" );

}


#endif
