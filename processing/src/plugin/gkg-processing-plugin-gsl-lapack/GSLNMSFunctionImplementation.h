#ifndef _gkg_processing_plugin_gsl_lapack_GSLNMSFunctionImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLNMSFunctionImplementation_h_


#include <gsl/gsl_multimin.h>


namespace gkg
{


class Vector;
class NelderMeadSimplexFunction;


class GSLNMSFunctionImplementation
{

  public:

    GSLNMSFunctionImplementation(
                         NelderMeadSimplexFunction* nelderMeadSimplexFunction );
    ~GSLNMSFunctionImplementation();

    NelderMeadSimplexFunction* getNelderMeadSimplexFunction() const;
    gsl_multimin_function* getMultiminFunctionPointer();

  private:

    static double getValueAt( const gsl_vector* parameters,
                              void* otherParameters );

    NelderMeadSimplexFunction* _nelderMeadSimplexFunction;
    gsl_multimin_function _gslMultiminFunction;

};


}


#endif
