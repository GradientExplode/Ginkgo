#ifndef _gkg_dmri_tractography_AlgorithmFactory_h_
#define _gkg_dmri_tractography_AlgorithmFactory_h_


#include <string>
#include <map>
#include <list>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-plugin-functors/MpiTest/Algorithm.h>


namespace gkg
{


class AlgorithmFactory : public Singleton< AlgorithmFactory >
{

  public:

    typedef Algorithm* ( *AlgorithmCreator )( const std::string& );

    ~AlgorithmFactory();

    bool registerAlgorithm( const std::string& name,
                            AlgorithmCreator AlgorithmCreator );

    Algorithm* create( const std::string& name, const std::string& ) const;

    bool hasAlgorithm( const std::string& name ) const;
    std::list< std::string > getAlgorithmNames() const;

  protected:

    friend class Singleton< AlgorithmFactory >;

    AlgorithmFactory();

    std::map< std::string, AlgorithmCreator > _AlgorithmCreators;

};


}


#define RegisterPopulationCreator( NameSpace, ConcreteAlgorithm )             \
static bool registeredPopulationCreator_##NameSpace##_##ConcretePopulation     \
  __attribute__((unused)) =                                                    \
  gkg::PopulationFactory::getInstance().registerPopulationCreator(             \
               gkg::ConcreteAlgorithm::getStaticName(),                       \
               gkg::ConcreteAlgorithm::createInstance )


#endif

