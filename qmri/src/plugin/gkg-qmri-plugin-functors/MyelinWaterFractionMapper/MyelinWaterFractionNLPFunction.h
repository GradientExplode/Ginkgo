#ifndef _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_MyelinWaterFractionNLPFunction_h_
#define _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_MyelinWaterFractionNLPFunction_h_


#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionMCMCFunction.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class MyelinWaterFractionNLPFunction : public NonLinearProgrammingFunction
{

  public:

    MyelinWaterFractionNLPFunction(
       RCPointer< MyelinWaterFractionMCMCFunction > 
                                              myelinWaterFractionMCMCFunction );
    virtual ~MyelinWaterFractionNLPFunction();

    void setRealMeasurements( const gkg::Vector& realMeasurements );

    double getValueAt( const Vector& parameters ) const;

  protected:

    double getLogLikelihood( const Vector& currentMeasurements ) const;

    RCPointer< MyelinWaterFractionMCMCFunction >
                                               _myelinWaterFractionMCMCFunction;
    Vector _realMeasurements;

    int32_t _t2WeightedVolumeCount;
    std::vector< int32_t > _t2WeightedCounts;
    int32_t _globalT2WeightedCount;
    std::vector< double > _t2NoiseVariances;
    std::vector< double > _twiceT2NoiseVariances;
    int32_t _t1WeightedCount;
    double _t1NoiseVariance;
    double _twiceT1NoiseVariance;
    double _twoPi;
    int32_t _measurementCount;
    mutable Vector _currentMeasurements;

};


}


#endif

