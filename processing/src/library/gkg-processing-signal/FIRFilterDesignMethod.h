#ifndef _gkg_processing_signal_FIRFilterDesignMethod_h_
#define _gkg_processing_signal_FIRFilterDesignMethod_h_


#include <gkg-core-cppext/StdInt.h>
#include <vector>
#include <complex>


namespace gkg
{


class FIRFilterDesignMethod
{

  public:

    virtual ~FIRFilterDesignMethod();

    virtual std::vector< std::complex< double > >
      getImpulseResponse( int32_t sampleCount,
                          double timeBandWidth,
                          double passBandRipple,
                          double stopBandRipple ) const = 0;

  protected:

    FIRFilterDesignMethod();

};


}


#endif
