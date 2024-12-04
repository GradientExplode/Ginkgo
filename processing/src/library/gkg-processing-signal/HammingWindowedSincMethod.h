#ifndef _gkg_processing_signal_HammingWindowedSincMethod_h_
#define _gkg_processing_signal_HammingWindowedSincMethod_h_


#include <gkg-processing-signal/FIRFilterDesignMethod.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class HammingWindowedSincMethod : public FIRFilterDesignMethod,
                                  public Singleton< HammingWindowedSincMethod >
{

  public:

    ~HammingWindowedSincMethod();

    std::vector< std::complex< double > >
      getImpulseResponse( int32_t sampleCount,
                          double timeBandWidth,
                          double passBandRipple,
                          double stopBandRipple ) const;

  protected:

    friend class Singleton< HammingWindowedSincMethod >;

    HammingWindowedSincMethod();

};


}


#endif
