#ifndef _gkg_dmri_microstructure_axcaliber_AxCaliberNLPFunction_h_
#define _gkg_dmri_microstructure_axcaliber_AxCaliberNLPFunction_h_


#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberMCMCFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class AxCaliberNLPFunction : public NonLinearProgrammingFunction
{

  public:

    AxCaliberNLPFunction(
                       RCPointer< AxCaliberMCMCFunction > axCaliberMCMCFunction,
                       const Vector& realMeasurements );
    virtual ~AxCaliberNLPFunction();

    double getValueAt( const Vector& parameters ) const;


  protected:

    double getLogLikelihood( const Vector& currentMeasurements ) const;

    RCPointer< AxCaliberMCMCFunction > _axCaliberMCMCFunction;
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
