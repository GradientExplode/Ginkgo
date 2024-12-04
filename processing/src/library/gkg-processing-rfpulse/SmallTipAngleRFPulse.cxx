#include <gkg-processing-rfpulse/SmallTipAngleRFPulse.h>
#include <gkg-processing-rfpulse/CayleyKleinParameters.h>
#include <gkg-processing-rfpulse/SmallTipAngleRippleTransform.h>
#include <gkg-processing-signal/FIRFilterDesignMethod.h>


gkg::SmallTipAngleRFPulse::SmallTipAngleRFPulse(
                             gkg::CayleyKleinParameters* cayleyKleinParameters,
                             int32_t sampleCount,
                             double timeBandWidth,
                             double passBandRipple,
                             double stopBandRipple,
                             gkg::FIRFilterDesignMethod* firFilterDesignMethod )
                          : gkg::RFPulse( timeBandWidth,
                                          passBandRipple,
                                          stopBandRipple,
                                          firFilterDesignMethod ),
                            _cayleyKleinParameters( cayleyKleinParameters ),
                            _rippleTransform( 
                             &gkg::SmallTipAngleRippleTransform::getInstance() )
{

  _samples = _firFilterDesignMethod->getImpulseResponse( 
                       sampleCount,
                       _timeBandWidth,
                       _rippleTransform->getPassBandRipple( _passBandRipple ),
                       _rippleTransform->getStopBandRipple( _stopBandRipple ) );

}


gkg::SmallTipAngleRFPulse::~SmallTipAngleRFPulse()
{
}
