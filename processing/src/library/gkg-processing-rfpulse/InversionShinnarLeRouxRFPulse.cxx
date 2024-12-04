#include <gkg-processing-rfpulse/InversionShinnarLeRouxRFPulse.h>
#include <gkg-processing-signal/FIRFilterDesignMethod.h>
#include <gkg-processing-rfpulse/InversionCayleyKleinParameters.h>
#include <gkg-processing-rfpulse/InversionRippleTransform.h>


gkg::InversionShinnarLeRouxRFPulse::InversionShinnarLeRouxRFPulse(
                             int32_t sampleCount,
                             double timeBandWidth,
                             double passBandRipple,
                             double stopBandRipple,
                             gkg::FIRFilterDesignMethod* firFilterDesignMethod )
                   : gkg::ShinnarLeRouxRFPulse(
                        &gkg::InversionCayleyKleinParameters::getInstance(),
                        &gkg::InversionRippleTransform::getInstance(),
                        sampleCount,
                        timeBandWidth,
                        passBandRipple,
                        stopBandRipple,
                        firFilterDesignMethod )
{
}


gkg::InversionShinnarLeRouxRFPulse::~InversionShinnarLeRouxRFPulse()
{
}
