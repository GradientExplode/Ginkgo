#ifndef _gkg_simulation_virtual_tissue_PopulationFactory_h_
#define _gkg_simulation_virtual_tissue_PopulationFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-simulation-virtual-tissue/Population.h>
#include <gkg-core-object/Dictionary.h>
#include <map>
#include <set>
#include <fstream>


namespace gkg
{


class PopulationFactory : public Singleton< PopulationFactory >
{

  public:

    typedef Population* ( *Creator )( int32_t,
                                      const VirtualTissue*,
                                      const Dictionary&,
                                      bool,
                                      std::ofstream* );

    ~PopulationFactory();

    bool registerPopulationCreator( const std::string& name,
                                    Creator creator );

    Population* create( int32_t id, 
                        const VirtualTissue* parent,
                        const Dictionary& dictionary,
                        bool verbose,
                        std::ofstream* osLog ) const;

    std::set< std::string > getPopulationTypeNames() const; 

  protected:

    friend class Singleton< PopulationFactory >;

    PopulationFactory();

    std::map< std::string, Creator > _creators;


};


}


#define RegisterPopulationCreator( NameSpace, ConcretePopulation )             \
static bool registeredPopulationCreator_##NameSpace##_##ConcretePopulation     \
  __attribute__((unused)) =                                                    \
  gkg::PopulationFactory::getInstance().registerPopulationCreator(             \
               gkg::ConcretePopulation::getStaticName(),                       \
               gkg::ConcretePopulation::createInstance )


#endif
