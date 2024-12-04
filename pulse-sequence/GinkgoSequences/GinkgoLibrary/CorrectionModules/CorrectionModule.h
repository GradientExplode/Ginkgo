#ifndef _gkg_pulse_sequence_GinkgoLibrary_CorrectionModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_CorrectionModule_h_


#include <Core/SequenceModule.h>


namespace gkg
{


class CorrectionModule : public SequenceModule
{

  public:

    virtual ~CorrectionModule();
    
    NLSStatus prepare( MrProt& mrProt,
                       SeqLim& seqLim,
                       MrProtocolData::SeqExpo& seqExpo );
                       
    void setGrappaFlag( bool grappaFlag );
    
    bool getGrappaFlag();

  protected:

    CorrectionModule();  
    
    // grappa management
    bool _grappaFlag;

    // RF-signal axis

    // Slice gradient axis (Z)

    // Phase gradient axis (Y)

    // Read gradient axis (X)

    // ADC signal data axis

    // Numeric Crystal Oscillator axis

};


}


#endif
