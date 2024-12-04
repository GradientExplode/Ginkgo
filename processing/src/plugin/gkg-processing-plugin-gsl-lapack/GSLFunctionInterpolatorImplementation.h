#ifndef _gkg_processing_plugin_gsl_lapack_GSLFunctionInterpolatorImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLFunctionInterpolatorImplementation_h_


#include <gkg-processing-numericalanalysis/FunctionInterpolatorImplementation.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>


namespace gkg
{


class GSLFunctionInterpolatorImplementation : public FunctionInterpolatorImplementation
{

  public:

    GSLFunctionInterpolatorImplementation(
                                     FunctionInterpolator* functionInterpolator,
                                     FunctionInterpolator::Type type,
                                     const std::vector< double >& xs,
                                     const std::vector< double >& ys );
    GSLFunctionInterpolatorImplementation(
                                     FunctionInterpolator* functionInterpolator,
                                     FunctionInterpolator::Type type,
                                     const Vector& xs,
                                     const Vector& ys );
    ~GSLFunctionInterpolatorImplementation();

    double getValueAt( double x ) const;


    static GSLFunctionInterpolatorImplementation*
                getImplementation( FunctionInterpolator* functionInterpolator );

  private:

    gsl_interp_accel* _accelerator;
    gsl_spline* _spline;

};


}


#endif
