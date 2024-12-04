#ifndef _gkg_processing_plugin_gsl_lapack_GSLRandomGeneratorImplementation_h_
#define _gkg_processing_plugin_gsl_lapack_GSLRandomGeneratorImplementation_h_


#include <gkg-processing-numericalanalysis/RandomGeneratorImplementation.h>
#include <gsl/gsl_rng.h>


namespace gkg
{


class GSLRandomGeneratorImplementation : public RandomGeneratorImplementation
{

  public:


    GSLRandomGeneratorImplementation( RandomGenerator* randomGenerator,
                                      RandomGenerator::Type type );
    ~GSLRandomGeneratorImplementation();

    void setSeed( uint32_t s );

    gsl_rng* getGSLRandomGeneratorPointer() const;

    static GSLRandomGeneratorImplementation*
             getImplementation( RandomGenerator* randomGenerator );

  private:

    gsl_rng* _gslRandomGeneratorPointer;

};


}


#endif
