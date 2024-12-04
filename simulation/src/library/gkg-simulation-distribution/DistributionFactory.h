#ifndef _gkg_simulation_distribution_DistributionFactory_h_
#define _gkg_simulation_distribution_DistributionFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-core-object/Dictionary.h>
#include <map>
#include <set>


namespace gkg
{


class DistributionFactory : public Singleton< DistributionFactory >
{

  public:

    typedef ParameterDistribution* ( *Creator )( const Dictionary& );

    ~DistributionFactory();

    bool registerDistributionCreator( const std::string& name,
                                      Creator creator );

    ParameterDistribution* create( const Dictionary& dictionary ) const;

    std::set< std::string > getDistributionTypeNames() const; 

  protected:

    friend class Singleton< DistributionFactory >;

    DistributionFactory();

    std::map< std::string, Creator > _creators;


};


}


#define RegisterDistributionCreator( NameSpace, ConcreteDistribution )         \
static bool                                                                    \
registeredDistributionCreator_##NameSpace##_##ConcreteDistribution             \
  __attribute__((unused)) =                                                    \
  gkg::DistributionFactory::getInstance().registerDistributionCreator(         \
               gkg::ConcreteDistribution::getStaticName(),                     \
               gkg::ConcreteDistribution::createInstance )


#endif
