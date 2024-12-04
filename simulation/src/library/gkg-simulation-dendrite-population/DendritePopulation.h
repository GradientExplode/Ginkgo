#ifndef _gkg_simulation_dendrite_population_DendritePopulation_h_
#define _gkg_simulation_dendrite_population_DendritePopulation_h_


#include <gkg-simulation-virtual-tissue/Population.h>
#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class Dendrite;


class DendritePopulation : public Population,
                           public Creator5Arg< DendritePopulation,
                                               Population,
                                               int32_t,
                                               const VirtualTissue*,
                                               const Dictionary&,
                                               bool,
                                               std::ofstream* >
{

  public:

    virtual ~DendritePopulation();

    DendritePopulation( const DendritePopulation& other );
    
    DendritePopulation& operator=( const DendritePopulation& other );

    std::string getTypeName() const; 

    static std::string getStaticName();

  protected:

    friend struct Creator5Arg< DendritePopulation,
                               Population,
                               int32_t,
                               const VirtualTissue*,
                               const Dictionary&,
                               bool,
                               std::ofstream* >;
    
    DendritePopulation( int32_t id,
                        const VirtualTissue* parent,
                        const Dictionary& parameters,
                        bool verbose,
                        std::ofstream* osLog );
                        
    float _atomOversamplingRatio;
    RCPointer< ParameterDistribution > _dendriteDiameterDistribution;    
    RCPointer< ParameterDistribution > _dendriteLengthDistribution;
    float _globalAngularDispersionInDegrees;
    bool _hasTortuosity;
    float _tortuosityMagnitude;
    float _tortuosityAngularDispersionInDegrees;
    float _tortuosityWaveLength;
    Vector3d< float > _meanOrientation;
    float _volumeFraction;

    std::vector< gkg::RCPointer< gkg::Dendrite > > _dendrites;
    

};


}


#endif
