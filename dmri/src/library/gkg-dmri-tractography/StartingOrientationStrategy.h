#ifndef _gkg_dmri_tractography_StartingOrientationStrategy_h_
#define _gkg_dmri_tractography_StartingOrientationStrategy_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>


namespace gkg
{



class StartingOrientationStrategy
{

  public:

    virtual ~StartingOrientationStrategy();

    virtual int32_t getStartingOrientation(
                       const OrientationDistributionFunction& odf ) = 0;

  protected:

    StartingOrientationStrategy();

};


}


#endif
