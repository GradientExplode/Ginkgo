#ifndef _gkg_simulation_fiber_population_Fiber_h_
#define _gkg_simulation_fiber_population_Fiber_h_


#include <gkg-simulation-virtual-tissue/Cell.h>
#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


class FiberPopulation;


class Fiber : public Cell
{

  public:

    Fiber( int32_t id,
           const FiberPopulation* parent,
           const BoundingBox< float >& fieldOfView,
           const Vector3d< float >& startingAtomCenter,
           float radiusOfSphereContainingFieldOfView,
           float atomOversamplingRatio,
           const Vector3d< float >& meanOrientation,
           float axonDiameter,
           float outerMyelinSheathDiameter,
           float internodalLength,
           float ranvierNodeWidth,
           float ranvierNodeOffset,
           bool hasTortuosity,
           bool hasBeading,
           bool hasMyelinSheath,
           bool hasRanvierNodes );
    Fiber( const Fiber& other );
    virtual ~Fiber();

    Fiber& operator=( const Fiber& other );

    float getLength() const;
    float getVolume() const;

    float addTortuosity(
                        const BoundingBox< float >& fieldOfView,
                        float magnitude,
                        float waveLength,
                        const RandomGenerator& randomGenerator,
                        const NumericalAnalysisImplementationFactory* factory );
    void addBeading( const RCPointer< ParameterDistribution >&
                                                 interbeadingLengthDistribution,
                     const RCPointer< ParameterDistribution >&
                                                       beadingWidthDistribution,
                     const RCPointer< ParameterDistribution >&
                                              beadingMagnitudeRatioDistribution,
                     const RandomGenerator& randomGenerator,
                     const NumericalAnalysisImplementationFactory* factory );

    void fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                       meshAccumulators ) const;
    void addDetails();

    LightCurve3d< float > 
      getLightCurve3d( int32_t subSamplingFactor = 1 ) const;
    std::vector< float > 
      getAxonRadii( int32_t subSamplingFactor = 1,
                    float scaling = 1.0f ) const;
    std::vector< float > 
      getMyelinSheathRadii( int32_t subSamplingFactor = 1,
                            float scaling = 1.0f ) const;

  protected:


    float computeLocalAngularDeviation( int32_t lowerAtomIndex, 
                                        int32_t upperAtomIndex ) const;
    float getBeadingRadiusRatio( float curvilinearAbscissa,
                                 float interBeadingLength,
                                 float beadingWidth,
                                 float beadingMagnitudeRatio ) const;

    float _atomOversamplingRatio;
    Vector3d< float > _meanOrientation;
    float _axonDiameter;
    float _outerMyelinSheathDiameter;
    float _internodalLength;
    float _ranvierNodeWidth;
    float _ranvierNodeOffset;
    bool _hasTortuosity;
    bool _hasBeading;
    bool _hasMyelinSheath;
    bool _hasRanvierNodes;

    int32_t _lastEnvelopeIndex;

};



}


#endif
