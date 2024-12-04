#ifndef _gkg_processing_plugin_gsl_lapack_GSLSAConfigurationImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLSAConfigurationImplementation_h_


#include <gsl/gsl_siman.h>


namespace gkg
{


class SimulatedAnnealingConfiguration;


class GSLSAConfigurationImplementation
{

  public:

    GSLSAConfigurationImplementation(
               SimulatedAnnealingConfiguration* simulatedAnnealingConfiguration,
               bool verbose );
    ~GSLSAConfigurationImplementation();

    SimulatedAnnealingConfiguration* getSimulatedAnnealingConfiguration() const;

    gsl_siman_Efunc_t getSimanEFuncPointer();
    gsl_siman_step_t getSimanStepPointer();
    gsl_siman_metric_t getSimanMetricPointer();
    gsl_siman_print_t getSimanPrintPointer();
    gsl_siman_copy_t getSimanCopyPointer();
    gsl_siman_copy_construct_t getSimanCopyConstructPointer();
    gsl_siman_destroy_t getSimanDestroyPointer();


  private:

    static double simanEFunc( void* p );
    static void simanStep( const gsl_rng* r, void* p, double stepSize );
    static double simanMetric( void* p1, void* p2 );
    static void simanPrint( void* p );
    static void simanNoPrint( void* p );
    static void simanCopy( void* source, void* destination );
    static void* simanCopyConstruct( void* p );
    static void simanDestroy( void* p );

    SimulatedAnnealingConfiguration* _simulatedAnnealingConfiguration;

    gsl_siman_Efunc_t _simanEFunc;
    gsl_siman_step_t _simanStep;
    gsl_siman_metric_t _simanMetric;
    gsl_siman_print_t _simanPrint;
    gsl_siman_copy_t _simanCopy;
    gsl_siman_copy_construct_t _simanCopyConstruct;
    gsl_siman_destroy_t _simanDestroy;

};


}


#endif
