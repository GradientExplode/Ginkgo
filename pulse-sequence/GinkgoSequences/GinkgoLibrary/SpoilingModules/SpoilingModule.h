#ifndef _gkg_pulse_sequence_GinkgoLibrary_SpoilingModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_SpoilingModule_h_


#include <Core/SequenceModule.h>


namespace gkg
{


class SpoilingModule : public SequenceModule
{

  public:

    virtual ~SpoilingModule();
    
    void setGrappaFlag( bool grappaFlag );
    
    bool getGrappaFlag();

  protected:

    SpoilingModule();  
    
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
