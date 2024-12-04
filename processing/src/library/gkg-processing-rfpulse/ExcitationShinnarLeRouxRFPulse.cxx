#include <gkg-processing-rfpulse/ExcitationShinnarLeRouxRFPulse.h>
#include <gkg-processing-signal/FIRFilterDesignMethod.h>
#include <gkg-processing-rfpulse/ExcitationCayleyKleinParameters.h>
#include <gkg-processing-rfpulse/ExcitationRippleTransform.h>


gkg::ExcitationShinnarLeRouxRFPulse::ExcitationShinnarLeRouxRFPulse(
                             int32_t sampleCount,
                             double timeBandWidth,
                             double passBandRipple,
                             double stopBandRipple,
                             gkg::FIRFilterDesignMethod* firFilterDesignMethod )
                   : gkg::ShinnarLeRouxRFPulse(
                        &gkg::ExcitationCayleyKleinParameters::getInstance(),
                        &gkg::ExcitationRippleTransform::getInstance(),
                        sampleCount,
                        timeBandWidth,
                        passBandRipple,
                        stopBandRipple,
                        firFilterDesignMethod )
{
}


gkg::ExcitationShinnarLeRouxRFPulse::~ExcitationShinnarLeRouxRFPulse()
{
}
