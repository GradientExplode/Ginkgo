#ifndef _gkg_pulse_sequence_GinkgoLibrary_PreEncodingModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_PreEncodingModule_h_


#include <Core/SequenceModule.h>
#include <Core/RCPointer.h>


namespace gkg
{


class PreEncodingModule : public SequenceModule
{

  public:
    virtual ~PreEncodingModule();
    
    void initialize( SeqLim& seqLim );
    
    void setGrappaFlag( bool grappaFlag );
    
    bool getGrappaFlag();

  protected:

    PreEncodingModule();
    
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
