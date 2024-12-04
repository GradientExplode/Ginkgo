#ifndef _gkg_simulation_distribution_ConstantDistribution_h_
#define _gkg_simulation_distribution_ConstantDistribution_h_


#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class ConstantDistribution : public ParameterDistribution,
                             public Creator1Arg< ConstantDistribution,
                                                 ParameterDistribution,
                                                 const Dictionary& >
{


  public:

    virtual ~ConstantDistribution();

    virtual float getRandomValue() const;

    static std::string getStaticName();

  protected:

    friend struct Creator1Arg< ConstantDistribution,
                               ParameterDistribution,
                               const Dictionary& >;

    ConstantDistribution( const Dictionary& parameters );

    float _constantValue;

};


}



#endif


