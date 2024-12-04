#ifndef _gkg_simulation_neuron_population_Neuron_h_
#define _gkg_simulation_neuron_population_Neuron_h_


#include <gkg-simulation-virtual-tissue/Cell.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <vector>
#include <string>


namespace gkg
{


class NeuronPopulation;


class Neuron : public Cell
{

  public:

    Neuron( int32_t id,
            const NeuronPopulation* parent,
            const VirtualTissue& neuronModel,
            float neuronModelDiameter,
            const Vector3d< float >& neuronModelCenter,
            float neuronModelVolume,
            const Vector3d< float >& randomRotationParameters,
            float targetDiameter,
            const Vector3d< float >& targetCenter );
    Neuron( const Neuron& other );
    virtual ~Neuron();

    Neuron& operator=( const Neuron& other );

    float getDiameter() const;
    const Vector3d< float >& getCenter() const;
    float getVolume() const;

    void fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                       meshAccumulators ) const;
    void addDetails();

  protected:

    float _diameter;
    Vector3d< float > _center;
    float _volume;

};



}


#endif
