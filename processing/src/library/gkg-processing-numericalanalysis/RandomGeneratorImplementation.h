#ifndef _gkg_processing_numericalanalysis_RandomGeneratorImplementation_h_
#define _gkg_processing_numericalanalysis_RandomGeneratorImplementation_h_


#include <gkg-processing-numericalanalysis/RandomGenerator.h>


namespace gkg
{


class RandomGeneratorImplementation
{

  public:

    virtual ~RandomGeneratorImplementation();

    virtual void setSeed( uint32_t s ) = 0;

    virtual RandomGenerator* getTarget() const;

  protected:

    RandomGeneratorImplementation( RandomGenerator* randomGenerator,
                                   RandomGenerator::Type type );

    RandomGenerator* _target;

};


}


#endif
