#ifndef _gkg_fmri_paradigm_Paradigm_h
#define _gkg_fmri_paradigm_Paradigm_h


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-fmri-stimulus/Stimulus.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <list>
#include <vector>
#include <string>


namespace gkg
{


class Paradigm : public HeaderedObject
{

  public:

    Paradigm();
    virtual ~Paradigm();

    void addDiscreteStimulus( const std::string& name,
                              const Vector& times,
                              const Vector& durations,
                              const Vector& magnitudes );

    void addContinuousStimulus( const std::string& name,
                                double startingTime,
                                double samplingPeriod,
                                const Vector& magnitudes );

    int32_t getRegressorCount() const;
    const std::list< Stimulus* >& getStimuli() const;

    void getDesignMatrix( const Vector& times,
                          const HemodynamicResponseFunction& hrf,
                          int32_t driftPolynomOrder,
                          Matrix& designMatrix,
                          bool verbose = false );

    void getDesignLabels( const gkg::Vector& times,
                          std::vector< std::string >& labels );

  protected:

    std::list< Stimulus* > _stimuli;
    int32_t _regressorCount;
    int32_t _driftPolynomOrder;
    bool _firstCall;

};


}


#endif
