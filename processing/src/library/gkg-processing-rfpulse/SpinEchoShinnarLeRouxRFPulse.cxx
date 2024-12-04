#include <gkg-processing-rfpulse/SpinEchoShinnarLeRouxRFPulse.h>
#include <gkg-processing-signal/FIRFilterDesignMethod.h>
#include <gkg-processing-rfpulse/SpinEchoCayleyKleinParameters.h>
#include <gkg-processing-rfpulse/SpinEchoRippleTransform.h>


gkg::SpinEchoShinnarLeRouxRFPulse::SpinEchoShinnarLeRouxRFPulse(
                             int32_t sampleCount,
                             double timeBandWidth,
                             double passBandRipple,
                             double stopBandRipple,
                             gkg::FIRFilterDesignMethod* firFilterDesignMethod )
                   : gkg::ShinnarLeRouxRFPulse(
                        &gkg::SpinEchoCayleyKleinParameters::getInstance(),
                        &gkg::SpinEchoRippleTransform::getInstance(),
                        sampleCount,
                        timeBandWidth,
                        passBandRipple,
                        stopBandRipple,
                        firFilterDesignMethod )
{
}


gkg::SpinEchoShinnarLeRouxRFPulse::~SpinEchoShinnarLeRouxRFPulse()
{
}
