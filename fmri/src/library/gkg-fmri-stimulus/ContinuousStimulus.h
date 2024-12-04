#ifndef _gkg_fmri_stimulus_ContinuousStimulus_h_
#define _gkg_fmri_stimulus_ContinuousStimulus_h_


#include <gkg-fmri-stimulus/Stimulus.h>


namespace gkg
{


class ContinuousStimulus : public Stimulus
{

  public:

    ContinuousStimulus( const std::string& name,
                        double startingTime,
                        double samplingPeriod,
                        const Vector& magnitudes );
    virtual ~ContinuousStimulus();

    double getStartingTime() const;
    double getSamplingPeriod() const;
    const Vector& getMagnitudes() const;

    void sample( const Vector& times,
                 Vector& result ) const;
    void convolve( const HemodynamicResponseFunction& hrf,
                   const Vector& times,
                   Vector& result ) const;

  protected:

    double _startingTime;
    double _samplingPeriod;
    Vector _magnitudes;

};


}


#endif

