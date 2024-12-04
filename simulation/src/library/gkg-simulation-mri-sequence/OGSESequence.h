#ifndef _gkg_simulation_mri_sequence_OGSESequence_h_
#define _gkg_simulation_mri_sequence_OGSESequence_h_


#include <gkg-simulation-mri-sequence/MRISequence.h>
#include <gkg-simulation-mri-sequence/TrapezoidSineFunction.h>
#include <gkg-simulation-mri-sequence/TrapezoidCosineFunction.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class OGSESequence : public MRISequence,
                     public Creator4Arg< OGSESequence,
                                         MRISequence,
                                         const Dictionary&,
                                         float,
                                         bool,
                                         std::ofstream* >
{

  public:

    virtual ~OGSESequence();

    void getNormalizedValueAt( float timeInUs, 
                               float& normalizedGradientValue ) const;
    void getNormalizedIntegralValueAt(
                               float timeInUs, 
                               float& normalizedIntegralGradientValue ) const;

    void displayInformation( std::ofstream* osLog ) const;

    void saveDiffusionInformation( const std::string& fileName ) const;

    static std::string getStaticName();

  protected:

    friend struct Creator4Arg< OGSESequence,
                               MRISequence,
                               const Dictionary&,
                               float,
                               bool,
                               std::ofstream* >;

    OGSESequence( const Dictionary& parameters,
                  float timeStepInUs,
                  bool verbose,
                  std::ofstream* osLog );

    void getNormalizedTrapezoidSineValueAt(
                                 float timeInUs,
                                 float& normalizedGradientValue ) const;
    void getNormalizedTrapezoidSineIntegralValueAt(
                                 float timeInUs,
                                 float& normalizedIntegralGradientValue ) const;

    void getNormalizedTrapezoidCosineValueAt(
                                 float timeInUs,
                                 float& normalizedGradientValue ) const;
    void getNormalizedTrapezoidCosineIntegralValueAt(
                                 float timeInUs,
                                 float& normalizedIntegralGradientValue ) const;

    void getNormalizedSineValueAt(
                                 float timeInUs,
                                 float& normalizedGradientValue ) const;
    void getNormalizedSineIntegralValueAt(
                                 float timeInUs,
                                 float& normalizedIntegralGradientValue ) const;

    void getNormalizedCosineValueAt(
                                 float timeInUs,
                                 float& normalizedGradientValue ) const;
    void getNormalizedCosineIntegralValueAt(
                                 float timeInUs,
                                 float& normalizedIntegralGradientValue ) const;

    std::string _waveformType;
    float _bigDeltaInUs;
    float _periodInUs;
    int32_t _periodCount;

    float _angularFrequency;
    float _t0;
    float _t1;
    float _t2;
    float _t3;
    float _t4;
    float _t5;
    float _t6;
    float _t7;
    
    RCPointer< TrapezoidSineFunction > _trapezoidSineFunction;
    RCPointer< TrapezoidCosineFunction > _trapezoidCosineFunction;

};



}



#endif
