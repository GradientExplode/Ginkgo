#ifndef _gkg_processing_plugin_gsl_lapack_GSLConvexQuadraticProgrammingSolver2_h_
#define _gkg_processing_plugin_gsl_lapack_GSLConvexQuadraticProgrammingSolver2_h_


#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-plugin-gsl-lapack/GSLMatrixImplementation.h>
#include <gkg-processing-plugin-gsl-lapack/GSLVectorImplementation.h>


namespace gkg
{



class GSLConvexQuadraticProgrammingSolver2
{

  public:

    GSLConvexQuadraticProgrammingSolver2( const Matrix& Q,
                                          const Vector& q,
                                          const Matrix& A,
                                          const Vector& b,
                                          const Matrix& C,
                                          const Vector& d,
                                          Vector& x );
    GSLConvexQuadraticProgrammingSolver2( const Matrix& Q,
                                          const Vector& q,
                                          const Matrix& C,
                                          const Vector& d,
                                          Vector& x );
    virtual ~GSLConvexQuadraticProgrammingSolver2();

    // returning true if the process is successful and false in case of failure
    bool solve( int32_t maximumIterationCount = 100,
                double epsilonGap = 1e-10,
                double epsilonResiduals = 1e-10,
                double epsilonInfeasible = 1e-10,
                bool verbose = false );

  protected:

    void prepare();

    int32_t _N;
    int32_t _Me;
    int32_t _Mi;

    gsl_matrix* _Q;
    gsl_vector* _q;

    //gsl_matrix* _A;
    //gsl_vector* _b;

    gsl_matrix* _C;
    gsl_vector* _d;

    gsl_vector* _x;



};



}


#endif

