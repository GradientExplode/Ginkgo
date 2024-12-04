#ifndef _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMapperLoopContext_h_
#define _gkg_qmri_plugin_functors_SingleCompartmentRelaxometryMapper_SingleCompartmentRelaxometryMapperLoopContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSet.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>
#include <string>


namespace gkg
{


class SingleCompartmentRelaxometryMapperGauge;


class SingleCompartmentRelaxometryMapperLoopContext :
                                                   public LoopContext< int32_t >
{

  public:

    SingleCompartmentRelaxometryMapperLoopContext(
      SingleCompartmentRelaxometryMapperGauge& gauge,
      const std::vector< Volume< float > >& inputVolumes,
      const std::vector< Vector3d< int32_t > >& sites,
      const std::string& qMriMappingType,
      const std::vector< double >& optimizerParameters,
      const std::vector< double >& scalarParameters,
      const RCPointer< AcquisitionParameterSet >& acquisitionParameterSet,
      const int32_t& inputVolumeCount,
      const std::vector< int32_t >& measurementCounts,
      const int32_t& globalMeasurementCount,
      const bool& verbose,
      Volume< float >& protonDensityVolume,
      Volume< float >& relaxationTimeVolume,
      Volume< float >& scalingVolume,
      const bool& computeFittedMeasurements,
      Volume< float >& fittedMeasurementVolume );
    virtual ~SingleCompartmentRelaxometryMapperLoopContext();

    void doIt( int32_t startIndex, int32_t count ); 

  protected:

    const std::vector< gkg::Volume< float > >& _inputVolumes;
    const std::vector< Vector3d< int32_t > >& _sites;
    const std::string& _qMriMappingType;
    const std::vector< double >& _optimizerParameters;
    const std::vector< double >& _scalarParameters;
    const RCPointer< AcquisitionParameterSet >& _acquisitionParameterSet;
    const int32_t& _inputVolumeCount;
    const std::vector< int32_t >& _measurementCounts;
    const int32_t& _globalMeasurementCount;
    const bool& _verbose;
    Volume< float >& _protonDensityVolume;
    Volume< float >& _relaxationTimeVolume;
    Volume< float >& _scalingVolume;
    const bool& _computeFittedMeasurements;
    Volume< float >& _fittedMeasurementVolume;

    NumericalAnalysisImplementationFactory* _factory; 

    Vector _initialParameters;
    Vector _lowerParameterBoundaries;
    Vector _upperParameterBoundaries;
    std::vector< bool > _isFixedParameters;
    Vector _deltaParameters;

    std::vector< RCPointer< OptimizerConstraint > > _constraints;
    std::vector< double > _nlpOptimizerParameters;

    bool _applyMCMC;
    int32_t _mcmcBurninCount;
    int32_t _mcmcSampleCount;
    int32_t _mcmcIntervalCount;
    int32_t _mcmcMaximumIterationCount;

};


}


#endif

