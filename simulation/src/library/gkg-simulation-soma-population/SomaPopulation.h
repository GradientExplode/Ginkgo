#ifndef _gkg_simulation_soma_population_SomaPopulation_h_
#define _gkg_simulation_soma_population_SomaPopulation_h_


#include <gkg-simulation-virtual-tissue/Population.h>
#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-pattern/RCPointer.h>



namespace gkg
{


class Soma;


class SomaPopulation : public Population,
                       public Creator5Arg< SomaPopulation,
                                           Population,
                                           int32_t,
                                           const VirtualTissue*,
                                           const Dictionary&,
                                           bool,
                                           std::ofstream* >
{

  public:

    virtual ~SomaPopulation();

    SomaPopulation( const SomaPopulation& other );

    SomaPopulation& operator=( const SomaPopulation& other );

    std::string getTypeName() const; 

    static std::string getStaticName();

  protected:

    friend struct Creator5Arg< SomaPopulation,
                               Population,
                               int32_t,
                               const VirtualTissue*,
                               const Dictionary&,
                               bool,
                               std::ofstream* >;

    SomaPopulation( int32_t id,
                    const VirtualTissue* parent,
                    const Dictionary& parameters,
                    bool verbose,
                    std::ofstream* osLog );

    RCPointer< ParameterDistribution > _somaDiameterDistribution;
    float _volumeFraction;

    std::vector< gkg::RCPointer< gkg::Soma > > _somas;


};


}


#endif
