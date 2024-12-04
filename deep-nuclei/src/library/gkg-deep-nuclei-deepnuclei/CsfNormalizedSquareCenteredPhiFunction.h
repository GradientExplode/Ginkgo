#ifndef _gkg_deep_nuclei_deepnuclei_CsfNormalizedSquareCenteredPhiFunction_h_
#define _gkg_deep_nuclei_deepnuclei_CsfNormalizedSquareCenteredPhiFunction_h_


#include <gkg-processing-numericalanalysis/PhiFunction.h>


namespace gkg
{


class CsfNormalizedSquareCenteredPhiFunction : public PhiFunction
{

  public:

    CsfNormalizedSquareCenteredPhiFunction( double mean,
                                            double stdDev,
                                            double factor = 1.0 );
    virtual ~CsfNormalizedSquareCenteredPhiFunction();

    double getValue( double x ) const;

  protected:

    double _mean;
    double _stdDev;
    double _factorOverTwo;

};


}


#endif
