#ifndef _gkg_dmri_microstructure_axcaliber_AxCaliberMCMCParameterSet_h_
#define _gkg_dmri_microstructure_axcaliber_AxCaliberMCMCParameterSet_h_


#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainParameterSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{

//
// 0 -> HinderedDiffusivity (?m2/s)
// 1 -> IntracellularFraction
// 2 -> IntracellularPerpendicularDiffusivity (1.7e-9 m2/s)
// 3 -> IntracellularParallelDiffusivity (?m2/s)
// 4 -> DiameterMean
// 5 -> DiameterStdDev
//

class AxCaliberMCMCParameterSet : public MonteCarloMarkovChainParameterSet
{

  public:

    AxCaliberMCMCParameterSet( const Vector& lowerBounds,
                               const Vector& upperBounds,
                               const std::vector< bool >& fixed,
                               const Vector& steps );
    virtual ~AxCaliberMCMCParameterSet();

    // return true if the new parameters are well located within their lower and
    // upper bounds, and return false if not
    bool updateParameters();

  protected:

    NumericalAnalysisImplementationFactory* _factory;
    RCPointer< RandomGenerator > _randomGenerator;

};


}


#endif
