#ifndef _gkg_dmri_microstructure_noddi_NoddiMCMCParameterSet_h_
#define _gkg_dmri_microstructure_noddi_NoddiMCMCParameterSet_h_


#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainParameterSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class NoddiMCMCParameterSet : public MonteCarloMarkovChainParameterSet
{

  public:

    NoddiMCMCParameterSet( const Vector& lowerBounds,
                           const Vector& upperBounds,
                           const std::vector< bool >& fixed,
                           const Vector& steps );
    virtual ~NoddiMCMCParameterSet();

    // return true if the new parameters are well located within their lower and
    // upper bounds, and return false if not
    virtual bool updateParameters() = 0;

  protected:

    NumericalAnalysisImplementationFactory* _factory;
    RCPointer< RandomGenerator > _randomGenerator;

};


}


#endif
