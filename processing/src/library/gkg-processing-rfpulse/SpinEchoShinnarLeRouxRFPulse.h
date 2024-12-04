#ifndef _gkg_processing_rfpulse_SpinEchoShinnarLeRouxRFPulse_h_
#define _gkg_processing_rfpulse_SpinEchoShinnarLeRouxRFPulse_h_


#include <gkg-processing-rfpulse/ShinnarLeRouxRFPulse.h>


namespace gkg
{


class FIRFilterDesignMethod;


class SpinEchoShinnarLeRouxRFPulse : public ShinnarLeRouxRFPulse
{

  public:

    SpinEchoShinnarLeRouxRFPulse(
                                 int32_t sampleCount,
                                 double timeBandWidth,
                                 double passBandRipple,
                                 double stopbandRipple,
                                 FIRFilterDesignMethod* firFilterDesignMethod );
    virtual ~SpinEchoShinnarLeRouxRFPulse();

};


}


#endif
