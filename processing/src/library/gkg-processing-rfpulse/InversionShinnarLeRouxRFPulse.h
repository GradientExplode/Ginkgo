#ifndef _gkg_processing_rfpulse_InversionShinnarLeRouxRFPulse_h_
#define _gkg_processing_rfpulse_InversionShinnarLeRouxRFPulse_h_


#include <gkg-processing-rfpulse/ShinnarLeRouxRFPulse.h>


namespace gkg
{


class FIRFilterDesignMethod;


class InversionShinnarLeRouxRFPulse : public ShinnarLeRouxRFPulse
{

  public:

    InversionShinnarLeRouxRFPulse(
                                 int32_t sampleCount,
                                 double timeBandWidth,
                                 double passBandRipple,
                                 double stopbandRipple,
                                 FIRFilterDesignMethod* firFilterDesignMethod );
    virtual ~InversionShinnarLeRouxRFPulse();

};


}


#endif
