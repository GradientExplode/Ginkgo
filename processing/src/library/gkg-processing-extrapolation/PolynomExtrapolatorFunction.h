#ifndef _gkg_processing_extrapolation_PolynomExtrapolatorFunction_h_
#define _gkg_processing_extrapolation_PolynomExtrapolatorFunction_h_


#include <gkg-processing-extrapolation/ExtrapolatorFunction.h>


namespace gkg
{


class PolynomExtrapolatorFunction : public ExtrapolatorFunction
{

  public:

    PolynomExtrapolatorFunction();
    ~PolynomExtrapolatorFunction();

    double getValue( int32_t x, int32_t degree, int32_t size ) const;

};


}


#endif
