#ifndef _gkg_qmri_plugin_functors_MultiCompartmentRelaxometryMapper_MultiCompartmentRelaxometryMapperLoopContext_h_
#define _gkg_qmri_plugin_functors_MultiCompartmentRelaxometryMapper_MultiCompartmentRelaxometryMapperLoopContext_h_


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


class MultiCompartmentRelaxometryMapperGauge; 


class MultiCompartmentRelaxometryMapperLoopContext :
                                                  public LoopContext< int32_t >
{

  public:
  
    MultiCompartmentRelaxometryMapperLoopContext( 
             MultiCompartmentRelaxometryMapperGauge& gauge, 
             const std::vector< Vector3d< int32_t > >& sites, 
             const bool& t2Relaxometry, 
             const bool& t1Relaxometry, 
             const bool& competitiveT2, 
             const bool& competitiveT1, 
             const std::vector< Volume< float > >& t2WeightedVolumes, 
             const std::vector< Volume< float > >& t1WeightedVolumes, 
             const RCPointer< Volume< float > >& b1MappingVolumePointer,
             const RCPointer< Volume< float > >& hcMappingVolumePointer,
             const RCPointer< Volume< float > >& bcMappingVolumePointer,
             const std::vector< Vector >& echoTimes, 
             const Vector& flipAngles, 
             const std::vector< Vector >& repetitionTimes, 
             const std::vector< double >& t2NoiseStandardDeviations, 
             const std::vector< double >& t1NoiseStandardDeviations, 
             const std::vector< RCPointer< OptimizerConstraint > >& constraints, 
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
             Volume< float >& parenchymaFractionVolume, 
             Volume< float >& parenchymaProtonDensityVolume, 
             Volume< float >& parenchymaT1Volume, 
             Volume< float >& parenchymaT2Volume, 
             std::vector< Volume< float > >& arteriesFractionVolumes,
             Volume< float >& arteriesProtonDensityVolume, 
             std::vector< Volume< float > >& arteriesT1Volumes,
             std::vector< Volume< float > >& arteriesT2Volumes,
             Volume< float >& csfFractionVolume,
             Volume< float >& csfProtonDensity, 
             Volume< float >& csfT1Volume, 
             Volume< float >& csfT2Volume, 
             Volume< float >& t2WeightedScalingVolume, 
             Volume< float >& t1WeightedScalingVolume, 
             const bool& computeFittedData, 
             Volume< float >& fittedDataVolume, 
             const int32_t& t2WeightedVolumeCount, 
             const int32_t& t1WeightedVolumeCount, 
             const int32_t& globalT2WeightedMeasurementCount, 
             const int32_t& globalT1WeightedMeasurementCount, 
             const std::vector< int32_t >& t2WeightedMeasurementCounts, 
             const std::vector< int32_t >& t1WeightedMeasurementCounts, 
             const int32_t& arteryCount, 
             const int32_t& parameterCount, 
             const bool& hasCSF, 
             const bool& verbose ); 
    virtual ~MultiCompartmentRelaxometryMapperLoopContext(); 
    
    
    void doIt( int32_t startIndex, int32_t count ); 
    
  protected: 
  
    NumericalAnalysisImplementationFactory* _factory; 
    
    const std::vector< Vector3d< int32_t > >& _sites;
    
    const bool& _t2Relaxometry; 
    const bool& _t1Relaxometry; 

    const bool& _competitiveT2; 
    const bool& _competitiveT1; 
    
    const std::vector< Volume< float > >& _t2WeightedVolumes;
    const std::vector< Volume< float > >& _t1WeightedVolumes;
    const RCPointer< Volume< float > >& _b1MappingVolumePointer;
    const RCPointer< Volume< float > >& _hcMappingVolumePointer;
    const RCPointer< Volume< float > >& _bcMappingVolumePointer;

    const std::vector< Vector >& _echoTimes; 
    const Vector& _flipAngles; 
    const std::vector< Vector >& _repetitionTimes;
    
    const std::vector< double >& _t2NoiseStandardDeviations;
    const std::vector< double >& _t1NoiseStandardDeviations;

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

    Volume< float >& _parenchymaFractionVolume; 
    Volume< float >& _parenchymaProtonDensityVolume; 
    Volume< float >& _parenchymaT1Volume; 
    Volume< float >& _parenchymaT2Volume; 
    std::vector< Volume< float > >& _arteriesFractionVolumes;
    Volume< float >& _arteriesProtonDensityVolume; 
    std::vector< Volume< float > >& _arteriesT1Volumes; 
    std::vector< Volume< float > >& _arteriesT2Volumes; 
    Volume< float >& _csfFractionVolume;
    Volume< float >& _csfProtonDensityVolume;
    Volume< float >& _csfT1Volume;
    Volume< float >& _csfT2Volume;
    
    Volume< float >& _t2WeightedScalingVolume; 
    Volume< float >& _t1WeightedScalingVolume; 
    
    const bool& _computeFittedData;
    Volume< float >& _fittedDataVolume;

    const int32_t& _t2WeightedVolumeCount;
    const int32_t& _t1WeightedVolumeCount;
    const int32_t& _globalT2WeightedMeasurementCount;
    const int32_t& _globalT1WeightedMeasurementCount;
    const std::vector< int32_t >& _t2WeightedMeasurementCounts; 
    const std::vector< int32_t >& _t1WeightedMeasurementCounts; 
    
    const int32_t& _arteryCount; 
    const int32_t& _parameterCount; 
    const bool& _hasCSF; 

    const bool& _verbose;

};


}


#endif
