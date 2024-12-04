#ifndef _gkg_dmri_tractography_GibbsSampledStartingOrientationStrategy_h_
#define _gkg_dmri_tractography_GibbsSampledStartingOrientationStrategy_h_


#include <gkg-dmri-tractography/StartingOrientationStrategy.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>



namespace gkg
{


class GibbsSampledStartingOrientationStrategy : 
                                              public StartingOrientationStrategy
{

  public:

    GibbsSampledStartingOrientationStrategy( float temperature );
    virtual ~GibbsSampledStartingOrientationStrategy();

    int32_t getStartingOrientation(
                                   const OrientationDistributionFunction& odf );

  protected:

    RandomGenerator _randomGenerator;
    float _temperature;

};


}



#endif

