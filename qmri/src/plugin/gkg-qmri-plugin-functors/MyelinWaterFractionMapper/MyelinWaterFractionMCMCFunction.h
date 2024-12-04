#ifndef _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_MyelinWaterFractionMCMCFunction_h_
#define _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_MyelinWaterFractionMCMCFunction_h_


#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainFunction.h>

namespace gkg
{


class MyelinWaterFractionMCMCFunction : public MonteCarloMarkovChainFunction
{

  public:

    MyelinWaterFractionMCMCFunction(
                         const std::vector< Vector >& echoTimes,
                         const std::vector< double >& t2NoiseStandardDeviations,
                         int32_t t1WeightedCount,
                         double t1NoiseStandardDeviation );
    virtual ~MyelinWaterFractionMCMCFunction();

    double getLikelihoodRatio( const Vector& realMeasurements,
                               const Vector& currentMeasurements,
                               const Vector& newMeasurements ) const;

    int32_t getT2WeightedVolumeCount() const;
    const std::vector< int32_t >& getT2WeightedCounts() const;
    int32_t getT2WeightedCount( int32_t t2WeightedVolumeIndex ) const;
    int32_t getGlobalT2WeightedCount() const;

    const std::vector< double >& getT2NoiseStandardDeviations() const;
    double getT2NoiseStandardDeviation( int32_t t2WeightedVolumeIndex ) const;
    const std::vector< double >& getT2NoiseVariances() const;
    double getT2NoiseVariance( int32_t t2WeightedVolumeIndex ) const;

    int32_t getT1WeightedCount() const;
    double getT1NoiseStandardDeviation() const;
    double getT1NoiseVariance() const;

    int32_t getParameterCount() const;

  protected:

    std::vector< double > _t2NoiseStandardDeviations;
    int32_t _t1WeightedCount;
    double _t1NoiseStandardDeviation;
    double _t1NoiseVariance;

    int32_t _t2WeightedVolumeCount;
    std::vector< int32_t > _t2WeightedCounts;
    int32_t _globalT2WeightedCount;

    std::vector< double > _t2NoiseVariances;


};

}


#endif
