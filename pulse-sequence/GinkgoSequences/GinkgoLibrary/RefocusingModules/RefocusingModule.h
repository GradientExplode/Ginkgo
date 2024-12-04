#ifndef _gkg_pulse_sequence_GinkgoLibrary_RefocusingModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_RefocusingModule_h_


#include <Core/SequenceModule.h>
//~ #include <ExcitationModules/ExcitationModule.h>
#include <Core/RCPointer.h>


namespace gkg
{


class RefocusingModule : public SequenceModule
{

  public:
    virtual ~RefocusingModule();
    
    void initialize( SeqLim& seqLim );

  protected:

    RefocusingModule();

    //~ RCPointer< ExcitationModule > _excitationModule;

    // RF-signal axis

    // Slice gradient axis (Z)

    // Phase gradient axis (Y)

    // Read gradient axis (X)

    // ADC signal data axis

    // Numeric Crystal Oscillator axis 

};


}


#endif
