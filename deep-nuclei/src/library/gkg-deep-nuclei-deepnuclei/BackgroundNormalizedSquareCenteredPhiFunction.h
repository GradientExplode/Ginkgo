#ifndef _gkg_deep_nuclei_deepnuclei_BackgroundNormalizedSquareCenteredPhiFunction_h_
#define _gkg_deep_nuclei_deepnuclei_BackgroundNormalizedSquareCenteredPhiFunction_h_


#include <gkg-processing-numericalanalysis/PhiFunction.h>


namespace gkg
{


class BackgroundNormalizedSquareCenteredPhiFunction : public PhiFunction
{

  public:

    BackgroundNormalizedSquareCenteredPhiFunction( double grayMatterMean,
                                                  double grayMatterStdDev,
                                                  double whiteMatterMean,
                                                  double whiteMatterStdDev );
    virtual ~BackgroundNormalizedSquareCenteredPhiFunction();

    double getValue( double x ) const;

  protected:

    double _grayMatterMean;
    double _grayMatterStdDev;
    double _whiteMatterMean;
    double _whiteMatterStdDev;


};


}


#endif
