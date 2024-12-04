#ifndef _gkg_dmri_tractography_PrincipalStartingOrientationStrategy_h_
#define _gkg_dmri_tractography_PrincipalStartingOrientationStrategy_h_


#include <gkg-dmri-tractography/StartingOrientationStrategy.h>



namespace gkg
{


class PrincipalStartingOrientationStrategy : public StartingOrientationStrategy
{

  public:

    PrincipalStartingOrientationStrategy();
    virtual ~PrincipalStartingOrientationStrategy();

    int32_t getStartingOrientation(
                                   const OrientationDistributionFunction& odf );

};


}



#endif

