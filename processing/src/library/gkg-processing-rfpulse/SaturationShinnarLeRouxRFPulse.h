#ifndef _gkg_processing_rfpulse_SaturationShinnarLeRouxRFPulse_h_
#define _gkg_processing_rfpulse_SaturationShinnarLeRouxRFPulse_h_


#include <gkg-processing-rfpulse/ShinnarLeRouxRFPulse.h>


namespace gkg
{


class FIRFilterDesignMethod;


class SaturationShinnarLeRouxRFPulse : public ShinnarLeRouxRFPulse
{

  public:

    SaturationShinnarLeRouxRFPulse(
                                 int32_t sampleCount,
                                 double timeBandWidth,
                                 double passBandRipple,
                                 double stopbandRipple,
                                 FIRFilterDesignMethod* firFilterDesignMethod );
    virtual ~SaturationShinnarLeRouxRFPulse();

};


}


#endif
