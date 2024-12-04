#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMCMCParameterSet_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMCMCParameterSet_h_


#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainParameterSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class SingleCompartmentRelaxometryMCMCParameterSet :
                                        public MonteCarloMarkovChainParameterSet
{

  public:

    SingleCompartmentRelaxometryMCMCParameterSet(
                                               const Vector& lowerBounds,
                                               const Vector& upperBounds,
                                               const std::vector< bool >& fixed,
                                               const Vector& steps );
    virtual ~SingleCompartmentRelaxometryMCMCParameterSet();

    // return true if the new parameters are well located within their lower and
    // upper bounds, and return false if not
    bool updateParameters();

  protected:

    NumericalAnalysisImplementationFactory* _factory;
    RCPointer< RandomGenerator > _randomGenerator;

};


}



#endif
