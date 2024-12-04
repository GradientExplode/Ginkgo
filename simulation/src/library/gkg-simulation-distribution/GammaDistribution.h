#ifndef _gkg_simulation_distribution_GammaDistribution_h_
#define _gkg_simulation_distribution_GammaDistribution_h_


#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class GammaDistribution : public ParameterDistribution,
                          public Creator1Arg< GammaDistribution,
                                              ParameterDistribution,
                                              const Dictionary& >
{


  public:

    virtual ~GammaDistribution();

    virtual float getRandomValue() const;

    static std::string getStaticName();

  protected:

    friend struct Creator1Arg< GammaDistribution,
                               ParameterDistribution,
                               const Dictionary& >;

    GammaDistribution( const Dictionary& parameters );

    float _scale;
    float _shape;
    float _multiplicator;
    float _scaleTimesMultiplicator;
    bool _hasUpperBoundary;
    float _upperBoundary;

};


}



#endif


