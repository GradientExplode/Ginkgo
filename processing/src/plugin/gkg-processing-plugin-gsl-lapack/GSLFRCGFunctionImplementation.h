#ifndef _gkg_processing_plugin_gsl_lapack_GSLFRCGFunctionImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLFRCGFunctionImplementation_h_


#include <gsl/gsl_multimin.h>


namespace gkg
{


class Vector;
class FletcherReevesConjugateGradientFunction;


class GSLFRCGFunctionImplementation
{

  public:

    GSLFRCGFunctionImplementation( FletcherReevesConjugateGradientFunction*
                                      fletcherReevesConjugateGradientFunction );
    ~GSLFRCGFunctionImplementation();

    FletcherReevesConjugateGradientFunction* getFletcherReevesConjugateGradientFunction() const;
    gsl_multimin_function_fdf* getFdfPointer();

  private:

    static double getValueAt( const gsl_vector* parameters,
                              void* otherParameters );
    static void getDerivativeAt( const gsl_vector* parameters,
                                 void* otherParameters,
                                 gsl_vector* derivative );
    static void getValueAndDerivativeAt( const gsl_vector* parameters,
                                         void* otherParameters,
                                         double* value,
                                         gsl_vector* derivative );

    FletcherReevesConjugateGradientFunction* 
      _fletcherReevesConjugateGradientFunction;
    gsl_multimin_function_fdf _gslMultiminFunctionFdf;

};


}


#endif
