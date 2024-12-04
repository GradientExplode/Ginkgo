#ifndef _gkg_pulse_sequence_GinkgoLibrary_ExcitationModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_ExcitationModule_h_


#include <Core/SequenceModule.h>


namespace gkg
{


class ExcitationModule : public SequenceModule
{

  public:

    virtual ~ExcitationModule();

    void initialize( SeqLim& seqLim );

    bool isParallelTx() const;

    double getRfSpoilPhase() const;

  protected:

    ExcitationModule( bool isParallelTx );

    bool _isParallelTx;

    // RF-signal axis
    double _rfSpoilPhase;

    // Slice gradient axis (Z)

    // Phase gradient axis (Y)

    // Read gradient axis (X)

    // ADC signal data axis

    // Numeric Crystal Oscillator axis 

};


}


#endif
