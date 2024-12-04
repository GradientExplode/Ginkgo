#ifndef _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_MyelinWaterFractionMapperLoopContext_h_
#define _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_MyelinWaterFractionMapperLoopContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


class MyelinWaterFractionMapperGauge;


class MyelinWaterFractionMapperLoopContext : public LoopContext< int32_t >
{

  public:

    MyelinWaterFractionMapperLoopContext( 
                  MyelinWaterFractionMapperGauge& gauge,
                  const std::vector< Vector3d< int32_t > >& sites,
                  const std::vector< Volume< float > >& t2WeightedVolumes,
                  const Volume< float >& t1WeightedVolume,
                  const bool& t1MappingUsingSPGR,
                  const RCPointer< Volume< float > >& b1MappingVolumePointer,
                  const std::vector< Vector >& echoTimes,
                  const Vector& flipAngles,
                  const Vector& repetitionTimes,
                  const Vector& inversionTimes,
                  const std::vector< double >& t2NoiseStandardDeviations,
                  const double& t1NoiseStandardDeviation,
                  const std::vector< RCPointer< OptimizerConstraint > >&
                                                                    constraints,
                  const std::vector< double >& nlpOptimizerParameters,
                  const std::vector< double >& initialParameters,
                  const Vector& lowerParameterBoundaries,
                  const Vector& upperParameterBoundaries,
                  const std::vector< bool >& isFixedParameters,
                  const Vector& deltaParameters,
                  const bool& applyMCMC,
                  const int32_t& mcmcBurninCount,
                  const int32_t& mcmcSampleCount,
                  const int32_t& mcmcIntervalCount,
                  const int32_t& mcmcMaximumIterationCount,
                  Volume< float >& myelinWaterFractionVolume,
                  Volume< float >& myelinWaterT1Volume,
                  Volume< float >& myelinWaterT2Volume,
                  Volume< float >& grayWhiteFractionVolume,
                  Volume< float >& grayWhiteT1Volume,
                  Volume< float >& grayWhiteT2Volume,
                  Volume< float >& csfFractionVolume,
                  Volume< float >& csfT1Volume,
                  Volume< float >& csfT2Volume,
                  Volume< float >& protonDensityVolume,
                  Volume< float >& protonDensityScalingVolume,
                  Volume< float >& t2WeightedyScalingVolume,
                  const bool& computeFittedData,
                  Volume< float >& fittedDataVolume,
                  const bool& verbose );
    virtual ~MyelinWaterFractionMapperLoopContext();


    void doIt( int32_t startIndex, int32_t count );

  protected:

    NumericalAnalysisImplementationFactory* _factory;

    const std::vector< Vector3d< int32_t > >& _sites;
    const std::vector< Volume< float > >& _t2WeightedVolumes;
    const Volume< float >& _t1WeightedVolume;
    const bool& _t1MappingUsingSPGR;
    const RCPointer< Volume< float > >& _b1MappingVolumePointer;

    const std::vector< Vector >& _echoTimes;
    const Vector& _flipAngles;
    const Vector& _repetitionTimes;
    const Vector& _inversionTimes;
    const std::vector< double >& _t2NoiseStandardDeviations;
    const double& _t1NoiseStandardDeviation;

    const std::vector< RCPointer< OptimizerConstraint > >& _constraints;
    const std::vector< double >& _nlpOptimizerParameters;

    const std::vector< double >& _initialParameters;
    const Vector& _lowerParameterBoundaries;
    const Vector& _upperParameterBoundaries;
    const std::vector< bool >& _isFixedParameters;
    const Vector& _deltaParameters;
    const bool& _applyMCMC;
    const int32_t& _mcmcBurninCount;
    const int32_t& _mcmcSampleCount;
    const int32_t& _mcmcIntervalCount;
    const int32_t& _mcmcMaximumIterationCount;

    Volume< float >& _myelinWaterFractionVolume;
    Volume< float >& _myelinWaterT1Volume;
    Volume< float >& _myelinWaterT2Volume;
    Volume< float >& _grayWhiteFractionVolume;
    Volume< float >& _grayWhiteT1Volume;
    Volume< float >& _grayWhiteT2Volume;
    Volume< float >& _csfFractionVolume;
    Volume< float >& _csfT1Volume;
    Volume< float >& _csfT2Volume;
    Volume< float >& _protonDensityVolume;
    Volume< float >& _protonDensityScalingVolume;
    Volume< float >& _t2WeightedScalingVolume;

    const bool& _computeFittedData;
    Volume< float >& _fittedDataVolume;

    const bool& _verbose;

    int32_t _t2WeightedVolumeCount;
    int32_t _globalT2WeightedMeasurementCount;
    std::vector< int32_t > _t2WeightedMeasurementCounts;
    int32_t _t1WeightedMeasurementCount;


};


}


#endif
