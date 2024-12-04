#ifndef _gkg_simulation_glia_population_AstrocytePopulation_h_
#define _gkg_simulation_glia_population_AstrocytePopulation_h_


#include <gkg-simulation-glia-population/GliaPopulation.h>
#include <gkg-core-pattern/Creator.h>



namespace gkg
{


class Astrocyte;


class AstrocytePopulation : public GliaPopulation,
                            public Creator5Arg< AstrocytePopulation,
                                                Population,
                                                int32_t,
                                                const VirtualTissue*,
                                                const Dictionary&,
                                                bool,
                                                std::ofstream* >
{

  public:

    virtual ~AstrocytePopulation();

    AstrocytePopulation( const AstrocytePopulation& other );

    AstrocytePopulation& operator=( const AstrocytePopulation& other );

    std::string getTypeName() const; 

    static std::string getStaticName();

  protected:

    friend struct Creator5Arg< AstrocytePopulation,
                               Population,
                               int32_t,
                               const VirtualTissue*,
                               const Dictionary&,
                               bool,
                               std::ofstream* >;

    AstrocytePopulation( int32_t id,
                         const VirtualTissue* parent,
                         const Dictionary& parameters,
                         bool verbose,
                         std::ofstream* osLog );

    RCPointer< ParameterDistribution > _totalDiameterDistribution;

    std::vector< gkg::RCPointer< gkg::Astrocyte > > _astrocytes;

};


}


#endif
