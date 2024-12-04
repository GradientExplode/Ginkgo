#ifndef _gkg_processing_extrapolation_ExtrapolatorFunction_h_
#define _gkg_processing_extrapolation_ExtrapolatorFunction_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class ExtrapolatorFunction
{

  public:

    virtual ~ExtrapolatorFunction();

    virtual double getValue( int32_t x, int32_t degree, 
                             int32_t size ) const = 0;

  protected:

    ExtrapolatorFunction();


};


}


#endif
