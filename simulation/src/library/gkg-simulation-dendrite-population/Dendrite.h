#ifndef _gkg_simulation_dendrite_population_Dendrite_h_
#define _gkg_simulation_dendrite_population_Dendrite_h_


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


class DendritePopulation;


class Dendrite : public Cell
{

  public:

    Dendrite( int32_t id,
              const DendritePopulation* parent,
              const Vector3d< float >& startingAtomCenter,
              const Vector3d< float >& endingAtomCenter,
              float atomOversamplingRatio,
              float radius,
              float hasTortuosity,
              float tortuosityMagnitude,
              float tortuosityAngularDispersionInDegrees,
              float tortuosityWaveLength );
    Dendrite( const Dendrite& other );
    virtual ~Dendrite();

    Dendrite& operator=( const Dendrite& other );

    float getAtomOversamplingRatio() const;
    float getRadius() const;    
    bool getHasTortuosity() const;
    float getTortuosityMagnitude() const;
    float getTortuosityAngularDispersionInDegrees() const;
    float getTortuosityWaveLength() const;

    float getLength() const;

    float getVolume() const;

    void fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                       meshAccumulators ) const;
    void addDetails();

    LightCurve3d< float > 
      getLightCurve3d( int32_t subSamplingFactor = 1 ) const;

  protected:

    float _atomOversamplingRatio;
    float _radius;
    bool _hasTortuosity;
    float _tortuosityMagnitude;
    float _tortuosityAngularDispersionInDegrees;
    float _tortuosityWaveLength;


};



}


#endif
