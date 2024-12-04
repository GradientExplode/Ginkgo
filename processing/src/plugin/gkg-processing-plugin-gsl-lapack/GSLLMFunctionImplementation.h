#ifndef _gkg_processing_plugin_gsl_lapack_GSLLMFunctionImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLLMFunctionImplementation_h_


#include <gsl/gsl_multifit_nlin.h>


namespace gkg
{


class Matrix;
class Vector;
class LevenbergMarquardtFunction;


class GSLLMFunctionImplementation
{

  public:

    GSLLMFunctionImplementation(
                       const Matrix* x,
                       const Vector* y,
                       const Vector* w,
                       LevenbergMarquardtFunction* levenbergMarquardtFunction );
    ~GSLLMFunctionImplementation();

    const Matrix* getX() const;
    const Vector* getY() const;
    const Vector* getW() const;
    LevenbergMarquardtFunction* getLevenbergMarquardtFunction() const;
    gsl_multifit_function_fdf* getFdfPointer();

  private:

    static int32_t getValueAt( const gsl_vector* parameters,
                               void* otherParameters,
                               gsl_vector* costFunction );
    static int32_t getDerivativeAt( const gsl_vector* parameters,
                                    void* otherParameters,
                                    gsl_matrix* costFunctionJacobian );
    static int32_t getValueAndDerivativeAt( const gsl_vector* parameters,
                                            void* otherParameters,
                                            gsl_vector* costFunction,
                                            gsl_matrix* costFunctionJacobian );
  
    const Matrix* _x;
    const Vector* _y;
    const Vector* _w;
    LevenbergMarquardtFunction* _levenbergMarquardtFunction;
    gsl_multifit_function_fdf _fdf;

};


}


#endif
