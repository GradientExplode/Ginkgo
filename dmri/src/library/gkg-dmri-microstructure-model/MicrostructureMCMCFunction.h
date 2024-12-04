#ifndef _gkg_dmri_microstructure_model_MicrostructureMCMCFunction_h_
#define _gkg_dmri_microstructure_model_MicrostructureMCMCFunction_h_


#include <gkg-processing-numericalanalysis/MonteCarloMarkovChainFunction.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-object/Dictionary.h>

#include <vector>


namespace gkg
{


class MicrostructureMCMCFunction : public MonteCarloMarkovChainFunction
{

  public:

    MicrostructureMCMCFunction( RCPointer< Volume< float > > dw,
                                const OrientationSet& outputOrientationSet,
                                double noiseStandardDeviation );
    virtual ~MicrostructureMCMCFunction();

    double getLikelihoodRatio( const Vector& realMeasurements,
                               const Vector& currentMeasurements,
                               const Vector& newMeasurements ) const;

    virtual void getModelAt(
             const Vector& parameters,
             const std::vector< float >& multipleShellBValues,
             const std::vector< std::vector< float > >& gradientCharacteristics,
             int32_t inputOrientationCount,
             std::vector< std::vector< float > >& values,
             std::vector< float >& coordinates ) const;

    virtual OrientationDistributionFunction::BasisType getType() const = 0;
    virtual int32_t getParameterCount() const = 0;

    int32_t getInputOrientationCount() const;
    float getBValue( int32_t index ) const;
    const Vector3d< float >&  getInputOrientation( int32_t index ) const;
    void getMultipleShellBValues(
                   std::string& sequenceType,
                   std::vector< std::vector< float > >& gradientCharacteristics,
                   std::vector< float >& multipleShellBValues,
                   std::vector< std::vector< int32_t > >& orientationIndexSets,
                   float deltaBValue ) const;

    const Dictionary& getGradientCharacteristics() const;

    int32_t getOutputOrientationCount() const;
    const Vector3d< float >&  getOutputOrientation( int32_t index ) const;

    double getNoiseStandardDeviation() const;
    double getNoiseVariance() const;


  protected:

    NumericalAnalysisImplementationFactory* _factory;

    int32_t _inputOrientationCount;
    std::vector< float > _bValues;
    std::vector< Vector3d< float > > _inputOrientations;
    Dictionary _gradientCharacteristics;

    int32_t _outputOrientationCount;
    std::vector< Vector3d< float > > _outputOrientations;

    double _noiseStandardDeviation;
    double _noiseVariance;

};


}


#endif
