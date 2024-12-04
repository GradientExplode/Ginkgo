#ifndef _gkg_fmri_stimulus_DiscreteStimulus_h_
#define _gkg_fmri_stimulus_DiscreteStimulus_h_


#include <gkg-fmri-stimulus/Stimulus.h>


namespace gkg
{


class DiscreteStimulus : public Stimulus
{

  public:

    DiscreteStimulus( const std::string& name,
                      const Vector& times,
                      const Vector& durations,
                      const Vector& magnitudes );
    virtual ~DiscreteStimulus();

    const Vector& getTimes() const;
    const Vector& getDurations() const;
    const Vector& getMagnitudes() const;

    void sample( const Vector& times,
                 Vector& result ) const;
    void convolve( const HemodynamicResponseFunction& hrf,
                   const Vector& times,
                   Vector& result ) const;

  protected:

    Vector _times;
    Vector _durations;
    Vector _magnitudes;

};


}


#endif

