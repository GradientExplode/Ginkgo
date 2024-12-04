
#ifndef _gkg_simulation_mri_sequence_TrapezoidGradientPulse_h_
#define _gkg_simulation_mri_sequence_TrapezoidGradientPulse_h_


#include <vector>


namespace gkg
{


// gradient magnitudes are provided in mT/m


class TrapezoidGradientPulse 
{

  public:

    TrapezoidGradientPulse( float startingTimeInUs,
                            float startingRampDurationInUs,
                            float plateauDurationInUs,
                            float endingRampDurationInUs,
                            float magnitudeInMilliTeslaPerMeter );
    virtual ~TrapezoidGradientPulse();

    float getStartingRampDurationInUs() const;
    float getPlateauDurationInUs() const;
    float getEndingRampDurationInUs() const;
    float getMagnitudeInMilliTeslaPerMeter() const;

    virtual float getValueAt( float timeInUs ) const;
    virtual float getIntegralValueAt( float timeInUs ) const;

  protected:

    float _startingRampDurationInUs;
    float _plateauDurationInUs;
    float _endingRampDurationInUs;
    float _magnitudeInMilliTeslaPerMeter;

    float _t0;
    float _t1;
    float _t2;
    float _t3;

};


}


#endif
