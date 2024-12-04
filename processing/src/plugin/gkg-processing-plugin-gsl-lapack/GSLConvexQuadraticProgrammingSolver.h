#ifndef _gkg_processing_plugin_gsl_lapack_GSLConvexQuadraticProgrammingSolver_h_
#define _gkg_processing_plugin_gsl_lapack_GSLConvexQuadraticProgrammingSolver_h_


#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-plugin-gsl-lapack/GSLMatrixImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLVectorImplementation.h>


namespace gkg
{


//
// This method is an adaptation of the CQP C++ library which
// implements the Mehrotra-Gondzio primal-dual interior point method to 
// solve strictly convex quadratic programs 
//
//  The problem is in the form
//  argmin_x { 1/2 x^T Q x + q^T x }
//    subject to:
//          A x = b
//          C x >= d
//
// available at http://ra.uni-trier.de/~huebner/software.html under GPL LICENSE
//

class GSLConvexQuadraticProgrammingSolver
{

  public:

    GSLConvexQuadraticProgrammingSolver( const Matrix& Q,
                                         const Vector& q,
                                         const Matrix& A,
                                         const Vector& b,
                                         const Matrix& C,
                                         const Vector& d,
                                         Vector& x );
    GSLConvexQuadraticProgrammingSolver( const Matrix& Q,
                                         const Vector& q,
                                         const Matrix& C,
                                         const Vector& d,
                                         Vector& x );
    virtual ~GSLConvexQuadraticProgrammingSolver();

    // returning true if the process is successful and false in case of failure
    bool solve( int32_t maximumIterationCount = 100,
                double epsilonGap = 1e-10,
                double epsilonResiduals = 1e-10,
                double epsilonInfeasible = 1e-10,
                bool verbose = false );

  protected:

    void allocateYZ();
    void sanityChecks();
    void allocateStatusParameters();
    void settingInitialPoints();
    void buildKkt();
    void computeResiduals();
    double computeGapInfeasiblePoints();
    void iterate();
    bool testConvergence( double epsilonGap, double epsilonResiduals ) const;
    bool testInfeasibility( double epsilonInfeasible ) const;
    double getStepLength( const gsl_vector* deltaS,
                          const gsl_vector* deltaZ ) const;

    double getObjectiveFunction() const;


    int32_t _N;
    int32_t _Me;
    int32_t _Mi;

    gsl_matrix* _Q;
    gsl_vector* _q;

    gsl_matrix* _A;
    gsl_vector* _b;

    gsl_matrix* _C;
    gsl_vector* _d;

    gsl_vector* _x;

    // Lagrange-multiplier corresponding to Ax=b
    gsl_vector* _y;
    // Lagrange-multiplier corresponding to CX>=d
    gsl_vector* _z;

    double _gap;
    double _residualNorm;
    double _minimizerDataNorm;
    double _quantityOfInfeasibility;
    double _minimumQuantityOfInfeasibility;

    // slack vector for the constrain: Cx + s = d
    gsl_vector* _s;

    // kkt matrix
    gsl_matrix* _kkt;

    // vector of the right-hand side in the kkt-system r=(r_Q,r_A,r_C)
    gsl_vector* _r;

    // constant for the Mehrotra's heuristic
    double _tau;

    // Gondzio's parameters :
    // maximum number of corrections
    int32_t _maximumK;
    // minimum threshold values for outlier compl. products
    double _minimumBeta;
    // maximum threshold values for outlier compl. products
    double _maximumBeta;
    // required increase of stepsize
    double _deltaAlpha;
    // minimum acceptable increase of stepsize
    double _gamma;
    // norm of the problem data
    double _dataNorm;

};



}


#endif

