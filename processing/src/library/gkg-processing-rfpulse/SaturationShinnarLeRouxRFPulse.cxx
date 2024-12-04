#include <gkg-processing-rfpulse/SaturationShinnarLeRouxRFPulse.h>
#include <gkg-processing-signal/FIRFilterDesignMethod.h>
#include <gkg-processing-rfpulse/SaturationCayleyKleinParameters.h>
#include <gkg-processing-rfpulse/SaturationRippleTransform.h>


gkg::SaturationShinnarLeRouxRFPulse::SaturationShinnarLeRouxRFPulse(
                             int32_t sampleCount,
                             double timeBandWidth,
                             double passBandRipple,
                             double stopBandRipple,
                             gkg::FIRFilterDesignMethod* firFilterDesignMethod )
                   : gkg::ShinnarLeRouxRFPulse(
                        &gkg::SaturationCayleyKleinParameters::getInstance(),
                        &gkg::SaturationRippleTransform::getInstance(),
                        sampleCount,
                        timeBandWidth,
                        passBandRipple,
                        stopBandRipple,
                        firFilterDesignMethod )
{
}


gkg::SaturationShinnarLeRouxRFPulse::~SaturationShinnarLeRouxRFPulse()
{
}
