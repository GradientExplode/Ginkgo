#ifndef _gkg_processing_rfpulse_ExcitationShinnarLeRouxRFPulse_h_
#define _gkg_processing_rfpulse_ExcitationShinnarLeRouxRFPulse_h_


#include <gkg-processing-rfpulse/ShinnarLeRouxRFPulse.h>


namespace gkg
{


class FIRFilterDesignMethod;


class ExcitationShinnarLeRouxRFPulse : public ShinnarLeRouxRFPulse
{

  public:

    ExcitationShinnarLeRouxRFPulse(
                                 int32_t sampleCount,
                                 double timeBandWidth,
                                 double passBandRipple,
                                 double stopbandRipple,
                                 FIRFilterDesignMethod* firFilterDesignMethod );
    virtual ~ExcitationShinnarLeRouxRFPulse();

};


}


#endif
