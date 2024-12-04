#ifndef _gkg_processing_rfpulse_SmallTipAngleRFPulse_h_
#define _gkg_processing_rfpulse_SmallTipAngleRFPulse_h_


#include <gkg-processing-rfpulse/RFPulse.h>


namespace gkg
{


class CayleyKleinParameters;
class RippleTransform;


class SmallTipAngleRFPulse : public RFPulse
{

  public:

    SmallTipAngleRFPulse( CayleyKleinParameters* cayleyKleinParameters,
                          int32_t sampleCount,
                          double timeBandWidth,
                          double passBandRipple,
                          double stopBandRipple,
                          FIRFilterDesignMethod* firFilterDesignMethod );
    virtual ~SmallTipAngleRFPulse();

  protected:

    CayleyKleinParameters* _cayleyKleinParameters;
    RippleTransform* _rippleTransform;

};


}


#endif
