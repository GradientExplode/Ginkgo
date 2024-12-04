#ifndef _gkg_dmri_microstructure_axon_mapping_AxonMappingNLPFunction_h_
#define _gkg_dmri_microstructure_axon_mapping_AxonMappingNLPFunction_h_


#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-microstructure-axon-mapping/AxonMappingMCMCFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class AxonMappingNLPFunction : public NonLinearProgrammingFunction
{

  public:

    AxonMappingNLPFunction(
                   RCPointer< AxonMappingMCMCFunction > axonMappingMCMCFunction,
                   const Vector& realMeasurements );
    virtual ~AxonMappingNLPFunction();

    double getValueAt( const Vector& parameters ) const;

  protected:

    double getLogLikelihood( const Vector& currentMeasurements ) const;

    RCPointer< AxonMappingMCMCFunction > _axonMappingMCMCFunction;
    Vector _realMeasurements;
    NumericalAnalysisImplementationFactory* _factory;

    double _noiseVariance;
    double _twiceNoiseVariance;
    double _twoPi;
    int32_t _measurementCount;
    mutable Vector _currentMeasurements;

};


}


#endif
