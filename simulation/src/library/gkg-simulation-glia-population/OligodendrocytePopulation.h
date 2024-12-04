#ifndef _gkg_simulation_glia_population_OligodendrocytePopulation_h_
#define _gkg_simulation_glia_population_OligodendrocytePopulation_h_


#include <gkg-simulation-glia-population/GliaPopulation.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Creator.h>
#include <list>


namespace gkg
{


class Oligodendrocyte;


class OligodendrocytePopulation : public GliaPopulation,
                                  public Creator5Arg< OligodendrocytePopulation,
                                                      Population,
                                                      int32_t,
                                                      const VirtualTissue*,
                                                      const Dictionary&,
                                                      bool,
                                                      std::ofstream* >
{

  public:

    virtual ~OligodendrocytePopulation();

    OligodendrocytePopulation( const OligodendrocytePopulation& other );

    OligodendrocytePopulation& operator=(
                                       const OligodendrocytePopulation& other );

    std::string getTypeName() const;


    void addInteractionWithOtherPopulations();

    const RCPointer< Volume< RCPointer< std::list< Vector3d< float > > > > >&
      getFiberPopulationAtomCenterLut() const;
    bool getRandomSelectionOfClosestFiberAtomCenters(
                     const Vector3d< float >& somaCenter,
                     float searchRadius,
                     std::vector< Vector3d< float > >& fiberAtomCenters ) const;

    static std::string getStaticName();

  protected:

    friend struct Creator5Arg< OligodendrocytePopulation,
                               Population,
                               int32_t,
                               const VirtualTissue*,
                               const Dictionary&,
                               bool,
                               std::ofstream* >;

    OligodendrocytePopulation( int32_t id,
                               const VirtualTissue* parent,
                               const Dictionary& parameters,
                               bool verbose,
                               std::ofstream* osLog );

    RCPointer< ParameterDistribution > _myelinatedFiberSearchRadiusDistribution;

    std::vector< gkg::RCPointer< gkg::Oligodendrocyte > > _oligodendrocytes;

    RCPointer< Volume< RCPointer< std::list< Vector3d< float > > > > >
                                                  _fiberPopulationAtomCenterLut;

};


}


#endif
