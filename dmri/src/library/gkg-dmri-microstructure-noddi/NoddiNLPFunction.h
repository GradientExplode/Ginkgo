#ifndef _gkg_dmri_microstructure_noddi_NoddiNLPFunction_h_
#define _gkg_dmri_microstructure_noddi_NoddiNLPFunction_h_


#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class NoddiNLPFunction : public NonLinearProgrammingFunction
{

  public:

    NoddiNLPFunction( RCPointer< NoddiMCMCFunction > noddiMCMCFunction,
                      const Vector& realMeasurements );
    virtual ~NoddiNLPFunction();

    double getValueAt( const Vector& parameters ) const;


  protected:

    double getLogLikelihood( const Vector& currentMeasurements ) const;

    RCPointer< NoddiMCMCFunction > _noddiMCMCFunction;
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
