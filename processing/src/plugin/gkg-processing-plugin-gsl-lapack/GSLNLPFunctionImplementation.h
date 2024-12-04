#ifndef _gkg_processing_plugin_gsl_lapack_GSLNLPFunctionImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLNLPFunctionImplementation_h_


#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>
#include <gsl/gsl_multimin.h>


namespace gkg
{


class Vector;
class NonLinearProgrammingFunction;


class GSLNLPFunctionImplementation
{

  public:

    GSLNLPFunctionImplementation( 
           NonLinearProgrammingFunction* nonLinearProgrammingFunction,
           const std::vector< RCPointer< OptimizerConstraint > >& constraints );
    ~GSLNLPFunctionImplementation();

    NonLinearProgrammingFunction* getNonLinearProgrammingFunction() const;
    const OptimizerConstraint& getConstraint( int32_t p ) const;
    gsl_multimin_function* getFPointer();
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

    NonLinearProgrammingFunction* _nonLinearProgrammingFunction;
    std::vector< RCPointer< OptimizerConstraint > > _constraints;

    gsl_multimin_function _gslMultiminFunction;
    gsl_multimin_function_fdf _gslMultiminFunctionFdf;

};


}


#endif
