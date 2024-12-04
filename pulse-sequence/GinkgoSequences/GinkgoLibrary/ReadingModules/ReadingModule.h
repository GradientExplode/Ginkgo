#ifndef _gkg_pulse_sequence_GinkgoLibrary_ReadingModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_ReadingModule_h_

#include <Core/SequenceModule.h>
#include <ExcitationModules/ExcitationModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>


namespace gkg
{


class ReadingModule : public SequenceModule
{

  public:

    enum Type
    {

      CartesianSingleLine2d = 0U,
      CartesianEchoPlanar2d,
      CartesianEchoVolumar3d,
      Sparkling2d,
      Sparkling3d

    };

    virtual ~ReadingModule();
    
    void initialize( SeqLim& seqLim );
    NLSStatus prepare( MrProt& mrProt,
                       SeqLim& seqLim,
                       MrProtocolData::SeqExpo& seqExpo );

    void setGrappaFlag( bool grappaFlag );
    
    bool getGrappaFlag();
    
    Type getType() const;
    const RCPointer< ExcitationModule >& getExcitationModule() const;
    

  protected:

    ReadingModule( const Type& type,
                   RCPointer< ExcitationModule > excitationModule );

    long _defaultTE;
    long _defaultBWPerPixel;
    long _defaultBaseResolution;
    
    Type _type;
    RCPointer< ExcitationModule > _excitationModule;
    
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
