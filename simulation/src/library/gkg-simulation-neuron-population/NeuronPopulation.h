#ifndef _gkg_simulation_neuron_population_NeuronPopulation_h_
#define _gkg_simulation_neuron_population_NeuronPopulation_h_


#include <gkg-simulation-virtual-tissue/Population.h>
#include <gkg-simulation-neuron-population/Neuron.h>
#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-pattern/RCPointer.h>
#include <map>
#include <vector>


namespace gkg
{


class NeuronPopulation : public Population,
                         public Creator5Arg< NeuronPopulation,
                                             Population,
                                             int32_t,
                                             const VirtualTissue*,
                                             const Dictionary&,
                                             bool,
                                             std::ofstream* >
{

  public:

    virtual ~NeuronPopulation();

    NeuronPopulation( const NeuronPopulation& other );

    NeuronPopulation& operator=( const NeuronPopulation& other );

    std::string getTypeName() const; 

    static std::string getStaticName();

  protected:

    friend struct Creator5Arg< NeuronPopulation,
                               Population,
                               int32_t,
                               const VirtualTissue*,
                               const Dictionary&,
                               bool,
                               std::ofstream* >;

    NeuronPopulation( int32_t id,
                      const VirtualTissue* parent,
                      const Dictionary& parameters,
                      bool verbose,
                      std::ofstream* osLog );

    static
    std::map< std::string,
              std::map< std::string,
                        std::vector< VirtualTissue > > >
      _neuronModels;
 
    static
    std::map< std::string,
              std::map< std::string,
                        std::vector< float > > >
      _neuronModelDiameters;

    static
    std::map< std::string,
              std::map< std::string,
                        std::vector< Vector3d< float > > > >
      _neuronModelCenters;

    static
    std::map< std::string,
              std::map< std::string,
                        std::vector< float > > >
      _neuronModelVolumes;

    static void loadNeuronModels( const std::string& neuronDatabaseName,
                                  const std::string& neuronTypeName );

    std::string _neuronDatabaseName;
    std::string _neuronTypeName;
    RCPointer< ParameterDistribution > _somaDiameterDistribution;
    float _volumeFraction;

    std::vector< gkg::RCPointer< gkg::Neuron > > _neurons;


};


}


#endif
