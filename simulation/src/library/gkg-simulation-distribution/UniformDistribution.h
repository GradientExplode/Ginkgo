#ifndef _gkg_simulation_distribution_UniformDistribution_h_
#define _gkg_simulation_distribution_UniformDistribution_h_


#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class UniformDistribution : public ParameterDistribution,
                            public Creator1Arg< UniformDistribution,
                                                ParameterDistribution,
                                                const Dictionary& >
{


  public:

    virtual ~UniformDistribution();

    virtual float getRandomValue() const;

    static std::string getStaticName();

  protected:

    friend struct Creator1Arg< UniformDistribution,
                               ParameterDistribution,
                               const Dictionary& >;

    UniformDistribution( const Dictionary& parameters );

    double _lowerBoundary;
    double _upperBoundary;

};


}



#endif


