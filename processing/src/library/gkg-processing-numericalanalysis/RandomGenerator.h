#ifndef _gkg_processing_numericalanalysis_RandomGenerator_h_
#define _gkg_processing_numericalanalysis_RandomGenerator_h_


#include <iostream>


namespace gkg
{


class RandomGeneratorImplementation;


class RandomGenerator
{

  public:

    enum Type
    {

      Taus,
      Gfsr4,
      Mt19937

    };

    RandomGenerator( Type type );
    virtual ~RandomGenerator();

    void setSeed( uint32_t s );
    void setSeedWithTime();

    RandomGeneratorImplementation* getImplementation() const;

  protected:

    RandomGeneratorImplementation* _randomGeneratorImplementation;

};


}


#endif
