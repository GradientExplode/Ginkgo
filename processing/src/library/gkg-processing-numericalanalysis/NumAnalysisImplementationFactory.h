#ifndef _gkg_processing_numericalanalysis_NumAnalysisImplementationFactory_h_
#define _gkg_processing_numericalanalysis_NumAnalysisImplementationFactory_h_


#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/FunctionInterpolator.h>
#include <gkg-processing-numericalanalysis/SimulatedAnnealingConfiguration.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-cppext/StdInt.h>
#include <complex>
#include <vector>


namespace gkg
{


class Matrix;
class MatrixImplementation;
class Vector;
class VectorImplementation;
class CMatrix;
class CMatrixImplementation;
class CVector;
class CVectorImplementation;
class LevenbergMarquardtFunction;
class NelderMeadSimplexFunction;
class FletcherReevesConjugateGradientFunction;
class PowellNewUOAFunction;
class NonLinearProgrammingFunction;
class OptimizerConstraint;
class IntegrableFunction;
class DifferentiableFunction;


class NumericalAnalysisImplementationFactory
{

  public:

    enum EigenSystemSortType
    {

      Ascending,
      Descending,
      AbsoluteAscending,
      AbsoluteDescending

    };

    enum NLPOptimizerType
    {

      // without derivative
      NelderMeadSimplex,
      NelderMeadSimplex2,
      NelderMeadSimplex2Rand,

      // with derivative
      FletcherReevesConjugateGradient,
      PolakRibiereConjugateGradient,
      BroydenFletcherGoldfarbShanno,
      SteepestDescent,
      BroydenFletcherGoldfarbShanno2

    };

    enum NLPOptimizerType2
    {

      //////////////////////////////////////////////////////////////////////////
      // without derivative
      //////////////////////////////////////////////////////////////////////////

      // Constrained Optimization BY Linear Approximations
      LocalWoDerivative_COBYLA,
      LocalWoDerivative_NEWUOA,
      LocalWoDerivative_NEWUOA_BOUND,

      //////////////////////////////////////////////////////////////////////////
      // without derivative
      //////////////////////////////////////////////////////////////////////////

      // Method of Moving Asymptotes
      LocalWithDerivative_MMA

    };

    virtual ~NumericalAnalysisImplementationFactory();


    //// real implementations

    // matrix
    virtual MatrixImplementation*
      createMatrixImplementation(
        Matrix* matrix,
        int32_t size1,
        int32_t size2 ) const = 0;

    // vector
    virtual VectorImplementation*
      createVectorImplementation(
        Vector* vector,
        int32_t size ) const = 0;

    // min, max operators
    virtual double getMinimumItem( const Vector& vector,
                                   int32_t* index = 0 ) const;
    virtual double getMaximumItem( const Vector& vector,
                                   int32_t* index = 0 ) const;
    virtual double getAbsMinimumItem( const Vector& vector,
                                      int32_t* index = 0 ) const;
    virtual double getAbsMaximumItem( const Vector& vector,
                                      int32_t* index = 0 ) const;

    virtual double getMinimumItem( const Matrix& matrix,
                                   int32_t* index1 = 0,
                                   int32_t* index2 = 0 ) const;
    virtual double getMaximumItem( const Matrix& matrix,
                                   int32_t* index1 = 0,
                                   int32_t* index2 = 0 ) const;
    virtual double getAbsMinimumItem( const Matrix& matrix,
                                      int32_t* index1 = 0,
                                      int32_t* index2 = 0 ) const;
    virtual double getAbsMaximumItem( const Matrix& matrix,
                                      int32_t* index1 = 0,
                                      int32_t* index2 = 0 ) const;

    // singular value decomposition A = U S V^T
    virtual void getSingularValueDecomposition( Matrix& A,
                                                Matrix& V,
                                                Vector& S ) const = 0;

    // singular value decomposition A = U S V^T
    virtual void getSingularValueDecomposition2( const Matrix& A,
                                                 Matrix& U,
                                                 Vector& S,
                                                 Matrix& V ) const = 0;

    // solution of equation Ax=b with singular value decomposition A = U S V^T
    virtual void getSingularValueSolution( const Matrix& U,
                                           const Vector& S,
                                           const Matrix& V,
                                           const Vector& b,
                                           Vector& x ) const = 0;

    // Nth order fit y = ao * x^0 + a1 * x^1 + ... + aN * x^N
    virtual void getSingularValueNthOrderFit( const Vector& x,
                                              const Vector& y,
                                              Vector& a ) const;

    // eigen system decomposition A = V D V^T
    virtual void getEigenSystem( const Matrix& A,
                                 Vector& D,
                                 Matrix& V ) const = 0;
    virtual void sortEigenSystem( Vector& D,
                                  Matrix& V,
                                  EigenSystemSortType 
                                    eigenSystemSortType ) const = 0;

    // non-symmetric eigen system decomposition A = V D V^T
    // by computing the eigenvalues and eigenvectors of the
    // Vandermonde matrix V(x;i,j)=x_i^(n-j)
    virtual void getNonSymmetricEigenSystem(
                                 const Matrix& A,
                                 CVector& D,
                                 CMatrix& V ) const = 0;
    virtual void sortNonSymmetricEigenSystem(
                                  CVector& D,
                                  CMatrix& V,
                                  EigenSystemSortType 
                                    eigenSystemSortType ) const = 0;

    // efficient computing eigen values of a large matrix A 
    virtual void getEigenValues( const Matrix& A,
                                 Vector& D,
                                 EigenSystemSortType 
                                    eigenSystemSortType ) const = 0;

    // Householder solver for linear system AX=B
    virtual void getHouseHolderSolution( const Matrix& A,
                                         const Vector& B,
                                         Vector& X ) const = 0;

    // LU solver for linear system AX=B
    virtual void getLUSolution( const Matrix& A, 
                                const Vector& B,
                                Vector& X ) const = 0;

    // Cholesky decomposition of a matrix A = L L^T with L being a lower 
    // triangular matrix
    virtual void getCholeskyDecomposition( const Matrix& A,
                                           Matrix& L ) const = 0;

    // condition number of a real matrix
    virtual double getConditionNumber( const Matrix& A ) const;

    // Frobenius norm of a complex matrix
    virtual double getFrobeniusNorm( const Matrix& A ) const;

    // Euclidean norm of a complex matrix
    virtual double getEuclideanNorm( const Matrix& A ) const;


    // inverse of a square matrix
    virtual void getInverse( const Matrix& A,
                             Matrix& Aplus ) const = 0;

    // Moore-Penrose pseudo-inverse
    virtual void getMoorePenrosePseudoInverse( const Matrix& A,
                                               Matrix& Aplus ) const;


    // Thikonov pseudo-inverse
    virtual void getThikonovPseudoInverse( const Matrix& A,
                                           const Matrix& R,
                                           double regularizationFactor,
                                           Matrix& Aplus ) const;

    // power of of matrix
    virtual void getPower( const Matrix& A,
                           double power,
                           Matrix& P ) const;

    //// complex implementations

    // matrix
    virtual CMatrixImplementation*
      createCMatrixImplementation(
        CMatrix* cmatrix,
        int32_t size1,
        int32_t size2 ) const = 0;

    // vector
    virtual CVectorImplementation*
      createCVectorImplementation(
        CVector* cvector,
        int32_t size ) const = 0;

    // E singular value decomposition
    virtual void getESingularValueDecomposition( const CMatrix& A,
                                                 CMatrix& U,
                                                 Vector& S,
                                                 CMatrix& V ) const = 0;

    // eigen system decomposition A = V D V^T
    virtual void getEigenSystem( const CMatrix& A,
                                 Vector& D,
                                 CMatrix& V ) const = 0;
    virtual void sortEigenSystem( Vector& D,
                                  CMatrix& V,
                                  EigenSystemSortType 
                                    eigenSystemSortType ) const = 0;

    // condition number of a complex matrix
    virtual double getConditionNumber( const CMatrix& A ) const;

    // Frobenius norm of a complex matrix
    virtual double getFrobeniusNorm( const CMatrix& A ) const;

    // Euclidean norm of a complex matrix
    virtual double getEuclideanNorm( const CMatrix& A ) const;

    // Moore-Penrose pseudo-inverse
    virtual void getMoorePenrosePseudoInverse( const CMatrix& A,
                                               CMatrix& Aplus ) const;


    //// random number generation

    // random generator
    virtual RandomGeneratorImplementation*
      createRandomGeneratorImplementation(
        RandomGenerator* randomGenerator,
        RandomGenerator::Type type ) const = 0;

    // gaussian random number
    virtual double getGaussianPdf( double x,
                                   double mean,
                                   double sigma ) const = 0;
    virtual double getGaussianCdfP( double x,
                                    double mean,
                                    double sigma ) const = 0;
    virtual double getGaussianCdfQ( double x,
                                    double mean,
                                    double sigma ) const = 0;
    virtual double getGaussianCdfInverseP( double P,
                                           double mean,
                                           double sigma ) const = 0;
    virtual double getGaussianCdfInverseQ( double Q,
                                           double mean,
                                           double sigma ) const = 0;
    virtual double getGaussianRandomNumber(
              const RandomGenerator& randomGenerator,
              double mean,
              double sigma ) const = 0;

    // uniform random number
    virtual double getUniformPdf( double x,
                                  double a,
                                  double b ) const = 0;
    virtual double getUniformCdfP( double x,
                                   double a,
                                   double b ) const = 0;
    virtual double getUniformCdfQ( double x,
                                   double a,
                                   double b ) const = 0;
    virtual double getUniformCdfInverseP( double P,
                                          double mean,
                                          double sigma ) const = 0;
    virtual double getUniformCdfInverseQ( double Q,
                                          double mean,
                                          double sigma ) const = 0;
					  
    virtual uint32_t getUniformRandomUInt32(
              const RandomGenerator& randomGenerator,
              uint32_t maximum ) const = 0;
      				  
    virtual double getUniformRandomNumber(
              const RandomGenerator& randomGenerator,
              double a,
              double b ) const = 0;

    // gamma random number
    virtual double getGammaPdf( double x,
                                double a,
                                double b ) const = 0;
    virtual double getGammaCdfP( double x,
                                 double a,
                                 double b ) const = 0;
    virtual double getGammaCdfQ( double x,
                                 double a,
                                 double b ) const = 0;
    virtual double getGammaCdfInverseP( double P,
                                        double a,
                                        double b ) const = 0;
    virtual double getGammaCdfInverseQ( double Q,
                                        double a,
                                        double b ) const = 0;
    virtual double getGammaRandomNumber(
              const RandomGenerator& randomGenerator,
              double a,                  // shape or k
              double b ) const = 0;      // scale or theta

    // t random number
    virtual double getTDistPdf( double x,
                                double nu ) const = 0;
    virtual double getTDistCdfP( double x,
                                 double nu ) const = 0;
    virtual double getTDistCdfQ( double x,
                                 double nu ) const = 0;
    virtual double getTDistCdfInverseP( double P,
                                        double nu ) const = 0;
    virtual double getTDistCdfInverseQ( double Q,
                                        double nu ) const = 0;
    virtual double getTDistRandomNumber(
              const RandomGenerator& randomGenerator,
              double nu ) const = 0;

    // f random number
    virtual double getFDistPdf( double x,
                                double nu1,
                                double nu2 ) const = 0;
    virtual double getFDistCdfP( double x,
                                 double nu1,
                                 double nu2 ) const = 0;
    virtual double getFDistCdfQ( double x,
                                 double nu1,
                                 double nu2 ) const = 0;
    virtual double getFDistRandomNumber(
              const RandomGenerator& randomGenerator,
              double nu1,
              double nu2 ) const = 0;

    // chi2 random number
    virtual double getChi2CdfP( double x,
                                double nu ) const = 0;
    virtual double getChi2CdfQ( double x,
                                double nu ) const = 0;
    virtual double getChi2CdfInverseP( double P,
                                       double nu ) const = 0;
    virtual double getChi2CdfInverseQ( double Q,
                                       double nu ) const = 0;

    //// multilinear fit
    virtual void getMultiLinearFit( const Matrix& X,
                                    const Vector& y,
                                    Vector& c,
                                    Matrix& covariance,
                                    double& chisq ) const = 0;
    virtual void getWeightedMultiLinearFit( const Matrix& X,
                                            const Vector& w,
                                            const Vector& y,
                                            Vector& c,
                                            Matrix& covariance,
                                            double& chisq ) const = 0;

    //// complex trigonometry
    virtual std::complex< double >
              arccos( const std::complex< double >& z ) const = 0;
    virtual std::complex< double >
              arcsin( const std::complex< double >& z ) const = 0;
    virtual std::complex< double >
              arctan( const std::complex< double >& z ) const = 0;
    virtual std::complex< float >
              arccos( const std::complex< float >& z ) const;
    virtual std::complex< float >
              arcsin( const std::complex< float >& z ) const;
    virtual std::complex< float >
              arctan( const std::complex< float >& z ) const;


    //// Factorial function(s)
    // x! = ( x * ( x - 1 ) * ( x - 2 ) * ...
    virtual double getFactorialFunction( double x ) const;
    // x! = ( x * ( x - 2 ) * ( x - 4 ) * ...
    virtual double getDoubleFactorialFunction( double x ) const;

    //// Gamma function
    virtual double getGammaFunction( const double& x ) const = 0;
    virtual float getGammaFunction( const float& x ) const = 0;

    //// Hermite polynomials
    virtual double getHermitePolynomialFunction( int32_t order, 
                                                 const double& x ) const;
    virtual float getHermitePolynomialFunction( int32_t order, 
                                                 const float& x ) const;


    //// Bessel's functions
    virtual double getRegularBesselFunctionJ0( const double& x ) const = 0;
    virtual float getRegularBesselFunctionJ0( const float& x ) const;

    virtual double getRegularBesselFunctionJ1( const double& x ) const = 0;
    virtual float getRegularBesselFunctionJ1( const float& x ) const;

    virtual double getRegularBesselFunctionI0( const double& x ) const = 0;
    virtual float getRegularBesselFunctionI0( const float& x ) const;

    virtual double getRegularBesselFunctionI1( const double& x ) const = 0;
    virtual float getRegularBesselFunctionI1( const float& x ) const;

    virtual double getRegularBesselFunctionIn( const int32_t& n, 
                                               const double& x ) const = 0;
    virtual float getRegularBesselFunctionIn( const int32_t& n, 
                                              const float& x ) const;

    virtual double getRegularBesselFunctionFractionalOrder( 
                                                 double l, double x ) const = 0;
    virtual double getZeroBesselFunctionJ0( const uint32_t& k ) const = 0;
    virtual double getZeroBesselFunctionJ1( const uint32_t& k ) const = 0;
    
    virtual double getZeroBesselFunctionJn( const double& n,
                                            const uint32_t& k ) const = 0;

    virtual float getZeroBesselDerivativeFunctionJ0(
                                              const uint32_t& rootIndex ) const;
    virtual float getZeroBesselDerivativeFunctionJ1(
                                              const uint32_t& rootIndex ) const;

    virtual double getBesselDerivative( double n,
                                        double x,
                                        double h,
                                        double* error = 0 ) const = 0;

    virtual double getZeroBesselDerivative( const uint32_t& n,
                                            const uint32_t& rootIndex ) const;


    // Hypergeometric functions
    virtual double getHypergeometricFunction1F1(
                                       double a, double b, double x ) const = 0;


    // Watson's distribution
    virtual double getWatsonDistribution(
                           double kappa,
                           const Vector3d< float >& principalOrientation,
                           const Vector3d< float >& currentOrientation ) const;
    virtual double getWatsonDistribution(
                           double kappa,
                           const Vector3d< double >& principalOrientation,
                           const Vector3d< double >& currentOrientation ) const;
    virtual Vector getCanonicalWatsonSphericalHarmonicsDecomposition(
              double kappa,
              int32_t maximumSphericalHarmonicsOrder,
              const std::vector< gkg::Vector3d< float > >& orientations ) const;
    virtual void getRandomWatsonOrientation(
                                 double kappa,
                                 const Vector3d< float >& principalOrientation,
                                 const RandomGenerator& randomGenerator,
                                 Vector3d< float >& orientation ) const;
    virtual void getRandomWatsonOrientation(
                                 double kappa,
                                 const Vector3d< double >& principalOrientation,
                                 const RandomGenerator& randomGenerator,
                                 Vector3d< double >& orientation ) const;
    virtual double getWatsonKappaParameterFromAngularDispersion(
                                                double angularDispersion, 
                                                bool degrees = true,
                                                int32_t orientationCount = 1000,
                                                double epsilon = 0.1 ) const;
    virtual double getPrecomputedWatsonKappaParameterFromAngularDispersion(
                                                double angularDispersion, 
                                                bool degrees = true ) const;

    // Bingham's distribution
    virtual double getBinghamDistribution(
                           double kappa1,
                           double kappa2,
                           const Vector3d< float >& principalOrientation,
                           const Vector3d< float >& secondaryOrientation,
                           const Vector3d< float >& currentOrientation ) const;
    virtual double getBinghamDistribution(
                           double kappa1,
                           double kappa2,
                           const Vector3d< double >& principalOrientation,
                           const Vector3d< double >& secondaryOrientation,
                           const Vector3d< double >& currentOrientation ) const;


    // cf article Paine et al. : An elliptically symmetric angular
    // Gaussian distribution (2016)
    virtual void getRandomESAGOrientation( 
                                 const std::pair< double, double >& gammas,
                                 const Vector3d< double >& principalOrientation,
                                 const RandomGenerator& randomGenerator,
                                 Vector3d< double >& orientation ) const;

    // Dawson's integral
    virtual double getDawsonIntegral( double x ) const = 0;

    /// non-linear least-squares fitting of parametric function y = f( x; a )
    //  which derivative is known and where a is the searched parameters
    //  in return, get the number of iterations performed
    virtual int32_t getLevenbergMarquardtFit( 
                                           const Matrix& x,
                                           const Vector& y,
                                           const Vector& w,
                                           LevenbergMarquardtFunction& f,
                                           Matrix& covariance,
                                           double& chiSquarePerDegreeOfFreedom,
                                           int32_t maximumIterationCount,
                                           double absoluteError,
                                           double relativeError ) const = 0;

    // Nelder-Mead Simplex optimizer: optimize the parameter set p to get
    // the minimum value of f( p ); the initial parameters and delta 
    // parameters are provided as input parameters and also as output
    // results
    virtual int32_t getNelderMeadSimplexOptimumParameters( 
                                           Vector& parameters,
                                           Vector& deltaParameters,
                                           double &testSize,
                                           NelderMeadSimplexFunction& f,
                                           int32_t maximumIterationCount,
                                           double maximumTestSize,
                                           bool verbose ) const = 0;

    // Fletcher-Reeves Conjugate Gradient optimizer: optimize the parameter set 
    // p to get the minimum value of f( p ); the initial parameters are
    // provided as input parameters and also as output results
    virtual int32_t getFletcherReevesConjugateGradientOptimumParameters( 
                                     Vector& parameters,
                                     FletcherReevesConjugateGradientFunction& f,
                                     int32_t maximumIterationCount,
                                     double stepSize,
                                     double maximumTestGradient,
                                     double maximumTolerance,
                                     bool verbose ) const = 0;

    // Powell's NEWUOA optimizer: optimize the parameter set 
    // p to get the minimum value of f( p ); the initial parameters are
    // provided as input parameters and also as output results
    virtual double getPowellNewUOAOptimumParameters( 
                                                Vector& parameters,
                                                PowellNewUOAFunction& f,
                                                double initialTrustRegionRadius,
                                                int32_t maximumIterationCount,
                                                double maximumTestSize,
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
    virtual int32_t getConstrainedNonLinearProgrammingOptimumParameters( 
             NLPOptimizerType optimizerType,
             Vector& parameters,
             const std::vector< RCPointer< OptimizerConstraint > >& constraints,
             NonLinearProgrammingFunction& f,
             const std::vector< double >& optimizerParameters,
             bool verbose ) const = 0;


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
    virtual void getConstrainedNonLinearProgrammingOptimumParameters2( 
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
             double relativeTolerance = 1e-4,
             int32_t* achievedIterationCount = 0,
             double* reachedMinimumObjectiveValue = 0,
             bool verbose = false ) const = 0;



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
    // the method returns true if successful and false in case of failure
    virtual bool getConvexQuadraticProgrammingSolution(
                      const Matrix& Q,
                      const Vector& q,
                      const Matrix& A,
                      const Vector& b,
                      const Matrix& C,
                      const Vector& d,
                      Vector& x,
                      int32_t maximumIterationCount = 100,
                      double epsilonGap = 1e-10,
                      double epsilonResiduals = 1e-10,
                      double epsilonInfeasible = 1e-10,
                      bool verbose = false ) const = 0;
 
    virtual bool getConvexQuadraticProgrammingSolution(
                      const Matrix& Q,
                      const Vector& q,
                      const Matrix& C,
                      const Vector& d,
                      Vector& x,
                      int32_t maximumIterationCount = 100,
                      double epsilonGap = 1e-10,
                      double epsilonResiduals = 1e-10,
                      double epsilonInfeasible = 1e-10,
                      bool verbose = false ) const = 0;
 

    // Simulated Annealing optimizer: optimize the parameters p to get the 
    // minimum value of f(p)
    virtual int32_t getSimulatedAnnealingOptimumConfiguration(
                     SimulatedAnnealingConfiguration& configuration,
                     int32_t trialCountPerStep,
                     int32_t iterationCountPerTemperature,
                     double maximumStepSize,
                     const SimulatedAnnealingConfiguration::BoltzmannParameters&
                                                            boltzmannParameters,
                     const RandomGenerator& randomGenerator,
                     bool verbose ) const = 0;


    //// Legendre polynomial(s)

    // Legendre Pl(x) function
    virtual double getLegendrePolynomial( int32_t l, double x ) const = 0;

    // Legendre spherical Plm(x) function
    virtual double getLegendrePolynomialSpherical( int32_t l,
                                                   int32_t m,
                                                   double x ) const = 0;

    //// Spherical harmonic(s)

    // careful: in physics, theta is azimuth  and phi is colatitule,
    // whereas for spherical harmonics, theta is colatitude and phi is
    // azimuth, so theta and phi must be swapped in SH function call(s)

    // cartesian to spherical coordinates
    virtual void getCartesianToSphericalCoordinates( double x,
                                                     double y,
                                                     double z,
                                                     double& radial,
                                                     double& azimuth,
                                                     double& colatitude ) const;

    virtual void getSphericalToCartesianCoordinates( double radial,
                                                     double azimuth,
                                                     double colatitude,
                                                     double& x,
                                                     double& y,
                                                     double& z ) const;
    
    // normalized spherical harmonic of order l and phase factor m
    // i.e. Y(l,m)( theta, phi ) = sqrt( ( ( 2l + 1 )( l - m )! ) /
    //                                   ( 4 Pi ( l + m )! ) ) *
    //                             P(l,m)( cos(theta) ) * exp(i m phi ) 
    // with l= 0, 2, 3, ..., lmax
    //      m=-l,...,0,...,+l
    //      theta in [0;Pi]
    //      phi in [0;2Pi[
    virtual std::complex< double >
              getSphericalHarmonic( int32_t l,
                                    int32_t m,
                                    double theta,
                                    double phi ) const = 0;

    // symmetrical spherical harmonic of order j = ( l^2 + l ) / 2 + m
    // i.e. 
    // Yj = | sqrt(2) * Re( Y(l,|m|) )         if -l <= m < 0
    //      | Y(l,0)                           if m = 0
    //      | sqrt(2) * (-1)^(m+1)Im( Y(l,m) ) if 0 < m <= l
    // with theta in [0;Pi]
    //      phi in [0;2Pi[
    virtual double
              getSymmetricalSphericalHarmonic( int32_t j,
                                               double theta,
                                               double phi ) const = 0;
    // processing (l,m) order and phase factor of symmetrical spherical
    // harmonic from j index
    virtual void
      getSymmetricalSphericalHarmonicLM( int32_t j,
					 int32_t& l,
					 int32_t& m ) const = 0;
    
    virtual double
      getSharpeningDeconvolutionTransformCoefficient( int32_t l,
						      double e1,
						      double e2) const = 0;
    virtual double getDiracCoefficient( int32_t j,
					double theta,
					double phi ) const = 0;

    // supposes n is odd
    // n!! = n*(n-2)*...*3*1
    virtual double getOddFactorial( double n ) const = 0;

    // supposes n is even
    // n!! = n*(n-2)*...*4*2*1
    virtual double getEvenFactorial( double n ) const = 0;

    // Pochhammer symbol
    virtual double getPochhammer( double a, double x ) const = 0;

    /// Error function(s)

    // compute erf(x) = ( 2 / sqrt(PI) ) x Int(0,x)( exp( -t^2 )dt ) 
    virtual double getErf( double x ) const = 0;

    // compute erfc(x) = 1 - erf( x )
    virtual double getErfC( double x ) const = 0;

    // compute erfZ(x) = ( 1 / 2.PI ) x exp( -x^2 / 2 ) 
    virtual double getErfZ( double x ) const = 0;

    // compute erfQ(x) = ( 1 / 2.PI ) x Int(x,+inf)( exp( -t^2 / 2 )dt ) 
    virtual double getErfQ( double x ) const = 0;

    /// Numerical integration

    // QAGS integrator : process int_a^b[ f(x)dx ]
    //  Q - quadrature routine
    //  A - adaptative integrator
    //  G - general integrand
    //  S - singularities can be more readily integrated
    virtual double getQAGSIntegral(
                                   const IntegrableFunction& integrableFunction,
                                   double a,
                                   double b,
                                   double epsilonAbsolute,
                                   double epsilonRelative,
                                   double* error = 0,
                                   int32_t subIntervalCount = 1000 ) const = 0;

    //// Function interpolation

    // function interpolator
    virtual FunctionInterpolatorImplementation*
      createFunctionInterpolatorImplementation(
        FunctionInterpolator* functionInterpolator,
        FunctionInterpolator::Type type,
        const std::vector< double >& xs,
        const std::vector< double >& ys ) const = 0;

    // function interpolator
    virtual FunctionInterpolatorImplementation*
      createFunctionInterpolatorImplementation(
        FunctionInterpolator* functionInterpolator,
        FunctionInterpolator::Type type,
        const Vector& xs,
        const Vector& ys ) const = 0;

    // Parzen Window interpolation
    double getParzenWindowInterpolation( double x,
                                         const std::vector< double >& xs,
                                         const std::vector< double >& ys,
                                         double kernelWidth ) const;
    double getParzenWindowInterpolation( double x,
                                         const Vector& xs,
                                         const Vector& ys,
                                         double kernelWidth ) const;

    /// Polynomial roots
    virtual void getPolynomialRoots( const Vector& polynomCoefficients,
                                     CVector& roots ) const = 0;

    /// Gaussian Mixture Model(s)

    // Gaussian Mixture Model inference using Expectation Maximization algo
    // return the root mean square difference over the distribution means
    virtual double 
      getExpectationMaximizationGaussianMixtureModel(
        const Matrix& inputData,
        int32_t gaussianDistributionCount,
        std::vector< Vector >& distributionMeans,
        std::vector< Matrix >& distributionStandardDeviations,
        std::vector< double >& distributionScales,
        double tolerance = 1e-4,
        int32_t maximumIterationCount = 1000,
        bool verbose = false ) const = 0;

    virtual double 
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
        bool verbose ) const = 0;

    virtual double
      getMultivariateGaussianPdf(
        const Vector& values,
        const Vector& means,
        const Matrix& standardDeviations,
        double scale ) const = 0;
    virtual double
      getMultivariateGaussianLogPdf(
        const Vector& values,
        const Vector& means,
        const Matrix& standardDeviations,
        double scale ) const = 0;
        

  protected:

    NumericalAnalysisImplementationFactory();

    double newuoa( int32_t n,
                   int32_t npt,
                   double* x,
                   double rhobeg,
                   double rhoend, 
                   int32_t* ret_nf,
                   int32_t maxfun,
                   double* w,
                   const PowellNewUOAFunction& func, 
                   bool verbose ) const;

    double newuob( int32_t n,
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
                   const PowellNewUOAFunction& func,
                   bool verbose ) const;

    int32_t biglag( int32_t n, 
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
                    const PowellNewUOAFunction& func ) const;

    int32_t bigden( int32_t n, 
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
                    double* prod ) const;

    int32_t trl( int32_t n,
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
                 double* crvmin, 
                 int32_t xpt_dim1, 
                 int32_t xpt_offset, 
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
                 double angtest ) const;

    int32_t trsapp( int32_t n, 
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
                    double* crvmin ) const;


    int32_t update( int32_t n, 
                    int32_t npt, 
                    double* bmat, 
                    double* zmat, 
                    int32_t* idz, 
                    int32_t* ndim, 
                    double* vlag, 
                    double* beta, 
                    int32_t* knew, 
                    double* w ) const;


    double L50L70(
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
        double xoptsq ) const;

    double L70L90(
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
        double xoptsq ) const;

    double L90L100(
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
        double xoptsq ) const;

    double L100L120(
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
        double xoptsq ) const;

    double L120L290(
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
        double xoptsq ) const;

    double L290L310(
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
        double xoptsq ) const;

    double L310L410(
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
        double xoptsq ) const;

    double L410L460(
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
        double xoptsq ) const;

    double L460L490(
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
        double xoptsq ) const;

    double L490L530(
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
        double xoptsq ) const;

    double L530end(
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
        double xoptsq ) const;

};


}


#endif
