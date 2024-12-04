#ifndef _gkg_processing_plugin_gsl_lapack_GSLNumAnalysisImplementationFactory_h_
#define _gkg_processing_plugin_gsl_lapack_GSLNumAnalysisImplementationFactory_h_


#include <gkg-processing-numericalanalysis/NumAnalysisImplementationFactory.h>
#include <gkg-core-pattern/SingletonAccessor.h>



namespace gkg
{


class GSLNumericalAnalysisImplementationFactory : 
           public NumericalAnalysisImplementationFactory,
           public SingletonAccessor< GSLNumericalAnalysisImplementationFactory,
                                     NumericalAnalysisImplementationFactory >
{

  public:

    ~GSLNumericalAnalysisImplementationFactory();

    //// real linear algebra implementations

    // matrix
    MatrixImplementation* 
      createMatrixImplementation(
        Matrix* matrix,
        int32_t size1,
        int32_t size2 ) const;

    // vector
    VectorImplementation* 
      createVectorImplementation(
        Vector* vector,
        int32_t size ) const;

    // singular value decomposition A = U S V^T
    void getSingularValueDecomposition( Matrix& A,
                                        Matrix& V,
                                        Vector& S ) const;

    // singular value decomposition A = U S V^T
    void getSingularValueDecomposition2( const Matrix& A,
                                         Matrix& U,
                                         Vector& S,
                                         Matrix& V ) const;

    // solution of equation Ax=b with singular value decomposition A = U S V^T
    void getSingularValueSolution( const Matrix& U,
                                   const Vector& S,
                                   const Matrix& V,
                                   const Vector& b,
                                   Vector& x ) const;

    // eigen system decomposition A = V D V^T
    void getEigenSystem( const Matrix& A,
                         Vector& D,
                         Matrix& V ) const;
    void sortEigenSystem( Vector& D,
                          Matrix& V,
                          EigenSystemSortType 
                            eigenSystemSortType ) const;

    // non-symmetric eigen system decomposition A = V D V^T
    // by computing the eigenvalues and eigenvectors of the
    // Vandermonde matrix V(x;i,j)=x_i^(n-j)
    void getNonSymmetricEigenSystem( const Matrix& A,
                                     CVector& D,
                                     CMatrix& V ) const;
    void sortNonSymmetricEigenSystem( CVector& D,
                                      CMatrix& V,
                                      EigenSystemSortType 
                                        eigenSystemSortType ) const;

    // efficient computing eigen values of a large matrix A 
    void getEigenValues( const Matrix& A,
                         Vector& D,
                         EigenSystemSortType eigenSystemSortType ) const;

    // Householder solver for linear system Ax=b
    void getHouseHolderSolution( const Matrix& A,
                                 const Vector& b,
                                 Vector& x ) const;

    // LU solver for linear system Ax=b
    void getLUSolution( const Matrix& A, const Vector& b, Vector& x ) const;

    // Cholesky decomposition of a matrix A = L L^T with L being a lower 
    // triangular matrix
    void getCholeskyDecomposition( const Matrix& A, Matrix& L ) const;


    // inverse of a square matrix
    void getInverse( const Matrix& A, Matrix& Aplus ) const;


    //// complex linear algebra implementations

    // matrix
    CMatrixImplementation*
      createCMatrixImplementation(
        CMatrix* cmatrix,
        int32_t size1,
        int32_t size2 ) const;

    // vector
    CVectorImplementation*
      createCVectorImplementation(
        CVector* cvector,
        int32_t size ) const;

    // E singular value decomposition
    void getESingularValueDecomposition( const CMatrix& A,
                                         CMatrix& U,
                                         Vector& S,
                                         CMatrix& V ) const;

    // eigen system decomposition A = V D V^T
    void getEigenSystem( const CMatrix& A,
                         Vector& D,
                         CMatrix& V ) const;
    void sortEigenSystem( Vector& D,
                          CMatrix& V,
                          EigenSystemSortType 
                            eigenSystemSortType ) const;

    //// random number generation

    // random generator
    RandomGeneratorImplementation*
      createRandomGeneratorImplementation(
        RandomGenerator* randomGenerator,
        RandomGenerator::Type type ) const;

    // gaussian random number
    double getGaussianPdf( double x,
                           double mean,
                           double sigma ) const;
    double getGaussianCdfP( double x,
                            double mean,
                            double sigma ) const;
    double getGaussianCdfQ( double x,
                            double mean,
                            double sigma ) const;
    double getGaussianCdfInverseP( double P,
                                   double mean,
                                   double sigma ) const;
    double getGaussianCdfInverseQ( double Q,
                                   double mean,
                                   double sigma ) const;
    double getGaussianRandomNumber(
      const RandomGenerator& randomGenerator,
      double mean,
      double sigma ) const;

    // uniform random number
    double getUniformPdf( double x,
                          double a,
                          double b ) const;
    double getUniformCdfP( double x,
                           double a,
                           double b ) const;
    double getUniformCdfQ( double x,
                           double a,
                           double b ) const;
    double getUniformCdfInverseP( double P,
                                  double mean,
                                  double sigma ) const;
    double getUniformCdfInverseQ( double Q,
                                  double mean,
                                  double sigma ) const;
    
    uint32_t getUniformRandomUInt32(
      const RandomGenerator& randomGenerator,
      uint32_t maximum ) const; 
				 
    double getUniformRandomNumber(
      const RandomGenerator& randomGenerator,
      double a,
      double b ) const;

    // gamma random number
    double getGammaPdf( double x,
                        double a,
                        double b ) const;
    double getGammaCdfP( double x,
                         double a,
                         double b ) const;
    double getGammaCdfQ( double x,
                         double a,
                         double b ) const;
    double getGammaCdfInverseP( double P,
                                double a,
                                double b ) const;
    double getGammaCdfInverseQ( double Q,
                                double a,
                                double b ) const;
    double getGammaRandomNumber(
      const RandomGenerator& randomGenerator,
      double a,
      double b ) const;

    // t random number
    double getTDistPdf( double x,
                        double nu ) const;
    double getTDistCdfP( double x,
                         double nu ) const;
    double getTDistCdfQ( double x,
                         double nu ) const;
    double getTDistCdfInverseP( double P,
                                double nu ) const;
    double getTDistCdfInverseQ( double Q,
                                double nu ) const;
    double getTDistRandomNumber(
      const RandomGenerator& randomGenerator,
      double nu ) const;

    // f random number
    double getFDistPdf( double x,
                        double nu1,
                        double nu2 ) const;
    double getFDistCdfP( double x,
                         double nu1,
                         double nu2 ) const;
    double getFDistCdfQ( double x,
                         double nu1,
                         double nu2 ) const;
    double getFDistRandomNumber(
      const RandomGenerator& randomGenerator,
      double nu1,
      double nu2 ) const;

    // chi2 random number
    double getChi2CdfP( double x,
                        double nu ) const;
    double getChi2CdfQ( double x,
                        double nu ) const;
    double getChi2CdfInverseP( double P,
                               double nu ) const;
    double getChi2CdfInverseQ( double Q,
                               double nu ) const;

    //// multilinear fit
    void getMultiLinearFit( const Matrix& X,
                            const Vector& y,
                            Vector& c,
                            Matrix& covariance,
                            double& chisq ) const;
    void getWeightedMultiLinearFit( const Matrix& X,
                                    const Vector& w,
                                    const Vector& y,
                                    Vector& c,
                                    Matrix& covariance,
                                    double& chisq ) const;

    //// complex trigonometry
    std::complex< double >
      arccos( const std::complex< double >& z ) const;
    std::complex< double >
      arcsin( const std::complex< double >& z ) const;
    std::complex< double >
      arctan( const std::complex< double >& z ) const;

    //// Gamma function
    double getGammaFunction( const double& x ) const;
    float getGammaFunction( const float& x ) const;

    //// Bessel's functions
    double getRegularBesselFunctionJ0( const double& x ) const;
    double getRegularBesselFunctionJ1( const double& x ) const;

    double getRegularBesselFunctionI0( const double& x ) const;
    double getRegularBesselFunctionI1( const double& x ) const;
    double getRegularBesselFunctionIn( const int& n, const double& x ) const;

    double getRegularBesselFunctionFractionalOrder( double l, double x ) const;

    double getZeroBesselFunctionJ0( const uint32_t& k ) const;
    double getZeroBesselFunctionJ1( const uint32_t& k ) const;
    
    double getZeroBesselFunctionJn( const double& n,
                                    const uint32_t& k ) const;

    double getBesselDerivative( double n,
                                double x,
                                double h,
                                double* error = 0 ) const;

    // Hypergeometric functions
    double getHypergeometricFunction1F1( double a, double b, double x ) const;

    /// non-linear least-squares fitting of parametric function y = f( x; a )
    //  which derivative is known and where a is the searched parameters
    //  in return, get the number of iterations performed
    int32_t getLevenbergMarquardtFit( const Matrix& x,
                                      const Vector& y,
                                      const Vector& w,
                                      LevenbergMarquardtFunction& f,
                                      Matrix& covariance,
                                      double& chiSquarePerDegreeOfFreedom,
                                      int32_t maximumIterationCount,
                                      double absoluteError,
                                      double relativeError ) const;

    // Nelder-Mead Simplex optimizer: optimize the parameter set p to get
    // the minimum value of f( x; p ); the initial parameters and delta 
    // parameters are provided as input parameters and also as output
    // results
    int32_t getNelderMeadSimplexOptimumParameters(
                                                  Vector& parameters,
                                                  Vector& deltaParameters,
                                                  double &testSize,
                                                  NelderMeadSimplexFunction& f,
                                                  int32_t maximumIterationCount,
                                                  double maximumTestSize,
                                                  bool verbose ) const;

    // Fletcher-Reeves Conjugate Gradient optimizer: optimize the parameter set 
    // p to get the minimum value of f( p ); the initial parameters are
    // provided as input parameters and also as output results
    int32_t getFletcherReevesConjugateGradientOptimumParameters( 
                                     Vector& parameters,
                                     FletcherReevesConjugateGradientFunction& f,
                                     int32_t maximumIterationCount,
                                     double stepSize,
                                     double maximumTestGradient,
                                     double maximumTolerance,
                                     bool verbose ) const;

    // Constrained Non Linear Programming optimizer : optimize the parameter set
    // p to get the minimum value of f( x; p ); the initial parameters and delta
    // parameters are provided as input parameters and also as output
    // results
    // optimizerParameters are set according to the optimizer type.
    // for NelderMeadSimplex
    //     NelderMeadSimplex2
    //     NelderMeadSimplex2Rand:
    //   - optimizerParameters[ 0 ] -> maximum iteration count
    //   - optimizerParameters[ 1 ] -> maximum simplex size
    //   - optimizerParameters[ 2 .. <parameterCount+2> ] -> variations of
    //                                                       parameters
    //
    // for FletcherReevesConjugateGradient
    //     PolakRibiereConjugateGradient
    //     BroydenFletcherGoldfarbShanno
    //     SteepestDescent
    //     BroydenFletcherGoldfarbShanno2:
    //   - optimizerParameters[ 0 ] -> maximum iteration count
    //   - optimizerParameters[ 2 ] -> step size of first trial step
    //   - optimizerParameters[ 2 ] -> line linimization tolerance
    //   - optimizerParameters[ 3 ] -> precision
    int32_t getConstrainedNonLinearProgrammingOptimumParameters( 
             NLPOptimizerType optimizerType,
             Vector& parameters,
             const std::vector< RCPointer< OptimizerConstraint > >& constraints,
             NonLinearProgrammingFunction& f,
             const std::vector< double >& optimizerParameters,
             bool verbose ) const;


    //
    // Other constrained Non Linear Programming optimizer
    // Various algorithms:
    //   - local without derivatives:
    //       LocalWoDerivative_COBYLA,
    //       LocalWoDerivative_NEWUOA,
    //       LocalWoDerivative_NEWUOA_BOUND,
    //
    //   - local with derivatives
    //       LocalWithDerivative_MMA
    //
    // Lower and upper bounds of the parameters can be provided. If the vectors
    // are let empty, -HUGE_VAL and +HUGE_VAL are automatically set as
    // default lower and upper bounds
    //
    // The tolerance of both inequalities and equalities can be set
    // independently. If those vectors are empty, the setting it to 1e-8 by
    // default.
    //
    //  The relative tolerance can also be set.
    //
    // The achievedIterationCount can be provided on output, as well as
    // the reahced minimum objective function value.
    //
    void getConstrainedNonLinearProgrammingOptimumParameters2( 
             NLPOptimizerType2 optimizerType,
             Vector& parameters,
             const std::vector< RCPointer< NonLinearProgrammingFunction > >&
                                                          inequalityConstraints,
             const std::vector< RCPointer< NonLinearProgrammingFunction > >&
                                                          equalityConstraints,
             NonLinearProgrammingFunction& f,
             const std::vector< double >& parameterLowerBounds,
             const std::vector< double >& parameterUpperBounds,
             const std::vector< double >& inequalityTolerances,
             const std::vector< double >& equalityTolerances,
             double relativeTolerance,
             int32_t* achievedIterationCount,
             double* reachedMinimumObjectiveValue,
             bool verbose ) const;

    /// convex quadratic programming solver
    // This method is implements the Mehrotra-Gondzio primal-dual interior point
    // method to solve strictly convex quadratic programs 
    //
    //  The problem is in the form
    //  argmin_x { 1/2 x^T Q x + q^T x }
    //    subject to:
    //          A x = b
    //          C x >= d
    //
    bool getConvexQuadraticProgrammingSolution(
                      const Matrix& Q,
                      const Vector& q,
                      const Matrix& A,
                      const Vector& b,
                      const Matrix& C,
                      const Vector& d,
                      Vector& x,
                      int32_t maximumIterationCount,
                      double epsilonGap,
                      double epsilonResiduals,
                      double epsilonInfeasible,
                      bool verbose ) const;
 
    bool getConvexQuadraticProgrammingSolution(
                      const Matrix& Q,
                      const Vector& q,
                      const Matrix& C,
                      const Vector& d,
                      Vector& x,
                      int32_t maximumIterationCount,
                      double epsilonGap,
                      double epsilonResiduals,
                      double epsilonInfeasible,
                      bool verbose ) const;

    // Simulated Annealing optimizer: optimize the parameters p to get the 
    // minimum value of f(p)
    int32_t getSimulatedAnnealingOptimumConfiguration(
                     SimulatedAnnealingConfiguration& configuration,
                     int32_t trialCountPerStep,
                     int32_t iterationCountPerTemperature,
                     double maximumStepSize,
                     const SimulatedAnnealingConfiguration::BoltzmannParameters&
                                                            boltzmannParameters,
                     const RandomGenerator& randomGenerator,
                     bool verbose ) const;


    //// Legendre polynomial(s)

    // Legendre Pl(x) function
    double getLegendrePolynomial( int32_t l, double x ) const;

    // Legendre spherical Plm(x) function
    double getLegendrePolynomialSpherical( int32_t l,
                                           int32_t m,
                                           double x ) const;

    //// Spherical harmonic(s)

    // careful: in physics, theta is azimuth  and phi is colatitule,
    // whereas for spherical harmonics, theta is colatitude and phi is
    // azimuth, so theta and phi must be swapped in SH function call(s)

    // normalized spherical harmonic of order l and phase factor m
    // i.e. Y(l,m)( theta, phi ) = sqrt( ( ( 2l + 1 )( l - m )! ) /
    //                                   ( 4 Pi ( l + m )! ) ) *
    //                             P(l,m)( cos(theta) ) * exp(i m phi ) 
    // with l= 0, 2, 3, ..., lmax
    //      m=-l,...,0,...,+l
    //      theta in [0;Pi]
    //      phi in [0;2Pi[
    std::complex< double > getSphericalHarmonic( int32_t l,
                                                 int32_t m,
                                                 double theta,
                                                 double phi ) const;
    // symmetrical spherical harmonic of order j = ( l^2 + l ) / 2 + m
    // i.e. 
    // Yj = | sqrt(2) * Re( Y(l,m) )        if -l <= m < 0
    //      | Y(l,0)                        if m = 0
    //      | sqrt(2) * Im( Y(l,m) )        if 0 < m <= l
    // with theta in [0;Pi]
    //      phi in [0;2Pi[
    double getSymmetricalSphericalHarmonic( int32_t j,
                                            double theta,
                                            double phi ) const;

    // See Descoteaux et al TMI 2008 Eq. 13
    double
      getSharpeningDeconvolutionTransformCoefficient( int32_t l,
						      double e1,
						      double e2) const;
    // See Tournier et al NeuroImage 2007, Appendix
    double getDiracCoefficient( int32_t j,
				double theta,
				double phi ) const;
      
    // processing (l,m) order and phase factor of symmetrical spherical
    // harmonic from j index
    void getSymmetricalSphericalHarmonicLM( int32_t j,
                                            int32_t& l,
                                            int32_t& m ) const;

    // supposes n is odd
    // n!! = n*(n-2)*...*3*1
    double getOddFactorial( double n ) const;
    
    // supposes n is even
    // n!! = n*(n-2)*...*4*2*1
    double getEvenFactorial( double n ) const;

    // Pochhammer symbol
    double getPochhammer( double a, double x ) const;

    /// Error function(s)

    // compute erf(x) = ( 2 / sqrt(PI) ) x Int(0,x)( exp( -t^2 )dt ) 
    double getErf( double x ) const;

    // compute erfc(x) = 1 - erf( x )
    double getErfC( double x ) const;

    // compute erfZ(x) = ( 1 / 2.PI ) x exp( -x^2 / 2 ) 
    double getErfZ( double x ) const;

    // compute erfQ(x) = ( 1 / 2.PI ) x Int(x,+inf)( exp( -t^2 / 2 )dt ) 
    double getErfQ( double x ) const;

    // compute Dawson's integral  Exp[-x^2] Integral[ Exp[t^2], {t,0,x}]
    double getDawsonIntegral( double x ) const;

    /// Numerical integration

    // QAGS integrator : process int_a^b[ f(x)dx ]
    //  Q - quadrature routine
    //  A - adaptative integrator
    //  G - general integrand
    //  S - singularities can be more readily integrated
    double getQAGSIntegral( const IntegrableFunction& integrableFunction,
                            double a,
                            double b,
                            double epsilonAbsolute,
                            double epsilonRelative,
                            double* error = 0,
                            int32_t subIntervalCount = 1000 ) const;

    //// Function interpolation

    // funtion interpolator
    FunctionInterpolatorImplementation*
      createFunctionInterpolatorImplementation(
        FunctionInterpolator* functionInterpolator,
        FunctionInterpolator::Type type,
        const std::vector< double >& xs,
        const std::vector< double >& ys ) const;

    // funtion interpolator
    FunctionInterpolatorImplementation*
      createFunctionInterpolatorImplementation(
        FunctionInterpolator* functionInterpolator,
        FunctionInterpolator::Type type,
        const Vector& xs,
        const Vector& ys ) const;

    /// Polynomial roots
    void getPolynomialRoots( const Vector& polynomCoefficients,
                             CVector& roots ) const;

    /// Gaussian Mixture Model(s)

    // Gaussian Mixture Model inference using Expectation Maximization algo
    // the output Gaussian mixture is stored in a matrix:
    // - first column = means
    // - second column = covariances
    // - third column = scales
    
    double 
      getExpectationMaximizationGaussianMixtureModel(
        const Matrix& inputData,
        int32_t gaussianDistributionCount,
        std::vector< Vector >& distributionMeans,
        std::vector< Matrix >& distributionStandardDeviations,
        std::vector< double >& distributionScales,
        double tolerance,
        int32_t maximumIterationCount,
        bool verbose ) const;

    double 
      getExpectationMaximizationGaussianMixtureModelAndLabels(
        const Matrix& inputData,
        int32_t gaussianDistributionCount,
        std::vector< Vector >& distributionMeans,
        std::vector< Matrix >& distributionStandardDeviations,
        std::vector< double >& distributionScales,
        std::vector< int32_t >& labels,
        Matrix& probabilities,
        double& bic,
        double tolerance,
        int32_t maximumIterationCount,
        bool verbose ) const;

    double
      getMultivariateGaussianPdf(
        const Vector& values,
        const Vector& means,
        const Matrix& standardDeviations,
        double scale ) const;
    double
      getMultivariateGaussianLogPdf(
        const Vector& values,
        const Vector& means,
        const Matrix& standardDeviations,
        double scale ) const;

  protected:

    friend class Singleton< GSLNumericalAnalysisImplementationFactory >;

    GSLNumericalAnalysisImplementationFactory();

};


}


#endif
