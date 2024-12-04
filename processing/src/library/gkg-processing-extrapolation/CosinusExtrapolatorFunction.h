#ifndef _gkg_processing_extrapolation_CosinusExtrapolatorFunction_h_
#define _gkg_processing_extrapolation_CosinusExtrapolatorFunction_h_


#include <gkg-processing-extrapolation/ExtrapolatorFunction.h>


namespace gkg
{


class CosinusExtrapolatorFunction : public ExtrapolatorFunction
{

  public:

    CosinusExtrapolatorFunction();
    ~CosinusExtrapolatorFunction();

    double getValue( int32_t x, int32_t degree, int32_t size ) const;

};


}


#endif
