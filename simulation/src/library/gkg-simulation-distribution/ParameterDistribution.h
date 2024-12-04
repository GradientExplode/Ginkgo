#ifndef _gkg_simulation_distribution_ParameterDistribution_h_
#define _gkg_simulation_distribution_ParameterDistribution_h_


#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>


namespace gkg
{


class ParameterDistribution
{

  public:

    virtual ~ParameterDistribution();

    virtual float getRandomValue() const = 0;

  protected:

    ParameterDistribution();

    RandomGenerator _randomGenerator;
    NumericalAnalysisImplementationFactory* _factory;

};



}



#endif

